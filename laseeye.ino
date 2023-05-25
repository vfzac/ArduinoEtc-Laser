#include <IRremote.hpp>
#include <Servo.h>
#include <Stepper.h>

#define STEPS 2048
#define STEP1 48  //normal step count
#define STEP2 STEPS / 12 //skip step count
#define STEPSPEED 14

Servo servo1;
Stepper stepper(STEPS, 4, 5, 6, 7);
int RECV_PIN = 2;
int SERVO_PIN = 3;
//const int iRCom[21] = 
//  {69,70,71,  //row1
//   68,64,67,
//   7 ,21,9 ,  //row3
//   22,25,13,
//   12,24,94,  //row5
//   8 ,28,90,
//   66,82,74}; //DEC codes for the specific remote sorted from left to right, up to down
const int iUp = 70, iLeft = 68, iRight = 67, iDown = 21,
          iSkipUp = 69, iSkipLeft = 7, iSkipRight = 9, iSkipDown = 71,
          iBow = 22, i90CCW = 25, i90CW = 13, i180CCW = 24, i180CW = 94,
          i20Deg = 12, iDefaultPos = 64; //keybinds

void setup(){
  Serial.begin(9600);
  Serial.println(F("Serial 9600"));
  IrReceiver.begin(RECV_PIN); // Start the receiver
  Serial.println(F("IrReceiver.begin"));
  servo1.attach(SERVO_PIN);// Start the servo
  servo1.write(0); //set the servo to zero position
  stepper.setSpeed(STEPSPEED);
}

void loop(){
  
  ttemp();
  
}

void ttemp(){
  //check if there is an incoming IR signal
  if (IrReceiver.decode()){

    //save the command code to an int
    int iCommandCodeOut(IR_Remote_F());

    //check if the value is not unknown
    if(iCommandCodeOut != 0){
      //everything else here(tentative)
      Servo_Handler(iCommandCodeOut);
      Stepper_Motor_Handler(iCommandCodeOut);
      Presets_Function(iCommandCodeOut);
    }
    IrReceiver.resume(); // Enable receiving of the next value
  }
}

//IRremote input function
int IR_Remote_F(){
      int i = IrReceiver.decodedIRData.command;
      if (i == iUp || i == iLeft || i == iRight || i == iDown) return i; //Serial.println(IrReceiver.decodedIRData.command);}
      else if(IrReceiver.decodedIRData.protocol == 0) return 0; //return zero if the protocol is unknown::happened when the remote is not pointed in the receiver
      else if(IrReceiver.decodedIRData.flags == 0) return i;//zero here means there's no repeat for holding down the remote button
      else return 0;
}

//stepper motor Handler
void Stepper_Motor_Handler(int iCommandCodeOut){
  
  switch (iCommandCodeOut){
    case iLeft:
      stepper.step(STEP1);
      break;
    case iRight:
      stepper.step(-STEP1);
      break;
    case iSkipLeft:
      stepper.step(STEP2);
      break;
    case iSkipRight:
      stepper.step(-STEP2);
      break;
  }
}

//servo handler function
void Servo_Handler(int iCommandCodeOut){
  int iCurPos = servo1.read();//save the current position of the servo
  int n = 5, m = 30;
  switch (iCommandCodeOut){
    case iUp:
      iCurPos+= n;//adds to the position value by n *only* if the value is below 180
      break;
    case iDown:
      iCurPos-= n;//subtracts to the position value by n *only* if the value is above zero
      break;
    case iSkipUp:
      iCurPos+= m;
      break;
    case iSkipDown:
      iCurPos-= m;
      break;
    }
  if(iCurPos != servo1.read() && (iCurPos <= 180 || iCurPos >=0)) {
    /////////////////
    servo1.write(iCurPos);// sets the servo position according to the new value
    /////////////////
  }
}

void Presets_Function(int iCommandCodeOut){
  switch (iCommandCodeOut){
    case iBow: //bow down
      servo1.write(120);
      delay(500);
      servo1.write(0);
      break;
    case iDefaultPos: //flip y, 0 or 180
      if (servo1.read() == 0) servo1.write(180);
      else servo1.write(0);
      //delay(450);
      break;
    case i90CCW: //x axis rotate
      stepper.step(-(STEPS / 4));
      break;
    case i90CW: //x axis rotate
      stepper.step(STEPS / 4);
      break;
    case i180CCW: //x axis rotate
      stepper.step(-(STEPS / 2));
      break;
    case i180CW: //x axis rotate
      stepper.step(STEPS / 2);
      break;
    case i20Deg:
      servo1.write(110);
      break;
  }
}



//////
//end
//////
