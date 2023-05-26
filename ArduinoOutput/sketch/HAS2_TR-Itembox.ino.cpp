#include <Arduino.h>
#line 1 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\HAS2_TR-Itembox.ino"
 /*
 *
 * @file TR_Itembox.ino
 * @author 홍진우 (kevinlike@naver.com)
 * @brief
 * @version 1.0
 * @date 2023-05-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "TR_Itembox.h"

#line 15 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\HAS2_TR-Itembox.ino"
void setup();
#line 24 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\HAS2_TR-Itembox.ino"
void loop();
#line 1 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\Game.ino"
void Game_Ready();
#line 5 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\Game.ino"
void Game_Selected();
#line 9 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\Game.ino"
void Game_OutTag_login();
#line 13 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\Game.ino"
void Game_Quiz();
#line 17 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\Game.ino"
void Game_OutTag_open();
#line 21 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\Game.ino"
void Game_InTag_open();
#line 25 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\Game.ino"
void Game_Used();
#line 1 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\encoder.ino"
long readEncoderValue(void);
#line 5 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\encoder.ino"
boolean isButtonPushDown(void);
#line 14 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\encoder.ino"
void Encoder_Setup();
#line 26 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\encoder.ino"
void Encoder_Loop();
#line 45 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\encoder.ino"
void Encoder_RevCount();
#line 72 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\encoder.ino"
void updateEncoder();
#line 1 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\neopixel.ino"
void NeopixelInit();
#line 7 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\neopixel.ino"
void NeoShowColor(int neo_code ,int color_code);
#line 17 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\neopixel.ino"
void EncoderPointNeo();
#line 15 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\HAS2_TR-Itembox.ino"
void setup() {
  Serial.begin(115200);
  // has2wifi.Setup("city");
  // has2wifi.Setup("badland");
  // has2wifi.Setup();
  has2wifi.Setup("KT_GiGA_6C64","ed46zx1198");
  NeopixelInit();
}

void loop() {

}

#line 1 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\Game.ino"
void Game_Ready(){
  
}

void Game_Selected(){
  
}

void Game_OutTag_login(){
  
}

void Game_Quiz(){
  
}

void Game_OutTag_open(){
  
}

void Game_InTag_open(){
  
}

void Game_Used(){
   
}
#line 1 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\encoder.ino"
long readEncoderValue(void){
    return encoderValue/4;
}

boolean isButtonPushDown(void){
  if(!digitalRead(buttonPin)){
    delay(5);
    if(!digitalRead(buttonPin))
      return true;
  }
  return false;
}

void Encoder_Setup(){
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(buttonPin, INPUT);

  digitalWrite(encoderPinA, HIGH); //turn pullup resistor on
  digitalWrite(encoderPinB, HIGH); //turn pullup resistor on

  attachInterrupt(encoderPinA, updateEncoder, CHANGE);
  attachInterrupt(encoderPinB, updateEncoder, CHANGE);
}

void Encoder_Loop(){                                // "encoder값, 버튼눌림" 을 표시
  Encoder_RevCount();
  EncoderPointNeo();
  Serial.print(readEncoderValue());
  Serial.print(" / ");
  Serial.print(encoder_revcount[0]);
  Serial.print(", ");
  Serial.print(encoder_revcount[1]);

  if(isButtonPushDown()){
    Serial.print(" / 1");
  }else{
    Serial.print(" / 0");
  }

  Serial.println("");
  delay(50);
}

void Encoder_RevCount(){
  RawVal = readEncoderValue();

  if(RawVal < 0){
    RawVal = 0;
    encoder_revcount[0] = 1;
    encoder_revcount[1] = 0;
  }
  else if(RawVal >= 0 && RawVal < 16){
    encoder_revcount[0] = 1;
    encoder_revcount[1] = RawVal;
  }
  else if(RawVal >= 16 && RawVal < 32){
    encoder_revcount[0] = 2;
    encoder_revcount[1] = RawVal - 16;
  }
  else if(RawVal >= 32 && RawVal < 48){
    encoder_revcount[0] = 3;
    encoder_revcount[1] = RawVal - 32;
  }
  else{
    RawVal = 47;
    encoder_revcount[0] = 3;
    encoder_revcount[1] = 15;
  }
}

void updateEncoder(){
  int MSB = digitalRead(encoderPinA); //MSB = most significant bit
  int LSB = digitalRead(encoderPinB); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue --;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue ++;


  lastEncoded = encoded; //store this value for next time
}
#line 1 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\mqtt.ino"

#line 1 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\neopixel.ino"
void NeopixelInit(){
  for(int i=0; i<NeoNum; i++){
    pixels[i].begin();
  }
}

void NeoShowColor(int neo_code ,int color_code){
  for(int i=0; i<NumPixels[neo_code]; i++){
    pixels[neo_code].setPixelColor(i, 0, 0, 0);
  }
  for(int i=0; i<NumPixels[neo_code]; i++){
    pixels[neo_code].lightColor(color[color_code]);
  }
  pixels[neo_code].show();
}

void EncoderPointNeo(){
  int rev = 0;
  int point = 0;
  rev = encoder_revcount[0];
  point = encoder_revcount[1];

  int rev_neo = BLUE0;                                  
  rev_neo = BLUE0 + (rev-1);                            // 나머지 구역 네오픽셀 :: 1번째 바퀴부터 BLUE0,1,2

  pixels[ENCODER].setPixelColor(point, color[RED]);     // 엔코더 위치 point 네오픽셀

  for(int i=0; i<point; i++){
    pixels[ENCODER].setPixelColor(i, color[rev_neo]);   // 나머지 구역 네오픽셀
  }
  for(int i=(point+1); i<NumPixels[ENCODER]; i++){
    pixels[ENCODER].setPixelColor(i, color[rev_neo]);
  }

  pixels[ENCODER].show();
}
#line 1 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\nextion.ino"

#line 1 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\rfid.ino"

