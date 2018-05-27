
#ifndef AT45_HPP_
#define AT45_HPP_

#include "stdint.h"
#include "singleton_template.hpp"  // Singleton Template

#include "LPC17xx.h"
#include "uart0_min.h"
#include <cstring>

#include <stdlib.h>

class AT45 : public SingletonTemplate<AT45>
{
private:
    AT45();
    friend class SingletonTemplate<AT45>;
    char *data;
public:
    void init();
    void update_dat(char *dat);
    void read();
    void write(const char* str);
    void read_ID();
    uint8_t SSP1exchangeByte(uint8_t out);
    ~AT45();
};

#endif
