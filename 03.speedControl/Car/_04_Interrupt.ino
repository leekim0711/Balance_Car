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
    CalculateAngle();

    lastAngle = currentAngle;
    currentAngle = degA;
    sp = (currentAngle - lastAngle)/0.05;
    sp = lowpassFilter.input( sp );
    avg_the = (degA+degB)/2.;

    if(move_flag){
      
      if(desire_sp>0)
        desire_phi = desire_sp*0.015;
      else if(desire_sp<0)
        desire_phi = desire_sp*0.02;
      else
        desire_phi = 0;
   
      err_phi = -desire_phi+(ypr[2]*1.25);
      err_sp = desire_sp - sp;

      Vs_sp = pid_sp.Update(err_sp);
      Vs_phi = pid_phi.Update(err_phi);

      if(Vs_sp > 5)
        Vs_sp = 5;
      else if(Vs_sp < -5)
        Vs_sp = -5;
    
      Vs = Vs_phi*phi_fraction + Vs_sp*sp_fraction;

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

      desire_sp = remote_sp;
    }

    else if(move_flag == false){

      err_the = desire_the - avg_the;
      Vs_the = pid_the.Update(err_the);

      if(Vs_the > 5)
        Vs_the = 5;
      else if(Vs_the <-5)
        Vs_the = -5;

      if(err_the >= 0)
        desire_phi = err_the*0.0001;
      else if(err_the < 0)
        desire_phi = err_the*0.00015;

      if(desire_phi >= 0.03)
        desire_phi = 0.03;
      if(desire_phi <= (-0.06))
        desire_phi = (-0.06);

      err_phi = -desire_phi+(ypr[2]*1.25);
      Vs_phi = pid_phi.Update(err_phi);
      
      Vs = Vs_phi*0.6 + Vs_the*0.4;

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
    /*if(desire_sp == 0 && sp < 0.9 && sp > -0.9){
      phi_fraction = 0.6;
      sp_fraction = 0;
      pid_phi.SetPID (54, 3000, 0.35 );
    }
    else{
      phi_fraction = 0.6;
      sp_fraction = 0.3;
      pid_phi.SetPID (54, 2000, 0.35 );
    }*/
}
