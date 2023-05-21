#include <IRremote.hpp>
#include <Servo.h>

Servo servo1;
int RECV_PIN = 2;
int SERVO_PIN = 3;
const int iUp = 24, iLeft = 8, iRight = 90, iDown = 82; //refers to the command code designated to be arrow keys
int iRemoteCommand[21] = 
  {69,70,71,
   68,64,67,
   7 ,21,9 ,
   22,25,13,
   12,24,94,
   8 ,28,90,
   66,82,74}; //DEC codes for the specific remote sorted from left to right, up to down

void setup(){
  Serial.begin(9600);
  Serial.println(F("Serial 9600"));
  IrReceiver.begin(RECV_PIN); // Start the receiver
  Serial.println(F("IrReceiver.begin"));
  servo1.attach(SERVO_PIN);// Start the servo
  servo1.write(0); //set the servo to zero position
}

void loop(){
  //check if there is an incoming IR signal
  if (IrReceiver.decode()){

    //save the command code to an int
    int iCommandCodeOut(IR_Remote_F());

    //check if the value is not unknown
    if(iCommandCodeOut != 0){
      
      //everything else here(tentative)
      Servo_Handler(iCommandCodeOut);
    }
    IrReceiver.resume(); // Enable receiving of the next value
  }
}

//IRremote input function
int IR_Remote_F(){
      int i = IrReceiver.decodedIRData.command;
      if (i == iUp || i == iLeft || i == iRight || i == iDown){ return i; //Serial.println(IrReceiver.decodedIRData.command);}
      else if(IrReceiver.decodedIRData.protocol == 0) return 0; //return zero if the protocol is unknown::happened when the remote is not pointed in the receiver
      else if(IrReceiver.decodedIRData.flags == 0){ //zero here means there's no repeat for holding down the remote button
       IrReceiver.printIRResultShort(&Serial); //print the data to serial
       return i;
      }else {return 0;}
}

//servo handler function
void Servo_Handler(int iCommandCodeOut){
  int curPos = servo1.read();//save the current position of the servo
    switch (iCommandCodeOut){
      case iUp:
        if(curPos<=179) curPos++;//adds to the position value by 1 *only* if the value is below 180
        break;
      case iDown:
        if (curPos>=0) curPos--;//subtracts to the position value by 1 *only* if the value is above zero
        break;
    }
  if(curPos != servo1.read()) servo1.write(curPos);// sets the servo position according to the new value
  delay(150);// delays the next requests in ms
}
