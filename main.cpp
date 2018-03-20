//Eclipse development Project used are available here: https://sourceforge.net/projects/armdevpkg/files/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tasks.hpp"
#include "utilities.h"
#include "io.hpp"

//created header files
#include "TFT/TFT_22_ILI9225.h"

class LCD: public scheduler_task {
private:
    TFT_22_ILI9225 lcd;
    bool flag = true;
    int x, y, x1, y1, x2, y2, x3, y3;
    uint8_t fnt_sz = 8;
    int side;
    int j;
    int tm;
public:
    LCD(uint8_t priority) :
            scheduler_task("LCD_interface", 1024 * 2, priority)
    {
    }

    bool init()
    {
        GPIO TFT_RST { 2, 1 }, TFT_RS { 2, 2 }, TFT_CS { 2, 0 }, TFT_LED { 2, 3 };
        lcd.setPINS(TFT_RST, TFT_RS, TFT_CS, TFT_LED);
        return true;
    }

    bool run(void *p)
    {
        if (flag) {
            lcd.begin();
            lcd.setFont((uint8_t *) Terminal6x8);
            flag = false;
            tm = 3;
        }

#if 0
        //-----natural scene of beach-----//
        lcd.drawLine(88,25,92,27,COLOR_BLACK);
        lcd.drawLine(92,27,90,29,COLOR_BLACK);
        lcd.drawLine(89,25,93,27,COLOR_BLACK);
        lcd.drawLine(93,27,91,29,COLOR_BLACK);
        //lcd.drawTriangle(0,0,50,100,150,100,COLOR_WHITE);
        lcd.fillCircle(25,25,15,COLOR_GOLD);
        lcd.fillRectangle(140,219,175,0,COLOR_SKYBLUE);
#endif

#if 0
        //----- ScreenSaver-1 -----//
        lcd.screenSS(0);
#endif

#if 1
        lcd.fillRectangle(0, 0, LCD_WIDTH, LCD_HEIGHT, COLOR_BLACK);
        lcd.drawTextINV(lcd.maxX() / 2 - (fnt_sz * 9), 20, "CMPE 240 Lab Assignment 2", COLOR_BLUE);
        //Squares
        for (int k = 0; k < 5; k++) {        //working code for squares
            x = rand() % LCD_WIDTH;
            y = rand() % LCD_HEIGHT;
            side = rand() % LCD_WIDTH;
            uint32_t color = rand() % 0xFFFFFFu;
            // x = 15;
            // y = 15;
            // side = LCD_WIDTH-50;
            x1 = x + side;
            y1 = y;
            x2 = x1;
            y2 = y + side;
            x3 = x;
            y3 = y2;

            //lcd.drawRectangle(x,y,x1,y1,(rand() % 16777215));
            if (x < LCD_WIDTH && x1 < LCD_WIDTH && x2 < LCD_WIDTH && x3 < LCD_WIDTH && y < LCD_HEIGHT && y1 < LCD_HEIGHT && y2 < LCD_HEIGHT && y3 < LCD_HEIGHT) {
                lcd.startSS1(x, y, x1, y1, x2, y2, x3, y3, color);
            }
            else
                k--;
        }
        vTaskDelay(2000);

#endif

#if 1
        lcd.fillRectangle(0, 0, LCD_WIDTH, LCD_HEIGHT, COLOR_BLACK);
        lcd.fillRectangle(0, 0, LCD_WIDTH, 60, COLOR_DARKGRAY);
        lcd.fillRectangle(0, 61, LCD_WIDTH, LCD_HEIGHT - 50, COLOR_SKYBLUE);
        //lcd.drawCircle(20,LCD_HEIGHT - 70,10,COLOR_GOLD);
        lcd.fillCircle(20, LCD_HEIGHT - 70, 10, COLOR_GOLD);

        //trees
        //x1 = x1 + 100;
        //y1 = 0;
        //y2 = 100;
        //x2 = x1;
        x1 = LCD_WIDTH / 2;
        y1 = 0;
        y2 = 40;
        x2 = x1;
        int levels = 10;
        int tree_cnt = 2;
        lcd.drawTextINV(lcd.maxX() / 2 - (fnt_sz * 9), 20, "CMPE 240 Lab Assignment 2", COLOR_BLUE);
        //uint32_t color = rand() % 0xFFFFFFu;
        if (x1 < LCD_WIDTH && y1 < LCD_HEIGHT / 2 && x1 >= 0 && y1 >= 0) {
            lcd.drawLine(x1, y1, x2, y2, COLOR_BROWN);
            lcd.draw_tree(x1, y1, x2, y2, COLOR_BROWN, levels);
        }
        for (int k = 0; k < tree_cnt; k++) {
            x1 = rand() % (LCD_WIDTH - 30 + 1 - 50) + 50;
            y1 = 0;
            y2 = rand() % (40 + 1 - 20) + 20;
            x2 = x1;

            if (x1 < LCD_WIDTH && y2 < LCD_HEIGHT / 2 && x1 >= 0 && y2 >= 0) {
                lcd.drawLine(x1, y1, x2, y2, COLOR_BROWN);
                lcd.draw_tree(x1, y1, x2, y2, COLOR_BROWN, levels);
            }
        }

        lcd.drawTextINV(lcd.maxX() / 2 - (fnt_sz * 2), 50, "Done!", COLOR_BLUE);
        while (tm > 0) {
            lcd.drawCharINV(lcd.maxX() - (lcd.maxX() / 2 + (fnt_sz * 3)), lcd.maxY() - 50, tm + '0', COLOR_BLUE);
            vTaskDelay(1000);
            tm--;
        }
        tm = 3;
        vTaskDelay(1000);
        lcd.clear();

//        int y1=0,y2;
//        int x2;
//        int x1=x2=rand()%300 + 100;
//        y1=0,y2=30;
//        for(int i=-7;i<=7;i++){
//        lcd.drawLine(x1+i,y1,x2+i,y2,COLOR_YELLOW);
//        }
//        lcd.draw_tree_recursive(x1,y1,x2,y2,COLOR_CYAN,7);
//        x1 = x2 = rand()%300 +100;
//        y1=0,y2=60;
//        for(int i=-7;i<=7;i++){
//            lcd.drawLine(x1+i,y1,x2+i,y2,COLOR_CYAN);
//        }
//        lcd.draw_tree_recursive(x1,y1,x2,y2,COLOR_CYAN,7);
#endif

#if 0
        //working code for squares
        x = rand()% LCD_WIDTH;
        y = rand()% LCD_HEIGHT;
        side = rand()% LCD_WIDTH;
        uint32_t color = rand()%0xFFFFFFu;
        // x = 15;
        // y = 15;
        // side = LCD_WIDTH-50;
        x1 = x+side;
        y1 = y;
        x2 = x1;
        y2 = y+side;
        x3 = x;
        y3 = y2;
        j=0;
        //lcd.drawRectangle(x,y,x1,y1,(rand() % 16777215));
        if (x < LCD_WIDTH && x1 < LCD_WIDTH && x2 < LCD_WIDTH && x3 < LCD_WIDTH &&
                y < LCD_HEIGHT && y1 < LCD_HEIGHT && y2 < LCD_HEIGHT && y3 < LCD_HEIGHT)
        {
            lcd.startSS1(x,y,x1,y1,x2,y2,x3,y3,color + j);
            j+=20;
        }
#endif

        if (SW.getSwitch(1))
        {
            lcd.clear();
        }

        return true;
    }
}
;

int main(void)
{
//scheduler_add_task(new terminalTask(PRIORITY_HIGH));

    scheduler_add_task(new LCD(PRIORITY_HIGH));

// Alias to vScheduleRSTart();
    scheduler_start();
    return -1;
}
