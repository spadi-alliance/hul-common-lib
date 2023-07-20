#include<fstream>
#include<cstdlib>
#include<iostream>
#include<cstdio>
#include<csignal>
#include<list>
#include<unistd.h>

#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/tcp.h>
#include<string.h>
#include<errno.h>

#include"DaqFuncs.hh"

namespace HUL::DAQ{

  int user_stop = 0;

  static const int32_t kNumData   {10000};
  static const int32_t kNumByte   {8};

  struct DataCont{
    uint8_t data[kNumData*kNumByte];
    uint32_t recv_bytes;
  };

  // signal -----------------------------------------------------------------
  void
  UserStop_FromCtrlC(int signal)
  {
    std::cout << "#D: Stop request" << std::endl;
    user_stop = 1;
  }


  // execute daq ------------------------------------------------------------
  void
  DoStrDaq(std::string ip, int32_t runno)
  {
    (void) signal(SIGINT, UserStop_FromCtrlC);

    // TCP socket
    int sock;
    if(-1 == (sock = ConnectSocket(ip))) return;
    std::cout << "#D: Socket connected" << std::endl;

    // Start DAQ
    std::string filename = "data/run" + std::to_string(runno) + ".dat";
    std::ofstream ofs(filename.c_str(), std::ios::binary);
    if(!ofs.is_open()){
      std::cerr << "#E: Data file cannot be created.\n"
		<< "    Does a 'data' directory exist in the current directory?"
		<< std::endl;
      std::exit(-1);
    }

    std::cout << "#D: Start DAQ" << std::endl;
    // DAQ Cycle
    std::list<DataCont> data_list;
    static const uint32_t kReadSize = sizeof(uint8_t)*kNumByte*kNumData;
    for(;;){
      DataCont dcont = {{0},0};
      int32_t recv_status = 0;

      while( kRecvTimeOut == ( recv_status = Recieve(sock, dcont.data, kReadSize, dcont.recv_bytes)) && !user_stop) continue;

      if(recv_status == kRecvZero){
	std::cout << "#E: Recv() returns 0." << std::endl;
	break;
      }

      if(recv_status == kRecvError){
	std::cout << "#E: Recv() returns -1." << std::endl;
	break;
      }

      printf("Recording...");
      data_list.push_back(dcont);
    
      if(user_stop == 1) break;
    }// For()

    std::list<DataCont>::iterator itr     = data_list.begin();
    std::list<DataCont>::iterator itr_end = data_list.end();
    for(; itr != itr_end; ++itr){
      //    std::cout << "N:" << (*itr).n_word << std::endl;
      ofs.write((char*)(*itr).data, (*itr).recv_bytes);
    }

    std::cout << "#D: Read remaining data in FPGA. Wait time out." << std::endl;
  
    sleep(1);
    DataCont dcont = {{0},0};
    while(kRecvTimeOut != Recieve(sock, dcont.data, kReadSize,dcont.recv_bytes));
  
    std::cout << "#D: Time out is detected. End of DAQ." << std::endl;

    close(sock);
    ofs.close();
  
    return;
  }

  // ConnectSocket ----------------------------------------------------------
  int
  ConnectSocket(std::string ip)
  {
    struct sockaddr_in SiTCP_ADDR;
    unsigned int port = 24;

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SiTCP_ADDR.sin_family      = AF_INET;
    SiTCP_ADDR.sin_port        = htons((unsigned short int) port);
    SiTCP_ADDR.sin_addr.s_addr = inet_addr(ip.c_str());

    struct timeval tv;
    tv.tv_sec  = 3;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));

    int flag = 1;
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));

    if(0 > connect(sock, (struct sockaddr*)&SiTCP_ADDR, sizeof(SiTCP_ADDR))){
      std::cerr << "#E : TCP connection error" << std::endl;
      close(sock);
      return -1;
    }
  
    return sock;
  }

  // Socket read ----------------------------------------------------------------
  int32_t
  Recieve(int sock, uint8_t* data_buf, uint32_t length, uint32_t& num_recieved_bytes)
  {
    uint32_t revd_size = 0;
    int      tmp_ret   = 0;
    int      ret_val   = HUL::DAQ::kRecvNormal;

    while(revd_size < length){
      tmp_ret = recv(sock, (char*)data_buf + revd_size, length -revd_size, 0);

      if(tmp_ret == 0){
	num_recieved_bytes = 0;
	ret_val            = HUL::DAQ::kRecvZero;
	return ret_val;
      }
    
      if(tmp_ret < 0){
	int errbuf = errno;
	perror("#D: TCP receive");
	if(errbuf == EAGAIN){
	  // socket read time out
	  std::cout << " recv() time out, recieved byte size: " << revd_size << std::endl;
	  num_recieved_bytes = static_cast<uint32_t>(revd_size);
	  ret_val            = HUL::DAQ::kRecvTimeOut;
	}else{
	  // something wrong
	  std::cerr << "#E: TCP error : " << errbuf << std::endl;

	  num_recieved_bytes = 0;
	  ret_val            = HUL::DAQ::kRecvError;
	}

	return ret_val;
      }

      revd_size += tmp_ret;
    }

    num_recieved_bytes = static_cast<uint32_t>(revd_size);
    return ret_val;
  }

};
