#ifndef _DONE_EMLOCK_CODE_
#define _DONE_EMLOCK_CODE_

#include "Library_and_pin.h"

//****************************************Game SETUP****************************************
void (*game_ptr)() = Game_Void;

void Game_Void();

//****************************************Neopixel SETUP****************************************
void NeopixelInit();
void AllNeoColor(int color_code);
void NeoBlink(int neo_code, int color_code, int blink_num, int blink_time);

const int NeoNum = 3;
const int NumPixels[3] = {28,24,24};

enum {PN532 = 0, INNER, ENCODER};
Adafruit_NeoPixel pixels[3] = {Adafruit_NeoPixel(NumPixels[PN532], PN532_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800),
                               Adafruit_NeoPixel(NumPixels[INNER], ENCODER_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800),
                               Adafruit_NeoPixel(NumPixels[ENCODER], INNER_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800)};

enum {WHITE = 0, RED, YELLOW, GREEN, BLUE, PURPLE, BLACK, BLUE0, BLUE1, BLUE2, BLUE3};     // Neopixel 색상정보
int color[11][3] = {    {20, 20, 20},   //WHITE
                        {40, 0, 0},     //RED
                        {40, 40, 0},    //YELLOW
                        {0, 40, 0},     //GREEN
                        {0, 0, 40},     //BLUE
                        {40, 0, 40},    //PURPLE
                        {0, 0, 0},      //BLACK
                        {0, 0, 20},     //ENCODERBLUE0
                        {0, 0, 40},     //ENCODERBLUE1
                        {0, 0, 60},     //ENCODERBLUE2
                        {0, 0, 80}};    //ENCODERBLUE3

//****************************************RFID SETUP****************************************
enum {OUTPN532 = 0, INPN532};
Adafruit_PN532 nfc[2] = {Adafruit_PN532(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS1),
                         Adafruit_PN532(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS2)};

//****************************************Encoder SETUP****************************************
long readEncoderValue(void);
boolean isButtonPushDown(void);

void Encoder_Setup();
void Encoder_Loop();
void Encoder_RevCount();
void updateEncoder();

volatile int lastEncoded = 0;
volatile long encoderValue = 0;
long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;

int RawVal = 0;
int Player_guess = 0;
volatile int encoder_revcount[2] = {0, 0};

void Quiz_System();
void Quiz_Check();

int QuizCount = 0;
int Quiz_answer[3] = {10, 40, 27};                      // 퀴즈 [1,2,3] 정답 (0~47)

//****************************************Nextion SETUP****************************************
HardwareSerial nexHwSerial(2);


#endif