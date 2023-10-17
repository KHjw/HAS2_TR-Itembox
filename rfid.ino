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

void RfidMMMMLoop(int pn532_code){
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
  Serial.println("Auto_PlayerCheck");
  String tagUser = "";
  String tagUserRole = "";
  for(int i=0; i<4; i++)    //GxPx 데이터만 배열에서 추출해서 string으로 저장
    tagUser += (char)rfidData[i];
  Serial.println("tag_user_data : <" + tagUser + ">");     // 1. 태그한 플레이어의 역할과 생명칩갯수, 최대생명칩갯수 등 읽어오기
  if(tagUser == "MMMM"){  //스태프카드 초기화
    ESP.restart();
  }

  tagUserRole = has2_mqtt.GetData(tagUser, "R");    // 2. 술래인지, 플레이어인지 구분
  Serial.println(tagUserRole);
  
  if(tagUserRole == "player"){                      // 3. 태그한 사용자 판단
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
  int playerMBP = 4;    // 임시 data

  if((playerBP + Device_BP) > playerMBP)
    IsPlayerBattFull = true;
  else
    IsPlayerBattFull = false;
}

void MasterCardCheck(uint8_t rfidData[32]){
  String tagUser = "";
  for(int i = 0; i < 4; i++)    //GxPx 데이터만 배열에서 추출해서 string으로 저장
    tagUser += (char)rfidData[i];
  Serial.println("tag_user_data : " + tagUser);     // 1. 태그한 플레이어의 역할과 생명칩갯수, 최대생명칩갯수 등 읽어오기
  switch(tagUser[3]){
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
    attachInterrupt(encoderPinA, updateEncoder, CHANGE);    // 엔코더 시작
    attachInterrupt(encoderPinB, updateEncoder, CHANGE);
    
    if(IsScenarioMode){
      Serial.println("encoderblink");
      Scenario_WaitBlink(ENCODER);
      device_ptr = Device_EncoderBlink;
    }  
    else{    // 타임아웃 게임타이머 활성화
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
  GameTimer.deleteTimer(GameTimerId);   // 타임아웃 게임타이머 비활성화

  ItemBoxSelfOpen = true;
  if(IsScenarioMode){
    Device_EXP = has2_mqtt.GetData("my", "EXP").toInt();
    Device_EXP = 50;
    Device_BP = has2_mqtt.GetData("my", "LCBP").toInt();
    Serial.println("RECV Device_BP : " + (String)(Device_BP));
    if(Device_BP <= 0)   Device_BP = 2;
  }
  else{
    int Device_EXP = 50;
    int Device_BP = 3;
  }
  ExpSend(Device_EXP);                  // 경험치 정보 
  BatteryPackSend(Device_BP);           // 배터리 개수 정보
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
    BlinkTimerStart(INNER, YELLOW);               //내부태그 노란색 점멸 시작
  }
}

void Item_Took(){
  Serial.println("ItemTook");
  AllNeoColor(RED);
}