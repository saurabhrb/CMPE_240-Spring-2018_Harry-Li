#include "LabSPI.hpp"

constexpr LPC_SSP_TypeDef * LabSPI::SSP[];
SemaphoreHandle_t LabSPI::mutex_SSP0;
SemaphoreHandle_t LabSPI::mutex_SSP1;

LabSPI::LabSPI()
{

}

LabSPI::~LabSPI()
{

}

bool LabSPI::init(Peripheral peripheral, uint8_t data_size_select, FrameModes format, uint8_t divide)
{
    mutex_SSP0 = xSemaphoreCreateMutex();
    mutex_SSP1 = xSemaphoreCreateMutex();
    if (!((peripheral >= SSP0 && peripheral <= SSP1) && (format >= SPI && format <= MICROWIRE) && (divide % 2 == 0 && divide >= 2)
            && (data_size_select >= 4 && data_size_select <= 16))) return false;

    if (peripheral == SSP0) {
        LPC_SC->PCONP |= (1 << 21); // enable SSP0
        LPC_SC->PCLKSEL1 &= ~(3 << 10); // Clear clock
        LPC_SC->PCLKSEL1 |= (1 << 10); // clock/1
        LPC_PINCON->PINSEL0 &= ~(3 << 30); // Clear bits SCK0
        LPC_PINCON->PINSEL1 &= ~((3 << 2) | (3 << 4)); // Clear bits MISO0, MOSI0
        LPC_PINCON->PINSEL0 |= (2 << 30); // Set SCK0
        LPC_PINCON->PINSEL0 |= ((2 << 2) | (2 << 4)); // Set MISO0, MOSI0
    }
    else if (peripheral == SSP1) {
        LPC_SC->PCONP |= (1 << 10); // enable SSP1
        LPC_SC->PCLKSEL0 &= ~(3 << 20); // Clear clock
        LPC_SC->PCLKSEL0 |= (1 << 20); // clock/1
        LPC_PINCON->PINSEL0 &= ~((3 << 14) | (3 << 16) | (3 << 18)); // Clear bits SCK1, MISO1, MOSI1
        LPC_PINCON->PINSEL0 |= ((2 << 14) | (2 << 16) | (2 << 18)); // Set SCK1, MISO1, MOSI1
    }
    SSP[peripheral]->CR0 &= ~(0x3f); //clear CR0
    SSP[peripheral]->CR0 |= (data_size_select - 1);
    SSP[peripheral]->CR0 |= (format << 4);
    SSP[peripheral]->CR1 = (1 << 1);
    SSP[peripheral]->CPSR = divide;
    return true;
}

uint8_t LabSPI::transfer(Peripheral peripheral, uint8_t send)
{
    SSP[peripheral]->DR = send;
    while (SSP[peripheral]->SR & (1 << 4))
        ; //transferring and receiving data
    return SSP[peripheral]->DR;
}
