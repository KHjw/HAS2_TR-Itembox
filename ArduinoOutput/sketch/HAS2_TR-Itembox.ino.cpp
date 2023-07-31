#include <Arduino.h>
#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\HAS2_TR-Itembox.ino"
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

#line 15 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\HAS2_TR-Itembox.ino"
void setup();
#line 28 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\HAS2_TR-Itembox.ino"
void loop();
#line 2 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\encoder.ino"
long readEncoderValue(void);
#line 6 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\encoder.ino"
boolean isButtonPushDown(void);
#line 15 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\encoder.ino"
void EncoderInit();
#line 24 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\encoder.ino"
void updateEncoder();
#line 44 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\encoder.ino"
void Encoder_Loop();
#line 51 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\encoder.ino"
void Encoder_RevCount();
#line 77 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\encoder.ino"
void Encoder_Print();
#line 95 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\encoder.ino"
void Quiz_System();
#line 104 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\encoder.ino"
void Quiz_Check();
#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\motor.ino"
void MotorInit();
#line 19 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\motor.ino"
void BoxClose();
#line 28 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\motor.ino"
void BoxOpen();
#line 37 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\motor.ino"
void MotorStop();
#line 43 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\motor.ino"
void Encoder_VibrationStrength(int answer);
#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\mqtt.ino"
void callback(char* topic, byte* payload, unsigned int length);
#line 17 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\mqtt.ino"
void Mqtt_myDN(String input_data);
#line 24 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\mqtt.ino"
void Mqtt_updateDS(String myDS);
#line 43 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\mqtt.ino"
void Device_Manual();
#line 51 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\mqtt.ino"
void Device_MiniGame();
#line 59 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\mqtt.ino"
void Device_Setting();
#line 64 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\mqtt.ino"
void Device_Ready();
#line 69 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\mqtt.ino"
void Device_Selected();
#line 74 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\mqtt.ino"
void Device_Login();
#line 81 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\mqtt.ino"
void Device_Quiz();
#line 104 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\mqtt.ino"
void Device_Opentag();
#line 110 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\mqtt.ino"
void Device_Itemtake();
#line 116 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\mqtt.ino"
void Device_Used();
#line 122 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\mqtt.ino"
void Device_Stelth();
#line 127 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\mqtt.ino"
void VoidFunc();
#line 130 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\mqtt.ino"
void Device_ptrPrint(String print);
#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\neopixel.ino"
void NeopixelInit();
#line 8 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\neopixel.ino"
void AllNeoColor(int color_code);
#line 14 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\neopixel.ino"
void NeoBlink(int neo_code, int color_code, int blink_num, int blink_time);
#line 23 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\neopixel.ino"
void Encoder_PointNeo();
#line 39 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\neopixel.ino"
void Scenario_WaitBlink(int neoID);
#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\nextion.ino"
void NextionInit();
#line 6 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\nextion.ino"
void SendCmd(String command);
#line 13 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\nextion.ino"
void ExpSend(int exp);
#line 18 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\nextion.ino"
void BatteryPackSend(int bp);
#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\rfid.ino"
void RfidInit();
#line 26 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\rfid.ino"
void RfidLoop(int pn532_code);
#line 46 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\rfid.ino"
void RfidLoop_All();
#line 52 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\rfid.ino"
void Manual_PlayerCheck(uint8_t rfidData[32]);
#line 91 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\rfid.ino"
void Auto_PlayerCheck(uint8_t rfidData[32]);
#line 120 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\rfid.ino"
void MiniGame_PlayerCheck(uint8_t rfidData[32]);
#line 123 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\rfid.ino"
void CheckPlayerBatt(String tagDN);
#line 134 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\rfid.ino"
void Quiz_Start();
#line 149 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\rfid.ino"
void Quiz_Solved();
#line 172 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\rfid.ino"
void Item_Take();
#line 192 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\rfid.ino"
void Item_Took();
#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\timer.ino"
void TimerInit();
#line 9 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\timer.ino"
void BlinkTimerStart(int Neo, int NeoColor);
#line 16 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\timer.ino"
void BlinkTimerFunc();
#line 28 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\timer.ino"
void BlinkAllTimerFunc();
#line 44 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\timer.ino"
void GameTimerFunc();
#line 15 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\HAS2_TR-Itembox.ino"
void setup() {
  Serial.begin(115200);
  has2_mqtt.Setup(callback);         // tp-link 접속; ALL, myMAC 구독;
  // has2_mqtt.AddSubscirbe("EI2");

  TimerInit();
  NeopixelInit();
  EncoderInit();
  NextionInit();
  RfidInit();
  MotorInit();
}

