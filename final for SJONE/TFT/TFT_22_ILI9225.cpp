#include "TFT_22_ILI9225.h"
#include "FreeRTOS.h"
#include "ssp1.h"
#include "task.h"
#include "math.h"

// Constructor when using software SPI.  All output pins are configurable.
TFT_22_ILI9225::TFT_22_ILI9225(GPIO rst, GPIO rs, GPIO cs, GPIO sdi, GPIO clk, GPIO led)
{
    _rst.setPP(rst.port, rst.pin);
    _rs.setPP(rs.port, rs.pin);
    _cs.setPP(cs.port, cs.pin);
    _sdi.setPP(sdi.port, sdi.pin);
    _clk.setPP(clk.port, clk.pin);
    _led.setPP(led.port, led.pin);
    _bgColor = COLOR_BLACK;
    hwSPI = false;
}

// Constructor when using hardware SPI.  Faster, but must use SPI pins
// specific to each board type (e.g. 11,13 for Uno, 51,52 for Mega, etc.)
TFT_22_ILI9225::TFT_22_ILI9225(GPIO rst, GPIO rs, GPIO cs, GPIO led)
{
    _rst.setPP(rst.port, rst.pin);
    _rs.setPP(rs.port, rs.pin);
    _cs.setPP(cs.port, cs.pin);
    _led.setPP(led.port, led.pin);
    hwSPI = true;
    SSP_FREQ = 16;
}
void TFT_22_ILI9225::setPINS(GPIO rst, GPIO rs, GPIO cs, GPIO led)
{
    _rst.setPP(rst.port, rst.pin);
    _rs.setPP(rs.port, rs.pin);
    _cs.setPP(cs.port, cs.pin);
    _led.setPP(led.port, led.pin);
    hwSPI = true;
    SSP_FREQ = 16;
}
TFT_22_ILI9225::TFT_22_ILI9225()
{

}
void TFT_22_ILI9225::_orientCoordinates(uint16_t &x1, uint16_t &y1)
{

    switch (_orientation) {
        case 0:  // ok
            break;
        case 1: // ok
            y1 = _maxY - y1 - 1;
            _swap(x1, y1);
            break;
        case 2: // ok
            x1 = _maxX - x1 - 1;
            y1 = _maxY - y1 - 1;
            break;
        case 3: // ok
            x1 = _maxX - x1 - 1;
            _swap(x1, y1);
            break;
    }
}

void TFT_22_ILI9225::_setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    _orientCoordinates(x0, y0);
    _orientCoordinates(x1, y1);

    if (x1 < x0) _swap(x0, x1);
    if (y1 < y0) _swap(y0, y1);

    _writeRegister(ILI9225_HORIZONTAL_WINDOW_ADDR1, x1);
    _writeRegister(ILI9225_HORIZONTAL_WINDOW_ADDR2, x0);

    _writeRegister(ILI9225_VERTICAL_WINDOW_ADDR1, y1);
    _writeRegister(ILI9225_VERTICAL_WINDOW_ADDR2, y0);

    _writeRegister(ILI9225_RAM_ADDR_SET1, x0);
    _writeRegister(ILI9225_RAM_ADDR_SET2, y0);

    _writeCommand(0x00, 0x22);
}

void TFT_22_ILI9225::setSPIfrequency(uint8_t freq)
{
    SSP_FREQ = freq;
}

