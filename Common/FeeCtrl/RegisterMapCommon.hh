#ifndef REGISTER_COMMON_HH
#define REGISTER_COMMON_HH

#include<stdint.h>

namespace LBUS{
// BIAS -------------------------------------------------------------------
// Module ID: 0x9
namespace BIAS{
  enum LocalAddress
    {
     kAddrTxd       = 0x90000000, // W,  [7:0] SPI TX data
     kAddrRxd       = 0x90100000, // R,  [7:0] SPI RX data
     kAddrExecWrite = 0x91000000, // W,
     kAddrExecRead  = 0x92000000  // R,
     
    };
};

// MIG --------------------------------------------------------------------
// Module ID: 0xA
namespace MIG{
  enum LocalAddress
    {
     kAddrNumWrite  = 0xa0000000, // W/R, [5:0] Number of writes to SDRAM
     kAddrNumRead   = 0xa0100000  // W/R, [5:0] Number of reads from SDRAM
    };
};
  
// C6C --------------------------------------------------------------------
// Module ID: 0xB
namespace C6C{
  enum LocalAddress
    {
     kAddrTxd       = 0xb0000000, // W,  [31:0] SPI TX data
     kAddrRxd       = 0xb0100000, // R,  [31:0] SPI RX data
     kAddrExecWrite = 0xb1000000, // W,
     kAddrExecRead  = 0xb2000000  // R,
    };
};
  
// SDS --------------------------------------------------------------------
// Module ID: 0xC
namespace SDS{
  enum LocalAddress
    {
     kAddrSdsStatus       = 0xc0000000, // R,   [7:0]
     kAddrXadcDrpMode     = 0xc0100000, // W/R, [0:0]
     kAddrXadcDrpAddr     = 0xc0200000, // W/R, [6:0]
     kAddrXadcDrpDin      = 0xc0300000, // W/R, [15:0] (2byte)
     kAddrXadcDrpDout     = 0xc0400000, // R,   [15:0] (2byte)
     kAddrXadcExecute     = 0xc0f00000, // W,
     kAddrSemCorCount     = 0xc1000000, // R    [15:0] (2byte)
     kAddrSemRstCorCount  = 0xc2000000, // W
     kAddrSemErrAddr      = 0xc3000000, // W    [39:0] (5byte)
     kAddrSemErrStrobe    = 0xc4000000  // W
    };

  enum SdsStatusBit
    {
     kXadcOverTemperature   = 0x01,
     kXadcUserTempAlarm     = 0x02,
     kXadcUserVccIntAlarm   = 0x04,
     kReserve1              = 0x08,
     kSemWatchdogAlarm      = 0x10,
     kSemUncorrectableAlarm = 0x20,
     kReserve2              = 0x40,
     kReserve3              = 0x80
    };
  
  enum DrpMode
    {
     kDrpReadMode  = 0,
     kDrpWriteMode = 1,
    };

  enum DrpAddress
    {
     kAddrDrpTemp    = 0x0,
     kAddrDrpVccInt  = 0x1,
     kAddrDrpVccAux  = 0x2,
     kAddrDrpVccBram = 0x6
    };

  enum SemErrCommand
    {
     kSetIdle    = 0xe,
     kSetObserve = 0xa,
     kSetError   = 0xc,
     kSetReset   = 0xb
     
    };
};

// FMP --------------------------------------------------------------------
// Module ID: 0xD
namespace FMP{
  enum LocalAddress
    {
     kAddrStatus          = 0xd0000000, // R,   [7:0]
     kAddrCtrl            = 0xd0100000, // W/R, [7:0]
     kAddrRegister        = 0xd0200000, // W/R, [7:0] W,R,D,A0,A1,A2,A3,I (8 byte)
     kAddrInstLength      = 0xd0300000, // W/R, [2:0]
     kAddrReadLength      = 0xd0400000, // W/R, [9:0]
     kAddrWriteLength     = 0xd0500000, // W/R, [9:0]
     kAddrReadCountFifo   = 0xd0600000, // R, [9:0]
     kAddrReadFifo        = 0xd0700000, // R, [7:0]
     kAddrWriteCountFifo  = 0xd0800000, // R, [9:0]
     kAddrWriteFifo       = 0xd0900000, // R, [7:0]
     kAddrExecute         = 0xd1000000  // W
    };

  static const int32_t kNumRegister = 8;
  enum IndexRegister
    {
     kIndexInst  = 0x00000,
     kIndexAddr3 = 0x10000,
     kIndexAddr2 = 0x20000,
     kIndexAddr1 = 0x30000,
     kIndexAddr0 = 0x40000,
     kIndexDummy = 0x50000,
     kIndexRead  = 0x60000,
     kIndexWrite = 0x70000
    };

  enum StatusRegister
    {
     kBusyCycle     = 0x1
    };

  enum CtrlRegister
    {
     kCtrlReadMode  = 0x0,
     kCtrlWriteMode = 0x1,
     kCtrlInstMode  = 0x2,
     kCtrlDummyMode = 0x4
    };
};

// BCT --------------------------------------------------------------------
// Module ID: 0xE
namespace BCT{
  enum LocalAddress
    {
     kAddrReset   = 0xe0000000, // W, assert reset signal from BCT
     kAddrVersion = 0xe0100000,// R, [31:0]
     kAddrReConfig= 0xe0200000 // W, Reconfig FPGA by SPI
    };
};
};

#endif
