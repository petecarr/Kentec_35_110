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
struct box titleBox= {0,  0,   320, 23, false, -1, whiteColour,
                      "Graph", 2};
struct box mainBox = {0,  23,  320, 240-23};

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

    myScreen.begin();
    myScreen.clear();
    myScreen.setOrientation(1);

    drawBox(&titleBox);
    myScreen.setPenSolid(false);
    myScreen.dRectangle(mainBox.x, mainBox.y, mainBox.dx, mainBox.dy,
                        whiteColour);

}

uint16_t x1_

void drawGraph(uint16_t yorigin, float yscale, float xscale){
    uint16_t color = whiteColour;
    float X;
    uint16_t x, y;

    // Horizontal
    myScreen.dLine(0, yorigin, SCREEN_WIDTH, 0, redColour);

    for (x = 0; x < SCREEN_WIDTH; x++) {
        X = ((float)x)*xscale*M_PI/180.0;
        // y increases from top to bottom so insert a minus here
        y = (int16_t)(-sinf(X)*yscale) + yorigin;
        myScreen.point(x, y, color);
    }
}



void loop() {
     uint16_t x, y;
     uint16_t color = whiteColour;
     int16_t yorigin = 100;

     float degrees_per_x1 = 3.0; //6.0;
     float degrees_per_x2 = 7.5; //15.0;
     // y increases from top to bottom
     float YSCALE = -25.0;
     float X1, X2;
     static int once = 1;

     if (once) {
        once = 0;

        // yorigin, yscale, xscale
        drawGraph(60, 34.0, degrees_per_x1);
        drawGraph(132, 34.0, degrees_per_x2);

      yorigin = 204;
       YSCALE = 17;
       myScreen.dLine(0, yorigin, SCREEN_WIDTH, 0, redColour);
       for (x = 0; x < SCREEN_WIDTH; x++) {
           X1 = ((float)x)*degrees_per_x1*M_PI/180.0;
           X2 = ((float)x)*degrees_per_x2*M_PI/180.0;
           y = -(int16_t)((sinf(X1) + sinf(X2))*YSCALE) + yorigin;

           myScreen.point(x, y, color);
        }
   }

}

