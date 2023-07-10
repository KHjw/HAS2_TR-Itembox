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
  has2_mqtt.Setup(callback); // tp-link 접속; ALL, myMAC 구독;
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
  game_ptr();
  BlinkTimer.run();
  GameTimer.run();
}
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\Game.ino"
//****************************************game_ptr Function****************************************
void Game_Manual(){
  Game_ptrPrint("Game_Manual");
  AllNeoColor(YELLOW);
  game_ptr = Game_Login;
}

void Game_Setting(){
  Game_ptrPrint("Game_Setting");
  AllNeoColor(WHITE);
}

void Game_Ready(){
  Game_ptrPrint("Game_Ready");
  AllNeoColor(BLACK);
}

void Game_Selected(){
  Game_ptrPrint("Game_Selected");
  BlinkTimer.deleteTimer(BlinkTimerId);
  BlinkTimerStart(ALLNEO, WHITE); // 전체 흰색 점멸 시작
  RfidLoop(OUTPN532);
}

void Game_Login(){
  Game_ptrPrint("Game_Login");
  AllNeoColor(YELLOW);
  rfid_ptr = Quiz_Start;
  RfidLoop(OUTPN532);
}

void Game_Quiz(){
  Game_ptrPrint("Game_Quiz");
  if(!(QuizCount == 3)){
    Quiz_System();
  }
  else{ //Quiz 3회 성공000
    Serial.println("CLEAR!!!");
    SendCmd("wQuizSolved.en=1");
    pixels[PN532].lightColor(color[YELLOW]);
    ledcWrite(VibrationLedChannel, 0); // 진동모터 비활성화
    detachInterrupt(13); // 엔코더 비활성화
    detachInterrupt(15);
    GameTimer.deleteTimer(GameTimerId); // 타임아웃 게임타이머 비활성화
    GameTimeCNT = 0;
    GameTimerId = GameTimer.setInterval(GameTime, GameTimerFunc);
    BlinkTimer.deleteTimer(BlinkTimerId); // 내부태그 노란색 점멸 시작
    BlinkTimerStart(PN532, YELLOW);

    QuizCount = 0;
    game_ptr = Game_Opentag;
  }
}

void Game_Opentag(){
  Game_ptrPrint("Game_Opentag");
  rfid_ptr = Quiz_Solved;
  RfidLoop(OUTPN532);
}

void Game_Itemtake(){
  Game_ptrPrint("Game_Itemtake");
  RfidLoop(INPN532);
}

void Game_Used(){
  Game_ptrPrint("Game_Used");
  RfidLoop(OUTPN532);
  RfidLoop(INPN532);
}

void Game_Void(){ //포인터 초기세팅용 void함수
}

void Game_ptrPrint(String print){ // 게임 포인터 상태 SerialPrint
  if(print != game_ptr_state){
    Serial.println("Game pointer at ::" + print);
    game_ptr_state = print;
  }
}

//****************************************rfid_ptr Function****************************************
void Quiz_Start(){
  Serial.println("Quiz Start");
  AllNeoColor(BLUE);
  Serial.println("Quiz Starting");

  QuizCount = 0;
  attachInterrupt(13, updateEncoder, 0x03); // 엔코더 시작
  attachInterrupt(15, updateEncoder, 0x03);
  GameTimer.deleteTimer(GameTimerId);
  GameTimerId = GameTimer.setInterval(GameTime, GameTimerFunc); // 타임아웃 게임타이머 활성화

  game_ptr = Game_Quiz;
  Serial.println("Quiz Started");
}

void Quiz_Solved(){
  Serial.println("Quiz Solved");
  GameTimer.deleteTimer(GameTimerId); // 타임아웃 게임타이머 비활성화

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
  BlinkTimer.deleteTimer(BlinkTimerId);
  BlinkTimerStart(INNER, YELLOW); //내부태그 노란색 점멸 시작

  game_ptr = Game_Itemtake;
  rfid_ptr = Item_Take;
}

void Item_Take(){
  Serial.println("ItemTake");
  SendCmd("page pgItemTaken");
  ItemBoxUsed = true;
  BlinkTimer.deleteTimer(BlinkTimerId);
  AllNeoColor(RED);
  rfid_ptr = Item_Took;
  game_ptr = Game_Used;
}

