#define USE_TINYUSB
#include <Arduino.h>
#include <Wire.h>
#include "../.pio/libdeps/seeed_xiao/Grove - AS5600/src/AS5600.h"
#include "../.pio/libdeps/seeed_xiao/TinyUSB Mouse and Keyboard/TinyUSB_Mouse_and_Keyboard.h"

AMS_5600 ams5600;

int ang, lang = 0;

int maxValue = 4096;
int split = 16;
int ignore = 12;
int triggerDiff = 32;
int lastReport = 0;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>> ");
    if (ams5600.detectMagnet() == 0) {
        while (true) {
            if (ams5600.detectMagnet() == 1) {
                Serial.print("Current Magnitude: ");
                Serial.println(ams5600.getMagnitude());
                break;
            } else {
                Serial.println("Can not detect magnet");
            }
            delay(1000);
        }
    }
    Keyboard.begin();
    lastReport = ams5600.getRawAngle();
}

int getDiff(int v1, int v2) {
    if (abs(v1 - v2) > (maxValue / 2)) {
        if (v1 > v2) {
            v1 = v1 - maxValue;
        } else {
            v2 = v2 - maxValue;
        }
    }
    return v1 - v2;
}

void loop() {
    ang = ams5600.getRawAngle();
    //no changes
    if (ang == lang) {
        return;
    }

    //debounce
    if (abs(getDiff(ang, lang)) > ignore * split) {
        return;
    }
//    Serial.println(String(ang, DEC));
    lang = ang;

    int reportDiff = getDiff(ang, lastReport);
    if (abs(reportDiff) >= triggerDiff * split) {
        Serial.println(String(reportDiff, DEC));
        Serial.println(String(lastReport, DEC));
        lastReport = ang;
        if (reportDiff > 0) {
            Serial.println("CW");
            Keyboard.write(KEY_DOWN_ARROW);

        } else {
            Serial.println("CCW");
            Keyboard.write(KEY_UP_ARROW);

        }
    }
}