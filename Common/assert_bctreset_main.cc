#include <iostream>
#include <cstdio>

#include "RegisterMapCommon.hh"
#include "network.hh"
#include "UDPRBCP.hh"
#include "FPGAModule.hh"
#include "FlashMemoryProgrammer.hh"
#include "rbcp.hh"

using namespace HUL;
int main(int argc, char* argv[])
{
  if(1 == argc){
    std::cout << "Usage\n";
    std::cout << "assert_bctreset[IP address]" << std::endl;
    return 0;
  }// usage
  
  // body ------------------------------------------------------
  std::vector<std::string> in_arg(argc-1);
  std::copy(argv+1, argv+argc, in_arg.begin());

  std::string ip_address = in_arg[0];
  
  RBCP::UDPRBCP udp_rbcp(ip_address, RBCP::gUdpPort, RBCP::UDPRBCP::kNoDisp);
  HUL::FPGAModule fpga_module(udp_rbcp);
  fpga_module.WriteModule(BCT::kAddrReset, 0);

  return 0;

}// main
