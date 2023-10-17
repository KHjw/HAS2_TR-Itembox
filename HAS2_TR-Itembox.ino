 /*
 *
 * @file TR_Itembox.ino
 * @author 홍진우 (kevinlike@naver.com)
 * @brief
 * @version 1.0
 * @date 2023-05-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "TR_Itembox.h"

void setup() {
  Serial.begin(115200);
  has2_mqtt.Setup("train_room", "Code3824@", callback);    // tp-link 접속; ALL, myMAC 구독;
  has2_mqtt.Situation("start", "my");
  TimerInit();
  NeopixelInit();
  EncoderInit();
  NextionInit();
  RfidInit();
  MotorInit();
}

void loop() {
  has2_mqtt.ReadSubscirbe();
  device_ptr();
  BlinkTimer.run();
  GameTimer.run();
}