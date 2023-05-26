void Game_Ready(){
  
}

void Game_Selected(){
  
}

void Game_PN532_login(){
  
}

void Game_Quiz(){
  if(!(QuizCount == 3)){
    Quiz_System();
  }
  else{                         //Quiz 3회 성공
    Serial.println("CLEAR!!!");
    QuizCount = 0;
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