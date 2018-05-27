//Eclipse development Project used are available here: https://sourceforge.net/projects/armdevpkg/files/

#include <stdio.h>
#include <stdlib.h>
#include "tasks.hpp"
#include "utilities.h"
#include "io.hpp"
#include "ssp1.h"
#include "ssp0.h"
#include "LabGPIOInterrupts.h"
#include "printf_lib.h"
#include "vector.hpp"

//created header files
#include "TFT/TFT_22_ILI9225.h"

#define magic 176/2
#define OFFSETX 176/2                  // offsets
#define OFFSETY 176/2
#define OFFSETZ 30

#define cube_width 5

//select 3D diagram(select ONLY ONE)
#define draw_cube 1
#define draw_arrow 0

#define draw_axis 0

float get_sine(float angle)
{
    float res = 0;
    switch (int(angle)) {
        case 0:
        case -1:
            res = 0;
            break;

        case 10:
        case 9:
            res = 0.173561;
            break;

        case 20:
        case 19:
            res = 0.341854;
            break;

        case 30:
        case 29:
            res = 0.49977;
            break;

        case 40:
        case 39:
            res = 0.642516;
            break;

        case 50:
        case 49:
            res = 0.76576;
            break;

        case 60:
        case 59:
            res = 0.86576;
            break;

        case 70:
        case 69:
            res = 0.939481;
            break;

        case 80:
        case 79:
            res = 0.984685;
            break;

        case 90:
        case 89:
            res = 1;
            break;

        case 100:
        case 99:
            res = 0.984961;
            break;

        case 110:
        case 109:
            res = 0.940025;
            break;

        case 120:
        case 119:
            res = 0.866556;
            break;

        case 130:
        case 129:
            res = 0.766783;
            break;

        case 140:
        case 139:
            res = 0.643736;
            break;

        case 150:
        case 149:
            res = 0.501149;
            break;

        case 160:
        case 159:
            res = 0.34335;
            break;

        case 170:
        case 169:
            res = 0.175129;
            break;

        case 180:
        case 179:
            res = 0.00159265;
            break;

        case 190:
        case 189:
            res = -0.171992;
            break;

        case 200:
        case 199:
            res = -0.340357;
            break;

        case 210:
        case 209:
            res = -0.49839;
            break;

        case 220:
        case 219:
            res = -0.641295;
            break;

        case 230:
        case 229:
            res = -0.764735;
            break;

        case 240:
        case 239:
            res = -0.864962;
            break;

        case 250:
        case 249:
            res = -0.938934;
            break;

        case 260:
        case 259:
            res = -0.984406;
            break;

        case 270:
        case 269:
            res = -0.999997;
            break;

        case 280:
        case 279:
            res = -0.985235;
            break;

        case 290:
        case 289:
            res = -0.940567;
            break;

        case 300:
        case 299:
            res = -0.86735;
            break;

        case 310:
        case 309:
            res = -0.767805;
            break;

        case 320:
        case 319:
            res = -0.644954;
            break;

        case 330:
        case 329:
            res = -0.502527;
            break;

        case 340:
        case 339:
            res = -0.344846;
            break;

        case 350:
        case 349:
            res = -0.176697;
            break;

        case 360:
        case 359:
            res = -0.0031853;
            break;

        default:
            res = 0;
            break;
    }
    return res;

}

float get_cosine(float angle)
{
    float res = 0;
    switch (int(angle)) {
        case 0:
        case -1:
            res = 1;
            break;

        case 10:
        case 9:
            res = 0.984823;
            break;

        case 20:
        case 19:
            res = 0.939753;
            break;

        case 30:
        case 29:
            res = 0.866158;
            break;

        case 40:
        case 39:
            res = 0.766272;
            break;

        case 50:
        case 49:
            res = 0.643126;
            break;

        case 60:
        case 59:
            res = 0.50046;
            break;

        case 70:
        case 69:
            res = 0.342602;
            break;

        case 80:
        case 79:
            res = 0.174345;
            break;

        case 90:
        case 89:
            res = 0.000796327;
            break;

        case 100:
        case 99:
            res = -0.172777;
            break;

        case 110:
        case 109:
            res = -0.341105;
            break;

        case 120:
        case 119:
            res = -0.49908;
            break;

        case 130:
        case 129:
            res = -0.641906;
            break;

        case 140:
        case 139:
            res = -0.765248;
            break;

        case 150:
        case 149:
            res = -0.865361;
            break;

        case 160:
        case 159:
            res = -0.939207;
            break;

        case 170:
        case 169:
            res = -0.984545;
            break;

        case 180:
        case 179:
            res = -0.999999;
            break;

        case 190:
        case 189:
            res = -0.985098;
            break;

        case 200:
        case 199:
            res = -0.940296;
            break;

        case 210:
        case 209:
            res = -0.866953;
            break;

        case 220:
        case 219:
            res = -0.767294;
            break;

        case 230:
        case 229:
            res = -0.644345;
            break;

        case 240:
        case 239:
            res = -0.501838;
            break;

        case 250:
        case 249:
            res = -0.344098;
            break;

        case 260:
        case 259:
            res = -0.175913;
            break;

        case 270:
        case 269:
            res = -0.00238898;
            break;

        case 280:
        case 279:
            res = 0.171208;
            break;

        case 290:
        case 289:
            res = 0.339608;
            break;

        case 300:
        case 299:
            res = 0.497699;
            break;

        case 310:
        case 309:
            res = 0.640684;
            break;

        case 320:
        case 319:
            res = 0.764221;
            break;

        case 330:
        case 329:
            res = 0.864562;
            break;

        case 340:
        case 339:
            res = 0.938659;
            break;

        case 350:
        case 349:
            res = 0.984265;
            break;

        case 360:
        case 359:
            res = 0.999995;
            break;

        default:
            res = 0;
            break;
    }
    return res;
}


