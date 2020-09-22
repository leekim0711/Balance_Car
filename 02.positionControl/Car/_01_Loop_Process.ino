/*=========================================================================================================
==== 不斷執行 =============================================================================================
=========================================================================================================*/
void LoopProcess() {
  char* c;  //motor controller
  
  /*if(myBT.available()){
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
    Kp_the = atof(Kp_buf);
    Ki_the = atof(Ki_buf);
    Kd_the = atof(Kd_buf);
    pid_the.SetPID (Kp_the, Ki_the, Kd_the );
  }

  String info = String("") + ypr[2]*RAD_TO_DEG + '~' + avg_the*RAD_TO_DEG + '|';
  char *result = info.c_str();
  myBT.WriteString(result);
  delay(30);*/
  /*countA = 0;
  countB = 0;
  desire_the_cp = 0;
  moveMode_cp = 0;
  delay(5000);
  //countA = 0;
  //countB = 0;
  desire_the_cp = 2200*(DEG_TO_RAD);
  moveMode_cp = 0;
  //while( countB < 1980 || countB > 2166 ){
  delay(1000);
  //}

  //desire_the = 0;
  desire_the_cp = 0;
  countA = 0;
  countB = 0;
  moveMode_cp = 1;
  desire_theR_cp = 240*(DEG_TO_RAD);
  desire_theL_cp = -240*(DEG_TO_RAD);
  delay(8000);*/
}
