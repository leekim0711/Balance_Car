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
    char dir_buf[1],sp_buf[10];
    delay(20);
    c = myBT.ReadString();
    while(c[index] != '#'){
      if(c[index] != ','){
        if(delimeter_flag == 0){
         dir_buf[sub_index] = c[index];
          sub_index++;
        }
        else if(delimeter_flag == 1){
          sp_buf[sub_index] = c[index];
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
    remote_sp = atof(sp_buf);
    if(dir_buf[0] == 'w'){
      remote_sp = remote_sp;
      move_flag = true;
    }
    if(dir_buf[0] == 's'){
      remote_sp = -remote_sp;
      move_flag = true;
    }
    if(dir_buf[0] == 'x'){
      remote_sp = 0;
      countA = 0;
      countB = 0;
      move_flag = false;
    }
    Serial.println(remote_sp);
    //pid_sp.SetPID (atof(Kp_buf), atof(Ki_buf), atof(Kd_buf) );
  }
  
  String info = String("") + ypr[2]*RAD_TO_DEG + '~' + sp + '|';
  char *result = info.c_str();
  myBT.WriteString(result);
  delay(30);
}
