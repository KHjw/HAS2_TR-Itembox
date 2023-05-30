void Game_Ready(){
  
}

void Game_Selected(){
  BlinkTimer.deleteTimer(blinkTimerId);
  BlinkAllTimerStart(WHITE);            // 전체 흰색 점멸 시작
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
  else{                         //Quiz 3회 성공000
    Serial.println("CLEAR!!!");
    SendCmd("wQuizSolved.en=1");

    pixels[PN532].lightColor(color[YELLOW]);
    QuizCount = 0;
    detachInterrupt(encoderPinA);               //엔코더 사용 막기
    detachInterrupt(encoderPinB);

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

void Game_Void(){               //포인터 초기세팅용 void함수
}