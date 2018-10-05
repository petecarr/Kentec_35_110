// For ADC - code from TI forums
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"

#include "Energia.h"
#include "K35_screen.h"
LCD_K35 myScreen;

uint16_t x, y, z;

uint32_t TempValueC = 20;
uint32_t TempValueF = 68;
char tempText[19] = "Temp = 20 C,  68 F";
//                   012345678901234567
uint32_t hours = 0;
uint32_t minutes = 0;
uint32_t seconds = 0;
char timeText[17] = "Time = 00:00:00 ";
//                   012345678901234567

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
                      "LED Control", 2};
struct box mainBox = {0,  23,  320, 240-23};
struct box redBox  = {60, 30,  200, 40, false,
                      RED_LED, redColour, "Red LED", 2};
struct box blueBox = {60, 80,  200, 40, false,
                      BLUE_LED, blueColour, "Blue LED", 2};
struct box greenBox= {60, 130, 200, 40, false,
                      GREEN_LED, greenColour, "Green LED", 2};
struct box tempBox = {60, 180, 200, 20, false,
                      -1, orangeColour, "Temperature", 1};
struct box timeBox = {60, 210, 200, 20, false,
                      -1, orangeColour, "Time", 1};
boolean inBox(int x, int y, struct box b )
{
    if ((x >= b.x) && (x < (b.x+b.dx))) {
        if ((y >= b.y) && (y < b.y+ b.dy)) {
            return true;
        }
    }
    return false;
}


void onTempPress()
{
    int TempValueC;
    // Update the temperature
    TempValueC = read_ADC();

    TempValueF = (TempValueC * 9 + 160) / 5;
    tempText[7] = TempValueC/10 + '0';
    tempText[8] = TempValueC%10 +'0';
    if (TempValueF >= 100) {
        tempText[13] = '1';
    } else {
        tempText[13] = ' ';
    }
    tempText[14] = TempValueF/10 + '0';
    tempText[15] = TempValueF%10 +'0';
}

void onTimePress()
{
    // Awaits, perhaps, an interrupt driven timer.
    // Increment seconds, repaint each minute (and update temperature)
    seconds += 1;   // for now

    if (seconds >= 60) {
        seconds = 0;
        minutes += 1;
        if (minutes >= 60) {
            minutes = 0;
            hours += 1;
            if (hours >= 24) {
                hours = 0;
            }
        }
    }
    timeText[7]  = hours/10 + '0';
    timeText[8]  = hours%10 +'0';
    timeText[9] = ':';
    timeText[10] = minutes/10 + '0';
    timeText[11] = minutes%10 + '0';
    timeText[12] = ':';
    timeText[13] = seconds/10 + '0';
    timeText[14] = seconds%10 + '0';
}


void setup_ADC(void)
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
  //SysCtlADCSpeedSet(SYSCTL_ADCSPEED_125KSPS); // 250
  ADCSequenceDisable(ADC0_BASE, 1);
  ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
  ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS);
  ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_TS);
  ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_TS);
  ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_TS |
                                            ADC_CTL_IE | ADC_CTL_END);
  ADCSequenceEnable(ADC0_BASE, 1);
}
uint32_t read_ADC() {
    volatile uint32_t ui32TempAvg;
    volatile uint32_t ui32TempValueC;
    volatile uint32_t ui32TempValueF;

    uint32_t ui32ADC0Value[4];

    // Clear interrupt flag
    ADCIntClear (ADC0_BASE, 1);
    // Trigger conversion with software
    ADCProcessorTrigger (ADC0_BASE, 1);
    // Wait for conversion to complete
    while (!ADCIntStatus (ADC0_BASE, 1, false)) {
    }
    ADCSequenceDataGet (ADC0_BASE, 1, ui32ADC0Value);
    ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2]
                        + ui32ADC0Value[3] + 2) / 4;
    // TEMP = 147.5 - ((75*(VREFP-VREFN)*ADC_VALUE)/4096
    // VREFP-VREFN == VDD == 3.3V
    // 3.3*75 = 247.5
    ui32TempValueC = (1475 - ((2475 * ui32TempAvg)) / 4096) / 10;
    // 160/5 == 32 (just avoiding rounding)
    //ui32TempValueF = ((ui32TempValueC * 9) + 160) / 5;
    return ui32TempValueC;
}



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
    Serial.begin(9600);
    delay(100);
    Serial.println("Slider example for Kentec 35-110");

    myScreen.begin();
    String s = myScreen.WhoAmI();
    Serial.println(s);

    setup_ADC();

    myScreen.showInformation(0,0);

    pinMode(RED_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, LOW);

    delay(2000);
    myScreen.clear();

    myScreen.setOrientation(1);
    myScreen.calibrateTouch();

    drawBox(&titleBox);

    myScreen.setPenSolid(false);
    myScreen.dRectangle(mainBox.x, mainBox.y, mainBox.dx, mainBox.dy,
                        whiteColour);

    drawBox(&redBox);
    drawBox(&blueBox);
    drawBox(&greenBox);
    drawBox(&timeBox);
    drawBox(&tempBox);
}