class fill_poly_ARROW: public scheduler_task {
private:
    TFT_22_ILI9225 lcd;

    VECTOR<Point3D> V, V_new;

    int num_vertices = 8;

    int num_edges = 21;

    int wires[21][2] = { //BORDER LINES
                       { 1, 8 },
                       { 2, 9 },
                       { 3,10 },
                       { 4,11 },
                       { 5,12 },
                       { 6,13 },
                       { 7,14 },
                       { 1, 2 },
                       { 2, 3 },
                       { 3, 4 },
                       { 4, 5 },
                       { 5, 6 },
                       { 6, 7 },
                       { 7, 1 },
                       { 8, 9 },
                       { 9,10 },
                       {10,11 },
                       {11,12 },
                       {12,13 },
                       {13,14 },
                       {14, 8 }
               };

    int num_surfaces = 9;
    int surf_size[9] = { 5, 5, 5, 5, 5, 5, 5, 8, 8 };

    int surf[9][8] = { //SURFACES
                    { 1, 8, 9, 2, 1, 0, 0, 0 }, //1
                    { 2, 9,10, 3, 2, 0, 0, 0 }, //2
                    { 3,10,11, 4, 3, 0, 0, 0 }, //3
                    { 5,12,11, 4, 5, 0, 0, 0 }, //4
                    { 6,13,12, 5, 6, 0, 0, 0 }, //5
                    { 7,14,13, 6, 7, 0, 0, 0 }, //6
                    { 1, 8,14, 7, 1, 0, 0, 0 }, //7
                    { 1, 2, 3, 4, 5, 6, 7, 1 }, //8
                    { 8, 9,10,11,12,13,14, 8 }  //9
            };

    VECTOR<int*> surf_sorted;
    int sx, sy, sz, ex, ey, ez;                    // define global vars for calling graphics subroutines

    bool changed; //to keep track of button pressed or not

    uint32_t color_[3] = { COLOR_RED, COLOR_BLUE, COLOR_GREEN }; //color for 3D axis lines

    float rotx = 0;                    // starting amount of x rotation
    float roty = 0;                    // starting amount of y rotation
    float rotz = 0;                    // starting amount of z rotation

    float xpos, ypos, zpos;

    float sinax, cosax, sinay, cosay, sinaz, cosaz;
public:

    fill_poly_ARROW(uint8_t priority) :
            scheduler_task("FILL_POLY", 1024 * 28, priority)
    {
    }

    //to draw the 3D object wireframe
    void draw3D_object(VECTOR<Point3D>& V, uint16_t color)
    {

        for (int i = 0; i < num_edges; i++) {
            sx = V[int(wires[i][0] - 1)].x;
            sy = abs(V[int(wires[i][0] - 1)].y);

            ex = V[int(wires[i][1] - 1)].x;
            ey = abs(V[int(wires[i][1] - 1)].y);

            lcd.drawLine(sx, sy, ex, ey, color);
        }
    }

    int minZ(VECTOR<Point3D>& V)
    {
        int minz = 200;
        for (int i = 0; i < V.size(); i++) {
            if (V[i].z < minz) minz = V[i].z;
        }

        return minz;
    }

    int maxZ(VECTOR<Point3D>& V)
        {
            int maxz = -200;
            for(int i=0;i<V.size();i++)
            {
                if(V[i].z > maxz)
                    maxz = V[i].z;
            }

            return maxz;
        }

