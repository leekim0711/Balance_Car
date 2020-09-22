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

void exam(){
  the = (degA-degB)*(3/19.);
  if(state == 0){
    pid_the.SetPID(0.24, 0, 0.08);
    moveMode = 0;
    desire_the = 1680;
    state = 1;
  }
  else if((degA >= 28.5 || degB >= 28.5) && state == 1){
    moveMode = 1;
    att[0] = ypr[0];
    countA = 0;
    countB = 0;
    desire_theR = 300;
    desire_theL = -300;
    state = 2;
  }
  else if(the >= 1.45 && state ==2){
    moveMode = 0;
    countA = 0;
    countB = 0;
    pid_the.SetPID(0.22, 0, 0.05);
    desire_the = 1620;
    state = 3;
  }
  else if((degA >= 28.5 || degB >= 28.5) && state == 3){
    moveMode = 1;
    countA = 0;
    countB = 0;
    desire_theR = 300;
    desire_theL = -300;
    state = 4;
  }
  else if(the >= 1.45 && state ==4){
    moveMode = 0;
    countA = 0;
    countB = 0;
    pid_the.SetPID(0.24, 0, 0.05);
    desire_the = 1620;
    state = 5;
  }
  else if((degA >= 28.5 || degB >= 28.5) && state == 5){
    moveMode = 1;
    countA = 0;
    countB = 0;
    desire_theR = -600;
    desire_theL = 600;
    state = 6;
  }
  else if(the <= -2.9 && state == 6){
    moveMode = 0;
    countA = 0;
    countB = 0;
    desire_the = 0;
    state = 7;
  }
}

