# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\HAS2_TR-Itembox.ino"
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
# 13 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\HAS2_TR-Itembox.ino"
# 14 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\HAS2_TR-Itembox.ino" 2

void setup() {
  Serial.begin(115200);
  has2_mqtt.Setup("train_room", "Code3824@", callback); // tp-link 접속; ALL, myMAC 구독;
  has2_mqtt.Situation("start", "my");
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
# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\encoder.ino"
//**************************************************Encoder**************************************************
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
  // Encoder_Print();                                 // 엔코더 값 시리얼 출력
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
    IsDsSkip = false;
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
# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\motor.ino"
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
    IsBoxOpen = false;
    delay(4000);
    Serial.println("BOX Closed");
}

void BoxOpen(){
    Serial.println("BOX Open");
    ledcWrite(MotorLedChannel, MotorMAX_DUTY_CYCLE - 1);
    digitalWrite(32, 0x1);
    digitalWrite(4, 0x0);
    IsBoxOpen = true;
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
# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\mqtt.ino"
void callback(char* topic, byte* payload, unsigned int length){
  String input_data = "";

  for(int i=0; i<length; i++)
    input_data += (char)payload[i];
  has2_mqtt.SaveByTopic(topic, input_data);

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] : ");
  Serial.println(input_data);

  Mqtt_updateDS(has2_mqtt.GetData("my", "DS"));
}

void Mqtt_updateDS(String myDS){
  if(myDS == "scenario"){
    IsScenarioMode = true;
    Mqtt_updateSCN(has2_mqtt.GetData("my", "SCN"));
  }
  if(myDS != device_ptr_state){
    Device_ptrPrint(myDS);
    SendCmd("sleep=0");
    if(myDS == "OTA"){
      AllNeoColor(GREEN);
      SendCmd("page pgWait");
      has2_mqtt.FirmwareUpdate("ex_itembox");
    }
    else if(myDS == "setting"){
      BoxClose();
      device_ptr = Device_Setting;
    }
    else if(myDS == "ready") device_ptr = Device_Ready;
    else if(myDS == "activate") device_ptr = Device_MiniGame;
    else if(myDS == "used") device_ptr = Device_Used;
    else if(myDS == "manual") device_ptr = Device_Manual;
    else if(myDS == "minigame") device_ptr = Device_MiniGame;
  }
  Device_ptrPrint(myDS);
}

void Mqtt_updateSCN(String mySCN){
  Serial.println("Scenario RCV :: " + mySCN);
  if(mySCN != "-1" && mySCN != current_scenario){
    Serial.println("Scenario at :: " + mySCN);
    IsDsSkip = true;
    GameTimer.deleteTimer(GameTimerId);

    String temp_mySCN = mySCN;
    if(temp_mySCN.startsWith("p")){ // 스텔스 모드
      temp_mySCN.remove(0,1);
      Serial.println("temp_mySCN : " + temp_mySCN);
      if(temp_mySCN.toInt()==1 || temp_mySCN.toInt()==4 || temp_mySCN.toInt()>=35){
        Serial.println("darkm0de");
        Serial.println("temp_mySCN : " + temp_mySCN);
        Device_Stelth();
        SendCmd("sleep=1");
        SendCmd("page pgWait");
        device_ptr = Device_Stelth;
        if(temp_mySCN.toInt()==1)
          BoxClose();
      }
    }

    if(mySCN == "p8")
      device_ptr = Device_Selected;
    else if(mySCN == "p9"){
      Scenario_WaitBlink(PN532);
      device_ptr = Device_Login;
    }
    else if(mySCN == "p13"){
      Dummy_Encoder();
    }
    else if(mySCN == "p14"){
      device_ptr = Quiz_Start;
    }
    else if(mySCN == "p21"){
      Dummy_Quizsolved();
    }
    else if(mySCN == "p23"){
      QuizCount = 3;
      device_ptr = Device_Quiz;
    }
    else if(mySCN == "p26"){
      Dummy_Itemtake();
    }
    else if(mySCN == "p29"){
      if(!IsBoxOpen)
        Quiz_Solved();
      else{
        device_ptr = Device_Itemtake;
        rfid_ptr = Item_Take;
      }
    }
    else if(mySCN == "p32"){
      IsPlayerBattFull = false;
      Item_Take();
    }
    else
      Serial.println("Receved Unsigned SCN");
    current_scenario = mySCN;
  }
}

