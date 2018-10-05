#include "Energia.h"
#include "K35_screen.h"
LCD_K35 myScreen;

// Screen width
#define SCREEN_WIDTH 320

struct box {
    int x;
    int y;
    int dx;
    int dy;
    boolean ledOn;
    int LED;
    int colour;
    char * label;
    int labelFontSize;
};
struct box titleBox= {0,  0,   320, 23, false, -1, greenColour,
                      "Sum of two sine waves.", 2};
struct box mainBox = {0,  23,  320, 240-23};

float sines[360];

void make_sines(){
    for (int16_t x = 0; x < 360; x++) {
        sines[x] = sinf((float)x*M_PI/180.0);
    }
}

// Debugging ************************************************************

void reverse(char s[]) {
    int c,i,j;
    for (i = 0,j=strlen(s)-1;  i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}
void itoa(int n, char s[]){

    int i, sign;
    if ((sign =n) < 0) n = -n;
    i = 0;
    do {
        s[i++] = n%10 + '0';
    } while ((n /= 10) > 0);
    if (sign < 0) s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

void myprint(int x, int y, char * txt, int val){
    char str[40];
    int ix;
    str[0] = ' ';
    str[1] = ' ';
    str[2] = ' ';
    str[3] = ' ';
    itoa(val, &str[0]);
    myScreen.gText(x, y, txt, whiteColour);
    ix = x+strlen(txt)*myScreen.fontSizeX();
    myScreen.dRectangle(ix, y, 4*myScreen.fontSizeX(), myScreen.fontSizeY(),
                   blackColour);
    myScreen.gText(ix, y, &str[0], whiteColour);
}
// End Debugging ************************************************************8


// Draw text centered in box
void cText(struct box *wBox, char * text){
    int textLen = strlen(text)*myScreen.fontSizeX();
    int textX = wBox->x + (wBox->dx - textLen)/2;
    int textY = wBox->y + (wBox->dy - myScreen.fontSizeY())/2;
    myScreen.gText(textX, textY, text);
}
void drawBox(struct box *wBox){
    int textLen;
    int textX;
    int textY;

    myScreen.setPenSolid(false);
    myScreen.dRectangle(wBox->x, wBox->y, wBox->dx, wBox->dy,
                        wBox->colour);
    myScreen.setFontSize(wBox->labelFontSize);
    cText(wBox, wBox->label);
}


void setup() {
    //setup code here, runs once

    myScreen.begin();
    myScreen.clear();

    myScreen.setOrientation(1);

    drawBox(&titleBox);

    myScreen.setPenSolid(false);
    myScreen.dRectangle(mainBox.x, mainBox.y, mainBox.dx, mainBox.dy,
                        greenColour);


}
void drawGraph(uint16_t yorigin, float yscale, float xscale, float start_d){
    static uint16_t color = whiteColour;
    float Xd;
    int lasty;
    uint16_t x, y;

    for (x = 1; x < SCREEN_WIDTH-1; x++) {
        int16_t idx;
        if (x > 1) {
            y = lasty;
            myScreen.point(x, y, blackColour);
            if (y == yorigin) {
                myScreen.point(x, y, redColour);
            }
        }

        Xd = start_d + (float)x*xscale;
        idx = (int)Xd %360;
        // y increases from top to bottom so insert a minus here
        y = (int16_t)(-sines[idx]*yscale) + yorigin;
        lasty = y;
        myScreen.point(x, y, color);
    }
}


float start_degree1 = 0.0;
float start_degree2 = 0.0;
void loop() {
     uint16_t x, y;
     static uint16_t color = whiteColour;
     int16_t yorigin = 100;

     float degrees_per_x1 = 3.0; //6.0;
     float degrees_per_x2 = 7.5; //15.0;
     // y increases from top to bottom
     float YSCALE = -25.0;
     float X1, X2;
     static boolean once = true;

     if (once) {
        once = false;
        make_sines();
        // Horizontal baseline 1
        yorigin = 60.0;
        myScreen.dLine(0, yorigin, SCREEN_WIDTH, 0, redColour);
        // Horizontal baseline 2
        yorigin = 132;
        myScreen.dLine(0, yorigin, SCREEN_WIDTH, 0, redColour);
        // Horizontal baseline 3 (sum of 1 and 2)
        yorigin = 204;
        myScreen.dLine(0, yorigin, SCREEN_WIDTH, 0, redColour);
     } else {

        yorigin = 60.0;
        // yorigin, yscale, xscale
        drawGraph(yorigin, 34.0, degrees_per_x1, start_degree1);

        yorigin = 132;
        drawGraph(yorigin, 34.0, degrees_per_x2, start_degree2);

        yorigin = 204;
        YSCALE = 17;
        int lasty;
        for (x = 1; x < SCREEN_WIDTH -2; x++) {
            int16_t idx1, idx2;
            if (x > 1) {
                y = lasty;
                myScreen.point(x+1, y, blackColour);
                if (y == yorigin) {
                    myScreen.point(x+1, y, redColour);
                }
            }
           X1 = start_degree1 + (float)x*degrees_per_x1;
            idx1 = (int)X1 % 360;
            X2 = start_degree2 + (float)x*degrees_per_x2;
            idx2 = (int)X2 % 360;
            y = -(int16_t)((sines[idx1] + sines[idx2])*YSCALE) + yorigin;
            lasty = y;

            myScreen.point(x, y, color);
        }
        start_degree1 += degrees_per_x1;
        if (start_degree1 >= 360.0) {
            start_degree1 = 0.0;
        }
        start_degree2 += degrees_per_x2;
        if (start_degree2 >= 360.0) {
            start_degree2 = 0.0;
        }
    }

}