void TimerInterrupt(){
    sei();
    runMPU();
    CalculateAngle();
    exam();
    //Serial.println(the);
    avg_the = (degA+degB)/2.;
    
    if(moveMode == 0){
      //pid_the.SetPID(0.35,0,0.08);
      err_the =  desire_the - avg_the;
    //=========Incline the car while moving===========//
      
        if(err_the >= 0)
          desire_phi = err_the*0.01;
        else if(err_the < 0)
          desire_phi = err_the*0.01;
        if(desire_phi >= 0){
          if(desire_phi >= 0.03)
            desire_phi = 0.03;
          else if(desire_phi <= (-0.03))
            desire_phi = (-0.03);
        }
        if(desire_phi < 0){
          if(desire_phi >= 0.06)
            desire_phi = 0.06;
          else if(desire_phi <= (-0.06))
            desire_phi = (-0.06);
        }
      err_phi = -desire_phi+ypr[2]*1.25;
      Vs_the = pid_the.Update(err_the);
      
      if(Vs_the > 5)
        Vs_the = 5;
      else if(Vs_the <-5)
        Vs_the = -5;

      Vs_phi = pid_phi.Update(err_phi);
      Vs = Vs_phi*0.6 + Vs_the*0.4;

      if(Vs > 12)
        Vs = 11;
      else if(Vs < -12)
        Vs = -11;
      
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

    if(moveMode == 1){
      //pid_the.SetPID ( 5, 0, 1 );
      pid_the.SetPID(0.8,0,0.3);
      att[1] = ypr[1];
      
      err_theL = desire_theL - degB;
      err_theR = desire_theR - degA;

      err_theR *= 2;
      err_theL *= 2;
      
      err_phi = ypr[2]*1.25;
      Vs_theR = pid_the.Update(err_theR);
      Vs_theL = pid_the.Update(err_theL);

      //For turn 90 degree, saturation is 2
      //For turn arch, saturation should at least 5
      if(Vs_theR > 1.5)
        Vs_theR = 1.5;
      else if(Vs_theR <-1.5)
        Vs_theR = -1.5;
      if(Vs_theL > 1.5)
        Vs_theL = 1.5;
      else if(Vs_theL <-1.5)
        Vs_theL = -1.5;

      Vs_phi = pid_phi.Update(err_phi);


      Vs_R = Vs_phi*0.6 + Vs_theR*0.5;
      Vs_L = Vs_phi*0.6 + Vs_theL*0.5;


      if(Vs_R > 12)
        Vs_R = 11;
      else if(Vs_R < -12)
        Vs_R = -11;
      if(Vs_L > 12)
        Vs_L = 11;
      else if(Vs_L < -12)
        Vs_L = -11;

      
      if(Vs_R<0){
        V_pwmR = (Vs_R*256.)/12.;
        move(2,-V_pwmR,0);
      }
      else if(Vs_R>0){
        V_pwmR = (Vs_R*256.)/12.;
        move(2,V_pwmR,1);
      }
      
      if(Vs_L<0){
        V_pwmL = (Vs_L*256.)/12.;
        move(1,-V_pwmL,0);
      }
      else if(Vs_L>0){
        V_pwmL = (Vs_L*256.)/12.;
        move(1,V_pwmL,1);
      }
    }

    ms_counter++;
    s_counter += ms_counter/100;
    ms_counter = ms_counter%100;
//    Serial.println(s_counter + ms_counter*0.01);

//- forward 1 to 2 -------------------------------------//
    /*if(s_counter == 7){
      pid_the.SetPID(0.32, 0, 0.08);
     
      desire_the = 1680 * (DEG_TO_RAD);
      //desire_theL = 1680 * (DEG_TO_RAD);
      moveMode = 0;
      state = 1;
    }
    
    if(s_counter == 17){
      moveMode = 0;
      countA = 0;
      countB = 0;
      desire_the = 0;
      desire_theR = 0;
      desire_theL = 0;
      state = 10;
    }*/
    
//- turn right 90 degree -------------------------------//
    /*if(s_counter == 19){
      countA = 0;
      countB = 0;
      desire_theR = 300*(DEG_TO_RAD);
      desire_theL = -300*(DEG_TO_RAD);
      moveMode = 1;
      state = 2;
    }
    if(s_counter == 23){
      pid_the.SetPID(0.12, 0, 0.08);
     
      desire_the = 1700 * (DEG_TO_RAD);
      moveMode = 0;
      state = 3;
    }
    if(s_counter == 30){
      moveMode = 0;
      countA = 0;
      countB = 0;
      desire_the = 0;
      state = 4;
    }
    if(s_counter == 35){
      countA = 0;
      countB = 0;
      desire_theR = 300*(DEG_TO_RAD);
      desire_theL = -300*(DEG_TO_RAD);
      moveMode = 1;
      state = 2;
    }
    if(s_counter == 37){
      pid_the.SetPID(0.12, 0, 0.08);
     
      desire_the = 1700 * (DEG_TO_RAD);
      moveMode = 0;
      state = 3;
    }
/*///- forward 2 to 3 -------------------------------------//
    /*if(s_counter + ms_counter*0.01 == c + 5){
      pid_the.SetPID(0.12, 0, 0.08);
      countA = 0;
      countB = 0;
      desire_the = 940 * (DEG_TO_RAD);
      moveMode = 0;
      state = 3;
    }
    if(avg_the == 940 * (DEG_TO_RAD)){
      pid_the.SetPID(0.12, 0, 0.08);
     
      desire_the += 940 * (DEG_TO_RAD);
      moveMode = 0;
      state = 3;
    }
//- turn right 90 degree -------------------------------//
    if(avg_the == 1880 * (DEG_TO_RAD)){
      countA = 0;
      countB = 0;
      desire_theR = 300*(DEG_TO_RAD);
      desire_theL = -300*(DEG_TO_RAD);
      moveMode = 1;
      state = 4;
      c = s_counter+ms_counter*0.01;
    }
//- forward 3 to 4 -------------------------------------//
    if(s_counter + ms_counter*0.01 == c + 5){
      pid_the.SetPID(0.12, 0, 0.08);
      countA = 0;
      countB = 0;
      desire_the = 940 * (DEG_TO_RAD);
      moveMode = 0;
      state = 5;
    }
    if(avg_the == 940 * (DEG_TO_RAD)){
      pid_the.SetPID(0.12, 0, 0.08);
     
      desire_the += 940 * (DEG_TO_RAD);
      moveMode = 0;
      state = 5;
    }
//- turn right 180 degree ------------------------------//
    if(avg_the == 1880 * (DEG_TO_RAD)){
      countA = 0;
      countB = 0;
      desire_theR = 600*(DEG_TO_RAD);
      desire_theL = -600*(DEG_TO_RAD);
      moveMode = 1;
      state = 6;
      c = s_counter+ms_counter*0.01;
    }
//- forward 4 to 2 -------------------------------------//
    if(s_counter + ms_counter*0.01 == c + 10){
      countA = 0;
      countB = 0;
      desire_theR = 7.7;
      desire_theL = 44.2;
      moveMode = 0;
      state = 6;
      c = s_counter+ms_counter*0.01;
    }*/
//- stop -----------------------------------------------//
    /*if(s_counter == 50){
      moveMode = 0;
      countA = 0;
      countB = 0;
      desire_the = 0;
      state = 10;
    }
    
    /*if( s_counter == 15 ) {
      desire_theR = 500*(DEG_TO_RAD);
      desire_theL = 0*(DEG_TO_RAD);
      countA = 0;
      countB = 0;
      moveMode = 1;
    }
    else if ( s_counter == 17 ) {
      countA = 0;
      countB = 0;
      desire_the += 1880*(DEG_TO_RAD);
      moveMode = 0;
    }*/
}