        float avg_Z(int* T,int sz,VECTOR<Point3D>& V)
        {
            float c =0;
            for(int i=0;i<sz;i++)
            {
                c+=V[T[i]-1].z;
            }
            return c/sz;
        }

        void sort_surf(VECTOR<Point3D>& V)
        {
//            u0_dbg_printf("1\n");
            if(surf_sorted.size() !=0)
                surf_sorted.clear();

//            u0_dbg_printf("2\n");
            float min_avg = 200;
            //sort surf in ascending order of z
            for(int i=0;i<num_surfaces;i++)
            {
                int *temp = new int[surf_size[i]];
                float sum = 0;
                for(int j=0;j<surf_size[i];j++)
                {
                    sum+= V[surf[i][j]-1].z;
                    temp[j]= (surf[i][j]);
                }
                float avg_z = sum/surf_size[i];
                if(surf_sorted.size()==0 || avg_z>avg_Z(surf_sorted[surf_sorted.size()-1],surf_size[i],V))
                    surf_sorted.push_back(temp);
                else if(avg_z<=avg_Z(surf_sorted[0],surf_size[i],V))
                    surf_sorted.push_front(temp);
                u0_dbg_printf("\n%d  %d\n",surf_sorted.size(),surf_sorted[0][0]);
            }

        }

        //to draw the 3D object SOLID
        void draw3D_object_FILL(VECTOR<Point3D>& V, uint16_t color)
        {
    //      uint16_t colors[6] = {COLOR_BLUEVIOLET, COLOR_BROWN, COLOR_GREENYELLOW, COLOR_ORANGE, COLOR_YELLOWGREEN, COLOR_WHITE};
            VECTOR<Point3D> temp;
            int maxz = maxZ(V);
    //        int minz = minZ(V);
            for (int i = 0; i < num_surfaces; i++) {
                bool fill = true;
                //change the orders of surfaces by controlling i variable
                for (int j = 0; j < surf_size[i]; j++) {
                    if(V[surf[i][j] - 1].z == maxz)
                        {
                            fill = false;
                            break;
                        }
                    temp.push_back(V[surf[i][j] - 1]);
                }

                    if (fill) {
                        lcd.poly_fill(temp, color);
    //                int temp_z = maxZ(temp);
                    for (int i1 = 0; i1 < temp.size()-1; i1++) {
                        sx = temp[i1].x;
                        sy = abs(temp[i1].y);

                        ex = temp[i1+1].x;
                        ey = abs(temp[i1+1].y);

    //                    if((temp[i].z == temp_z || temp[i+1].z == temp_z))
                            lcd.drawLine(sx, sy, ex, ey, COLOR_BLUE);

                    }

                }
                temp.clear();
            }
        }



    bool init()
    {

        GPIO TFT_RST { 2, 1 }, TFT_RS { 2, 2 }, TFT_CS { 2, 0 }, TFT_LED { 2, 3 };
        lcd.setPINS(TFT_RST, TFT_RS, TFT_CS, TFT_LED);

        rotx = roty = rotz = 0;
        xpos = ypos = zpos = 0;
        sinax = get_sine(rotx * 180 / 3.14);
        cosax = get_cosine(rotx * 180 / 3.14);

        sinay = get_sine(roty * 180 / 3.14);
        cosay = get_cosine(roty * 180 / 3.14);

        sinaz = get_sine(rotz * 180 / 3.14);
        cosaz = get_cosine(rotz * 180 / 3.14);
        changed = true;
        return true;
    }