void loop() {
  has2_mqtt.ReadSubscirbe();
  device_ptr();
  BlinkTimer.run();
  GameTimer.run();
}
#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\encoder.ino"
//**************************************************Encoder**************************************************
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

void EncoderInit(){
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(buttonPin, INPUT);

  digitalWrite(encoderPinA, HIGH); //turn pullup resistor on
  digitalWrite(encoderPinB, HIGH); //turn pullup resistor on
}

void updateEncoder(){
  GameTimeCNT = 0;        // 게임카운터 초기화

  int MSB = digitalRead(encoderPinA); //MSB = most significant bit
  int LSB = digitalRead(encoderPinB); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB;            //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded;  //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;


  lastEncoded = encoded; //store this value for next time

  //엔코더 값 최대최소 제한 걸어두기    
  if(encoderValue > NumPixels[ENCODER]*3*4)      encoderValue = NumPixels[ENCODER]*3*4;              
  else if(encoderValue < 0)                    encoderValue = 0;
}

void Encoder_Loop(){
  Encoder_RevCount();                                 // 엔코더 값 변환
  Encoder_PointNeo();                                 // 네오픽셀 값 반영
  Encoder_VibrationStrength(Quiz_answer[QuizCount]);  // 진동 값 반영
  // Encoder_Print();                                    // 엔코더 값 시리얼 출력
}

void Encoder_RevCount(){
  RawVal = encoderValue/4;
  if(RawVal < 0){
    RawVal = 0;
    encoder_revcount[0] = 1;
    encoder_revcount[1] = 0;
  }
  else if(RawVal >= 0 && RawVal < NumPixels[ENCODER]){
    encoder_revcount[0] = 1;
    encoder_revcount[1] = RawVal;
  }
  else if(RawVal >= NumPixels[ENCODER] && RawVal < NumPixels[ENCODER]*2){
    encoder_revcount[0] = 2;
    encoder_revcount[1] = RawVal - NumPixels[ENCODER];
  }
  else if(RawVal >= NumPixels[ENCODER]*2 && RawVal < NumPixels[ENCODER]*3){
    encoder_revcount[0] = 3;
    encoder_revcount[1] = RawVal - NumPixels[ENCODER]*2;
  }
  else{
    RawVal = NumPixels[ENCODER]*3-1;
    encoder_revcount[0] = 3;
    encoder_revcount[1] = NumPixels[ENCODER]-1;
  }
}