void TFT_22_ILI9225::begin()
{

    // Set up pins
    _rs.setAsOutput();
    _cs.setAsOutput();
    _rst.setAsOutput();
    _led.setAsOutput();

    if (hwSPI) { // Using hardware SPI
        //SPI.begin();
        ssp1_init();
        //SPI.setClockDivider(SPI_CLOCK_DIV2); // 4 MHz (half speed)
        ssp1_set_max_clock(SSP_FREQ);
        //SPI.setClockDivider(SPI_CLOCK_DIV4); // 4 MHz (half speed)
        //SPI.setBitOrder(MSBFIRST);
        //SPI.setDataMode(SPI_MODE0);
    }
    else {
        _clk.setAsOutput();
        _sdi.setAsOutput();
    }

    // Turn on backlight
    _led.set(1);

    // Initialization Code
    _rst.set(1);        // Pull the reset pin high to release the ILI9225C from the reset status
    vTaskDelay(1);
    _rst.set(0); // Pull the reset pin low to reset ILI9225
    vTaskDelay(10);
    _rst.set(1); // Pull the reset pin high to release the ILI9225C from the reset status
    vTaskDelay(50);

    /* Start Initial Sequence */
    /* Set SS bit and direction output from S528 to S1 */
    _writeRegister(ILI9225_POWER_CTRL1, 0x0000); // Set SAP,DSTB,STB
    _writeRegister(ILI9225_POWER_CTRL2, 0x0000); // Set APON,PON,AON,VCI1EN,VC
    _writeRegister(ILI9225_POWER_CTRL3, 0x0000); // Set BT,DC1,DC2,DC3
    _writeRegister(ILI9225_POWER_CTRL4, 0x0000); // Set GVDD
    _writeRegister(ILI9225_POWER_CTRL5, 0x0000); // Set VCOMH/VCOML voltage
    vTaskDelay(40);

    // Power-on sequence
    _writeRegister(ILI9225_POWER_CTRL2, 0x0018); // Set APON,PON,AON,VCI1EN,VC
    _writeRegister(ILI9225_POWER_CTRL3, 0x6121); // Set BT,DC1,DC2,DC3
    _writeRegister(ILI9225_POWER_CTRL4, 0x006F); // Set GVDD   /*007F 0088 */
    _writeRegister(ILI9225_POWER_CTRL5, 0x495F); // Set VCOMH/VCOML voltage
    _writeRegister(ILI9225_POWER_CTRL1, 0x0800); // Set SAP,DSTB,STB
    vTaskDelay(10);
    _writeRegister(ILI9225_POWER_CTRL2, 0x103B); // Set APON,PON,AON,VCI1EN,VC
    vTaskDelay(50);

    _writeRegister(ILI9225_DRIVER_OUTPUT_CTRL, 0x011C); // set the display line number and display direction
    _writeRegister(ILI9225_LCD_AC_DRIVING_CTRL, 0x0100); // set 1 line inversion
    _writeRegister(ILI9225_ENTRY_MODE, 0x1030); // set GRAM write direction and BGR=1.
    _writeRegister(ILI9225_DISP_CTRL1, 0x0000); // Display off
    _writeRegister(ILI9225_BLANK_PERIOD_CTRL1, 0x0808); // set the back porch and front porch
    _writeRegister(ILI9225_FRAME_CYCLE_CTRL, 0x1100); // set the clocks number per line
    _writeRegister(ILI9225_INTERFACE_CTRL, 0x0000); // CPU interface
    _writeRegister(ILI9225_OSC_CTRL, 0x0D01); // Set Osc  /*0e01*/
    _writeRegister(ILI9225_VCI_RECYCLING, 0x0020); // Set VCI recycling
    _writeRegister(ILI9225_RAM_ADDR_SET1, 0x0000); // RAM Address
    _writeRegister(ILI9225_RAM_ADDR_SET2, 0x0000); // RAM Address

    /* Set GRAM area */
    _writeRegister(ILI9225_GATE_SCAN_CTRL, 0x0000);
    _writeRegister(ILI9225_VERTICAL_SCROLL_CTRL1, 0x00DB);
    _writeRegister(ILI9225_VERTICAL_SCROLL_CTRL2, 0x0000);
    _writeRegister(ILI9225_VERTICAL_SCROLL_CTRL3, 0x0000);
    _writeRegister(ILI9225_PARTIAL_DRIVING_POS1, 0x00DB);
    _writeRegister(ILI9225_PARTIAL_DRIVING_POS2, 0x0000);
    _writeRegister(ILI9225_HORIZONTAL_WINDOW_ADDR1, 0x00AF);
    _writeRegister(ILI9225_HORIZONTAL_WINDOW_ADDR2, 0x0000);
    _writeRegister(ILI9225_VERTICAL_WINDOW_ADDR1, 0x00DB);
    _writeRegister(ILI9225_VERTICAL_WINDOW_ADDR2, 0x0000);

    /* Set GAMMA curve */
    _writeRegister(ILI9225_GAMMA_CTRL1, 0x0000);
    _writeRegister(ILI9225_GAMMA_CTRL2, 0x0808);
    _writeRegister(ILI9225_GAMMA_CTRL3, 0x080A);
    _writeRegister(ILI9225_GAMMA_CTRL4, 0x000A);
    _writeRegister(ILI9225_GAMMA_CTRL5, 0x0A08);
    _writeRegister(ILI9225_GAMMA_CTRL6, 0x0808);
    _writeRegister(ILI9225_GAMMA_CTRL7, 0x0000);
    _writeRegister(ILI9225_GAMMA_CTRL8, 0x0A00);
    _writeRegister(ILI9225_GAMMA_CTRL9, 0x0710);
    _writeRegister(ILI9225_GAMMA_CTRL10, 0x0710);

    _writeRegister(ILI9225_DISP_CTRL1, 0x0012);
    vTaskDelay(50);
    _writeRegister(ILI9225_DISP_CTRL1, 0x1017);

    setBacklight(true);
    setOrientation(0);

    // Initialize variables
    setBackgroundColor( COLOR_BLACK);

    clear();
}

void TFT_22_ILI9225::clear()
{
    uint8_t old = _orientation;
    setOrientation(0);
    fillRectangle(0, 0, _maxX - 1, _maxY - 1, COLOR_BLACK);
    setOrientation(old);
    vTaskDelay(10);
}

void TFT_22_ILI9225::invert(bool flag)
{
    _writeCommand(0x00, flag ? ILI9225C_INVON : ILI9225C_INVOFF);
}

void TFT_22_ILI9225::setBacklight(bool flag)
{
    //if (_led)
    //digitalWrite(_led, flag);
    _led.set(flag);
}

void TFT_22_ILI9225::setDisplay(bool flag)
{
    if (flag) {
        _writeRegister(0x00ff, 0x0000);
        _writeRegister(ILI9225_POWER_CTRL1, 0x0000);
        vTaskDelay(50);
        _writeRegister(ILI9225_DISP_CTRL1, 0x1017);
        vTaskDelay(200);
    }
    else {
        _writeRegister(0x00ff, 0x0000);
        _writeRegister(ILI9225_DISP_CTRL1, 0x0000);
        vTaskDelay(50);
        _writeRegister(ILI9225_POWER_CTRL1, 0x0003);
        vTaskDelay(200);
    }
}

