#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\롤백코드\\아박 롤백\\HAS2_TR-Itembox\\TR_Itembox.h"
#ifndef _DONE_EMLOCK_CODE_
#define _DONE_EMLOCK_CODE_

#include "Library_and_pin.h"

//****************************************MQTT SETUP****************************************
HAS2_MQTT has2_mqtt;

//****************************************Game SETUP****************************************
// game_ptr 함수
void Game_Void();
void Game_Manual();
void Game_Setting();
void Game_Ready();
void Game_Selected();
void Game_Login();
void Game_Quiz();
void Game_Opentag();
void Game_Itemtake();
void Game_Used();
void Game_ptrPrint(String print);
// rfid_ptr 함수
void Quiz_Start();
void Quiz_Solved();
void Item_Took();

void (*game_ptr)() = Game_Manual;
void (*rfid_ptr)() = Game_Void;

String Language = "Kor";
String game_ptr_state = "";
bool ItemBoxSelfOpen = false;
bool ItemBoxUsed = false;

// 게임 변수 세팅
int Quiz_answer_num = 3;                                // 퀴즈 정답 갯수
int Quiz_answer[3] = {13, 43, 21};                      // 퀴즈 [1,2,3] 정답 (0~47)
int Answer_Range = 2;                                   // 정답 범위
int Vibration_Range = 5;                                // 진동 범위
int Vibration_Strength[5] = {200,150,100,50,0};         // 진동 세기

//****************************************SimpleTimer SETUP****************************************
SimpleTimer BlinkTimer;
SimpleTimer GameTimer;

int BlinkTimerId;
int GameTimerId;
unsigned long BlinkTime = 1000;     // 1sec
unsigned long GameTime = 5000;      // 5sec
int GameTimeCNT = 0;                // 게임카운터 - 미조작시 초기화를 위함
int GameTimeCNT_Max = 6;            // 30초 카운팅 (5sec*6) 

void TimerInit();

void BlinkTimerStart(int Neo, int NeoColor);
void BlinkTimerFunc();
int blinkNeo = 0;
int blinkColor = 0;
bool blinkOn = false;

//****************************************Neopixel SETUP****************************************
void NeopixelInit();
void AllNeoColor(int color_code);
void NeoBlink(int neo_code, int color_code, int blink_num, int blink_time);
void Encoder_PointNeo();

const int NeoNum = 3;
enum {PN532 = 0, INNER, ENCODER, ALLNEO};
const int NumPixels[3] = {28,24,24};
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

//****************************************Encoder SETUP****************************************
long readEncoderValue(void);
boolean isButtonPushDown(void);

void EncoderInit();
void Encoder_Loop();
void Encoder_Print();
void Encoder_RevCount();
void updateEncoder();

volatile int lastEncoded = 0;
volatile long encoderValue = 41*4;          // 엔코더 초기값 _ 2rev 맨위

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