# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\HAS2_TR-Itembox.ino"
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
# 13 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\HAS2_TR-Itembox.ino"
# 14 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\HAS2_TR-Itembox.ino" 2

void setup() {
  Serial.begin(115200);
  // has2wifi.Setup("city");
  // has2wifi.Setup("badland");
  // has2wifi.Setup();
  // has2wifi.Setup("KT_GiGA_6C64","ed46zx1198");
  TimerInit();
  NeopixelInit();
  EncoderInit();
  MotorInit();
  NextionInit();
}

void loop() {
  game_ptr();
  BlinkTimer.run();
}
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\Game.ino"
void Game_Ready(){

}

void Game_Selected(){
  BlinkTimer.deleteTimer(blinkTimerId);
  BlinkAllTimerStart(WHITE); // 전체 흰색 점멸 시작
  RfidLoop(OUTPN532);
}

void Game_OUTPN532_login(){
  rfid_ptr = Quiz_Start;
  RfidLoop(OUTPN532);
}

void Game_Quiz(){
  if(!(QuizCount == 3)){
    Quiz_System();
  }
  else{ //Quiz 3회 성공000
    Serial.println("CLEAR!!!");
    SendCmd("wQuizSolved.en=1");

    pixels[PN532].lightColor(color[YELLOW]);
    QuizCount = 0;
    detachInterrupt(13); //엔코더 사용 막기
    detachInterrupt(15);

    game_ptr = Game_OUTPN532_open;
  }
}

void Game_OUTPN532_open(){
  rfid_ptr = Quiz_Solved;
  RfidLoop(OUTPN532);
}

void Game_InnerTag(){
  RfidLoop(INPN532);
  ItemBoxUsed = true;
  game_ptr = Game_Used;
}

void Game_Used(){

}

void Game_Void(){ //포인터 초기세팅용 void함수
}
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\encoder.ino"
//**************************************************Encoder**************************************************
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

void EncoderInit(){
  pinMode(13, 0x01);
  pinMode(15, 0x01);
  pinMode(34, 0x01);

  digitalWrite(13, 0x1); //turn pullup resistor on
  digitalWrite(15, 0x1); //turn pullup resistor on
}

