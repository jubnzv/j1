#ifndef IOCONSOLE_HPP
#define IOCONSOLE_HPP

#include <cstdint>

class IOConsole {
  public:
    static void Write(uint16_t addr);
    static void Read(uint16_t addr);
};

#endif // IOCONSOLE_HPP
