#include "mySSP1.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "printf_lib.h"
AT45::AT45()
{

}
void AT45::update_dat(char *dat)
{
    data = dat;
}
void AT45::init(void)
{
    unsigned int divider = 2;
        const unsigned int cpuClockMhz = sys_get_cpu_clock() / (1000 * 1000UL);

        // Keep scaling down divider until calculated is higher
        // Example:
        // 60, need 18
        // 60/2   18 < 30 YES
        // 60/4   18 < 15 NO
        while(4 < (cpuClockMhz / divider) && divider <= 254)
        {
            divider += 2;
        }

    //power up spi1
    LPC_SC->PCONP |= (1 << 10);
    //01 PCLK_peripheral = CCLK.. 01, since it's CCLK/1
    LPC_SC->PCLKSEL0 &= ~(3 << 20);
    LPC_SC->PCLKSEL0 |= (1 << 20);

    //P0.6 is used as a GPIO output and acts as a Slave select
    LPC_GPIO0->FIODIR |= (1 << 6);
    LPC_GPIO0->FIOCLR |= (1<<6);
    //LPC_GPIO0->FIOPIN |= (1 << 6);
    //P0.7:9 init
    LPC_PINCON->PINSEL0 &= ~((3 << 18) | (3 << 16) | (3 << 14));
    LPC_PINCON->PINSEL0 |= ((2 << 18) | (2 << 16) | (2 << 14));
    //data size set to 8 bits
    LPC_SSP1->CR0 = 0x07;
    //For AT45 flash SI pin is always latched on the rising edge of SCK, while output data
    //on the SO pin is always clocked out on the falling edge of SCK.
    //MS=0 (Master), SSE =1
    LPC_SSP1->CR1 = 0x2;
    LPC_SSP1->CPSR = divider; //SCK Frequency for Continuous Array Read(Low Frequency) is 33Mhx max. here we are setting it below it.
}

uint8_t AT45::SSP1exchangeByte(uint8_t out)
{
    LPC_SSP1->DR = out;
    while (LPC_SSP1->SR & (1 << 4))
        ;
    return LPC_SSP1->DR;
}

void AT45::read_ID()
{
    int i;
    this->init();
    for (i = 0; i < 10000; i++)
        ;
    uart0_puts("Device ID\n");
    LPC_GPIO0->FIOCLR = (1 << 6);
    u0_dbg_printf("%d\n",SSP1exchangeByte(0x9f));
    u0_dbg_printf("%d\n",SSP1exchangeByte(0x9f));
    u0_dbg_printf("%d\n",SSP1exchangeByte(0x00));
    u0_dbg_printf("%d\n",SSP1exchangeByte(0x00));
    LPC_GPIO0->FIOSET = (1 << 6);

}
void AT45::read()
{
    int i;
    init();
    uart0_puts("Reading from Buffer 1");
    LPC_GPIO0->FIOCLR = (1 << 6);
    //**************************Read from buff 1
    //Opcode for Read buff 1
    SSP1exchangeByte(0xd4);
    //Send 3 Bytes = 24 bits= 16 don't care bits + 8 buffer addr bits ????
    SSP1exchangeByte(0x00);
    SSP1exchangeByte(0x00);
    SSP1exchangeByte(0x00);
    SSP1exchangeByte(0x00); //trial for buff1
    //Send 1 dummy byte
    u0_dbg_printf("%c\n",SSP1exchangeByte(0x00));
    u0_dbg_printf("%c\n",SSP1exchangeByte(0x00));
    u0_dbg_printf("%c\n",SSP1exchangeByte(0x00));
    u0_dbg_printf("%c\n",SSP1exchangeByte(0x00));
    u0_dbg_printf("%c\n",SSP1exchangeByte(0x00));
    u0_dbg_printf("******************************************************************************\n");
    LPC_GPIO0->FIOSET = (1 << 6);

}

void AT45::write(const char* str)
{
    int i;
    init();
    u0_dbg_printf("Writing to Buffer 1\n");
    LPC_GPIO0->FIOCLR = (1 << 6);
    SSP1exchangeByte(0x84); //Send 3 Bytes = 24 bits= 16 don't care bits + 8 (1st Byte of buffer) bits
    //SSP1exchangeByte(0x00);
    //SSP1exchangeByte(0x00);
    //SSP1exchangeByte(0x00);
    //SSP1exchangeByte('0');
    //for (int j = 0; j < strlen(str); j++)
    //    SSP1exchangeByte(str[j]);
    SSP1exchangeByte('T');
    SSP1exchangeByte('e');
    SSP1exchangeByte('s');
    SSP1exchangeByte('t');
    LPC_GPIO0->FIOSET = (1 << 6);
}
