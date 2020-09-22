void runMPU(){
     mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if (fifoCount >= 1024) {     // check for overflow (this should never happen unless our code is too inefficient)
        mpu.resetFIFO();                                   // reset so we can continue cleanly
        fifoCount = mpu.getFIFOCount();  
        
        Serial.println(F("FIFO overflow!"));// otherwise, check for DMP data ready interrupt (this should happen frequently)
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
            mpu.getFIFOBytes(fifoBuffer, packetSize);         // read a packet from FIFO
        fifoCount -= packetSize;                              // track FIFO count here in case there is > 1 packet available
                                                              // (this lets us immediately read more without waiting for an interrupt)
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetAccel(&aa, fifoBuffer);      
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);    //getYPR
        mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);//getRealXYZ
        mpu.dmpGetEuler(euler, &q);                   //getEuler
    } 
    else if (fifoCount < 1024) {                           // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
            mpu.getFIFOBytes(fifoBuffer, packetSize);         // read a packet from FIFO
        fifoCount -= packetSize;                              // track FIFO count here in case there is > 1 packet available
                                                              // (this lets us immediately read more without waiting for an interrupt)
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetAccel(&aa, fifoBuffer);      
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);    //getYPR
        mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);//getRealXYZ
        mpu.dmpGetEuler(euler, &q);                   //getEuler
        
    }
    mpu.resetFIFO(); 
}

void getMPU(){
    runMPU();
    /*
    Serial.print(euler[0] * 180/M_PI);
    Serial.print(" ");
    Serial.print(euler[1] * 180/M_PI);
    Serial.print(" ");
    Serial.print(euler[2] * 180/M_PI);
    Serial.print(" ");
    Serial.print(aaReal.x);
    Serial.print(" ");
    Serial.print(aaReal.y);
    Serial.print(" ");
    Serial.print(aaReal.z);
    Serial.print(" ");*/
    Serial.print(ypr[0] * 180/M_PI);
    Serial.print(" ");
    Serial.print(ypr[1] * 180/M_PI);
    Serial.print(" ");
    Serial.println(ypr[2] * 180/M_PI);
}