void SituationSend(){
  if(IsScenarioMode && !IsDsSkip){
    Serial.println("Stituation \"tag\" :: Publish");
    has2_mqtt.Situation("tag", tagPlayerDN);
  }
}

//****************************************device_ptr Function****************************************
void Device_Manual(){
  IsScenarioMode = false;
  Serial.println("GameMode Set :: Manual");
  AllNeoColor(YELLOW);
  device_ptr = Device_Login;
}

void Device_MiniGame(){
  IsScenarioMode = false;
  Serial.println("GameMode Set :: MiniGame");
  AllNeoColor(YELLOW);
  device_ptr = Device_Login;
}

void Device_Setting(){
  AllNeoColor(WHITE);
  RfidMMMMLoop(OUTPN532);
  RfidMMMMLoop(INPN532);
}

void Device_Ready(){
  AllNeoColor(RED);
  RfidMMMMLoop(OUTPN532);
  RfidMMMMLoop(INPN532);
}

void Device_Selected(){
  AllNeoColor(YELLOW);
  RfidMMMMLoop(OUTPN532);
  RfidMMMMLoop(INPN532);
}

void Device_Login(){
  rfid_ptr = Quiz_Start;
  RfidLoop(OUTPN532);
  RfidMMMMLoop(INPN532);
}

void Device_EncoderBlink(){
  // if(IsScenarioMode && )
  if(encoderValue != 41*4){
    Serial.println("encoder val : " + (String)(encoderValue));
    Serial.println("knob touched, quiz start");
    BlinkTimer.deleteTimer(BlinkTimerId);
    device_ptr = Device_Quiz;
  }
}

void Device_Quiz(){
  if(!(QuizCount == 3)){
    AllNeoColor(BLUE);
    Quiz_System();
  }
  else{ //Quiz 3회 성공000
    if(IsScenarioMode && !IsDsSkip){
      SituationSend();
      Dummy_Quizsolved();
    }
    else{
      Serial.println("CLEAR!!!");
      SendCmd("wQuizSolved.en=1");
      AllNeoColor(GREEN);
      ledcWrite(VibrationLedChannel, 0); // 진동모터 비활성화
      detachInterrupt(13); // 엔코더 비활성화
      detachInterrupt(15);
      GameTimer.deleteTimer(GameTimerId); // 타임아웃 게임타이머 비활성화
      GameTimeCNT = 0;
      GameTimerId = GameTimer.setInterval(GameTime, GameTimerFunc);
      if(!IsScenarioMode){
        BlinkTimer.deleteTimer(BlinkTimerId); // 외부태그 노란색 점멸 시작
        BlinkTimerStart(PN532, YELLOW);
      }
      else{
        pixels[PN532].lightColor(color[WHITE]);
        Scenario_WaitBlink(PN532);
      }

      QuizCount = 0;
      device_ptr = Device_Opentag;
    }
  }
}

void Device_Opentag(){
  rfid_ptr = Quiz_Solved;
  RfidLoop(OUTPN532);
}

void Device_Itemtake(){
  RfidLoop(INPN532);
  RfidMMMMLoop(OUTPN532);
}

void Device_Used(){
  RfidLoop(OUTPN532);
  RfidLoop(INPN532);
}

void Device_Stelth(){
  AllNeoColor(BLACK);
  RfidMMMMLoop(OUTPN532);
  RfidMMMMLoop(INPN532);
}

//포인터 초기세팅용 void함수
void VoidFunc(){}

// 게임 포인터 상태 SerialPrint
void Device_ptrPrint(String print){
  if(print != device_ptr_state){
    if((char)print[1] == 'c' && (char)print[2] == 'n')
      Serial.println("Training Scenario :: " + print);
    else
      Serial.println("Device State :: " + print);
    device_ptr_state = print;
  }
}

// 모든 기능 비활성화
void Device_Kill(){
  Serial.println("DEVICE KILL");
  ledcWrite(VibrationLedChannel, 0); // 진동모터 비활성화
  detachInterrupt(13); // 엔코더 비활성화
  detachInterrupt(15);
  GameTimeCNT = 0;
  GameTimer.deleteTimer(GameTimerId); // 타임아웃 게임타이머 비활성화
  BlinkTimer.deleteTimer(BlinkTimerId); // 블링크 비활성화
}

//****************************************dummy Function****************************************
void Dummy_Encoder(){
  Serial.println("DUMMY ENCODER");
  pixels[PN532].lightColor(color[BLUE]);
  pixels[INNER].lightColor(color[BLUE]);
  pixels[ENCODER].lightColor(color[WHITE]);
  Scenario_WaitBlink(ENCODER);
}

