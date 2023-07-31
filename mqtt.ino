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