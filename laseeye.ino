#include <IRremote.hpp>
#include <Servo.h>

Servo servo1;
int RECV_PIN = 2;
int SERVO_PIN = 3;
int iRemoteCommand[21] = 
  {69,70,71,
   68,64,67,
   7 ,21,9 ,
   22,25,13,
   12,24,94,
   8 ,28,90,
   66,82,74};
//make a dictionary for each command = corresponding action

void setup(){
  Serial.begin(9600);
  Serial.println("Serial 9600");
  myservo.attach(SERVO_PIN);
  IrReceiver.begin(RECV_PIN); // Start the receiver
  Serial.println("IrReceiver.begin");
}

void loop(){
    if (IrReceiver.decode()){
      int iCommandNum(step1());
      if(iCommandNum != 0){
        Serial.println(iCommandNum);
      }
      IrReceiver.resume(); // Enable receiving of the next value
    }
}

//IRremote input function
int step1(){
    //if (IrReceiver.decode()){
      int i = IrReceiver.decodedIRData.command;
      
      if (i == iRemoteCommand[13] ||
      i == iRemoteCommand[15] ||
      i == iRemoteCommand[17] ||
      i == iRemoteCommand[19]){
       return i;
       //Serial.println(IrReceiver.decodedIRData.command);
      }
      else if(IrReceiver.decodedIRData.flags == 0){
       Serial.println("command: ");
       return i;
       //Serial.println(IrReceiver.decodedIRData.command);
       //IrReceiver.printIRResultShort(&Serial); // Print complete received data in one line
      }else {return 0;}
    //}
}