    bool taskEntry()
    {
        //ARROW POINTS
        Point3D P[14] = { //POINTS
                { 0, -20, 0 }, //1
                        { 10, -10, 0 }, //2
                        {  5,-10, 0 }, //3
                        {  5,  0, 0 }, //4
                        { -5,  0, 0 }, //5
                        { -5,-10, 0 }, //6
                        {-10,-10, 0 }, //7
                        {  0,-20, 5 }, //8
                        { 10,-10, 5 }, //9
                        {  5,-10, 5 }, //10
                        {  5,  0, 5 }, //11
                        { -5,  0, 5 }, //12
                        { -5,-10, 5 }, //13
                        {-10,-10, 5 }, //14
                };

        for (int i = 0; i < 14; i++){
            V.push_back(P[i]);
            V_new.push_back(P[i]);
        }

        lcd.setSPIfrequency(16);
        lcd.begin();
        lcd.setFont((uint8_t *) Terminal6x8);
        lcd.clear();
        lcd.drawChar(10, 10, 'X', COLOR_RED);
        lcd.drawChar(10, 20, 'Y', COLOR_BLUE);
        lcd.drawChar(10, 30, 'Z', COLOR_GREEN);
        return true;
    }
    void draw_char_axes_onlcd(float rot, uint16_t color, char c_)
    {

        //now draw them
        int ang = int(rot * (180 / 3.14));
        char _1 = '0' + ang / 100;
        char _2 = '0' + int(ang / 10) % 10;
        char _3 = '0' + (ang % 10);

        switch (c_) {
            case 'x':
                //clear old
                lcd.fillRectangle(10 + 5 + 2, 10, 10 + 15 + 5 + lcd.fontX(), 10 + lcd.fontY(), COLOR_BLACK);

                //draw new
                lcd.drawChar(10 + 5 + 2, 10, _1, color);
                lcd.drawChar(10 + 10 + 4, 10, _2, color);
                lcd.drawChar(10 + 15 + 5, 10, _3, color);
                break;
            case 'y':
                //clear old
                lcd.fillRectangle(10 + 5 + 2, 20, 10 + 15 + 5 + lcd.fontX(), 20 + lcd.fontY(), COLOR_BLACK);

                //draw new
                lcd.drawChar(10 + 5 + 2, 20, _1, color);
                lcd.drawChar(10 + 10 + 4, 20, _2, color);
                lcd.drawChar(10 + 15 + 5, 20, _3, color);
                break;
            case 'z':
                //clear old
                lcd.fillRectangle(10 + 5 + 2, 30, 10 + 15 + 5 + lcd.fontX(), 30 + lcd.fontY(), COLOR_BLACK);

                //draw new
                lcd.drawChar(10 + 5 + 2, 30, _1, color);
                lcd.drawChar(10 + 10 + 4, 30, _2, color);
                lcd.drawChar(10 + 15 + 5, 30, _3, color);
                break;
        }
    }

    bool run(void *p)
    {

        //handling button presses
        if (SW.getSwitch(1))
        {

            rotx = rotx + (10 * (3.14 / 180));                // rotate on X axis
            if((rotx * (180 / 3.14))>=359 )
            rotx =0;
            changed = true;
            sinax = get_sine(rotx * 180 / 3.14);
            cosax = get_cosine(rotx * 180 / 3.14);

            sinay = get_sine(roty * 180 / 3.14);
            cosay = get_cosine(roty * 180 / 3.14);

            sinaz = get_sine(rotz * 180 / 3.14);
            cosaz = get_cosine(rotz * 180 / 3.14);
        }

        if (SW.getSwitch(2))
        {
            roty = roty + (10*(3.14/180));                // rotate on Y axis
            if((roty * (180 / 3.14))>=359 )
            roty =0;
            changed = true;
            sinax = get_sine(rotx * 180 / 3.14);
            cosax = get_cosine(rotx * 180 / 3.14);

            sinay = get_sine(roty * 180 / 3.14);
            cosay = get_cosine(roty * 180 / 3.14);

            sinaz = get_sine(rotz * 180 / 3.14);
            cosaz = get_cosine(rotz * 180 / 3.14);
        }

        if (SW.getSwitch(3))
        {
            rotz = rotz + (10*(3.14/180));                // rotate on Z axis
            if((rotz * (180 / 3.14))>=359 )
            rotz =0;
            changed = true;
            sinax = get_sine(rotx * 180 / 3.14);
            cosax = get_cosine(rotx * 180 / 3.14);

            sinay = get_sine(roty * 180 / 3.14);
            cosay = get_cosine(roty * 180 / 3.14);

            sinaz = get_sine(rotz * 180 / 3.14);
            cosaz = get_cosine(rotz * 180 / 3.14);
        }

        if (SW.getSwitch(4))
        {
            lcd.clear();

            for(int i=0;i<V_new.size();i++)
            V_new[i] = V[i];

            rotx = roty = rotz = 0;
            changed = true;
            sinax = get_sine(rotx * 180 / 3.14);
            cosax = get_cosine(rotx * 180 / 3.14);

            sinay = get_sine(roty * 180 / 3.14);
            cosay = get_cosine(roty * 180 / 3.14);

            sinaz = get_sine(rotz * 180 / 3.14);
            cosaz = get_cosine(rotz * 180 / 3.14);
        }

        if (changed) {

//            sort_surf(V_new);

//            for(int i=0;i<surf_sorted.size();i++)
//            {
//                u0_dbg_printf("Surf %d => {",i);
//                for(int j=0;j<surf_size[i];j++)
//                    u0_dbg_printf("(%d,%d,%d), ",V_new[surf_sorted[i][j]-1].x,V_new[surf_sorted[i][j]-1].y,V_new[surf_sorted[i][j]-1].z);
//                u0_dbg_printf("}\n");
//            }
            draw3D_object_FILL(V_new,COLOR_BLACK);
            draw3D_object(V_new,COLOR_BLACK);

            //draw axis

            //new 3D axis coords
            for (int i = 0; i < V.size(); i++)// translate 3d vertex position to 2d screen position
            {
                xpos = xpos + 0.0;                // move the object
                ypos = ypos + 0.0;
                zpos = zpos + 0.0;

                float x = V[i].x;// get x for vertex i
                float y = V[i].y;// get y for vertex i
                float z = V[i].z;// get z for vertex i

                float yt = y * cosax - z * sinax;// rotate around the x axis
                float zt = y * sinax + z * cosax;// using the Y and Z for the rotation
                y = yt;
                z = zt;

                float xt = x * cosay - z * sinay;// rotate around the Y axis
                zt = x * sinay + z * cosay;// using X and Z
                x = xt;
                z = zt;

                xt = x * cosaz - y * sinaz;// finaly rotate around the Z axis
                yt = x * sinaz + y * cosaz;// using X and Y
                x = xt;
                y = yt;

                x = x + xpos;// add the object position offset
                y = y + ypos;// for both x and y
                z = z + OFFSETZ - zpos;// as well as Z

                Point3D P_;
                P_.x = (x * magic / z) + OFFSETX;// translate 3d to 2d coordinates for screen
                P_.y = (y * magic / z) - OFFSETY;
                P_.z = z;
                V_new[i] = (P_);

            }

            //draw angle values
            for (int i = 0; i < 3; i++) {
                if (i == 0) draw_char_axes_onlcd(rotx, color_[i], 'x');
                else if (i == 1) draw_char_axes_onlcd(roty, color_[i], 'y');
                else
                draw_char_axes_onlcd(rotz, color_[i], 'z');
            }


//            draw3D_object(V_new,COLOR_BLUE);

            draw3D_object_FILL(V_new,COLOR_WHITE);

            changed = false;
        }


        return true;
    }

};