void Encoder_Print(){              // "encoder값, 버튼눌림" 을 시리얼 출력
  Serial.print(RawVal);
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

//**************************************************Quiz**************************************************
void Quiz_System(){
  if(isButtonPushDown()){
    Quiz_Check();
  }
  else{
    Encoder_Loop();
  }
}

void Quiz_Check(){
  int Player_guess = RawVal;
  int DifferenceValue = abs(Quiz_answer[QuizCount] - Player_guess);

  if(DifferenceValue < Answer_Range){
    NeoBlink(ENCODER, GREEN, 5, 250);
    QuizCount ++;
    Serial.println("[[[[Quiz " + (String)(QuizCount) + "]]]] Success");
  }
  else{
    int QuizNum = QuizCount + 1;
    Serial.print("Quiz Fail ====> Player Guess :: " + (String)(Player_guess));        // 플레이어 선택
    Serial.println("// Q" + (String)(QuizNum) + " ANS :: " + (String)(Quiz_answer[QuizCount]));   // 퀴즈 정답
    NeoBlink(ENCODER, RED, 5, 250);
  }

  encoderValue = Player_guess*4;
  GameTimeCNT = 0;
}
#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\motor.ino"
void MotorInit(){
    //Linear Motor Init
    pinMode(BOXSWITCH_PIN,INPUT_PULLUP);
    pinMode(MOTOR_INA1_PIN, OUTPUT);
    pinMode(MOTOR_INA2_PIN, OUTPUT);
    ledcSetup(MotorLedChannel, MotorFreq, MotorResolution);
    ledcAttachPin(MOTOR_PWMA_PIN, MotorLedChannel);
    ledcWrite(MotorLedChannel, 0);
    BoxClose();
    
    //Vibration Motor Init
    pinMode(VIBRATION_ANSWER_PIN, OUTPUT);
    digitalWrite(VIBRATION_ANSWER_PIN, HIGH);
    ledcSetup(VibrationLedChannel, MotorFreq, MotorResolution);
    ledcAttachPin(VIBRATION_RANGE_PIN, VibrationLedChannel);
    ledcWrite(VibrationLedChannel, 0);
}

void BoxClose(){
    Serial.println("BOX Close");
    ledcWrite(MotorLedChannel, MotorMAX_DUTY_CYCLE - 1);
    digitalWrite(MOTOR_INA1_PIN, LOW);
    digitalWrite(MOTOR_INA2_PIN, HIGH);
    delay(4000);
    Serial.println("BOX Closed");
}

void BoxOpen(){
    Serial.println("BOX Open");
    ledcWrite(MotorLedChannel, MotorMAX_DUTY_CYCLE - 1);
    digitalWrite(MOTOR_INA1_PIN, HIGH);
    digitalWrite(MOTOR_INA2_PIN, LOW);
    delay(4000);
    Serial.println("BOX Opened");
}

void MotorStop(){
    Serial.println("모터 스탑");
    digitalWrite(MOTOR_INA1_PIN, LOW);
    digitalWrite(MOTOR_INA2_PIN, LOW);
}

void Encoder_VibrationStrength(int answer){
    int DifferenceValue = abs(answer - (encoderValue/4));
    int VibeStrength = 0;

    if(DifferenceValue < Answer_Range + Vibration_Range * 0)       VibeStrength = 0;    // 정답
    else if(DifferenceValue < Answer_Range + Vibration_Range * 1)  VibeStrength = 1;
    else if(DifferenceValue < Answer_Range + Vibration_Range * 2)  VibeStrength = 2;
    else if(DifferenceValue < Answer_Range + Vibration_Range * 3)  VibeStrength = 3;
    else VibeStrength = 4;
    ledcWrite(VibrationLedChannel, Vibration_Strength[VibeStrength]);
}

#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\mqtt.ino"
void callback(char* topic, byte* payload, unsigned int length){
  String input_data = "";

  for(int i=0; i<length; i++)
    input_data += (char)payload[i];
  has2_mqtt.SaveByTopic(topic, input_data);

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] : ");
  Serial.println(input_data);

  Mqtt_myDN(input_data);        // myDN 등록
  Mqtt_updateDS(has2_mqtt.GetData(myDN, "DS"));
}

void Mqtt_myDN(String input_data){
  if((char)myDN[1] != 'R'){
    for(int i=0; i<3; i++)
      myDN += (char)input_data[i];
  }
}

