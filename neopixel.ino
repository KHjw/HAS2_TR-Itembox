void NeopixelInit(){
  for(int i=0; i<NeoNum; i++){
    pixels[i].begin();
  }
  AllNeoColor(RED);
}

void AllNeoColor(int color_code){
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
  rev_neo = BLUE0 + (rev-1);           // 나머지 구역 네오픽셀 색상코드 :: 1번째 바퀴부터 BLUE0,1,2

  for(int i=0; i<NumPixels[ENCODER]; i++){
    pixels[ENCODER].setPixelColor(i, pixels[ENCODER].Color(color[rev_neo][0], color[rev_neo][1], color[rev_neo][2]));
  }
  pixels[ENCODER].setPixelColor(point, color[RED][0], color[RED][1], color[RED][2]);     // 엔코더 위치 point 네오픽셀  
  pixels[ENCODER].show();
} 

void Scenario_WaitBlink(int neoID){
  if(IsScenarioMode){
    BlinkTimer.deleteTimer(BlinkTimerId);
    BlinkTimerStart(neoID, WHITE);               //내부태그 노란색 점멸 시작
  }
}