void Dummy_Quizsolved(){
  Serial.println("DUMMY QUIZSOLVED");
  ledcWrite(VibrationLedChannel, 0); // 진동모터 비활성화
  detachInterrupt(13); // 엔코더 비활성화
  detachInterrupt(15);
  GameTimer.deleteTimer(GameTimerId); // 타임아웃 게임타이머 비활성화

  pixels[ENCODER].lightColor(color[GREEN]);
  pixels[INNER].lightColor(color[GREEN]);
  pixels[PN532].lightColor(color[WHITE]);
  Scenario_WaitBlink(PN532);

  device_ptr = VoidFunc;
}

void Dummy_Itemtake(){
  Serial.println("DUMMY ITEMTAKE");
  AllNeoColor(GREEN);
  Scenario_WaitBlink(INNER);

  Device_EXP = has2_mqtt.GetData("my", "EXP").toInt();
  Device_EXP = 50;
  Device_BP = has2_mqtt.GetData("my", "LCBP").toInt();
  Serial.println("RECV Device_BP : " + (String)(Device_BP));
  if(Device_BP <= 0) Device_BP = 2;

  ExpSend(Device_EXP); // 경험치 정보 
  BatteryPackSend(Device_BP); // 배터리 개수 정보
  delay(10);
  SendCmd("sleep=0");
  SendCmd("page pgItemOpen");
  delay(10);
  SendCmd("wOutTagged.en=1");
  BoxOpen();
}
# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\neopixel.ino"
void NeopixelInit(){
  for(int i=0; i<NeoNum; i++){
    pixels[i].begin();
  }
  AllNeoColor(RED);
}

void AllNeoColor(int color_code){
  BlinkTimer.deleteTimer(BlinkTimerId);
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

void Scenario_WaitBlink(int neoID){ // 시나리오 모드용 점멸
  if(IsScenarioMode){
    BlinkTimer.deleteTimer(BlinkTimerId);
    BlinkTimerStart(neoID, WHITE);
  }
}
# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\nextion.ino"
void NextionInit(){
  nexInit();
  nexHwSerial.begin(9600, 0x800001c, 39, 33);
}

void SendCmd(String command){ // 영문용 디스플레이 send, wait 화면 끄기
  if(command == "page pgWait") SendCmd("sleep=1");

  String cmd = "";
  if(command.startsWith("page") && Language=="Eng") cmd = "page E" + command.substring(5);
  else cmd = command;
  sendCommand(cmd.c_str());
}

void ExpSend(int exp){
  SendCmd("picExp.pic=2");
  SendCmd("pgItemOpen.vExp.val=" + (String)(exp)); // 경험치 +50 차있는걸로 기본세팅
}

void BatteryPackSend(int bp){
  SendCmd("picBatteryPack.pic=5");
  SendCmd("pgItemOpen.vBatteryPack.val=" + (String)(bp)); // 배터리팩 3개 차있는걸로 기본세팅
}
# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\rfid.ino"
void RfidInit(){
  RestartPn532:
  for (int i = 0; i < rfid_num; ++i)
  {
    nfc[i].begin();
    if (!(nfc[i].getFirmwareVersion())){
      Serial.print("PN532 연결실패 : ");
      Serial.println(i);
      AllNeoColor(WHITE);
      Serial.println("pn532 INIT 재실행");
      goto RestartPn532;
    }
    else{
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
        if(IsScenarioMode) Auto_PlayerCheck(data);
        else Manual_PlayerCheck(data);
      }
    }
  }
}