void Mqtt_updateDS(String myDS){
  if(myDS != device_ptr_state){
    IsScenarioMode = true;

    if(myDS == "OTA")               has2_mqtt.FirmwareUpdate("itembox");
    else if(myDS == "Setting")      device_ptr = Device_Setting;
    else if(myDS == "Ready")        device_ptr = Device_Ready;
    else if(myDS == "SCN_9")        device_ptr = Device_Selected;
    else if(myDS == "SCN_12")       device_ptr = Device_Login;
    else if(myDS == "SCN_15")       device_ptr = Device_Quiz;
    else if(myDS == "SCN_19_2")     device_ptr = Device_Opentag;
    else if(myDS == "SCN_22")       device_ptr = Device_Itemtake;
    else if(myDS == "Used")         device_ptr = Device_Used;
    else if(myDS == "Manual")       device_ptr = Device_Manual;
    else if(myDS == "MiniGame")     device_ptr = Device_MiniGame;
  }
}

//****************************************device_ptr Function****************************************
void Device_Manual(){
  IsScenarioMode = false;
  Serial.println("GameMode Set :: Manual");
  Device_ptrPrint("Manual");
  AllNeoColor(YELLOW);
  device_ptr = Device_Login;
}

void Device_MiniGame(){
  IsScenarioMode = false;
  Serial.println("GameMode Set :: MiniGame");
  Device_ptrPrint("MiniGame");
  AllNeoColor(YELLOW);
  device_ptr = Device_Login;
}

void Device_Setting(){
  Device_ptrPrint("Setting");
  AllNeoColor(WHITE);
}

void Device_Ready(){
  Device_ptrPrint("Ready");
  AllNeoColor(BLACK);
}

void Device_Selected(){
  Device_ptrPrint("SCN_9");
  RfidLoop(OUTPN532);
}

void Device_Login(){
  Device_ptrPrint("SCN_12");
  Scenario_WaitBlink(PN532);
  rfid_ptr = Quiz_Start;
  RfidLoop(OUTPN532);
}

void Device_Quiz(){
  Device_ptrPrint("SCN_15");
  if(!(QuizCount == 3)){
    Quiz_System();
  }
  else{                         //Quiz 3회 성공000
    Serial.println("CLEAR!!!");
    SendCmd("wQuizSolved.en=1");
    pixels[PN532].lightColor(color[YELLOW]);
    ledcWrite(VibrationLedChannel, 0);          // 진동모터 비활성화
    detachInterrupt(encoderPinA);               // 엔코더 비활성화
    detachInterrupt(encoderPinB);
    GameTimer.deleteTimer(GameTimerId);         // 타임아웃 게임타이머 비활성화
    GameTimeCNT = 0;
    GameTimerId = GameTimer.setInterval(GameTime, GameTimerFunc);
    BlinkTimer.deleteTimer(BlinkTimerId);       // 외부태그 노란색 점멸 시작
    BlinkTimerStart(PN532, YELLOW);

    QuizCount = 0;
    device_ptr = Device_Opentag;
  }
}

void Device_Opentag(){
  Device_ptrPrint("SCN_19_2");
  rfid_ptr = Quiz_Solved;
  RfidLoop(OUTPN532);
}

void Device_Itemtake(){
  Scenario_WaitBlink(INNER);
  Device_ptrPrint("SCN_22");
  RfidLoop(INPN532);
}

void Device_Used(){
  Device_ptrPrint("Used");
  RfidLoop(OUTPN532);
  RfidLoop(INPN532);
}

void Device_Stelth(){
  AllNeoColor(BLACK);
}

//포인터 초기세팅용 void함수
void VoidFunc(){}               

// 게임 포인터 상태 SerialPrint
void Device_ptrPrint(String print){
  if(print != device_ptr_state){
    if((char)print[3] == '_'){
      Serial.println("Training Scenario :: " + print);
    }
    else{
      Serial.println("Device State :: " + print);
    }
    device_ptr_state = print;
  }
}
#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\neopixel.ino"
void NeopixelInit(){
  for(int i=0; i<NeoNum; i++){
    pixels[i].begin();
  }
  AllNeoColor(RED);
}

void AllNeoColor(int color_code){
  for(int n=0; n<NeoNum; n++){
    pixels[n].lightColor(color[color_code]);
  }
}