void Encoder_Loop(){ // "encoder값, 버튼눌림" 을 표시
  Encoder_RevCount(); // 엔코더 값 변환
  Encoder_PointNeo(); // 네오픽셀 값 반영
  Encoder_VibrationStrength(Quiz_answer[QuizCount]); // 진동 값 반영

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
  RawVal = encoderValue/4;

  if(RawVal < 0){
    RawVal = 0;
    encoder_revcount[0] = 1;
    encoder_revcount[1] = 0;
  }
  else if(RawVal >= 0 && RawVal < NumPixels[PN532]){
    encoder_revcount[0] = 1;
    encoder_revcount[1] = RawVal;
  }
  else if(RawVal >= NumPixels[PN532] && RawVal < NumPixels[PN532]*2){
    encoder_revcount[0] = 2;
    encoder_revcount[1] = RawVal - NumPixels[PN532];
  }
  else if(RawVal >= NumPixels[PN532]*2 && RawVal < NumPixels[PN532]*3){
    encoder_revcount[0] = 3;
    encoder_revcount[1] = RawVal - NumPixels[PN532]*2;
  }
  else{
    RawVal = NumPixels[PN532]*3-1;
    encoder_revcount[0] = 3;
    encoder_revcount[1] = NumPixels[PN532]-1;
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

  //엔코더 값 최대최소 제한 걸어두기    
  if(encoderValue > NumPixels[PN532]*3*4) encoderValue = NumPixels[PN532]*3*4;
  else if(encoderValue < 0) encoderValue = 0;
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

  if(Player_guess == Quiz_answer[QuizCount]){
    Serial.println("[[[[Quiz " + (String)(QuizCount) + "]]]] Success");
    NeoBlink(ENCODER, GREEN, 5, 250);
    QuizCount += 1;
  }
  else{
    Serial.println("Quiz Fail"); //네오픽셀 빨강
    NeoBlink(ENCODER, RED, 5, 250);
  }
  RawVal = Player_guess;
}
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\motor.ino"
void MotorInit(){
    //Linear Motor Init
    pinMode(36,0x05);
    pinMode(32, 0x03);
    pinMode(4, 0x03);
    ledcSetup(MotorLedChannel, MotorFreq, MotorResolution);
    ledcAttachPin(22, MotorLedChannel);
    ledcWrite(MotorLedChannel, 0);
    BoxClose();

    //Vibration Motor Init
    pinMode(12, 0x03);
    digitalWrite(12, 0x1);
    ledcSetup(VibrationLedChannel, MotorFreq, MotorResolution);
    ledcAttachPin(14, VibrationLedChannel);
    ledcWrite(VibrationLedChannel, 0);
}

void BoxClose(){
    Serial.println("BOX Close");
    ledcWrite(MotorLedChannel, MotorMAX_DUTY_CYCLE - 1);
    digitalWrite(32, 0x0);
    digitalWrite(4, 0x1);
    delay(4000);
    Serial.println("BOX Closed");
}

void BoxOpen(){
    Serial.println("BOX Open");
    ledcWrite(MotorLedChannel, MotorMAX_DUTY_CYCLE - 1);
    digitalWrite(32, 0x1);
    digitalWrite(4, 0x0);
    delay(4000);
    Serial.println("BOX Opened");
}

void MotorStop(){
    Serial.println("모터 스탑");
    digitalWrite(32, 0x0);
    digitalWrite(4, 0x0);
}

void Encoder_VibrationStrength(int answer){
    int differenceValue = abs(answer - (encoderValue/4));
    int vibeStrength = 0;

    if(differenceValue < Answer_Range + Vibration_Range * 0) vibeStrength = 0; // 정답
    else if(differenceValue < Answer_Range + Vibration_Range * 1) vibeStrength = 1;
    else if(differenceValue < Answer_Range + Vibration_Range * 2) vibeStrength = 2;
    else if(differenceValue < Answer_Range + Vibration_Range * 3) vibeStrength = 3;
    else vibeStrength = 4;
    ledcWrite(VibrationLedChannel, Vibration_Strength[vibeStrength]);
}
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\mqtt.ino"
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\neopixel.ino"
void NeopixelInit(){
  for(int i=0; i<NeoNum; i++){
    pixels[i].begin();
  }
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
  point = encoder_revcount[1];

  int rev_neo = BLUE0;
  rev_neo = BLUE0 + (rev-1); // 나머지 구역 네오픽셀 :: 1번째 바퀴부터 BLUE0,1,2

  pixels[ENCODER].setPixelColor(point, color[RED][0], color[RED][1], color[RED][2]); // 엔코더 위치 point 네오픽셀

  for(int i=0; i<point; i++){
    pixels[ENCODER].setPixelColor(i, pixels[ENCODER].Color(color[rev_neo][0], color[rev_neo][1], color[rev_neo][2])); // 나머지 구역 네오픽셀
  }
  for(int i=(point+1); i<NumPixels[ENCODER]; i++){
    pixels[ENCODER].setPixelColor(i, pixels[ENCODER].Color(color[rev_neo][0], color[rev_neo][1], color[rev_neo][2]));
  }
  pixels[ENCODER].show();
}
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\nextion.ino"
void NextionInit()
{
   nexInit();
   nexHwSerial.begin(9600, 0x800001c, 39, 33);
}

void SendCmd(String command) // 영문용 디스플레이 send
{
    String cmd = "";
    if (command.startsWith("page") && Language=="Eng")
    {
        cmd = "page E" + command.substring(5);
    }
    else
    {
        cmd = command;
    }
    sendCommand(cmd.c_str());
}

void ExpSend(){
    SendCmd("picExp.pic=2");
    SendCmd("pgItemOpen.vExp.val=50"); // 경험치 +50 차있는걸로 기본세팅
}
void BatteryPackSend(){
    SendCmd("picBatteryPack.pic=5");
    SendCmd("pgItemOpen.vBatteryPack.val=3"); // 배터리팩 3개 차있는걸로 기본세팅
}
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\rfid.ino"
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
  uint8_t uidLength[] = {0}; // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t data[32];
  byte pn532_packetbuffer11[64];
  pn532_packetbuffer11[0] = 0x00;

  if (nfc[pn532_code].sendCommandCheckAck(pn532_packetbuffer11, 1)){ // rfid 통신 가능한 상태인지 확인
    if (nfc[pn532_code].startPassiveTargetIDDetection((0x00))){ // rfid에 tag 찍혔는지 확인용 //데이터 들어오면 uid정보 가져오기
      if (nfc[pn532_code].ntag2xx_ReadPage(7, data)){ // ntag 데이터에 접근해서 불러와서 data행열에 저장
        Serial.println("TAGGGED");
        CheckingPlayers(data);
      }
    }
  }
}

