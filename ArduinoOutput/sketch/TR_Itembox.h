#line 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\TR_Itembox.h"
#ifndef _DONE_EMLOCK_CODE_
#define _DONE_EMLOCK_CODE_

#include "Library_and_pin.h"

//****************************************Game SETUP****************************************
void Game_Ready();
void Game_Selected();
void Game_OUTPN532_login();
void Game_Quiz();
void Game_OUTPN532_open();
void Game_InnerTag();
void Game_Used();
void Game_Void();

void (*game_ptr)() = Game_Void;
void (*rfid_ptr)() = Game_Void;

String Language = "Kor";
bool ItemBoxSelfOpen = false;
bool ItemBoxUsed = false;

// 게임 변수 세팅
int Quiz_answer_num = 3;                                // 퀴즈 정답 갯수
int Quiz_answer[3] = {10, 40, 27};                      // 퀴즈 [1,2,3] 정답 (0~47)
int Answer_Range = 3;                                   // 정답 범위
int Vibration_Range = 5;                                // 진동 범위
int Vibration_Strength[5] = {200,150,100,50,0};         // 진동 세기

//****************************************SimpleTimer SETUP****************************************
SimpleTimer BlinkTimer;
void TimerInit();

void BlinkTimerStart(int Neo, int NeoColor);
void BlinkTimerFunc();
int blinkNeo = 0;
int blinkColor = 0;
bool blinkOn = false;

int blinkTimerId;

unsigned long blinkTime = 1300;   // 1sec

//****************************************Neopixel SETUP****************************************
void NeopixelInit();
void AllNeoColor(int color_code);
void NeoBlink(int neo_code, int color_code, int blink_num, int blink_time);
void Encoder_PointNeo();

const int NeoNum = 3;
const int NumPixels[3] = {28,24,24};

enum {PN532 = 0, INNER, ENCODER};
Adafruit_NeoPixel pixels[3] = {Adafruit_NeoPixel(NumPixels[PN532], PN532_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800),
                               Adafruit_NeoPixel(NumPixels[INNER], ENCODER_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800),
                               Adafruit_NeoPixel(NumPixels[ENCODER], INNER_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800)};

enum {WHITE = 0, RED, YELLOW, GREEN, BLUE, PURPLE, BLACK, BLUE0, BLUE1, BLUE2, BLUE3};     // Neopixel 색상정보
int color[11][3] = {  {20, 20, 20},   //WHITE
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
int rfid_num = 2;
enum {OUTPN532 = 0, INPN532};
Adafruit_PN532 nfc[2] = {Adafruit_PN532(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS1),
                         Adafruit_PN532(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS2)};

bool rfid_init_complete[2];
void RfidInit();
void RfidLoop(int pn532_code);
void RfidLoop_All();
void CheckingPlayers(uint8_t rfidData[32]);

void Quiz_Start();
void Quiz_Solved();
void Item_Took();

//****************************************Encoder SETUP****************************************
long readEncoderValue(void);
boolean isButtonPushDown(void);

void EncoderInit();
void Encoder_Loop();
void Encoder_RevCount();
void updateEncoder();

volatile int lastEncoded = 0;
volatile long encoderValue = 0;
long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;

int RawVal = 0;
volatile int encoder_revcount[2] = {0, 0};

void Quiz_System();
void Quiz_Check();

int QuizCount = 0;

//****************************************Nextion SETUP****************************************
HardwareSerial nexHwSerial(2);

void NextionInit();

//****************************************MOTOR SETUP****************************************************************
const int MotorFreq = 5000;
const int MotorResolution = 8;
const int MotorLedChannel = 3;
const int VibrationLedChannel = 4;
const int MotorMAX_DUTY_CYCLE = (int)(pow(2, MotorResolution) - 1);
const int VibtationMAX_DUTY_CYCLE = (int)(pow(2, MotorResolution) - 1);

void MotorInit();
void BoxClose();
void BoxOpen();
void MotorStop();
void Encoder_VibrationStrength(int answer);

#endif