void NeoBlink(int neo_code, int color_code, int blink_num, int blink_time){
  for(int n=0; n<blink_num; n++){
    pixels[neo_code].lightColor(color[BLACK]);
    delay(blink_time);
    pixels[neo_code].lightColor(color[color_code]);
    delay(blink_time);
  }
}

void Encoder_PointNeo(){
  int rev = 0;
  int point = 0;
  rev = encoder_revcount[0];
  point = NumPixels[ENCODER] - encoder_revcount[1] - 1;

  int rev_neo = BLUE0;                                  
  rev_neo = BLUE0 + (rev-1);           // 나머지 구역 네오픽셀 색상코드 :: 1번째 바퀴부터 BLUE0,1,2

  for(int i=0; i<NumPixels[ENCODER]; i++){
    pixels[ENCODER].setPixelColor(i, pixels[ENCODER].Color(color[rev_neo][0], color[rev_neo][1], color[rev_neo][2]));
  }
  pixels[ENCODER].setPixelColor(point, color[RED][0], color[RED][1], color[RED][2]);     // 엔코더 위치 point 네오픽셀  
  pixels[ENCODER].show();
} 

void Scenario_WaitBlink(int neoID){
  if(IsScenarioMode){
    BlinkTimer.deleteTimer(BlinkTimerId);
    BlinkTimerStart(neoID, WHITE);               //내부태그 노란색 점멸 시작
  }
}
#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\nextion.ino"
void NextionInit(){
  nexInit();
  nexHwSerial.begin(9600, SERIAL_8N1, NEXTIONHWSERIAL_RX_PIN, NEXTIONHWSERIAL_TX_PIN);
}

void SendCmd(String command){           // 영문용 디스플레이 send
  String cmd = "";
  if(command.startsWith("page") && Language=="Eng")    cmd = "page E" + command.substring(5);
  else    cmd = command;
  sendCommand(cmd.c_str());
}

void ExpSend(int exp){
  SendCmd("picExp.pic=2");
  SendCmd("pgItemOpen.vExp.val=" + (String)(exp));    // 경험치 +50 차있는걸로 기본세팅
}

void BatteryPackSend(int bp){
  SendCmd("picBatteryPack.pic=5");
  SendCmd("pgItemOpen.vBatteryPack.val=" + (String)(bp));    // 배터리팩 3개 차있는걸로 기본세팅
}

#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\rfid.ino"
void RfidInit(){
  RestartPn532:
  for (int i = 0; i < rfid_num; ++i)
  {
    nfc[i].begin();
    if (!(nfc[i].getFirmwareVersion()))
    {
      Serial.print("PN532 연결실패 : ");
      Serial.println(i);
      AllNeoColor(WHITE);
      Serial.println("pn532 INIT 재실행");
      goto RestartPn532;
    }
    else
    {
      nfc[i].SAMConfig();
      Serial.print("PN532 연결성공 : ");
      Serial.println(i);
      rfid_init_complete[i] = true;
      AllNeoColor(RED);
    }
    delay(100);
  }
}

void RfidLoop(int pn532_code){
  uint8_t uid[3][7] = {{0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0}}; // Buffer to store the returned UID
  uint8_t uidLength[] = {0};                   // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t data[32];
  byte pn532_packetbuffer11[64];
  pn532_packetbuffer11[0] = 0x00;

  if (nfc[pn532_code].sendCommandCheckAck(pn532_packetbuffer11, 1)){ // rfid 통신 가능한 상태인지 확인
    if (nfc[pn532_code].startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A)){                                       // rfid에 tag 찍혔는지 확인용 //데이터 들어오면 uid정보 가져오기
      if (nfc[pn532_code].ntag2xx_ReadPage(7, data)){ // ntag 데이터에 접근해서 불러와서 data행열에 저장
        Serial.println("TAGGGED");
        if(IsScenarioMode)  Auto_PlayerCheck(data);
        else                Manual_PlayerCheck(data);
      }
    }
  }
}