void handleSlider(struct box *wBox, int x) {
    int pwmVal = 0;
    // PWM value from 0 to 255
    pwmVal = (x-wBox->x)*255/wBox->dx;
    // Crude way to switch off the LED rather than try to slide PWM down to 0
    if (pwmVal < 20) {
        // Just debugging
        //myScreen.setFontSize(1);
        //myScreen.gText(15, 100, "OFF", redColour);

        myScreen.setPenSolid(true);
        myScreen.dRectangle(wBox->x, wBox->y, wBox->dx, wBox->dy,
                            blackColour);
        myScreen.setPenSolid(false);
        myScreen.dRectangle(wBox->x, wBox->y, wBox->dx, wBox->dy,
                            wBox->colour);
        myScreen.setFontSize(2);
        cText(wBox, wBox->label);
        //digitalWrite(wBox->LED, LOW);
        analogWrite(wBox->LED, 0);
        wBox->ledOn = false;
    } else {
        // Debugging
        //myScreen.setFontSize(1);
        //myScreen.gText(5, 100, "ON", greenColour);

        myScreen.setPenSolid(true);
        myScreen.dRectangle(wBox->x, wBox->y, x-wBox->x, wBox->dy,
                            wBox->colour);
        myScreen.dRectangle(x, wBox->y, wBox->x + wBox->dx-x, wBox->dy,
                            blackColour);
        myScreen.setFontSize(2);
        cText(wBox, wBox->label); //, blackColour, wBox->colour);
        //digitalWrite(wBox->LED, HIGH);
        analogWrite(wBox->LED, pwmVal);
        wBox->ledOn = true;
    }
}
// Arduino/Energia loop
void loop() {
    int col = blackColour;

    if (myScreen.getTouch(x, y, z)) {
        if (inBox(x,y,redBox)) {
            handleSlider(&redBox, x);
            col = redColour;
        } else if (inBox(x,y,blueBox)) {
            handleSlider(&blueBox, x);
            col = blueColour;
        } else if (inBox(x,y,greenBox)) {
            handleSlider(&greenBox, x);
            col = greenColour;
        } else if (inBox(x,y,tempBox)) {
            myScreen.setPenSolid(true);
            myScreen.dRectangle(tempBox.x+1,tempBox.y+1,
                                tempBox.dx-2,tempBox.dy-2,
                                blackColour);
            myScreen.setFontSize(1);
            onTempPress();
            cText(&tempBox, &tempText[0]);
            col = orangeColour;
        } else if (inBox(x,y,timeBox)) {
            myScreen.setPenSolid(true);
            myScreen.dRectangle(timeBox.x+1, timeBox.y+1,
                                timeBox.dx-2,timeBox.dy-2,
                                blackColour);
            myScreen.setFontSize(1);
            onTimePress();
            cText(&timeBox, &timeText[0]);
            col = orangeColour;
        }
        /* Debugging - uses the box color
        myScreen.setFontSize(1);
        myScreen.gText(5, 200,
            "x="+String(x/100)+String(x/10%10)+String(x%10), col);
        myScreen.gText(5, 210,
            "y="+String(y/100)+String(y/10%10)+String(y%10), col);
        myScreen.gText(5, 220,
            "z="+String(z/100)+String(z/10%10)+String(z%10), col);
        */
    }
    delay(100);
    x = y = z = 0;
}