void Item_Took(){
  Serial.println("ItemTook");
  if(ItemBoxUsed){
    AllNeoColor(RED);
  }
  else{
    Serial.println("CANNOT CARRY MORE BatteryPack");
    SendCmd("page pgItemTakeFail");
    NeoBlink(INNER,RED,2,500);
    BlinkTimer.deleteTimer(BlinkTimerId);
    BlinkTimerStart(INNER, YELLOW); //내부태그 노란색 점멸 시작
  }
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

void updateEncoder(){
  GameTimeCNT = 0; // 게임카운터 초기화

  int MSB = digitalRead(13); //MSB = most significant bit
  int LSB = digitalRead(15); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;


  lastEncoded = encoded; //store this value for next time

  //엔코더 값 최대최소 제한 걸어두기    
  if(encoderValue > NumPixels[ENCODER]*3*4) encoderValue = NumPixels[ENCODER]*3*4;
  else if(encoderValue < 0) encoderValue = 0;
}

void Encoder_Loop(){
  Encoder_RevCount(); // 엔코더 값 변환
  Encoder_PointNeo(); // 네오픽셀 값 반영
  Encoder_VibrationStrength(Quiz_answer[QuizCount]); // 진동 값 반영
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

void Encoder_Print(){ // "encoder값, 버튼눌림" 을 시리얼 출력
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
    Serial.print("Quiz Fail ====> Player Guess :: " + (String)(Player_guess)); // 플레이어 선택
    Serial.println("// Q" + (String)(QuizNum) + " ANS :: " + (String)(Quiz_answer[QuizCount])); // 퀴즈 정답
    NeoBlink(ENCODER, RED, 5, 250);
  }

  encoderValue = Player_guess*4;
  GameTimeCNT = 0;
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
    int DifferenceValue = abs(answer - (encoderValue/4));
    int VibeStrength = 0;

    if(DifferenceValue < Answer_Range + Vibration_Range * 0) VibeStrength = 0; // 정답
    else if(DifferenceValue < Answer_Range + Vibration_Range * 1) VibeStrength = 1;
    else if(DifferenceValue < Answer_Range + Vibration_Range * 2) VibeStrength = 2;
    else if(DifferenceValue < Answer_Range + Vibration_Range * 3) VibeStrength = 3;
    else VibeStrength = 4;
    ledcWrite(VibrationLedChannel, Vibration_Strength[VibeStrength]);
}
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\mqtt.ino"
void callback(char* topic, byte* payload, unsigned int length)
{
  static bool start = false;
  if(!start){
    start = true;
    has2_mqtt.Publish(my_topic, "start");
    return ;
  }

  String input_data = "";

  for (int i = 0; i < length; i++)
    input_data += (char)payload[i];

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] : ");
  Serial.println(input_data);

  if(input_data == "OTA") has2_mqtt.FirmwareUpdate("itembox");
  else if(input_data == "Setting") game_ptr = Game_Setting;
  else if(input_data == "Ready") game_ptr = Game_Ready;
  else if(input_data == "Activate") game_ptr = Game_Login;
  else if(input_data == "Selected") game_ptr = Game_Selected;
  else if(input_data == "Manual") game_ptr = Game_Manual;

  if((String)(topic) == "ALL"){
  }
  else if((String)(topic) == my_topic){
  }
}
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\neopixel.ino"
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
  rev_neo = BLUE0 + (rev-1); // 나머지 구역 네오픽셀 색상코드 :: 1번째 바퀴부터 BLUE0,1,2

  for(int i=0; i<NumPixels[ENCODER]; i++){
    pixels[ENCODER].setPixelColor(i, pixels[ENCODER].Color(color[rev_neo][0], color[rev_neo][1], color[rev_neo][2]));
  }
  pixels[ENCODER].setPixelColor(point, color[RED][0], color[RED][1], color[RED][2]); // 엔코더 위치 point 네오픽셀  
  pixels[ENCODER].show();
}
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\nextion.ino"
void NextionInit(){
  nexInit();
  nexHwSerial.begin(9600, 0x800001c, 39, 33);
}

void SendCmd(String command){ // 영문용 디스플레이 send
  String cmd = "";
  if(command.startsWith("page") && Language=="Eng") cmd = "page E" + command.substring(5);
  else cmd = command;
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
      AllNeoColor(RED);
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
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\timer.ino"
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
    if(Neo == ALLNEO) BlinkTimerId = BlinkTimer.setInterval(BlinkTime, BlinkAllTimerFunc);
    else BlinkTimerId = BlinkTimer.setInterval(BlinkTime, BlinkTimerFunc);
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
            ledcWrite(VibrationLedChannel, 0); // 진동모터 비활성화
            detachInterrupt(13); // 엔코더 비활성화
            detachInterrupt(15);
            BlinkTimer.deleteTimer(BlinkTimerId);
            GameTimer.deleteTimer(GameTimerId);
            lastEncoded = 0;
            encoderValue = 41*4;
            GameTimeCNT = 0;
            SendCmd("page pgWait");
            game_ptr = Game_Login;
        }
        else{
            long TimeLeft = (GameTimeCNT_Max - GameTimeCNT)*GameTime/1000;
            Serial.println("TimeOut in " + (String)(TimeLeft) + "sec");
        }
    }
    GameTimeCNT++;
}
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Itembox\\wifi.ino"