void TFT_22_ILI9225::setOrientation(uint8_t orientation)
{

    _orientation = orientation % 4;

    switch (_orientation) {
        case 0:
            _maxX = ILI9225_LCD_WIDTH;
            _maxY = ILI9225_LCD_HEIGHT;
            break;
        case 1:
            _maxX = ILI9225_LCD_HEIGHT;
            _maxY = ILI9225_LCD_WIDTH;
            break;
        case 2:
            _maxX = ILI9225_LCD_WIDTH;
            _maxY = ILI9225_LCD_HEIGHT;
            break;
        case 3:
            _maxX = ILI9225_LCD_HEIGHT;
            _maxY = ILI9225_LCD_WIDTH;
            break;
    }
}

uint8_t TFT_22_ILI9225::getOrientation()
{
    return _orientation;
}

void TFT_22_ILI9225::drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    drawLine(x1, y1, x1, y2, color);
    drawLine(x1, y1, x2, y1, color);
    drawLine(x1, y2, x2, y2, color);
    drawLine(x2, y1, x2, y2, color);
}

void TFT_22_ILI9225::fillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{

    _setWindow(x1, y1, x2, y2);

    for (uint16_t t = (y2 - y1 + 1) * (x2 - x1 + 1); t > 0; t--)
        _writeData(color >> 8, color);
}

void TFT_22_ILI9225::drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{

    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    drawPixel(x0, y0 + r, color);
    drawPixel(x0, y0 - r, color);
    drawPixel(x0 + r, y0, color);
    drawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

void TFT_22_ILI9225::fillCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint16_t color)
{

    int16_t f = 1 - radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * radius;
    int16_t x = 0;
    int16_t y = radius;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawLine(x0 + x, y0 + y, x0 - x, y0 + y, color); // bottom
        drawLine(x0 + x, y0 - y, x0 - x, y0 - y, color); // top
        drawLine(x0 + y, y0 - x, x0 + y, y0 + x, color); // right
        drawLine(x0 - y, y0 - x, x0 - y, y0 + x, color); // left
    }
    fillRectangle(x0 - x, y0 - y, x0 + x, y0 + y, color);
}

void TFT_22_ILI9225::drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{

    // Classic Bresenham algorithm
    int16_t steep = abs(y2 - y1) > abs(x2 - x1);
    int16_t dx, dy;

    if (steep) {
        _swap(x1, y1);
        _swap(x2, y2);
    }

    if (x1 > x2) {
        _swap(x1, x2);
        _swap(y1, y2);
    }

    dx = x2 - x1;
    dy = abs(y2 - y1);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y1 < y2) ystep = 1;
    else
        ystep = -1;

    for (; x1 <= x2; x1++) {
        if (steep) drawPixel(y1, x1, color);
        else
            drawPixel(x1, y1, color);

        err -= dy;
        if (err < 0) {
            y1 += ystep;
            err += dx;
        }
    }
}

void TFT_22_ILI9225::drawPixel(uint16_t x1, uint16_t y1, uint16_t color)
{

    if ((x1 < 0) || (x1 >= _maxX) || (y1 < 0) || (y1 >= _maxY)) return;

    _setWindow(x1, y1, x1 + 1, y1 + 1);
    _orientCoordinates(x1, y1);
    _writeData(color >> 8, color);

}

uint16_t TFT_22_ILI9225::maxX()
{
    return _maxX;
}

uint16_t TFT_22_ILI9225::maxY()
{
    return _maxY;
}

uint16_t TFT_22_ILI9225::setColor(uint8_t red8, uint8_t green8, uint8_t blue8)
{
    // rgb16 = red5 green6 blue5
    return (red8 >> 3) << 11 | (green8 >> 2) << 5 | (blue8 >> 3);
}

void TFT_22_ILI9225::splitColor(uint16_t rgb, uint8_t &red, uint8_t &green, uint8_t &blue)
{
    // rgb16 = red5 green6 blue5
    red = (rgb & 0b1111100000000000) >> 11 << 3;
    green = (rgb & 0b0000011111100000) >> 5 << 2;
    blue = (rgb & 0b0000000000011111) << 3;
}

void TFT_22_ILI9225::_swap(uint16_t &a, uint16_t &b)
{
    uint16_t w = a;
    a = b;
    b = w;
}

// Utilities
void TFT_22_ILI9225::_writeCommand(uint8_t HI, uint8_t LO)
{
    //digitalWrite(_rs, LOW);
    _rs.set(0);
    //digitalWrite(_cs, LOW);
    _cs.set(0);
    //ssp1_exchange_byte(HI); //SPI.transfer(HI);
    ssp1_exchange_byte(HI);
    //ssp1_exchange_byte(LO); //SPI.transfer(LO);
    ssp1_exchange_byte(LO);
    //digitalWrite(_cs, HIGH);
    _cs.set(1);

}

