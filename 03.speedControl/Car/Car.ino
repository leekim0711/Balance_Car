#include <BalanbotController.h>

/*-- 程式庫 ---------------------------------------------------------------------------------------------*/
#include "bluetooth.h"
#include <Filters.h>
#include "MsTimer2.h"
#include "I2Cdev.h"
#include "BalanbotController.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

/*-- 腳位定義 -------------------------------------------------------------------------------------------*/
 // If batterys are at backward & user see the car from the backward
 // motor A(right motor) connected between A01 and A02
 // motor B(left motor) connected between B01 and B02
 const int PWMA = 5, PWMB = 9;               // Speed control 
 const int AIN1 = 6, BIN1 = 11;              // Direction +
 const int AIN2 = 4, BIN2 = 10;              // Direction -
 const int STBY = 7;                         // standby(停止)

 #define INT_PINA 2   //Encoder interrupt pin
 #define DIR_PINA A3  //Encoder direction
 #define INT_PINB 3   //Encoder interrupt pin
 #define DIR_PINB 8   //Encoder direction
 
/*-- For theta calculation -------------------------------------------------------------------------------------------*/
 const int SECTION_NUMBER = 390;
 int countA = 0;
 int countB = 0;
 float degA = 0;
 float degB = 0;
 float avg_the;
 
 /*-- For controller -------------------------------------------------------------------------------------------*/
 float Vs,Vs_phi,Vs_sp,V_pwm;
 float desire_sp=0,remote_sp=0;
 float desire_phi=0;
 float err_phi,err_sp;
 PIDController pid_phi,pid_sp,pid_the;
 float Kp_the = 0.317, Ki_the = 0, Kd_the = 0.05;
 char Kp_buf[10],Ki_buf[10],Kd_buf[10];
 int delimeter_flag = 0;

 bool move_flag = false;
 float desire_the=0, err_the, Vs_the;

 float sp;
 float lastAngle = 0.0;
 float currentAngle = 0.0;
 float filterFrequency = 1;
 float phi_fraction = 0.6, sp_fraction = 0.3;
 FilterOnePole lowpassFilter( LOWPASS, filterFrequency ); 
 
/*-- 藍芽傳輸設定 ---------------------------------------------------------------------------------------*/
Bluetooth myBT( 12 , 13);

/*-- 車輪速度轉換相關 -----------------------------------------------------------------------------------*/
 int LEFT = 1;
 int RIGHT = 2;
 float maxPowerRemap = 255/100;

/*-- 車體角度 -----------------------------------------------------------------------------------*/
// MPU control/status vars 參數定義
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars 參數定義
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements

// packet structure for InvenSense teapot demo
// uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

MPU6050 mpu;

/*-- 副函式定義 -----------------------------------------------------------------------------------------*/
 void SetupProcess();
 void LoopProcess();
 void parseCmd();
 void initMotor();
 void stop();
 void moveMotor(int, float);
 void move(int motor, int speed, int direction);

 void EncoderInterruptA();
 void EncoderInterruptB();
 void CalculateAngle();

 void MpuInitialize();
 void runMPU();
 void getMPU();         //trans info of MPU

 void TimerInterrupt();

/*=========================================================================================================
==== Setup ================================================================================================
=========================================================================================================*/
void setup() {
  SetupProcess();
}
/*=========================================================================================================
==== Loop =================================================================================================
=========================================================================================================*/
void loop() {
  LoopProcess();
}
