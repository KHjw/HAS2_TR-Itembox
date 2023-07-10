#line 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\Library_and_pin.h"
#ifndef _LIBRARY_AND_PIN_
#define _LIBRARY_AND_PIN_

#include <Arduino.h>

#include <Adafruit_PN532.h>
#include <Adafruit_NeoPixel.h>

#include <Nextion.h>
#include <HAS2_MQTT.h>
#include <SimpleTimer.h>

#define NEXTIONHWSERIAL_RX_PIN 39
#define NEXTIONHWSERIAL_TX_PIN 33

#define PN532_SCK 18
#define PN532_MISO 19
#define PN532_MOSI 23
#define PN532_SS1 5
#define PN532_SS2 21

#define PN532_NEOPIXEL_PIN 25
#define ENCODER_NEOPIXEL_PIN 26 
#define INNER_NEOPIXEL_PIN 27

#define MOTOR_INA1_PIN  32
#define MOTOR_INA2_PIN  4
#define MOTOR_PWMA_PIN  22

#define RELAY_PIN  14               //MOSFET으로 변경

#define BOXSWITCH_PIN  36

#define encoderPinA 13
#define encoderPinB 15
#define buttonPin   34

#define VIBRATION_RANGE_PIN     14
#define VIBRATION_ANSWER_PIN    12

#endif