void RfidLoop_All(){
  RfidLoop(OUTPN532);
  RfidLoop(INPN532);
}

//****************************************PlayerCheck Function****************************************
void Manual_PlayerCheck(uint8_t rfidData[32]){ //어떤 카드가 들어왔는지 확인용
  String tagUser = "";
  for(int i = 0; i < 4; i++)    //GxPx 데이터만 배열에서 추출해서 string으로 저장
    tagUser += (char)rfidData[i];
  Serial.println("tag_user_data : " + tagUser);     // 1. 태그한 플레이어의 역할과 생명칩갯수, 최대생명칩갯수 등 읽어오기
  switch(tagUser[3]){
    case '1':
      Serial.println("Tagger Tagged");
      break;
    case '2':
      Serial.println("Ghost Tagged");
      break;
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
      Serial.println("Player Tagged");
      rfid_ptr();
      break;
    case 'M':
      AllNeoColor(RED);
      ESP.restart();
      break;
    case 'E':
      Language = "Eng";
      SendCmd("page pgWait");                       //영문 대기페이지 로드
      delay(10);
      Serial.println("Language set to English");
      NeoBlink(PN532,PURPLE,2,100);                 //영문 변환 표시용
      AllNeoColor(YELLOW);
      break;
    default: 
    Serial.println("Wrong TAG");
    break;
  }
}

void Auto_PlayerCheck(uint8_t rfidData[32]){
  String tagUser = "";
  String tagUserRole = "";
  for(int i=0; i<4; i++)    //GxPx 데이터만 배열에서 추출해서 string으로 저장
    tagUser += (char)rfidData[i];
  Serial.println("tag_user_data : " + tagUser);     // 1. 태그한 플레이어의 역할과 생명칩갯수, 최대생명칩갯수 등 읽어오기
  if(tagUser == "MMMM"){  //스태프카드 초기화
    ESP.restart();
  }

  tagUserRole = has2_mqtt.GetData(tagUser, "R");    // 2. 술래인지, 플레이어인지 구분
  
  if(tagUserRole == "player"){                      // 3. 태그한 사용자 판단
    Serial.println("Player Tagged");
    tagPlayerDN = tagUser;
    CheckPlayerBatt(tagUser);
    rfid_ptr();
  }
  else if(tagUserRole == "tagger"){
    Serial.println("Tagger Tagged");
  }
  else if(tagUserRole == "ghost"){
    Serial.println("Ghost Tagged");
  }
  else{
    Serial.println("Wrong TAG");
  }
}

void MiniGame_PlayerCheck(uint8_t rfidData[32]){
}

void CheckPlayerBatt(String tagDN){
  int playerBP = has2_mqtt.GetData(tagDN, "BP").toInt();
  int playerMBP = has2_mqtt.GetData(tagDN, "MBP").toInt();

  if((playerBP + Device_BP) > playerMBP)
    IsPlayerBattFull = true;
  else
    IsPlayerBattFull = false;
}

//****************************************rfid_ptr Function****************************************
void Quiz_Start(){
  has2_mqtt.Situation("tag", tagPlayerDN);
  Serial.println("Quiz Start");
  AllNeoColor(BLUE);

  QuizCount = 0;
  attachInterrupt(encoderPinA, updateEncoder, CHANGE);    // 엔코더 시작
  attachInterrupt(encoderPinB, updateEncoder, CHANGE);
  GameTimer.deleteTimer(GameTimerId);
  GameTimerId = GameTimer.setInterval(GameTime, GameTimerFunc); // 타임아웃 게임타이머 활성화

  device_ptr = Device_Quiz;
  Serial.println("Quiz Started");
}

