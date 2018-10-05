/***************************************************
  This was originally an example sketch for the Adafruit 1.8" SPI display.
  Below is the AdaFruit description.
  This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/358
  as well as Adafruit raw 1.8" TFT display
  ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
/* Adapted for the Kentec 35-110 display running on a Texas Instruments
   Tiva board. Uses the LCD_screen library suite.
*****************************************************/
#include "Energia.h"
#include "K35_screen.h"
LCD_K35 myScreen;

int width; // must be initialized in setup
int height; // must be initialized in setup

float p = 3.1415926;


void setup(void) {

  myScreen.begin();
  myScreen.setOrientation(1);
  myScreen.setFontSize(1);
  myScreen.setPenSolid(false);
  width = myScreen.screenSizeX();
  height = myScreen.screenSizeY();

  myScreen.clear();

  delay(500);

}
void test() {
  myScreen.clear();
  myScreen.setFontSize(3);
  testdrawtext("Kentec 35-110 display", cyanColour);
  delay(2000);
  myScreen.clear();

  testweb();
  delay(5000);
  myScreen.clear();

  // large block of text
  myScreen.setFontSize(1);
  testdrawtext(
"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere.  Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ",
whiteColour);
  delay(2000);

  // print function!
  printTest();
  delay(2000);

  // a single pixel
  //myScreen.point(width/2, height/2, greenColour);
  //delay(2000);

  // line draw test
  testlines(yellowColour);
  delay(2000);

  // optimized lines
  testfastlines(redColour, blueColour);
  delay(2000);

  testdrawrects(greenColour);
  delay(2000);

  testfillrects(yellowColour, magentaColour);
  delay(2000);

  myScreen.clear();
  testfillcircles(10, blueColour);
  testdrawcircles(10, whiteColour);
  delay(2000);

  testroundrects();
  delay(2000);

  testtriangles();
  delay(2000);

  mediabuttons();

  delay(1000);
}

void loop() {
  test();
  //myScreen.invertDisplay(true);
  delay(500);
  //myScreen.invertDisplay(false);
  delay(500);
}
void testArc(){
    // arc is not implemented in the library
    int x = width/2;
    int y = height/2;
    int rad = 100;

    myScreen.arc(x, y, rad, 0, 89, redColour);
    myScreen.arc(x, y, rad+10, 90, 179, yellowColour);
    myScreen.arc(x, y, rad+20, 180, 269, greenColour);
    myScreen.arc(x, y, rad+30, 270, 359, magentaColour);
}


void testlines(uint16_t color) {
  myScreen.clear();
  for (int16_t x=0; x < width; x+=6) {
    myScreen.dLine(0, 0, x, height-1, color);
    //myScreen.line(0, 0, x, height-1, color);
  }
  for (int16_t y=0; y < height; y+=6) {
    myScreen.dLine(0, 0, width-1, y, color);
  }

  delay(1000);

  myScreen.clear();
  for (int16_t x=0; x < width; x+=6) {
    myScreen.line(width-1, 0, x, height-1, color);
  }
  for (int16_t y=0; y < height; y+=6) {
    myScreen.line(width-1, 0, 0, y, color);
  }
  delay(1000);

  myScreen.clear();
  for (int16_t x=0; x < width; x+=6) {
    myScreen.line(0, height-1, x, 0, color);
  }
  for (int16_t y=0; y < height; y+=6) {
    myScreen.line(0, height-1, width-1, y, color);
  }
  delay(1000);
  myScreen.clear();
  for (int16_t x=0; x < width; x+=6) {
    myScreen.line(width-1, height-1, x, 0, color);
  }
  for (int16_t y=0; y < height; y+=6) {
    myScreen.line(width-1, height-1, 0, y, color);
  }
  delay(1000);
}

void testweb(){
    int x2 = 0;
    int xincr = 8;
    int yincr = 6;
    int color;

    color = blueColour;
    for (int16_t y1 = 0; y1 < height; y1+= 3){
        myScreen.line(0, y1, x2, height-1, color);
        x2 += 4;
    }
    color = redColour;
    x2 = width-1;
    for (int16_t y1 = 0; y1 < height; y1+= 3){
        myScreen.line(width-1, y1, x2, height-1, color);
        x2 -= 4;
    }
}

/* Debug using the display */
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
    itoa(val, &str[0]);
    myScreen.gText(x, y, txt, whiteColour);
    myScreen.gText(x+strlen(txt)*myScreen.fontSizeX(), y, &str[0], whiteColour);
}

/* End of debugging code */

void testdrawtext(char *text, uint16_t color) {
  int fsize = myScreen.fontSizeX();
  //myprint(0,80, "fsize=", fsize);
  //myprint(0,90, "width=", width);
  int strmax = width/fsize;
  int charsLeft = strlen(text);
  char str[256];

 if (charsLeft > strmax) {
      int y = 0;
      int idx = 0;
      while (charsLeft > 0) {
         int cslen;
         //myprint(0,100, "charsLeft=", charsLeft);
         //myprint(0,110, "strmax=", strmax);
         if (charsLeft < strmax) {
             cslen = charsLeft;
         } else {
             cslen = strmax;
         }
         strncpy(str, &text[idx], cslen);
         str[cslen] = '\0';
         //myprint(0,120, "cslen=", cslen);
         myScreen.gText(0, y, str, color, blackColour);
         y += myScreen.fontSizeY();
         if (y > height) return;
         idx += strmax;
         charsLeft -= strmax;
      }
  } else {
      myScreen.gText(0,0,text,color, blackColour);
  }
}