class fill_poly_CUBE: public scheduler_task {
private:
    TFT_22_ILI9225 lcd;

    VECTOR<Point3D> V, V_new;

    int num_vertices = 8;

    int num_edges = 12;

    int wires[12][2] = { //BORDER LINES
                       { 1, 2 },
                       { 2, 3 },
                       { 3, 4 },
                       { 4, 1 },
                       { 5, 6 },
                       { 6, 7 },
                       { 7, 8 },
                       { 8, 5 },
                       { 1, 5 },
                       { 2, 6 },
                       { 3, 7 },
                       { 4, 8 }
               };

    int num_surfaces = 6;
    int surf_size[6] = { 5, 5, 5, 5, 5, 5 };

    int surf[6][5] = { //SURFACES
                    { 1, 2, 3, 4, 1 }, //1
                    { 5, 6, 7, 8, 5 }, //2
                    { 1, 5, 6, 2, 1 }, //3
                    { 2, 6, 7, 3, 2 }, //4
                    { 1, 5, 8, 4, 1 }, //5
                    { 4, 8, 7, 3, 4 }, //6
            };
    int surf_z[6] = {1,2,3,4,5,6};
    int sx, sy, sz, ex, ey, ez;                    // define global vars for calling graphics subroutines

    bool changed; //to keep track of button pressed or not

    uint32_t color_[3] = { COLOR_RED, COLOR_BLUE, COLOR_GREEN }; //color for 3D axis lines

    float rotx = 0;                    // starting amount of x rotation
    float roty = 0;                    // starting amount of y rotation
    float rotz = 0;                    // starting amount of z rotation

    float xpos, ypos, zpos;

    float sinax, cosax, sinay, cosay, sinaz, cosaz;
public:

    fill_poly_CUBE(uint8_t priority) :
            scheduler_task("FILL_POLY", 1024 * 28, priority)
    {
    }

    //to draw the 3D object wireframe
    void draw3D_object(VECTOR<Point3D>& V, uint16_t color)
    {

        for (int i = 0; i < num_edges; i++) {
            sx = V[int(wires[i][0] - 1)].x;
            sy = abs(V[int(wires[i][0] - 1)].y);

            ex = V[int(wires[i][1] - 1)].x;
            ey = abs(V[int(wires[i][1] - 1)].y);

            lcd.drawLine(sx, sy, ex, ey, color);
        }
    }

