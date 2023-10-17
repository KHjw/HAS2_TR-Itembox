//**************************************************Encoder**************************************************
boolean isButtonPushDown(void){
  if(!digitalRead(buttonPin)){
    delay(5);
    if(!digitalRead(buttonPin))
      return true;
  }
  return false;
}

void EncoderInit(){
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(buttonPin, INPUT);

  digitalWrite(encoderPinA, HIGH); //turn pullup resistor on
  digitalWrite(encoderPinB, HIGH); //turn pullup resistor on
}

void updateEncoder(){
  GameTimeCNT = 0;        // 게임카운터 초기화

  int MSB = digitalRead(encoderPinA); //MSB = most significant bit
  int LSB = digitalRead(encoderPinB); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB;            //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded;  //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;


  lastEncoded = encoded; //store this value for next time

  //엔코더 값 최대최소 제한 걸어두기    
  if(encoderValue > NumPixels[ENCODER]*3*4)    encoderValue = NumPixels[ENCODER]*3*4;              
  else if(encoderValue < 0)                    encoderValue = 0;
}

void Encoder_Loop(){
  Encoder_RevCount();                                 // 엔코더 값 변환
  Encoder_PointNeo();                                 // 네오픽셀 값 반영
  Encoder_VibrationStrength(Quiz_answer[QuizCount]);  // 진동 값 반영
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

void Encoder_Print(){              // "encoder값, 버튼눌림" 을 시리얼 출력
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
    Serial.print("Quiz Fail ====> Player Guess :: " + (String)(Player_guess));        // 플레이어 선택
    Serial.println("// Q" + (String)(QuizNum) + " ANS :: " + (String)(Quiz_answer[QuizCount]));   // 퀴즈 정답
    NeoBlink(ENCODER, RED, 5, 250);
  }

  encoderValue = Player_guess*4;
  GameTimeCNT = 0;
}