#include <IRremote.hpp>
#include <Servo.h>
#include <Stepper.h>

#define STEPS 2048
#define STEP1 16  //normal step count
#define STEP2 128 //skip step count

Servo servo1;
Stepper stepper(STEPS, 4, 5, 6, 7);
int RECV_PIN = 2;
int SERVO_PIN = 3;
int iRemoteCommand[21] = 
  {69,70,71,
   68,64,67,
   7 ,21,9 ,
   22,25,13,
   12,24,94,
   8 ,28,90,
   66,82,74}; //DEC codes for the specific remote sorted from left to right, up to down
const int iUp = 70, iLeft = 68, iRight = 67, iDown = 21,
          iSkipUp = 69, iSkipLeft = 7, iSkipRight = 9, iSkipDown = 71; //keybind

void setup(){
  Serial.begin(9600);
  Serial.println(F("Serial 9600"));
  IrReceiver.begin(RECV_PIN); // Start the receiver
  Serial.println(F("IrReceiver.begin"));
  servo1.attach(SERVO_PIN);// Start the servo
  servo1.write(0); //set the servo to zero position
  stepper.setSpeed(5);
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
    }
    IrReceiver.resume(); // Enable receiving of the next value
  }
}

//IRremote input function
int IR_Remote_F(){
      int i = IrReceiver.decodedIRData.command;
      if (i == iUp || i == iLeft || i == iRight || i == iDown) return i; //Serial.println(IrReceiver.decodedIRData.command);}
      else if(IrReceiver.decodedIRData.protocol == 0) return 0; //return zero if the protocol is unknown::happened when the remote is not pointed in the receiver
      else if(IrReceiver.decodedIRData.flags == 0){ //zero here means there's no repeat for holding down the remote button
       //IrReceiver.printIRResultShort(&Serial); //print the data to serial
       return i;
      }else {return 0;}
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
  int n = 3,m = 30;
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
  if(iCurPos != servo1.read() && (iCurPos <= 180 || iCurPos >=0)) servo1.write(iCurPos);// sets the servo position according to the new value
  Serial.print(iCurPos);
  Serial.print(" ");
  Serial.println(servo1.read());
  delay(10);// delays the next requests in ms
}