void RfidMMMMLoop(int pn532_code){
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
        MasterCardCheck(data);
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

void Auto_PlayerCheck(uint8_t rfidData[32]){
  Serial.println("Auto_PlayerCheck");
  String tagUser = "";
  String tagUserRole = "";
  for(int i=0; i<4; i++) //GxPx 데이터만 배열에서 추출해서 string으로 저장
    tagUser += (char)rfidData[i];
  Serial.println("tag_user_data : <" + tagUser + ">"); // 1. 태그한 플레이어의 역할과 생명칩갯수, 최대생명칩갯수 등 읽어오기
  if(tagUser == "MMMM"){ //스태프카드 초기화
    ESP.restart();
  }

  tagUserRole = has2_mqtt.GetData(tagUser, "R"); // 2. 술래인지, 플레이어인지 구분
  Serial.println(tagUserRole);

  if(tagUserRole == "player"){ // 3. 태그한 사용자 판단
    Serial.println("Player Tagged");
    tagPlayerDN = tagUser;
    IsDsSkip = false;
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
  // int playerMBP = has2_mqtt.GetData(tagDN, "MBP").toInt();
  int playerMBP = 4; // 임시 data

  if((playerBP + Device_BP) > playerMBP)
    IsPlayerBattFull = true;
  else
    IsPlayerBattFull = false;
}

void MasterCardCheck(uint8_t rfidData[32]){
  String tagUser = "";
  for(int i = 0; i < 4; i++) //GxPx 데이터만 배열에서 추출해서 string으로 저장
    tagUser += (char)rfidData[i];
  Serial.println("tag_user_data : " + tagUser); // 1. 태그한 플레이어의 역할과 생명칩갯수, 최대생명칩갯수 등 읽어오기
  switch(tagUser[3]){
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

//****************************************rfid_ptr Function****************************************
void Quiz_Start(){
  if(IsScenarioMode && !IsDsSkip){
    SituationSend();
    Dummy_Encoder();
  }
  else{
    Serial.println("Quiz Start");
    pixels[PN532].lightColor(color[BLUE]);
    pixels[ENCODER].lightColor(color[WHITE]);
    pixels[INNER].lightColor(color[BLUE]);

    encoderValue = 41*4;
    // RawVal = 0;
    QuizCount = 0;
    attachInterrupt(13, updateEncoder, 0x03); // 엔코더 시작
    attachInterrupt(15, updateEncoder, 0x03);

    if(IsScenarioMode){
      Serial.println("encoderblink");
      Scenario_WaitBlink(ENCODER);
      device_ptr = Device_EncoderBlink;
    }
    else{ // 타임아웃 게임타이머 활성화
      GameTimer.deleteTimer(GameTimerId);
      GameTimerId = GameTimer.setInterval(GameTime, GameTimerFunc);
      device_ptr = Device_Quiz;
    }
    Serial.println("Quiz Started");
  }
}

void Quiz_Solved(){
  SituationSend();
  Serial.println("Quiz Solved");
  GameTimer.deleteTimer(GameTimerId); // 타임아웃 게임타이머 비활성화

  ItemBoxSelfOpen = true;
  if(IsScenarioMode){
    Device_EXP = has2_mqtt.GetData("my", "EXP").toInt();
    Device_EXP = 50;
    Device_BP = has2_mqtt.GetData("my", "LCBP").toInt();
    Serial.println("RECV Device_BP : " + (String)(Device_BP));
    if(Device_BP <= 0) Device_BP = 2;
  }
  else{
    int Device_EXP = 50;
    int Device_BP = 3;
  }
  ExpSend(Device_EXP); // 경험치 정보 
  BatteryPackSend(Device_BP); // 배터리 개수 정보
  delay(10);
  SendCmd("sleep=0");
  SendCmd("page pgItemOpen");
  delay(10);
  SendCmd("wOutTagged.en=1");
  BoxOpen();
  pixels[PN532].lightColor(color[BLUE]);
  pixels[ENCODER].lightColor(color[BLUE]);
  pixels[INNER].lightColor(color[YELLOW]);
  BlinkTimer.deleteTimer(BlinkTimerId);
  BlinkTimerStart(INNER, YELLOW); //내부태그 노란색 점멸 시작

  device_ptr = Device_Itemtake;
  if(IsScenarioMode && !IsDsSkip){
    Serial.println("");
    rfid_ptr = VoidFunc;
  }
  else{
    Serial.println("rfid takeeee");
    rfid_ptr = Item_Take;
  }
}

void Item_Take(){
  if(!IsPlayerBattFull){
    SituationSend();
    Serial.println("ItemTake");
    SendCmd("sleep=0");
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
    BlinkTimerStart(INNER, YELLOW); //내부태그 노란색 점멸 시작
  }
}

void Item_Took(){
  Serial.println("ItemTook");
  AllNeoColor(RED);
}
# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Itembox\\timer.ino"
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
            device_ptr = Device_Login;
        }
        else{
            long TimeLeft = (GameTimeCNT_Max - GameTimeCNT)*GameTime/1000;
            Serial.println("TimeOut in " + (String)(TimeLeft) + "sec");
        }
    }
    GameTimeCNT++;
}