    int print2largest(VECTOR<Point3D>& V)
    {
        int i, first, second;

        /* There should be atleast two elements */
//        if (arr_size < 2)
//        {
//            printf(" Invalid Input ");
//            return;
//        }

        first = second = -200;
        for (i = 0; i < V.size() ; i ++)
        {
            /* If current element is smaller than first
               then update both first and second */
            if (V[i].z > first)
            {
                second = first;
                first = V[i].z;
            }

            /* If arr[i] is in between first and
               second then update second  */
            else if (V[i].z > second && V[i].z != first)
                second = V[i].z;
        }

        if (second == -200) return first;
        else
            return second;

    }

    int minZ(VECTOR<Point3D>& V)
    {
        int minz = 200;
        for(int i=0;i<V.size();i++)
        {
            if(V[i].z < minz)
                minz = V[i].z;
        }

        return minz;
    }

    int maxZ(VECTOR<Point3D>& V)
    {
        int maxz = -200;
        for(int i=0;i<V.size();i++)
        {
            if(V[i].z > maxz)
                maxz = V[i].z;
        }

        return maxz;
    }

    void sort_surf(VECTOR<Point3D>& V)
    {
        //sort surf in ascending order of z
        for(int i=0;i<num_surfaces;i++)
        {
            int minz = V[surf[i][0]-1].z;
            for(int j=1;j<surf_size[i];j++)
            {
                if(V[surf[i][j]-1].z < minz)
                    minz = V[surf[i][j]-1].z;
            }
        }

    }

    //to draw the 3D object SOLID
    void draw3D_object_FILL(VECTOR<Point3D>& V, uint16_t color)
    {
//      uint16_t colors[6] = {COLOR_BLUEVIOLET, COLOR_BROWN, COLOR_GREENYELLOW, COLOR_ORANGE, COLOR_YELLOWGREEN, COLOR_WHITE};
        VECTOR<Point3D> temp;
//        int maxz = maxZ(V);
//        int minz = minZ(V);
        int maxz = -200;
        int minz = 200;
        int min_c = 1,max_c = 1;
        int minz_x=0, minz_y=0, maxz_x=0, maxz_y=0;
        for (int i = 0; i < V.size(); i++) {
            Point3D P = V[i];
            if (P.z <= minz) {
                minz = P.z;
                minz_x= P.x;
                minz_y= P.y;
                min_c++;
            }
            if (P.z > maxz) {
                maxz = P.z;
                maxz_x= P.x;
                maxz_y= P.y;
                max_c++;
            }
        }

        VECTOR<int> X,Y;
        for(int i=0;i<V.size();i++)
        {
            if(V[i].z == maxz)
                {
                    X.push_back(V[i].x);
                    Y.push_back(V[i].y);
                }
        }

//        if(X.size()==1){
//            ;
//        }
//        else if(X.size()==2){
//            maxz_x = (X[0]+X[1])/2;
//            maxz_y = (Y[0]+Y[1])/2;
//        }
//        else if(X.size()==4){
//            if((rotx==0||rotx==359)&&(roty==0||roty==359))
//            {
                maxz_x = (0 * magic / cube_width) + OFFSETX;// translate 3d to 2d coordinates for screen
                maxz_y = (0 * magic / cube_width) - OFFSETY;
//            }else{
//            int minX=X[0], maxX=X[0];
//            int minY=Y[0], maxY=Y[0];
//            for(int k=1;k<4;k++)
//            {
//                if(minX>X[k]) minX = X[k];
//                if(maxX<X[k]) maxX = X[k];
//
//                if(minY>Y[k]) minX = Y[k];
//                if(maxY<Y[k]) maxX = Y[k];
//            }
//            maxz_x = (minX+maxX)/2;
//            maxz_y = (minY+maxY)/2;}
//        }

        X.clear();Y.clear();
        Point3D Pz_min = {minz_x, minz_y, minz}, Pz_max = {maxz_x, maxz_y, maxz};

        for (int i = 0; i < num_surfaces; i++) {
            bool fill = true;
            //change the orders of surfaces by controlling i variable
            for (int j = 0; j < surf_size[i]; j++) {
                if(V[surf[i][j] - 1].z == maxz)
                    {
                        fill = false;
                        break;
                    }
                temp.push_back(V[surf[i][j] - 1]);
            }

                if (fill) {
//                    lcd.poly_fill(temp, color);
                    lcd.poly_fill_diffuse(temp, color,Pz_min, Pz_max);
//                int temp_z = maxZ(temp);
                for (int i1 = 0; i1 < temp.size()-1; i1++) {
                    sx = temp[i1].x;
                    sy = abs(temp[i1].y);

                    ex = temp[i1+1].x;
                    ey = abs(temp[i1+1].y);

//                    if((temp[i].z == temp_z || temp[i+1].z == temp_z))
                        lcd.drawLine(sx, sy, ex, ey, COLOR_BLUE);

                }

            }
            temp.clear();
        }
    }


