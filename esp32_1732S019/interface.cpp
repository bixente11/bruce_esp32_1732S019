#include "core/powerSave.h"
#include <driver/adc.h>
#include <esp_adc_cal.h>
#include <globals.h>
#include <interface.h>
#include <soc/adc_channel.h>
#include <soc/soc_caps.h>

#include <RotaryEncoder.h>
// extern RotaryEncoder encoder;
extern RotaryEncoder *encoder;
IRAM_ATTR void checkPosition();

/***************************************************************************************
** Function name: _setup_gpio()
** Location: main.cpp
** Description:   initial setup for the device
***************************************************************************************/

// Power handler for battery detection
#include <Wire.h>
//#include <XPowersLib.h>
//XPowersPPM PPM;

void _setup_gpio() {

    pinMode(SEL_BTN, INPUT_PULLUP); // Sets the power btn as an INPUT
    //pinMode(BK_BTN, INPUT);

    pinMode(CC1101_SS_PIN, OUTPUT);
    pinMode(NRF24_SS_PIN, OUTPUT);

    digitalWrite(CC1101_SS_PIN, HIGH);
    digitalWrite(NRF24_SS_PIN, HIGH);
    // Starts SPI instance for CC1101 and NRF24 with CS pins blocking communication at start

    bruceConfig.rfModule = CC1101_SPI_MODULE;
    bruceConfig.irRx = RXLED;
    Wire.setPins(GROVE_SDA, GROVE_SCL);
    Wire.begin();

    pinMode(ENCODER_KEY, INPUT);
    // use TWO03 mode when PIN_IN1, PIN_IN2 signals are both LOW or HIGH in latch position.
    encoder = new RotaryEncoder(ENCODER_INA, ENCODER_INB, RotaryEncoder::LatchMode::FOUR3);

    // register interrupt routine
    attachInterrupt(digitalPinToInterrupt(ENCODER_INA), checkPosition, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_INB), checkPosition, CHANGE);
}

/***************************************************************************************
** Function name: getBattery()
** location: display.cpp
** Description:   Delivers the battery value from 1-100+
***************************************************************************************/
int getBattery() { return false; }

/*********************************************************************
** Function: setBrightness
** location: settings.cpp
** set brightness value
**********************************************************************/
void _setBrightness(uint8_t brightval) {
    if (brightval == 0) {
        analogWrite(TFT_BL, brightval);
    } else {
        int bl = MINBRIGHT + round(((255 - MINBRIGHT) * brightval / 100));
        analogWrite(TFT_BL, bl);
    }
}

// RotaryEncoder encoder(ENCODER_INA, ENCODER_INB, RotaryEncoder::LatchMode::TWO03);
RotaryEncoder *encoder = nullptr;
IRAM_ATTR void checkPosition() {
    encoder->tick(); // just call tick() to check the state.
}

/*********************************************************************
** Function: InputHandler
** Handles the variables PrevPress, NextPress, SelPress, AnyKeyPress and EscPress
**********************************************************************/
void InputHandler(void) {
    static unsigned long lastBtnPress = 0;
    static bool isBtnPressed = false;
    static unsigned long tm2 = millis();

    static int _last_dir = 0;
    static bool waitingSecondClick = false;
    static unsigned long firstClickTime = 0;

    bool selRaw = digitalRead(SEL_BTN) == BTN_ACT;

    // Gestion rotation encodeur
    _last_dir = (int)encoder->getDirection();
    if (_last_dir != 0) {
        if (!wakeUpScreen()) {
            AnyKeyPress = true;
            if (_last_dir > 0) PrevPress = true;
            if (_last_dir < 0) NextPress = true;
            _last_dir = 0;
            tm2 = millis();
        }
    }

    // Appui du bouton
    if (selRaw && !isBtnPressed) {
        lastBtnPress = millis();
        isBtnPressed = true;
    }

    // Relâchement
    if (!selRaw && isBtnPressed) {
        unsigned long pressDuration = millis() - lastBtnPress;

        if (pressDuration >= 40 && pressDuration < 600) {
            if (waitingSecondClick && millis() - firstClickTime < 250) {
                EscPress = true;
                AnyKeyPress = true;
                waitingSecondClick = false;
                Serial.println("Esc (double click)");
            } else {
                waitingSecondClick = true;
                firstClickTime = millis();
            }
        }

        isBtnPressed = false;
    }

    // Si délai de second clic dépassé, c’était un simple clic
    if (waitingSecondClick && (millis() - firstClickTime >= 250)) {
        if (millis() - tm2 > 200) {
            SelPress = true;
            AnyKeyPress = true;
            tm2 = millis();
        }
        waitingSecondClick = false;
    }
}
