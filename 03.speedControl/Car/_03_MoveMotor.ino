/*=========================================================================================================
==== 定義每一隻接腳的狀態 =================================================================================
=========================================================================================================*/
void initMotor() {
  pinMode(STBY, OUTPUT);                                                    // for standby
  pinMode(PWMA, OUTPUT);  pinMode(AIN1, OUTPUT);  pinMode(AIN2, OUTPUT);    // for motor A
  pinMode(PWMB, OUTPUT);  pinMode(BIN1, OUTPUT);  pinMode(BIN2, OUTPUT);    // for motor B
}

/*=========================================================================================================
==== 停止馬達驅動版(煞車) =================================================================================
=========================================================================================================*/
void stop(){
  digitalWrite(STBY, LOW); 
}

/*=========================================================================================================
==== 馬達轉動底層控制函式 =================================================================================
=========================================================================================================*/
void move(int motor, int speed, int direction) {
 // Move specific motor at speed and direction
 // motor: 1 for A, 2 for B
 // speed: 0 is off, and 255 is full speed
 // direction: 0 clockwise, 1 counter-clockwise
  boolean inPin1 = LOW;                         // 初始轉動方向為 clockwise
  boolean inPin2 = HIGH;
  digitalWrite(STBY, HIGH);                     // disable standby(可動)
  if(direction == 1) {                          // 若 direction 指定為 1 時，轉動方向為 counterclockwise
    inPin1 = HIGH;
    inPin2 = LOW;
  }
  if(motor == 1) {                              // 指定 motor A
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  }
  else if(motor == 2) {                         // 指定 motor B
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }
}
