#include "core/powerSave.h"
#include <driver/adc.h>
#include <esp_adc_cal.h>
#include <globals.h>
#include <interface.h>
#include <soc/adc_channel.h>
#include <soc/soc_caps.h>

#include <RotaryEncoder.h>
extern RotaryEncoder *encoder;
IRAM_ATTR void checkPosition();

/***************************************************************************************
** Function name: _setup_gpio()
** Location: main.cpp
** Description:   initial setup for the device
***************************************************************************************/

#include <Wire.h>

void _setup_gpio() {

    pinMode(SEL_BTN, INPUT_PULLUP); // Sets the power btn as an INPUT
    

    pinMode(CC1101_SS_PIN, OUTPUT);
    pinMode(NRF24_SS_PIN, OUTPUT);

    digitalWrite(CC1101_SS_PIN, HIGH);
    digitalWrite(NRF24_SS_PIN, HIGH);
    // Starts SPI instance for CC1101 and NRF24 with CS pins blocking communication at start

    bruceConfig.rfModule = CC1101_SPI_MODULE;
    bruceConfig.irRx = RXLED;
    Wire.setPins(GROVE_SDA, GROVE_SCL);
    Wire.begin();
    
    pinMode(BK_BTN, INPUT_PULLUP);
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
    static unsigned long tm = 0;
    if (millis() - tm < 200 && !LongPress) return;

    // lecture boutons
    bool _s = digitalRead(SEL_BTN);  // bouton central
    bool _e = digitalRead(BK_BTN);   // bouton ESC

    // lecture encodeur (direction)
    int dir = encoder ? (int)encoder->getDirection() : 0;

    // si une action est détectée (bouton ou encodeur)
    if (!_s || !_e || dir != 0) {
        tm = millis();
        if (!wakeUpScreen()) {
            AnyKeyPress = true; // juste réveil
        } else {
            return; // si réveil → on sort, pas d’action
        }
    }

    // gestion encodeur
    if (dir > 0) {
        PrevPress = true;
    }
    if (dir < 0) {
        NextPress = true;
    }

    // gestion bouton Select
    if (!_s) {
        SelPress = true;
    }

    // gestion bouton ESC
    if (!_e) {
        EscPress = true;
    }
}
