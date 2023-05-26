# 1 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\HAS2_TR-Itembox.ino"
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
# 13 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\HAS2_TR-Itembox.ino"
# 14 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\HAS2_TR-Itembox.ino" 2

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
# 1 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\Game.ino"
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
# 1 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\encoder.ino"
long readEncoderValue(void){
    return encoderValue/4;
}

boolean isButtonPushDown(void){
  if(!digitalRead(34)){
    delay(5);
    if(!digitalRead(34))
      return true;
  }
  return false;
}

void Encoder_Setup(){
  pinMode(13, 0x01);
  pinMode(15, 0x01);
  pinMode(34, 0x01);

  digitalWrite(13, 0x1); //turn pullup resistor on
  digitalWrite(15, 0x1); //turn pullup resistor on

  attachInterrupt(13, updateEncoder, 0x03);
  attachInterrupt(15, updateEncoder, 0x03);
}

void Encoder_Loop(){ // "encoder값, 버튼눌림" 을 표시
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
  int MSB = digitalRead(13); //MSB = most significant bit
  int LSB = digitalRead(15); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue --;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue ++;


  lastEncoded = encoded; //store this value for next time
}
# 1 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\mqtt.ino"
# 1 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\neopixel.ino"
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
  rev_neo = BLUE0 + (rev-1); // 나머지 구역 네오픽셀 :: 1번째 바퀴부터 BLUE0,1,2

  pixels[ENCODER].setPixelColor(point, color[RED]); // 엔코더 위치 point 네오픽셀

  for(int i=0; i<point; i++){
    pixels[ENCODER].setPixelColor(i, color[rev_neo]); // 나머지 구역 네오픽셀
  }
  for(int i=(point+1); i<NumPixels[ENCODER]; i++){
    pixels[ENCODER].setPixelColor(i, color[rev_neo]);
  }

  pixels[ENCODER].show();
}
# 1 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\nextion.ino"
# 1 "c:\\Users\\HAS1\\Desktop\\홍진우 git\\HAS2_TR\\HAS2_TR-Itembox\\rfid.ino"
