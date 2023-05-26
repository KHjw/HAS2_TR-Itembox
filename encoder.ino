//**************************************************Encoder**************************************************
long readEncoderValue(void){
    return encoderValue/4;
}

boolean isButtonPushDown(void){
  if(!digitalRead(buttonPin)){
    delay(5);
    if(!digitalRead(buttonPin))
      return true;
  }
  return false;
}

void Encoder_Setup(){
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(buttonPin, INPUT);

  digitalWrite(encoderPinA, HIGH); //turn pullup resistor on
  digitalWrite(encoderPinB, HIGH); //turn pullup resistor on

  attachInterrupt(encoderPinA, updateEncoder, CHANGE);
  attachInterrupt(encoderPinB, updateEncoder, CHANGE);
}

void Encoder_Loop(){                                // "encoder값, 버튼눌림" 을 표시
  Encoder_RevCount();
  EncoderPointNeo();
  Serial.print(readEncoderValue());
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

void Encoder_RevCount(){
  RawVal = readEncoderValue();

  if(RawVal < 0){
    RawVal = 0;
    encoder_revcount[0] = 1;
    encoder_revcount[1] = 0;
  }
  else if(RawVal >= 0 && RawVal < NumPixels[PN532]){
    encoder_revcount[0] = 1;
    encoder_revcount[1] = RawVal;
  }
  else if(RawVal >= NumPixels[PN532] && RawVal < NumPixels[PN532]*2){
    encoder_revcount[0] = 2;
    encoder_revcount[1] = RawVal - NumPixels[PN532];
  }
  else if(RawVal >= NumPixels[PN532]*2 && RawVal < NumPixels[PN532]*3){
    encoder_revcount[0] = 3;
    encoder_revcount[1] = RawVal - NumPixels[PN532]*2;
  }
  else{
    RawVal = NumPixels[PN532]*3-1;
    encoder_revcount[0] = 3;
    encoder_revcount[1] = NumPixels[PN532]-1;
  }
}

void updateEncoder(){
  int MSB = digitalRead(encoderPinA); //MSB = most significant bit
  int LSB = digitalRead(encoderPinB); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue --;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue ++;


  lastEncoded = encoded; //store this value for next time

  //95*4엔코더 값 최대최소 제한 걸어두기    
  if(encoderValue > 380)      encoderValue = 380;              
  else if(encoderValue < 0)   encoderValue = 0;
}

//**************************************************Quiz**************************************************
void Quiz_System(){
  if(isButtonPushDown()){
    GameQuiz_check();
  }
  else{
    Encoder_Progress_Loop();
  }
}

void Quiz_Check(){
  Player_guess = RawVal;

  if(Player_guess == Quiz_answer[QuizCount]){
    Serial.println("[[[[Quiz " + (String)(QuizCount) + "]]]] Success");
    NeoBlink(ENCODER, GREEN, 5, 250);
    QuizCount += 1;
  }
  else{
    Serial.println("Quiz Fail");                            //네오픽셀 빨강
    NeoBlink(ENCODER, RED, 5, 250);
  }
}