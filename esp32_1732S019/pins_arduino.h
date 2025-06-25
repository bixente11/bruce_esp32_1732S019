#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include "soc/soc_caps.h"
#include <stdint.h>

static const uint8_t TX = 43;
static const uint8_t RX = 44;

static const uint8_t SDA = 47;
static const uint8_t SCL = 48;

// Modified elsewhere
#define SPI_SCK_PIN 17
#define SPI_MOSI_PIN 15
#define SPI_MISO_PIN 16
#define SPI_SS_PIN 19
static const uint8_t SS = SPI_SS_PIN;
static const uint8_t MOSI = SPI_MOSI_PIN;
static const uint8_t SCK = SPI_MISO_PIN;
static const uint8_t MISO = SPI_SCK_PIN;

#define SERIAL_RX 2
#define SERIAL_TX 1
#define BAD_RX SERIAL_RX
#define BAD_TX SERIAL_TX
#define USB_as_HID 1

// Encoder
#define HAS_ENCODER
#define ENCODER_INA 40
#define ENCODER_INB 41
#define ENCODER_KEY 42
#define HAS_BTN 1
#define BTN_ALIAS "\"Mid\""
#define SEL_BTN ENCODER_KEY
#define UP_BTN -1
#define DW_BTN -1
// #define BK_BTN 38     // push button encoder - Back -- double clic
#define BTN_ACT LOW

#define RXLED 4
#define LED 5
#define LED_ON HIGH
#define LED_OFF LOW

#define USE_CC1101_VIA_SPI
#define CC1101_GDO0_PIN 38
#define CC1101_GDO2_PIN 45
#define CC1101_SS_PIN 6
#define CC1101_MOSI_PIN SPI_MOSI_PIN
#define CC1101_SCK_PIN SPI_SCK_PIN
#define CC1101_MISO_PIN SPI_MISO_PIN

//#define USE_NRF24_VIA_SPI
//#define NRF24_CE_PIN 18
//#define NRF24_SS_PIN 19
//#define NRF24_MOSI_PIN SPI_MOSI_PIN
//#define NRF24_SCK_PIN SPI_SCK_PIN
//#define NRF24_MISO_PIN SPI_MISO_PIN

#define FP 1
#define FM 2
#define FG 3

#define HAS_SCREEN 1
#define ROTATION 1
#define MINBRIGHT (uint8_t)1

#define USER_SETUP_LOADED 1
#define ST7789_DRIVER 1
#define TFT_RGB_ORDER 0
#define TFT_WIDTH 170
#define TFT_HEIGHT 320
#define TFT_BACKLIGHT_ON 1
#define TFT_BL 14
#define TFT_RST 1
#define TFT_DC 11
#define TFT_MISO -1
#define TFT_MOSI 13
#define TFT_SCLK 12
#define TFT_CS 10
#define TOUCH_CS -1 // SDCARD_CS to make sure SDCard works
#define SMOOTH_FONT 1
#define SPI_FREQUENCY 40000000
#define SPI_READ_FREQUENCY 20000000

#define SDCARD_CS 21
#define SDCARD_SCK 17
#define SDCARD_MISO 16
#define SDCARD_MOSI 15

// === I2C Grove ===
#define GROVE_SDA SDA
#define GROVE_SCL SCL

#endif /* Pins_Arduino_h */
