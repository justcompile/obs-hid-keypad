/*
 *
 *
 *    obs-streampad-v1-2.ino
 *
 *    15 keypad application base.
 *
 *    License: GNU General Public License Version 3.0.
 *    
 *    Copyright (C) 2020-2021 Phil Hayes M0PIT All rights reserved.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *    
 *    This program is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *    
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see: http://www.gnu.org/licenses/
 *
 *
 */


#if 1

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

#define MINPRESSURE 200
#define MAXPRESSURE 1000

MCUFRIEND_kbv tft;


//******* Three lines TFT Calibration *************************************

// Touch panels and wiring maybe different
// copy-paste results from TouchScreen_Calibr_native.ino

const int XP = 7, XM = A1, YP = A2, YM = 6; //ID=0x9341

const int TS_LEFT = 131, TS_RT = 914; //X=map values (TS_LEFT = LEFT), (TS_RT= RT).

const int TS_TOP = 95, TS_BOT = 936; //Y=map values  (TS_TOP = TOP), (TS_BOT = BOT).

//******** End of TFT calibation ******************************************


TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button SW1_btn, SW2_btn, SW3_btn, SW4_btn, SW5_btn, SW6_btn, SW7_btn, SW8_btn, SW9_btn, SW10_btn, SW11_btn, SW12_btn, SW13_btn, SW14_btn, SW15_btn;

int pixel_x, pixel_y;     //Touch_getXY() updates global vars
bool Touch_getXY(void)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
    }
    return pressed;
}

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


//------------------------------Define HID Key Output-------------------------------------------

#define KEY_N1  89
#define KEY_N2  90
#define KEY_N3  91
#define KEY_N4  92
#define KEY_N5  93
#define KEY_N6  94
#define KEY_N7  95
#define KEY_N8  96
#define KEY_N9  97

#define KEY_F1  58
#define KEY_F2  59
#define KEY_F3  60
#define KEY_F4  61
#define KEY_F5  62
#define KEY_F6  63

//------------------------------End HID Keys Output-------------------------------------------

    uint8_t buf[8] = {
      0 };      //Keyboard Report Buffer


void setup(void)
{

    Serial.begin(9600);
    uint16_t ID = tft.readID();
    if (ID == 0xD3D3) ID = 0x9486; // write-only shield
    tft.begin(ID);
    tft.setRotation(0);            //PORTRAIT
    tft.fillScreen(BLACK);


//------------------------------Draw Keys, Colours & Text-----------------------------------------

    SW1_btn.initButton(&tft,   55, 50, 90, 85, WHITE, MAGENTA, BLACK, "SCR1", 3);
    SW2_btn.initButton(&tft,  160, 50, 90, 85, WHITE, MAGENTA, BLACK, "SCR2", 3);
    SW3_btn.initButton(&tft,  265, 50, 90, 85, WHITE, MAGENTA, BLACK, "SCR3", 3);
    
    SW4_btn.initButton(&tft,   55, 145, 90, 85, WHITE, BLUE, BLACK, "SCR4", 3);
    SW5_btn.initButton(&tft,  160, 145, 90, 85, WHITE, BLUE, BLACK, "SCR5", 3);
    SW6_btn.initButton(&tft,  265, 145, 90, 85, WHITE, BLUE, BLACK, "SCR6", 3);
    
    SW7_btn.initButton(&tft,   55, 240, 90, 85, WHITE, GREEN, BLACK, "SCR7", 3);
    SW8_btn.initButton(&tft,  160, 240, 90, 85, WHITE, GREEN, BLACK, "SCR8", 3);
    SW9_btn.initButton(&tft,  265, 240, 90, 85, WHITE, GREEN, BLACK, "SCR9", 3);

    SW10_btn.initButton(&tft,   55, 335, 90, 85, WHITE, CYAN, BLACK, "WIPE", 3);
    SW11_btn.initButton(&tft,  160, 335, 90, 85, WHITE, CYAN, BLACK, "CUT", 3);
    SW12_btn.initButton(&tft,  265, 335, 90, 85, WHITE, CYAN, BLACK, "LUM", 3);

    SW13_btn.initButton(&tft,   55, 430, 90, 85, WHITE, YELLOW, BLACK, "BOT", 3);
    SW14_btn.initButton(&tft,  160, 430, 90, 85, WHITE, YELLOW, BLACK, "TOP", 3); 
    SW15_btn.initButton(&tft,  265, 430, 90, 85, WHITE, YELLOW, BLACK, "EXT", 3);   

//-------------------------------END Draw KEYS----------------------------------------------------
    
    SW1_btn.drawButton(false);
    SW2_btn.drawButton(false);
    SW3_btn.drawButton(false);
    SW4_btn.drawButton(false);
    SW5_btn.drawButton(false);
    SW6_btn.drawButton(false);
    SW7_btn.drawButton(false);
    SW8_btn.drawButton(false);
    SW9_btn.drawButton(false);
    SW10_btn.drawButton(false);
    SW11_btn.drawButton(false);
    SW12_btn.drawButton(false);
    SW13_btn.drawButton(false);
    SW14_btn.drawButton(false);
    SW15_btn.drawButton(false);

    releaseKey();
}


