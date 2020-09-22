void EncoderInterruptA(){
  if(analogRead(DIR_PINA) > 512)
    countA--;
  else
    countA++;
  //countA%=390;
}

void EncoderInterruptB(){
  if(digitalRead(DIR_PINB) == HIGH)
    countB++;
  else
    countB--;
  //countB%=390;
}

void CalculateAngle(){
    degA = 2*PI*((float)countA/(float)SECTION_NUMBER);
    degB = 2*PI*((float)countB/(float)SECTION_NUMBER);
}

void TimerInterrupt(){
    sei();
    runMPU();
    /*CalculateAngle();
    avg_the = (degB+degA)/2.;
    
    //=========Incline the car while moving===========//
    if((err_the >= 0.005 ||err_the <= -0.005)){
      desire_phi = err_the*0.01;
      if(desire_phi >= (0.5*DEG_TO_RAD))
        desire_phi = (0.5*DEG_TO_RAD);
      else if(desire_phi <= (-0.5*DEG_TO_RAD))
        desire_phi = (-0.5*DEG_TO_RAD);
    }
    //=========If in some range,desire phi is 0===========//
    else if(err_the < 0.002 && err_the > -0.002){
      desire_phi = 0;
    }
    */
    err_phi = -desire_phi+ypr[2];
    /*err_the = desire_the - avg_the;
    
    Vs_the = pid_the.Update(err_the);
    //=========To avoid car moving too fast===========//
    if(Vs_the > 1)
      Vs_the = 1;
    else if(Vs_the <-1)
      Vs_the = -1;*/

    Vs_phi = pid_phi.Update(err_phi);
    Vs = Vs_phi;

    
      if(Vs<0){
        V_pwm = (Vs*256.)/12.;
        move(1,-V_pwm,0);
        move(2,-V_pwm,0);
      }
      else if(Vs>0){
        V_pwm = (Vs*256.)/12.;
        move(1,V_pwm,1);
        move(2,V_pwm,1);
      }
    
    
}