    bool init()
    {


        GPIO TFT_RST { 2, 1 }, TFT_RS { 2, 2 }, TFT_CS { 2, 0 }, TFT_LED { 2, 3 };
        lcd.setPINS(TFT_RST, TFT_RS, TFT_CS, TFT_LED);

        rotx = roty = rotz = 0;
        xpos = ypos = zpos = 0;
        sinax = get_sine(rotx * 180 / 3.14);
        cosax = get_cosine(rotx * 180 / 3.14);

        sinay = get_sine(roty * 180 / 3.14);
        cosay = get_cosine(roty * 180 / 3.14);

        sinaz = get_sine(rotz * 180 / 3.14);
        cosaz = get_cosine(rotz * 180 / 3.14);
        changed = true;
        return true;
    }

    bool taskEntry()
    {

        //CUBE POINTS
        Point3D P[num_vertices] = { //POINTS
                        { cube_width, cube_width,-cube_width }, //1
                        {-cube_width, cube_width,-cube_width }, //2
                        {-cube_width,-cube_width,-cube_width }, //3
                        { cube_width,-cube_width,-cube_width }, //4
                        { cube_width, cube_width, cube_width }, //5
                        {-cube_width, cube_width, cube_width }, //6
                        {-cube_width,-cube_width, cube_width }, //7
                        { cube_width,-cube_width, cube_width }  //8
        };

        for (int i = 0; i < num_vertices; i++){
            V.push_back(P[i]);
            V_new.push_back(P[i]);
        }

        lcd.setSPIfrequency(16);
        lcd.begin();
        lcd.setFont((uint8_t *) Terminal6x8);
        lcd.clear();
        lcd.drawChar(10, 10, 'X', COLOR_RED);
        lcd.drawChar(10, 20, 'Y', COLOR_BLUE);
        lcd.drawChar(10, 30, 'Z', COLOR_GREEN);
        return true;
    }
    void draw_char_axes_onlcd(float rot, uint16_t color, char c_)
    {

        //now draw them
        int ang = int(rot * (180 / 3.14));
        char _1 = '0' + ang / 100;
        char _2 = '0' + int(ang / 10) % 10;
        char _3 = '0' + (ang % 10);

        switch (c_) {
            case 'x':
                //clear old
                lcd.fillRectangle(10 + 5 + 2, 10, 10 + 15 + 5 + lcd.fontX(), 10 + lcd.fontY(), COLOR_BLACK);

                //draw new
                lcd.drawChar(10 + 5 + 2, 10, _1, color);
                lcd.drawChar(10 + 10 + 4, 10, _2, color);
                lcd.drawChar(10 + 15 + 5, 10, _3, color);
                break;
            case 'y':
                //clear old
                lcd.fillRectangle(10 + 5 + 2, 20, 10 + 15 + 5 + lcd.fontX(), 20 + lcd.fontY(), COLOR_BLACK);

                //draw new
                lcd.drawChar(10 + 5 + 2, 20, _1, color);
                lcd.drawChar(10 + 10 + 4, 20, _2, color);
                lcd.drawChar(10 + 15 + 5, 20, _3, color);
                break;
            case 'z':
                //clear old
                lcd.fillRectangle(10 + 5 + 2, 30, 10 + 15 + 5 + lcd.fontX(), 30 + lcd.fontY(), COLOR_BLACK);

                //draw new
                lcd.drawChar(10 + 5 + 2, 30, _1, color);
                lcd.drawChar(10 + 10 + 4, 30, _2, color);
                lcd.drawChar(10 + 15 + 5, 30, _3, color);
                break;
        }
    }

