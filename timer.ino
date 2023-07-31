void TimerInit(){
    BlinkTimerId = BlinkTimer.setInterval(BlinkTime, BlinkTimerFunc);
    BlinkTimer.deleteTimer(BlinkTimerId);
    GameTimerId = GameTimer.setInterval(GameTime, GameTimerFunc);
    GameTimer.deleteTimer(GameTimerId);
}

//****************************************Blink Timer****************************************
void BlinkTimerStart(int Neo, int NeoColor){
    blinkNeo = Neo;
    blinkColor = NeoColor;
    if(Neo == ALLNEO)   BlinkTimerId = BlinkTimer.setInterval(BlinkTime, BlinkAllTimerFunc);
    else                BlinkTimerId = BlinkTimer.setInterval(BlinkTime, BlinkTimerFunc);
}

void BlinkTimerFunc(){
    Serial.println("Blink!");
    if(blinkOn == true){
        pixels[blinkNeo].lightColor(color[blinkColor]);
        blinkOn = false;
    }
    else{
        pixels[blinkNeo].lightColor(color[BLACK]);
        blinkOn = true;
    }
}

void BlinkAllTimerFunc(){
    Serial.println("ALL Blink!");
    if(blinkOn == true){
        for(int i=0; i<NeoNum; i++){
            pixels[i].lightColor(color[blinkColor]);
        }
        blinkOn = false;
    }
    else{
        for(int i=0; i<NeoNum; i++){
            pixels[i].lightColor(color[BLACK]);
        }
        blinkOn = true;
    }
}

void GameTimerFunc(){
    if(GameTimeCNT >= 1){
        if(GameTimeCNT >= GameTimeCNT_Max){
            Serial.println("TimeOut ==> Login Reset");
            ledcWrite(VibrationLedChannel, 0);          // 진동모터 비활성화
            detachInterrupt(encoderPinA);               // 엔코더 비활성화
            detachInterrupt(encoderPinB);
            BlinkTimer.deleteTimer(BlinkTimerId);
            GameTimer.deleteTimer(GameTimerId);
            lastEncoded = 0;
            encoderValue = 41*4; 
            GameTimeCNT = 0; 
            SendCmd("page pgWait");
            device_ptr = Device_Login;
        }
        else{
            long TimeLeft = (GameTimeCNT_Max - GameTimeCNT)*GameTime/1000;
            Serial.println("TimeOut in " + (String)(TimeLeft) + "sec");
        }
    }
    GameTimeCNT++;
}