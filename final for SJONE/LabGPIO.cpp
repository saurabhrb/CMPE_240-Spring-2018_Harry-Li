#include "LabGPIO.hpp"

LabGPIO_X::LabGPIO_X()
{


}
LabGPIO_X::~LabGPIO_X()
{
    delete FIOXDIR;
    delete FIOXMASK;
    delete FIOXPIN;
    delete FIOXSET;
    delete FIOXCLR;

}

LabGPIO_X::LabGPIO_X(uint8_t port, uint8_t pin)
{
    //port => 0-4
    //pin => 0-31
    this->port = port;
    this->pin = pin;
    FIOXDIR = (uint32_t *) (LPC_GPIO_BASE + (uint32_t) ((port * 2) << 4));
    FIOXMASK = (uint32_t *) (LPC_GPIO_BASE + (uint32_t) ((port * 2) << 4) + 0x10);
    FIOXPIN = (uint32_t *) (LPC_GPIO_BASE + (uint32_t) ((port * 2) << 4) + 0x14);
    FIOXSET = (uint32_t *) (LPC_GPIO_BASE + (uint32_t) ((port * 2) << 4) + 0x18);
    FIOXCLR = (uint32_t *) (LPC_GPIO_BASE + (uint32_t) ((port * 2) << 4) + 0x1c);
}

void LabGPIO_X::setPP(uint8_t port, uint8_t pin)
{
    this->port = port;
    this->pin = pin;
    FIOXDIR = (uint32_t *) (LPC_GPIO_BASE + (uint32_t)((port * 2) << 4));
    FIOXMASK = (uint32_t *) (LPC_GPIO_BASE + (uint32_t)((port * 2) << 4) + 0x10);
    FIOXPIN = (uint32_t *) (LPC_GPIO_BASE + (uint32_t)((port * 2) << 4) + 0x14);
    FIOXSET = (uint32_t *) (LPC_GPIO_BASE + (uint32_t)((port * 2) << 4) + 0x18);
    FIOXCLR = (uint32_t *) (LPC_GPIO_BASE + (uint32_t)((port * 2) << 4) + 0x1c);
}

void LabGPIO_X::setAsInput()
{
    *FIOXDIR &= ~(1 << pin);
}
/**
 * Should alter the hardware registers to set the pin as an input
 */
void LabGPIO_X::setAsOutput()
{
    *FIOXDIR |= (1 << pin);
}

/**
 * Should alter the set the direction output or input depending on the input.
 *
 * @param {bool} output - true => output, false => set pin to input
 */
void LabGPIO_X::setDirection(bool output)
{
    if (output) this->setAsOutput();
    else
        this->setAsInput();
}
/**
 * Should alter the hardware registers to set the pin as high
 */
void LabGPIO_X::setHigh()
{
    *FIOXPIN |= (1 << pin);
}
/**
 * Should alter the hardware registers to set the pin as low
 */
void LabGPIO_X::setLow()
{
    *FIOXPIN &= ~(1 << pin);
}
/**
 * Should alter the hardware registers to set the pin as low
 *
 * @param {bool} high - true => pin high, false => pin low
 */
void LabGPIO_X::set(bool high)
{
    if (high) setHigh();
    else
        setLow();
}
/**
 * Should return the state of the pin (input or output, doesn't matter)
 *
 * @return {bool} level of pin high => true, low => false
 */
bool LabGPIO_X::getLevel()
{
    return (*FIOXPIN & (1 << pin));
}
