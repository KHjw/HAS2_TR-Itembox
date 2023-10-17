void MotorInit(){
    //Linear Motor Init
    pinMode(BOXSWITCH_PIN,INPUT_PULLUP);
    pinMode(MOTOR_INA1_PIN, OUTPUT);
    pinMode(MOTOR_INA2_PIN, OUTPUT);
    ledcSetup(MotorLedChannel, MotorFreq, MotorResolution);
    ledcAttachPin(MOTOR_PWMA_PIN, MotorLedChannel);
    ledcWrite(MotorLedChannel, 0);
    BoxClose();
    
    //Vibration Motor Init
    pinMode(VIBRATION_ANSWER_PIN, OUTPUT);
    digitalWrite(VIBRATION_ANSWER_PIN, HIGH);
    ledcSetup(VibrationLedChannel, MotorFreq, MotorResolution);
    ledcAttachPin(VIBRATION_RANGE_PIN, VibrationLedChannel);
    ledcWrite(VibrationLedChannel, 0);
}

void BoxClose(){
    Serial.println("BOX Close");
    ledcWrite(MotorLedChannel, MotorMAX_DUTY_CYCLE - 1);
    digitalWrite(MOTOR_INA1_PIN, LOW);
    digitalWrite(MOTOR_INA2_PIN, HIGH);
    IsBoxOpen = false;
    delay(4000);
    Serial.println("BOX Closed");
}

void BoxOpen(){
    Serial.println("BOX Open");
    ledcWrite(MotorLedChannel, MotorMAX_DUTY_CYCLE - 1);
    digitalWrite(MOTOR_INA1_PIN, HIGH);
    digitalWrite(MOTOR_INA2_PIN, LOW);
    IsBoxOpen = true;
    delay(4000);
    Serial.println("BOX Opened");
}

void MotorStop(){
    Serial.println("모터 스탑");
    digitalWrite(MOTOR_INA1_PIN, LOW);
    digitalWrite(MOTOR_INA2_PIN, LOW);
}

void Encoder_VibrationStrength(int answer){
    int DifferenceValue = abs(answer - (encoderValue/4));
    int VibeStrength = 0;

    if(DifferenceValue < Answer_Range + Vibration_Range * 0)       VibeStrength = 0;    // 정답
    else if(DifferenceValue < Answer_Range + Vibration_Range * 1)  VibeStrength = 1;
    else if(DifferenceValue < Answer_Range + Vibration_Range * 2)  VibeStrength = 2;
    else if(DifferenceValue < Answer_Range + Vibration_Range * 3)  VibeStrength = 3;
    else VibeStrength = 4;
    ledcWrite(VibrationLedChannel, Vibration_Strength[VibeStrength]);
}