void TFT_22_ILI9225::_writeData(uint8_t HI, uint8_t LO)
{
    //digitalWrite(_rs, HIGH);
    _rs.set(1);
    //digitalWrite(_cs, LOW);
    _cs.set(0);
    ssp1_exchange_byte(HI); //SPI.transfer(HI);
    ssp1_exchange_byte(LO); //SPI.transfer(LO);
    //digitalWrite(_cs, HIGH);
    _cs.set(1);
}

void TFT_22_ILI9225::_writeRegister(uint16_t reg, uint16_t data)
{
    _writeCommand(reg >> 8, reg & 255);
    _writeData(data >> 8, data & 255);
}

void TFT_22_ILI9225::drawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
{
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x3, y3, color);
    drawLine(x3, y3, x1, y1, color);
}

void TFT_22_ILI9225::fillTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
{

    uint16_t a, b, y, last;

    // Sort coordinates by Y order (y3 >= y2 >= y1)
    if (y1 > y2) {
        _swap(y1, y2);
        _swap(x1, x2);
    }
    if (y2 > y3) {
        _swap(y3, y2);
        _swap(x3, x2);
    }
    if (y1 > y2) {
        _swap(y1, y2);
        _swap(x1, x2);
    }

    if (y1 == y3) { // Handle awkward all-on-same-line case as its own thing
        a = b = x1;
        if (x2 < a) a = x2;
        else if (x2 > b) b = x2;
        if (x3 < a) a = x3;
        else if (x3 > b) b = x3;
        drawLine(a, y1, b, y1, color);
        return;
    }

    uint16_t dx11 = x2 - x1, dy11 = y2 - y1, dx12 = x3 - x1, dy12 = y3 - y1, dx22 = x3 - x2, dy22 = y3 - y2, sa = 0, sb = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y2=y3 (flat-bottomed triangle), the scanline y2
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y2 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y1=y2
    // (flat-topped triangle).
    if (y2 == y3) last = y2;   // Include y2 scanline
    else
        last = y2 - 1; // Skip it

    for (y = y1; y <= last; y++) {
        a = x1 + sa / dy11;
        b = x1 + sb / dy12;
        sa += dx11;
        sb += dx12;
        /* longhand:
         a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
         b = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
         */
        if (a > b) _swap(a, b);
        drawLine(a, y, b, y, color);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y2=y3.
    sa = dx22 * (y - y2);
    sb = dx12 * (y - y1);
    for (; y <= y3; y++) {
        a = x2 + sa / dy22;
        b = x1 + sb / dy12;
        sa += dx22;
        sb += dx12;
        /* longhand:
         a = x2 + (x3 - x2) * (y - y2) / (y3 - y2);
         b = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
         */
        if (a > b) _swap(a, b);
        drawLine(a, y, b, y, color);
    }
}

void TFT_22_ILI9225::setBackgroundColor(uint16_t color)
{
    _bgColor = color;
}

uint8_t TFT_22_ILI9225::fontX()
{
    return cfont.width;
}

uint8_t TFT_22_ILI9225::fontY()
{
    return cfont.height;
}
void TFT_22_ILI9225::setFont(uint8_t* font)
{
    cfont.font = font;
    cfont.width = font[0];
    cfont.height = font[1];
    cfont.offset = font[2];
    cfont.numchars = font[3];
    cfont.nbrows = cfont.height / 8;

    if (cfont.height % 8) cfont.nbrows++;  // Set number of bytes used by height of font in multiples of 8
}

uint16_t TFT_22_ILI9225::drawText(uint16_t x, uint16_t y, str s, uint16_t color)
{

    uint16_t currx = x;

    // Print every character in string
    for (uint8_t k = 0; k < s.getLen(); k++) {
        currx += drawChar(currx, y, s[k], color) + 1;
    }
    return currx;
}

uint16_t TFT_22_ILI9225::drawTextINV(uint16_t x, uint16_t y, str s, uint16_t color)
{

    uint16_t currx = x;

    // Print every character in string
    for (uint8_t k = 0; k < s.getLen(); k++) {
        currx += drawCharINV(maxX() - currx, maxY() - y, s[k], color) + 1;
    }
    return currx;
}

uint16_t TFT_22_ILI9225::drawChar(uint16_t x, uint16_t y, uint16_t ch, uint16_t color)
{

    uint8_t charData, charWidth;
    uint8_t h, i, j, k;
    uint16_t charOffset;

    charOffset = (cfont.width * cfont.nbrows) + 1;  // bytes used by each character
    charOffset = (charOffset * (ch - cfont.offset)) + FONT_HEADER_SIZE;  // char offset (add 4 for font header)
    charWidth = readFontByte(charOffset);  // get font width from 1st byte
    charOffset++;  // increment pointer to first character data byte

    for (i = 0; i <= charWidth; i++) {  // each font "column" (+1 blank column for spacing)
        h = 0;  // keep track of char height
        for (j = 0; j < cfont.nbrows; j++) {  // each column byte
            if (i == charWidth) charData = (uint8_t) 0x0; // Insert blank column
            else
                charData = cfont.font[charOffset];
            charOffset++;

            // Process every row in font character
            for (k = 0; k < 8; k++) {
                if (h >= cfont.height) break;  // No need to process excess bits
                if ((charData >> k) & 1) drawPixel(x + i, y + (j * 8) + k, color);
                else
                    drawPixel(x + i, y + (j * 8) + k, _bgColor);
                h++;
            };
        };
    };
    return charWidth;
}

uint16_t TFT_22_ILI9225::drawCharINV(uint16_t x, uint16_t y, uint16_t ch, uint16_t color)
{

    uint8_t charData, charWidth;
    uint8_t h, i, j, k;
    uint16_t charOffset;

    charOffset = (cfont.width * cfont.nbrows) + 1;  // bytes used by each character
    charOffset = (charOffset * (ch - cfont.offset)) + FONT_HEADER_SIZE;  // char offset (add 4 for font header)
    charWidth = readFontByte(charOffset);  // get font width from 1st byte
    //charOffset++;  // increment pointer to first character data byte

    charOffset += 6;
    for (i = 0; i <= charWidth; i++) {  // each font "column" (+1 blank column for spacing)
        h = 0;  // keep track of char height
        for (j = 0; j < cfont.nbrows; j++) {  // each column byte
            if (i == charWidth) charData = (uint8_t) 0x0; // Insert blank column
            else
                charData = cfont.font[charOffset];
            charOffset--;

            // Process every row in font character
            for (k = 0; k < 8; k++) {
                if (h >= cfont.height) break;  // No need to process excess bits
                if ((charData << k) & 0x80) drawPixel((x + i), (y + (j * 8) + k), color);
                else
                    drawPixel((x + i), (y + (j * 8) + k), _bgColor);
                h++;
            };
        };
    };
    return charWidth;
}

//#define angle = 45*3.14159/180
#define cos_ = 0.70710725027
#define sin_ = 0.70710725027

void TFT_22_ILI9225::screenSS(uint32_t miliseconds)
{
    int x, y, x1, y1;
    int counter = 0;
    if (miliseconds > 100) {
        miliseconds = 100;
    }
    else if (miliseconds < 50) {
        miliseconds = 50;
    }

    while (miliseconds--) {
        x = rand() % LCD_WIDTH;
        y = rand() % LCD_HEIGHT;
        x1 = rand() % LCD_WIDTH;
        y1 = rand() % LCD_HEIGHT;
        drawRectangle(x, y, x1, y1, (rand() % 16777215));
        counter++;
    }
    fillRectangle(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1, (rand() % 16777215));
}

void TFT_22_ILI9225::startSS1(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color)
{

    int X0[10], Y0[10], X1[10], Y1[10], X2[10], Y2[10], X3[10], Y3[10];
    X0[0] = x;
    Y0[0] = y;
    X1[0] = x1;
    Y1[0] = y1;
    X2[0] = x2;
    Y2[0] = y2;
    X3[0] = x3;
    Y3[0] = y3;
    drawLine(x, y, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x3, y3, color);
    drawLine(x3, y3, x, y, color);

    for (int i = 1; i < 10; i++) {
        X0[i] = X0[i - 1] - 0.2 * (X0[i - 1] - X1[i - 1]);
        Y0[i] = Y0[i - 1] + 0.2 * (Y1[i - 1] - Y0[i - 1]);

        X1[i] = X1[i - 1] - 0.2 * (X1[i - 1] - X2[i - 1]);
        Y1[i] = Y1[i - 1] - 0.2 * (Y1[i - 1] - Y2[i - 1]);

        X2[i] = X2[i - 1] + 0.2 * (X3[i - 1] - X2[i - 1]);
        Y2[i] = Y2[i - 1] - 0.2 * (Y2[i - 1] - Y3[i - 1]);

        X3[i] = X3[i - 1] + 0.2 * (X0[i - 1] - X3[i - 1]);
        Y3[i] = Y3[i - 1] + 0.2 * (Y0[i - 1] - Y3[i - 1]);

        drawLine(X0[i], Y0[i], X1[i], Y1[i], color - i);
        drawLine(X1[i], Y1[i], X2[i], Y2[i], color - i);
        drawLine(X2[i], Y2[i], X3[i], Y3[i], color - i);
        drawLine(X3[i], Y3[i], X0[i], Y0[i], color - i);
        vTaskDelay(100);
    }

    vTaskDelay(100);

}

void TFT_22_ILI9225::draw_tree_loops(int xb, int yb, uint32_t color)
{

    float lamda = 0.8;
    int x_base = xb;
    int y_base = yb;
    int x[10][100], y[10][100];

    //trunk(level 0)
    x[0][0] = x_base;
    y[0][0] = LCD_HEIGHT / 2;
    drawLine(x[0][0], y[0][0], x_base, y_base, color);

    //level 1 mid
    x[1][1] = x_base;
    y[1][1] = y[0][0] + (lamda * (y[0][0] - y_base));
    vTaskDelay(100);
    drawLine(x[0][0], y[0][0], x[1][1], y[1][1], color);

    //level 1 left
    x[1][0] = x[1][1] * 0.87 + y[1][1] * 0.5 - x[0][0] * 0.87 - y[0][0] * 0.5 + x[0][0]; //left
    y[1][0] = -(x[1][1] * 0.5) + y[1][1] * 0.87 + (x[0][0] * 0.5) - y[0][0] * 0.87 + y[0][0]; //left
    vTaskDelay(100);
    drawLine(x[0][0], y[0][0], x[1][0], y[1][0], color);

    //level 1 right
    x[1][2] = x[1][1] * 0.87 - y[1][1] * 0.5 - x[0][0] * 0.87 + y[0][0] * 0.5 + x[0][0]; //right
    y[1][2] = (x[1][1] * 0.5) + y[1][1] * 0.87 - (x[0][0] * 0.5) - y[0][0] * 0.87 + y[0][0]; // right
    vTaskDelay(100);
    drawLine(x[0][0], y[0][0], x[1][2], y[1][2], color);

//    //level 2 mid(hardcoded)
//    x[2][1] = x[1][0] + (lamda * (x[1][0] - x[0][0]));
//    y[2][1] = y[1][0] + (lamda * (y[1][0] - y[0][0]));
//    vTaskDelay(100);
//    drawLine( x[2][1],y[2][1],x[1][0],y[1][0],color);

    //subsequent levels
    for (int i = 2; i < 4; i++) {
        for (int j = 0; j < pow(3, i - 1); j++) {
            //mid lines
            x[i][3 * j + 1] = x[i - 1][j] + (lamda * (x[i - 1][j] - x[i - 2][j % 3]));
            y[i][3 * j + 1] = y[i - 1][j] + (lamda * (y[i - 1][j] - y[i - 2][j % 3]));
            vTaskDelay(100);
            drawLine(x[i][3 * j + 1], y[i][3 * j + 1], x[i - 1][j], y[i - 1][j], color);

            //left lines
            x[i][3 * j] = 0.87 * x[i][3 * j + 1] + 0.5 * y[i][3 * j + 1] - 0.87 * x[i - 1][(j % 3)] - 0.5 * y[i - 1][(j % 3)] + x[i - 1][(j % 3)];
            y[i][3 * j] = -0.5 * x[i][3 * j + 1] + 0.87 * y[i][3 * j + 1] + 0.5 * x[i - 1][(j % 3)] - 0.87 * y[i - 1][(j % 3)] + y[i - 1][(j % 3)];
            vTaskDelay(100);
            drawLine(x[i][3 * j], y[i][3 * j], x[i - 1][(j % 3)], y[i - 1][(j % 3)], color - i);

            //right lines
            x[i][3 * j + 2] = 0.87 * x[i][3 * j + 1] + 0.5 * y[i][3 * j + 1] - 0.87 * x[i - 1][(j % 3)] - 0.5 * y[i - 1][(j % 3)] + x[i - 1][(j % 3)];
            y[i][3 * j + 2] = (0.5 * x[i][3 * j + 1]) + 0.87 * y[i][3 * j + 1] - 0.5 * x[i - 1][(j % 3)] - 0.87 * y[i - 1][(j % 3)] + y[i - 1][(j % 3)];
            vTaskDelay(100);
            drawLine(x[i][3 * j + 2], y[i][3 * j + 2], x[i - 1][(j % 3)], y[i - 1][(j % 3)], color - i);
        }

    }
    vTaskDelay(1000);

}

void TFT_22_ILI9225::rotate_point(int *x2, int *y2, int x1, int y1, int angle)
{
    int xt, yt, xr, yr;
    float c, s, ra;
    ra = angle * (3.14159265359 / 180);
    xt = *x2 - x1;
    yt = *y2 - y1;
    c = cos(ra);
    s = sin(ra);
    xr = xt * c - yt * s;
    yr = xt * s + yt * c;
    *x2 = xr + x1;
    *y2 = yr + y1;
    return;
}

void TFT_22_ILI9225::draw_tree(int x1, int y1, int x2, int y2, uint16_t color, int num)
{
    int x3, y3, count;
    if (num <= 0) return;
    if (x2 > LCD_WIDTH || y2 > LCD_HEIGHT || x2 < 0 || y2 < 0) return;
    x3 = x1, y3 = y1;
    x1 = x2, y1 = y2;
    y2 = y2 + (y1 - y3) * .8;
    x2 = x2 + (x1 - x3) * .8;
    if (x2 > LCD_WIDTH || y2 > LCD_HEIGHT || x2 < 0 || y2 < 0) return;

    if ((10 - num) < 3) color = COLOR_BROWN;
    else
        color = COLOR_DARKGREEN;
    rotate_point(&x2, &y2, x1, y1, 330);
    count = 0;
    //for (count = num * -1; count < num; count++) {

    drawLine(x1 + count, y1 + count, x2 + count, y2 + count, color);
    //}
    draw_tree(x1, y1, x2, y2, COLOR_YELLOW, num - 1);
    rotate_point(&x2, &y2, x1, y1, 30);
    //for (count = num * -1; count < num; count++) {
    drawLine(x1 + count, y1 + count, x2 + count, y2 + count, color);
    //}
    draw_tree(x1, y1, x2, y2, COLOR_SKYBLUE, num - 1);
    rotate_point(&x2, &y2, x1, y1, 30);
    //for (count = num * -1; count < num; count++) {
    drawLine(x1 + count, y1 + count, x2 + count, y2 + count, color);
    //}
    draw_tree(x1, y1, x2, y2, COLOR_CYAN, num - 1);
}

void TFT_22_ILI9225::draw_polygon_fill(uint16_t px[], uint16_t py[], uint16_t px_old[], uint16_t py_old[], uint8_t n, uint16_t color)
{

    int i, j;
//    glClear (GL_COLOR_BUFFER_BIT);  // Clear display window.
//    glColor3f(1.0, 0.0, 0.0);
//    scrPt p1;
    uint16_t p1X, p1Y;
    //First of all:determine the rectangle that polygon inserted in it:
    //purpose: avoiding calculate additional points that we sure dont lie in polygon area.
    uint16_t minx = ILI9225_LCD_WIDTH + 1;
    for (i = 0; i < n; i++)
        if (px[i] < minx) minx = px[i];
    uint16_t miny = ILI9225_LCD_HEIGHT + 1;
    for (i = 0; i < n; i++)
        if (py[i] < miny) miny = py[i];
    uint16_t maxx = -1;
    for (i = 0; i < n; i++)
        if (px[i] > maxx) maxx = px[i];
    uint16_t maxy = -1;
    for (i = 0; i < n; i++)
        if (py[i] > maxy) maxy = py[i];

    uint16_t minx_old = ILI9225_LCD_WIDTH + 1;
    for (i = 0; i < n; i++)
        if (px_old[i] < minx_old) minx_old = px_old[i];
    uint16_t miny_old = ILI9225_LCD_HEIGHT + 1;
    for (i = 0; i < n; i++)
        if (py_old[i] < miny_old) miny_old = py_old[i];
    uint16_t maxx_old = -1;
    for (i = 0; i < n; i++)
        if (px_old[i] > maxx_old) maxx_old = px_old[i];
    uint16_t maxy_old = -1;
    for (i = 0; i < n; i++)
        if (py_old[i] > maxy_old) maxy_old = py_old[i];

    u0_dbg_printf("minx = %f, miny = %f\nminx_old = %f, miny_old = %f\n", minx, miny, minx_old, miny_old);
    uint16_t MINx = (minx_old <= minx ? minx_old : minx);
    uint16_t MINy = (miny_old <= miny ? miny_old : miny);

    uint16_t MAXx = (maxx_old >= maxx ? maxx_old : maxx);
    uint16_t MAXy = (maxy_old >= maxy ? maxy_old : maxy);

    u0_dbg_printf("%d", MAXy);
    uint16_t *blackX = new uint16_t[(MAXx - MINx + 1) * (MAXy - MINy + 1)], *blackY = new uint16_t[(MAXx - MINx + 1) * (MAXy - MINy + 1)];
    uint16_t *colrX = new uint16_t[(MAXx - MINx + 1) * (MAXy - MINy + 1)], *colrY = new uint16_t[(MAXx - MINx + 1) * (MAXy - MINy + 1)];
    uint8_t bl_num = 0, cl_num = 0;
    //Second: check pixels of polygon rectangle
    for (i = MINx; i < MAXx; i++) {
        for (j = MINy; j < MAXy; j++) {
            p1X = i;
            p1Y = j;

            bool inside_new = true; //floodFill4(px, py, n, p1X, p1Y);
            bool inside_old = true; //floodFill4(px_old, py_old, n, p1X, p1Y);

            if (inside_new && inside_old) {
                //dont do anything
                colrX[cl_num++] = p1X;
                colrY[cl_num++] = p1Y;
//                u0_dbg_printf("C%d, %d\n", p1X, p1Y);

            }
            else if (!inside_new && inside_old) {
                blackX[bl_num++] = p1X;
                blackY[bl_num++] = p1Y;
//                u0_dbg_printf("B%d, %d\n", p1X, p1Y);
            }
            else if (inside_new && !inside_old) {
                colrX[cl_num++] = p1X;
                colrY[cl_num++] = p1Y;
//                u0_dbg_printf("C%d, %d\n", p1X, p1Y);
            }

        }
    }

    u0_dbg_printf("C");
    //clear buffer on display
    if (bl_num != 0) {
        for (j = 0; j < bl_num; j++)
            drawPixel(blackX[j], blackY[j], COLOR_BLACK);
    }
    u0_dbg_printf("D");
    //draw buffer on display
    if (cl_num != 0) {
        for (j = 0; j < bl_num; j++)
            drawPixel(colrX[j], colrX[j], color);
    }
}

bool TFT_22_ILI9225::max(int x, int y)
{
    return (x > y ? x : y);
}

bool TFT_22_ILI9225::min(int x, int y)
{
    return (x < y ? x : y);
}
int TFT_22_ILI9225::Is_Left(const Point3D &p0, const Point3D &p1, const Point3D &point)
{
    return ((p1.x - p0.x) * (point.y - p0.y) - (point.x - p0.x) * (p1.y - p0.y));
}

bool TFT_22_ILI9225::Is_Inside(const Point3D &point, const VECTOR<Point3D> &points_list)
{
    // The winding number counter.
    int winding_number = 0;

    // Loop through all edges of the polygon.
    //typedef VECTOR<Point>::size_type size_type;

    int size = points_list.size();

    for (int i = 0; i < size; ++i)             // Edge from point1 to points_list[i+1]
            {
        Point3D point1(points_list[i]);
        Point3D point2;

        // Wrap?
        if (i == (size - 1)) {
            point2 = points_list[0];
        }
        else {
            point2 = points_list[i + 1];
        }

        if (point1.y <= point.y)                                    // start y <= point.y
                {
            if (point2.y > point.y)                                 // An upward crossing
                    {
                if (Is_Left(point1, point2, point) > 0)             // Point left of edge
                        {
                    ++winding_number;                               // Have a valid up intersect
                }
            }
        }
        else {
            // start y > point.y (no test needed)
            if (point2.y <= point.y)                                // A downward crossing
                    {
                if (Is_Left(point1, point2, point) < 0)             // Point right of edge
                        {
                    --winding_number;                               // Have a valid down intersect
                }
            }
        }
    }

    return (winding_number != 0);
}

float TFT_22_ILI9225::dist2D(Point3D& P1,Point3D& P2)
{
   return sqrt(((P2.x-P1.x)*(P2.x-P1.x)) + ((P2.y-P1.y)*(P2.y-P1.y)));
}

float TFT_22_ILI9225::dist3D(Point3D& P1,Point3D& P2)
{
    return sqrt(((P2.x-P1.x)*(P2.x-P1.x)) + ((P2.y-P1.y)*(P2.y-P1.y)) + ((P2.z-P1.z)*(P2.z-P1.z)));
}

void TFT_22_ILI9225::poly_fill_diffuse(const VECTOR<Point3D> &V, uint32_t color,Point3D& Pz_min, Point3D& Pz_max)
{
    int n = V.size();

      int minx = ILI9225_LCD_WIDTH + 1;
      int maxy = 0;
      int maxx = -1;
      int miny = -(ILI9225_LCD_HEIGHT + 1);
      int minz_x,minz_y, maxz_x, maxz_y;

      for (int i = 0; i < n; i++) {
          Point3D P = V[i];
          if (P.x < minx) minx = P.x;
          if (P.y < maxy) maxy = P.y;
          if (P.x > maxx) maxx = P.x;
          if (P.y > miny) miny = P.y;
      }

      //for debug
  //    drawRectangle(minx,abs(miny),maxx,abs(maxy),COLOR_BLUE);


      for (int j = miny; j > (maxy); j--) {
          for (int i = minx; i < (maxx); i++) {
              if (Is_Inside( { i, j }, V)) {
                  Point3D P = {i,j,0};
                  float di = dist2D(P,Pz_max);
//                  if(di>15)
//                      drawPixel(i, abs(j), color);
//                  else
                      drawPixel(i, abs(j), color-di);
              }
          }
      }


}

void TFT_22_ILI9225::poly_fill(const VECTOR<Point3D> &V, uint32_t color)
{

    int n = V.size();

    int minx = ILI9225_LCD_WIDTH + 1;
    int maxy = 0;
    int maxx = -1;
    int miny = -(ILI9225_LCD_HEIGHT + 1);
//    int minz = 200;
//    int maxz = -200;
    int minz_x,minz_y, maxz_x, maxz_y;

    for (int i = 0; i < n; i++) {
        Point3D P = V[i];
        if (P.x < minx) minx = P.x;
        if (P.y < maxy) maxy = P.y;
        if (P.x > maxx) maxx = P.x;
        if (P.y > miny) miny = P.y;
//        if (P.z <= minz) {
//            minz = P.z;
//            minz_x = P.x;
//            minz_y = P.y;
//        }
//        if (P.z >= maxz) {
//            maxz = P.z;
//            maxz_x = P.x;
//            maxz_y = P.y;
//        }
    }
    //for debug
//    drawRectangle(minx,abs(miny),maxx,abs(maxy),COLOR_BLUE);


    for (int j = miny; j > (maxy); j--) {
        for (int i = minx; i < (maxx); i++) {
            if (Is_Inside( { i, j }, V)) {
                Point3D P = {i,j,0};
                drawPixel(i, abs(j), color);
            }
        }
    }

}

void TFT_22_ILI9225::poly_fill3D(const VECTOR<Point3D> &V, uint32_t color)
{

    int n = V.size();

    int minx = ILI9225_LCD_WIDTH + 1;
    for (int i = 0; i < n; i++) {
        Point3D P = V[i];
        if (P.x < minx) minx = P.x;
    }

    int miny = (ILI9225_LCD_HEIGHT + 1);
    for (int i = 0; i < n; i++) {
        Point3D P = V[i];
        if (-P.y < miny) miny = -P.y;
    }

    int minz = 30;
    for (int i = 0; i < n; i++) {
        Point3D P = V[i];
        if (P.z < minz) minz = P.z;
    }

    int maxx = -1;
    for (int i = 0; i < n; i++) {
        Point3D P = V[i];
        if (P.x > maxx) maxx = P.x;
    }

    int maxy = -1;
    for (int i = 0; i < n; i++) {
        Point3D P = V[i];
        if (-P.y > maxy) maxy = -P.y;
    }

    int maxz = -30;
    for (int i = 0; i < n; i++) {
        Point3D P = V[i];
        if (P.z > maxz) maxz = P.z;
    }

    u0_dbg_printf("%d->%d\n", minz, maxz);
    for (int k = minz; k < maxz; k++) {
        for (int j = miny; j < maxy; j++) {
            for (int i = minx; i < maxx; i++) {
                if (Is_Inside( { i, -j }, V)) {
                    if (color == COLOR_BLACK) drawPixel(i, j, COLOR_BLACK);
                    else
                        drawPixel(i, j, color + k);
                }
            }
        }
    }
}