void RfidLoop_All(){
  RfidLoop(OUTPN532);
  RfidLoop(INPN532);
}

void CheckingPlayers(uint8_t rfidData[32]){ //어떤 카드가 들어왔는지 확인용
  String tagUser = "";
  for(int i = 0; i < 4; i++) //GxPx 데이터만 배열에서 추출해서 string으로 저장
    tagUser += (char)rfidData[i];
  Serial.println("tag_user_data : " + tagUser); // 1. 태그한 플레이어의 역할과 생명칩갯수, 최대생명칩갯수 등 읽어오기
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
      ESP.restart();
      break;
    case 'E':
      Language = "Eng";
      SendCmd("page pgWait"); //영문 대기페이지 로드
      delay(10);
      Serial.println("Language set to English");
      NeoBlink(PN532,PURPLE,2,100); //영문 변환 표시용
      AllNeoColor(YELLOW);
      break;
    default:
    Serial.println("Wrong TAG");
    break;
  }
}

//****************************************rfid_ptr Function****************************************
void Quiz_Start(){
  Serial.println("Quiz Start");

  AllNeoColor(BLUE);
  attachInterrupt(13, updateEncoder, 0x03);
  attachInterrupt(15, updateEncoder, 0x03);
  game_ptr = Game_Quiz;
}

void Quiz_Solved(){
  Serial.println("Quiz Solved");

  ItemBoxSelfOpen = true;
  AllNeoColor(BLUE);
  ExpSend(); // 경험치 정보 
  BatteryPackSend(); // 배터리 개수 정보
  delay(10);
  SendCmd("page pgItemOpen");
  delay(10);
  SendCmd("wOutTagged.en=1");
  BoxOpen();
  pixels[INNER].lightColor(color[YELLOW]);

  BlinkTimer.deleteTimer(blinkTimerId);
  BlinkTimerStart(INNER, YELLOW); //내부태그 노란색 점멸 시작

  game_ptr = Game_InnerTag;
  rfid_ptr = Item_Took;
}

void Item_Took(){
  Serial.println("ItemTook");
  if(true){
    SendCmd("page pgItemTaken");
    AllNeoColor(RED);
    // game_ptr = RfidLoopBoth;
    rfid_ptr = Game_Void;
    BlinkTimer.deleteTimer(blinkTimerId);
    ItemBoxUsed = true;
  }
  else{
    Serial.println("CANNOT CARRY MORE BatteryPack");
    SendCmd("page pgItemTakeFail");
    NeoBlink(INNER,RED,2,500);
    BlinkTimer.deleteTimer(blinkTimerId);
    BlinkTimerStart(INNER, YELLOW); //내부태그 노란색 점멸 시작
  }
}
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\timer.ino"
void TimerInit(){
    blinkTimerId = BlinkTimer.setInterval(blinkTime,BlinkTimerFunc);
    BlinkTimer.deleteTimer(blinkTimerId);
}

void BlinkTimerStart(int Neo, int NeoColor){
    blinkNeo = Neo;
    blinkColor = NeoColor;
    blinkTimerId = BlinkTimer.setInterval(blinkTime,BlinkTimerFunc);
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

void BlinkAllTimerStart(int NeoColor){
    blinkColor = NeoColor;
    blinkTimerId = BlinkTimer.setInterval(blinkTime,BlinkTimerFunc);
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
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\wifi.ino"
