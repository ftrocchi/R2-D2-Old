#ifndef __I2C_COMMON_H__
#define __I2C_COMMON_H__

struct I2C_TargetDevice {
  enum Value {
    Logic = 0,
    PSI = 1
  };
};

struct I2C_PSI_Commands {
  enum Value {
    On = 0,
    Off = 1,
    Brightness = 2
  };
};

#endif

