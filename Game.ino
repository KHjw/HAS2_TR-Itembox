void Game_Ready(){
  
}

void Game_Selected(){
  
}

void Game_PN532_login(){

  game_ptr = Game_Quiz;

  attachInterrupt(encoderPinA, updateEncoder, CHANGE);
  attachInterrupt(encoderPinB, updateEncoder, CHANGE);
}

void Game_Quiz(){
  if(!(QuizCount == 3)){
    Quiz_System();
  }
  else{                         //Quiz 3회 성공
    Serial.println("CLEAR!!!");
    SendCmd("wQuizSolved.en=1");

    pixels[PN532].lightColor(color[YELLOW]);
    QuizCount = 0;
    detachInterrupt(encoderPinA);               //엔코더 사용 막기
    detachInterrupt(encoderPinB);

    game_ptr = Game_PN532_open;
  }
}

void Game_PN532_open(){
  // 리니어모터 오픈;
  game_ptr = Game_InnerTag;
}

void Game_InnerTag(){
  game_ptr = Game_Used;
}

void Game_Used(){

}

void Game_Void(){               //포인터 초기세팅용 void함수
}