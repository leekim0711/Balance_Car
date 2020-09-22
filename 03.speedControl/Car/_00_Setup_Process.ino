/*=========================================================================================================
==== 初始化設定 ===========================================================================================
=========================================================================================================*/
void SetupProcess() {
  initMotor();              // 定義每一隻接腳的連接狀態
  myBT.Begin();
  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(INT_PINA),
                  EncoderInterruptA,
                  RISING);
  attachInterrupt(digitalPinToInterrupt(INT_PINB),
                  EncoderInterruptB,
                  RISING);

  MpuInitialize();

  //pid_phi.SetPID (30.56, 0, 1 );
  pid_phi.SetPID (47, 2000, 0.35 );
  pid_sp.SetPID (1, 200, 0 );
  pid_the.SetPID (0.357, 0, 0.2 );
  
  //desire_sp = desire_the*(DEG_TO_RAD);
  MsTimer2::set(1,TimerInterrupt);
  MsTimer2::start();
}

void MpuInitialize(){
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXAccelOffset(-951);
    mpu.setYAccelOffset(641);
    mpu.setZAccelOffset(1596);
    mpu.setXGyroOffset(340);
    mpu.setYGyroOffset(15);
    mpu.setZGyroOffset(114);
    

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        mpu.setDMPEnabled(true);
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } 
    else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

}