void testfastlines(uint16_t color1, uint16_t color2) {
  myScreen.clear();
  for (int16_t y=0; y < height; y+=5) {
    myScreen.dLine(0, y, width, 1, color1);
  }
  for (int16_t x=0; x < width; x+=5) {
    myScreen.dLine(x, 0, 1, height, color2);
  }
}

void testdrawrects(uint16_t color) {
  int y;
  myScreen.clear();
  myScreen.setPenSolid(false);
  y = 0;
  for (int16_t x=0; x < width/2; x+=6) {
    myScreen.dRectangle(x, y, width-2*x, height-2*y, color);
    y +=4;
  }
}

void testfillrects(uint16_t color1, uint16_t color2) {
  int y;
  myScreen.clear();
  int x = 0;
  for (y=0; y< height/2; y+=4) {
    myScreen.setPenSolid(true);
    myScreen.dRectangle(x, y, width-2*x, height-2*y, color1);
    myScreen.setPenSolid(false);
    myScreen.dRectangle(x, y , width-2*x, height-2*y, color2);
    x += 4;
  }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  myScreen.setPenSolid(true);
  for (int16_t x=radius; x < width; x+=radius*2) {
    for (int16_t y=radius; y < height; y+=radius*2) {
      myScreen.circle(x, y, radius, color);
    }
  }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  myScreen.setPenSolid(false);
  for (int16_t x=0; x < width+radius; x+=radius*2) {
    for (int16_t y=0; y < height+radius; y+=radius*2) {
      myScreen.circle(x, y, radius, color);
    }
  }
}

void testtriangles() {
  myScreen.clear();
  int color = 0xF800;
  int t;
  int x1 = width/2; int y1 = 0;
  int x2 = 0;       int y2 = height-1;
  int x3 = width-1;   int y3 = height-1;
  for(t = 0 ; t <= 14; t+=1) {
    myScreen.triangle(x1, y1, x2, y2, x3, y3, color);
    y1 += 4;
    x2 += 5; y2 -= 4;
    x3 -= 5; y3 -= 4;
    color+=100;
  }
}

void roundRectangle(int x, int y, int dx, int dy, int rad, int color) {
    // Arcs are not implemented
    //myScreen.arc(x+rad, y+rad, rad, 270, 359, color);
    myScreen.dLine(x+rad, y,     dx-2*rad, 0,        color);
    myScreen.dLine(x+dx,  y+rad, 0,        dy-2*rad, color);
    //myScreen.arc(x+dx-rad, y+rad, rad, 0, 89, color);
    myScreen.dLine(x,     y+rad, 0,        dy-2*rad, color);
    //myScreen.arc(x+dx-rad, y+dy-rad, rad, 90, 179, color);
    myScreen.dLine(x+rad, y+dy,  dx-2*rad, 0,        color);
    //myScreen.arc(x+rad, y+dy-rad, rad, 180,  269, color);
}

void testroundrects() {
  myScreen.clear();
  int color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
    int x = 0;
    int y = 0;
    int dx = 319;
    int dy = 239;
    for(i = 0 ; i <= 24; i+=1) {
      //myScreen.rectangle(x, y, w, h, color);
      // TBD
      roundRectangle(x, y, dx, dy, 10, color);
      x+=4;
      y+=3;
      dx-=8;
      dy-=6;
      color+=1100;
    }
    color+=100;
  }
  delay(10000);
}

void printTest() {
  myScreen.clear();
  myScreen.setFontSize(0);
  myScreen.gText(0, 30, "Hello World!", redColour);
  myScreen.setFontSize(1);
  myScreen.gText(0, 60, "Hello World!", yellowColour);
  myScreen.setFontSize(2);
  myScreen.gText(0, 90, "Hello World!", greenColour);
  myScreen.setFontSize(3);
  myScreen.gText(0, 120, "Hello World!", blueColour);
}

void mediabuttons() {
  // play
  myScreen.clear();
  myScreen.setPenSolid(true);
  myScreen.dRectangle(25, 10, 78, 60, whiteColour);
  myScreen.triangle(42, 20, 42, 60, 90, 40, redColour);
  delay(500);
  // pause
  myScreen.dRectangle(25, 90, 78, 60,  whiteColour);
  myScreen.dRectangle(39, 98, 20, 45,  greenColour);
  myScreen.dRectangle(69, 98, 20, 45,  greenColour);
  delay(500);
  // play color
  myScreen.triangle(42, 20, 42, 60, 90, 40, blueColour);
  delay(50);
  // pause color
  myScreen.dRectangle(39, 98, 20, 45,  redColour);
  myScreen.dRectangle(69, 98, 20, 45,  redColour);
  // play color
  myScreen.triangle(42, 20, 42, 60, 90, 40, greenColour);
}