void loop(void)
{
    bool down = Touch_getXY();

//---------------------------------------------------------------------------------

    SW1_btn.press(down && SW1_btn.contains(pixel_x, pixel_y));
    SW2_btn.press(down && SW2_btn.contains(pixel_x, pixel_y));
    SW3_btn.press(down && SW3_btn.contains(pixel_x, pixel_y));
    SW4_btn.press(down && SW4_btn.contains(pixel_x, pixel_y));
    SW5_btn.press(down && SW5_btn.contains(pixel_x, pixel_y));
    SW6_btn.press(down && SW6_btn.contains(pixel_x, pixel_y));
    SW7_btn.press(down && SW7_btn.contains(pixel_x, pixel_y));
    SW8_btn.press(down && SW8_btn.contains(pixel_x, pixel_y));
    SW9_btn.press(down && SW9_btn.contains(pixel_x, pixel_y));
    SW10_btn.press(down && SW10_btn.contains(pixel_x, pixel_y));
    SW11_btn.press(down && SW11_btn.contains(pixel_x, pixel_y));
    SW12_btn.press(down && SW12_btn.contains(pixel_x, pixel_y));
    SW13_btn.press(down && SW13_btn.contains(pixel_x, pixel_y));
    SW14_btn.press(down && SW14_btn.contains(pixel_x, pixel_y));
    SW15_btn.press(down && SW15_btn.contains(pixel_x, pixel_y));

//--------------------------------------------------------------
    
    if (SW1_btn.justReleased())
        SW1_btn.drawButton();

    if (SW2_btn.justReleased())
        SW2_btn.drawButton();
        
    if (SW3_btn.justReleased())
        SW3_btn.drawButton();

    if (SW4_btn.justReleased())
        SW4_btn.drawButton();

    if (SW5_btn.justReleased())
        SW5_btn.drawButton();

    if (SW6_btn.justReleased())
        SW6_btn.drawButton();

    if (SW7_btn.justReleased())
        SW7_btn.drawButton();

    if (SW8_btn.justReleased())
        SW8_btn.drawButton();

    if (SW9_btn.justReleased())
        SW9_btn.drawButton();

    if (SW10_btn.justReleased())
        SW10_btn.drawButton();

    if (SW11_btn.justReleased())
        SW11_btn.drawButton();

    if (SW12_btn.justReleased())
        SW12_btn.drawButton();

    if (SW13_btn.justReleased())
        SW13_btn.drawButton();

    if (SW14_btn.justReleased())
        SW14_btn.drawButton();

    if (SW15_btn.justReleased())
        SW15_btn.drawButton();
       
// --------------Key Press Output to USB------------------------
        
    if (SW1_btn.justPressed()) {
        SW1_btn.drawButton(true);
        buf[2] = KEY_N1;
        Serial.write(buf,8);
        releaseKey();
    }
    
    if (SW2_btn.justPressed()) {
        SW2_btn.drawButton(true);
        buf[2] = KEY_N2;
        Serial.write(buf,8);
        releaseKey();
    }

    if (SW3_btn.justPressed()) {
        SW3_btn.drawButton(true);
        buf[2] = KEY_N3;
        Serial.write(buf,8);
        releaseKey();
    }

    if (SW4_btn.justPressed()) {
        SW4_btn.drawButton(true);
        buf[2] = KEY_N4;
        Serial.write(buf,8);
        releaseKey();
    }

    if (SW5_btn.justPressed()) {
        SW5_btn.drawButton(true);
        buf[2] = KEY_N5;
        Serial.write(buf,8);
        releaseKey();
    }

    if (SW6_btn.justPressed()) {
        SW6_btn.drawButton(true);
        buf[2] = KEY_N6;
        Serial.write(buf,8);
        releaseKey();
    }
    
    if (SW7_btn.justPressed()) {
        SW7_btn.drawButton(true);
        buf[2] = KEY_N7;
        Serial.write(buf,8);
        releaseKey();
    }

    if (SW8_btn.justPressed()) {
        SW8_btn.drawButton(true);
        buf[2] = KEY_N8;
        Serial.write(buf,8);
        releaseKey();
    }
    
     if (SW9_btn.justPressed()) {
        SW9_btn.drawButton(true);
        buf[2] = KEY_N9;
        Serial.write(buf,8);
        releaseKey();
    }

     if (SW10_btn.justPressed()) {
        SW10_btn.drawButton(true);
        buf[2] = KEY_F1;
        Serial.write(buf,8);
        releaseKey();
    }

     if (SW11_btn.justPressed()) {
        SW11_btn.drawButton(true);
        buf[2] = KEY_F2;
        Serial.write(buf,8);
        releaseKey();
    }

     if (SW12_btn.justPressed()) {
        SW12_btn.drawButton(true);
        buf[2] = KEY_F3;
        Serial.write(buf,8);
        releaseKey();
    }

     if (SW13_btn.justPressed()) {
        SW13_btn.drawButton(true);
        buf[2] = KEY_F4;
        Serial.write(buf,8);
        releaseKey();
    }
    
     if (SW14_btn.justPressed()) {
        SW14_btn.drawButton(true);
        buf[2] = KEY_F5;
        Serial.write(buf,8);
        releaseKey();
    }

     if (SW15_btn.justPressed()) {
        SW15_btn.drawButton(true);
        buf[2] = KEY_F6;
        Serial.write(buf,8);
        releaseKey();
    }
    
//**------------------End Key Press Output-----------------------------

   
}

void releaseKey ()
  {
    delay(150);
    
      buf[0] =0;
      buf[2] =0;
      Serial.write(buf, 8); //Release Key ********************

  }
 
 #endif
