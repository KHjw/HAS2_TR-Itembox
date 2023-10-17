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
    else if(myDS == "ready")       device_ptr = Device_Ready;
    else if(myDS == "activate")    device_ptr = Device_MiniGame;
    else if(myDS == "used")        device_ptr = Device_Used;
    else if(myDS == "manual")      device_ptr = Device_Manual;
    else if(myDS == "minigame")    device_ptr = Device_MiniGame;
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
    if(temp_mySCN.startsWith("p")){         // 스텔스 모드
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
    
    if(mySCN == "p3"){
      sendCommand("sleep=1");
      device_ptr = Device_RandBlink;
    }
    else if(mySCN == "p8")
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

void Device_RandBlink(){
  static bool bNeoBlink = false;
  long lnRandomDelay = random(2,8) *100;
  if(bNeoBlink)
    AllNeoColor(GREEN);
  else
    AllNeoColor(BLACK);
  bNeoBlink = !bNeoBlink;
  delay(lnRandomDelay);
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
  else{     //Quiz 3회 성공000
    if(IsScenarioMode && !IsDsSkip){
      SituationSend();
      Dummy_Quizsolved();
    }
    else{
      Serial.println("CLEAR!!!");
      SendCmd("wQuizSolved.en=1");
      AllNeoColor(GREEN);
      ledcWrite(VibrationLedChannel, 0);          // 진동모터 비활성화
      detachInterrupt(encoderPinA);               // 엔코더 비활성화
      detachInterrupt(encoderPinB);
      GameTimer.deleteTimer(GameTimerId);         // 타임아웃 게임타이머 비활성화
      GameTimeCNT = 0;
      GameTimerId = GameTimer.setInterval(GameTime, GameTimerFunc);
      if(!IsScenarioMode){
        BlinkTimer.deleteTimer(BlinkTimerId);       // 외부태그 노란색 점멸 시작
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
  ledcWrite(VibrationLedChannel, 0);          // 진동모터 비활성화
  detachInterrupt(encoderPinA);               // 엔코더 비활성화
  detachInterrupt(encoderPinB);
  GameTimeCNT = 0;
  GameTimer.deleteTimer(GameTimerId);         // 타임아웃 게임타이머 비활성화
  BlinkTimer.deleteTimer(BlinkTimerId);       // 블링크 비활성화
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
  ledcWrite(VibrationLedChannel, 0);          // 진동모터 비활성화
  detachInterrupt(encoderPinA);               // 엔코더 비활성화
  detachInterrupt(encoderPinB);
  GameTimer.deleteTimer(GameTimerId);         // 타임아웃 게임타이머 비활성화

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
  if(Device_BP <= 0)   Device_BP = 2;

  ExpSend(Device_EXP);                  // 경험치 정보 
  BatteryPackSend(Device_BP);           // 배터리 개수 정보
  delay(10);
  SendCmd("sleep=0");
  SendCmd("page pgItemOpen");
  delay(10);
  SendCmd("wOutTagged.en=1");
  BoxOpen();
}