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
  BlinkTimerStart(ALLNEO, WHITE);            // 전체 흰색 점멸 시작
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
    BlinkTimer.deleteTimer(BlinkTimerId);       // 내부태그 노란색 점멸 시작
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
  RfidMMMMLoop(OUTPN532);
}

void Game_Used(){
  Game_ptrPrint("Game_Used");
  RfidLoop(OUTPN532);
  RfidLoop(INPN532);
}

void Game_Void(){               //포인터 초기세팅용 void함수
}

void Game_ptrPrint(String print){           // 게임 포인터 상태 SerialPrint
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
  attachInterrupt(encoderPinA, updateEncoder, CHANGE);    // 엔코더 시작
  attachInterrupt(encoderPinB, updateEncoder, CHANGE);
  GameTimer.deleteTimer(GameTimerId);
  GameTimerId = GameTimer.setInterval(GameTime, GameTimerFunc); // 타임아웃 게임타이머 활성화

  game_ptr = Game_Quiz;
  Serial.println("Quiz Started");
}

void Quiz_Solved(){
  Serial.println("Quiz Solved");
  GameTimer.deleteTimer(GameTimerId);         // 타임아웃 게임타이머 비활성화

  ItemBoxSelfOpen = true;
  AllNeoColor(BLUE);
  ExpSend();                    // 경험치 정보 
  BatteryPackSend();            // 배터리 개수 정보
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