/*=========================================================================================================
==== 不斷執行 =============================================================================================
=========================================================================================================*/
void LoopProcess() {
  char* c;  //motor controller
  
 /* if(myBT.available()){
    delay(20);
    c = myBT.ReadString();
    Serial.println(c);
    if(!strcmp(c,"w")){ //move forward
      move(1, 75, 0);
      move(2, 75, 0);
    }
    else if(!strcmp(c,"s")){  //move backward
      move(1, 75, 1);
      move(2, 75, 1);
    }
    else if(!strcmp(c,"d")){  //move rightward
      move(1, 75, 1);
      move(2, 75, 0);
    }
    else if(!strcmp(c,"a")){  //move leftward
      move(1, 75, 0);
      move(2, 75, 1);
    }
    else{  //stop
      stop();
    }
  }*/
  if(myBT.available()){
    int index = 0;
    int sub_index = 0;
    char Kp_buf[10],Ki_buf[10],Kd_buf[10];
    delay(20);
    c = myBT.ReadString();
    while(c[index] != '#'){
      if(c[index] != ','){
        if(delimeter_flag == 0){
          Kp_buf[sub_index] = c[index];
          sub_index++;
        }
        else if(delimeter_flag == 1){
          Ki_buf[sub_index] = c[index];
          sub_index++;
        }
        else if(delimeter_flag == 2){
          Kd_buf[sub_index] = c[index];
          sub_index++;
        }
      }
      else{
        sub_index = 0;
        delimeter_flag++;
      }
      index++;
    }
    delimeter_flag = 0;
    pid_phi.SetPID (atof(Kp_buf), atof(Ki_buf), atof(Kd_buf) );
  }
  
  String info = String("") + ypr[2]*RAD_TO_DEG + '~' + avg_the*RAD_TO_DEG + '|';
  char *result = info.c_str();
  myBT.WriteString(result);
  delay(30);
}