void Quiz_Solved(){
  has2_mqtt.Situation("tag", tagPlayerDN);
  Serial.println("Quiz Solved");
  GameTimer.deleteTimer(GameTimerId);   // 타임아웃 게임타이머 비활성화

  ItemBoxSelfOpen = true;
  AllNeoColor(BLUE);
  ExpSend(Device_EXP);                  // 경험치 정보 
  BatteryPackSend(Device_BP);           // 배터리 개수 정보
  delay(10);
  SendCmd("page pgItemOpen");
  delay(10);
  SendCmd("wOutTagged.en=1");
  BoxOpen();

  pixels[INNER].lightColor(color[YELLOW]);
  BlinkTimer.deleteTimer(BlinkTimerId);
  BlinkTimerStart(INNER, YELLOW); //내부태그 노란색 점멸 시작

  device_ptr = Device_Itemtake;
  rfid_ptr = Item_Take;
}

void Item_Take(){
  if(!IsPlayerBattFull){
    has2_mqtt.Situation("tag", tagPlayerDN);
    Serial.println("ItemTake");
    SendCmd("page pgItemTaken");
    ItemBoxUsed = true;
    BlinkTimer.deleteTimer(BlinkTimerId);
    AllNeoColor(RED);
    rfid_ptr = Item_Took;
    device_ptr = Device_Used;
  }
  else{
    Serial.println("CANNOT CARRY MORE BatteryPack");
    SendCmd("page pgItemTakeFail");
    NeoBlink(INNER,RED,2,500);
    BlinkTimer.deleteTimer(BlinkTimerId);
    BlinkTimerStart(INNER, YELLOW);               //내부태그 노란색 점멸 시작
  }
}

void Item_Took(){
  Serial.println("ItemTook");
  AllNeoColor(RED);
}
#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\timer.ino"
void TimerInit(){
    BlinkTimerId = BlinkTimer.setInterval(BlinkTime, BlinkTimerFunc);
    BlinkTimer.deleteTimer(BlinkTimerId);
    GameTimerId = GameTimer.setInterval(GameTime, GameTimerFunc);
    GameTimer.deleteTimer(GameTimerId);
}

//****************************************Blink Timer****************************************
void BlinkTimerStart(int Neo, int NeoColor){
    blinkNeo = Neo;
    blinkColor = NeoColor;
    if(Neo == ALLNEO)   BlinkTimerId = BlinkTimer.setInterval(BlinkTime, BlinkAllTimerFunc);
    else                BlinkTimerId = BlinkTimer.setInterval(BlinkTime, BlinkTimerFunc);
}

void BlinkTimerFunc(){
    Serial.println("Blink!");
    if(blinkOn == true){
        pixels[blinkNeo].lightColor(color[blinkColor]);
        blinkOn = false;
    }
    else{
        pixels[blinkNeo].lightColor(color[BLACK]);
        blinkOn = true;
    }
}

void BlinkAllTimerFunc(){
    Serial.println("ALL Blink!");
    if(blinkOn == true){
        for(int i=0; i<NeoNum; i++){
            pixels[i].lightColor(color[blinkColor]);
        }
        blinkOn = false;
    }
    else{
        for(int i=0; i<NeoNum; i++){
            pixels[i].lightColor(color[BLACK]);
        }
        blinkOn = true;
    }
}

void GameTimerFunc(){
    if(GameTimeCNT >= 1){
        if(GameTimeCNT >= GameTimeCNT_Max){
            Serial.println("TimeOut ==> Login Reset");
            ledcWrite(VibrationLedChannel, 0);          // 진동모터 비활성화
            detachInterrupt(encoderPinA);               // 엔코더 비활성화
            detachInterrupt(encoderPinB);
            BlinkTimer.deleteTimer(BlinkTimerId);
            GameTimer.deleteTimer(GameTimerId);
            lastEncoded = 0;
            encoderValue = 41*4; 
            GameTimeCNT = 0; 
            SendCmd("page pgWait");
            device_ptr = Device_Login;
        }
        else{
            long TimeLeft = (GameTimeCNT_Max - GameTimeCNT)*GameTime/1000;
            Serial.println("TimeOut in " + (String)(TimeLeft) + "sec");
        }
    }
    GameTimeCNT++;
}