    bool run(void *p)
    {
        //handling button presses
        if (SW.getSwitch(1))
        {

            rotx = rotx + (10 * (3.14 / 180));                // rotate on X axis
            if((rotx * (180 / 3.14))>=359 )
            rotx =0;
            changed = true;
            sinax = get_sine(rotx * 180 / 3.14);
            cosax = get_cosine(rotx * 180 / 3.14);

            sinay = get_sine(roty * 180 / 3.14);
            cosay = get_cosine(roty * 180 / 3.14);

            sinaz = get_sine(rotz * 180 / 3.14);
            cosaz = get_cosine(rotz * 180 / 3.14);
        }

        if (SW.getSwitch(2))
        {
            roty = roty + (10*(3.14/180));                // rotate on Y axis
            if((roty * (180 / 3.14))>=359 )
            roty =0;
            changed = true;
            sinax = get_sine(rotx * 180 / 3.14);
            cosax = get_cosine(rotx * 180 / 3.14);

            sinay = get_sine(roty * 180 / 3.14);
            cosay = get_cosine(roty * 180 / 3.14);

            sinaz = get_sine(rotz * 180 / 3.14);
            cosaz = get_cosine(rotz * 180 / 3.14);
        }

        if (SW.getSwitch(3))
        {
            rotz = rotz + (10*(3.14/180));                // rotate on Z axis
            if((rotz * (180 / 3.14))>=359 )
            rotz =0;
            changed = true;
            sinax = get_sine(rotx * 180 / 3.14);
            cosax = get_cosine(rotx * 180 / 3.14);

            sinay = get_sine(roty * 180 / 3.14);
            cosay = get_cosine(roty * 180 / 3.14);

            sinaz = get_sine(rotz * 180 / 3.14);
            cosaz = get_cosine(rotz * 180 / 3.14);
        }

        if (SW.getSwitch(4))
        {
            lcd.clear();

            lcd.drawChar(10, 10, 'X', COLOR_RED);
            lcd.drawChar(10, 20, 'Y', COLOR_BLUE);
            lcd.drawChar(10, 30, 'Z', COLOR_GREEN);

            for(int i=0;i<V_new.size();i++)
            V_new[i] = V[i];

            rotx = roty = rotz = 0;
            changed = true;
            sinax = get_sine(rotx * 180 / 3.14);
            cosax = get_cosine(rotx * 180 / 3.14);

            sinay = get_sine(roty * 180 / 3.14);
            cosay = get_cosine(roty * 180 / 3.14);

            sinaz = get_sine(rotz * 180 / 3.14);
            cosaz = get_cosine(rotz * 180 / 3.14);
        }

        if (changed) {

            //sort surfaces in ascending order of z
            sort_surf(V_new);

            draw3D_object_FILL(V_new,COLOR_BLACK);
            draw3D_object(V_new,COLOR_BLACK);

//            lcd.drawLine(LCD_WIDTH/2,0,LCD_WIDTH/2,LCD_HEIGHT/2, COLOR_RED);
//            lcd.drawLine(LCD_WIDTH/2,LCD_HEIGHT/2,0,LCD_HEIGHT, COLOR_GREEN);
//            lcd.drawLine(LCD_WIDTH/2,LCD_HEIGHT/2,LCD_WIDTH,LCD_HEIGHT, COLOR_BLUE);

            //new 3D axis coords
            for (int i = 0; i < V.size(); i++)// translate 3d vertex position to 2d screen position
            {
                xpos = xpos + 0.0;                // move the object
                ypos = ypos + 0.0;
                zpos = zpos + 0.0;

                float x = V[i].x;// get x for vertex i
                float y = V[i].y;// get y for vertex i
                float z = V[i].z;// get z for vertex i

                float yt = y * cosax - z * sinax;// rotate around the x axis
                float zt = y * sinax + z * cosax;// using the Y and Z for the rotation
                y = yt;
                z = zt;

                float xt = x * cosay - z * sinay;// rotate around the Y axis
                zt = x * sinay + z * cosay;// using X and Z
                x = xt;
                z = zt;

                xt = x * cosaz - y * sinaz;// finaly rotate around the Z axis
                yt = x * sinaz + y * cosaz;// using X and Y
                x = xt;
                y = yt;

                x = x + xpos;// add the object position offset
                y = y + ypos;// for both x and y
                z = z + OFFSETZ - zpos;// as well as Z

                Point3D P_;
                P_.x = (x * magic / z) + OFFSETX;// translate 3d to 2d coordinates for screen
                P_.y = (y * magic / z) - OFFSETY;
                P_.z = z;
                V_new[i] = (P_);
            }


//            u0_dbg_printf("\nmaxZ = %d\nV[%d].x = %d",maxZ(V_new),z_max,V[abs(z_max)].x);
            //draw angle values
            for (int i = 0; i < 3; i++) {
                if (i == 0) draw_char_axes_onlcd(rotx, color_[i], 'x');
                else if (i == 1) draw_char_axes_onlcd(roty, color_[i], 'y');
                else
                draw_char_axes_onlcd(rotz, color_[i], 'z');
            }


//            draw3D_object(V_new,COLOR_BLUE);
            draw3D_object_FILL(V_new,COLOR_ORANGE);
//            draw3D_object_outline(V_new,COLOR_BLUE);
            changed = false;
        }

        return true;
    }

};


int main(void)
{

#if draw_arrow
    scheduler_add_task(new fill_poly_ARROW(PRIORITY_MEDIUM));
#endif

    #if draw_cube
    scheduler_add_task(new fill_poly_CUBE(PRIORITY_MEDIUM));
#endif
    scheduler_start();
    return -1;
}
