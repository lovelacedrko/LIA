/*
the goal of this program is to put all the code into one program in such a way that when the IR remote 
is pressed different modes will be preformed. when 1 is pressed, obstacle program is done ,and when 2 is 
pressed ,linetracking program is done and when 3 is pressed the manual programis done.
*/

// Include IR Remote Library by Ken Shirriff:
#include <IRremote.h>
#define F 16736925 // FORWARD
#define B 16754775 // BACKWARD
#define L 16720605 // LEFT
#define R 16761405 // RIGHT
#define S 16712445 // STOP
// Define sensor pin:
const int RECV_PIN = 9;

//movement
#define motor1 8
#define motor2 7
#define speed1 5
#define speed2 6

//obstacle
#include <Servo.h> //include servo library:
Servo servo_tilt; // servo 
#define front 90
#define lft 135
#define rgt 45
  //ultrasonic
#define trigPin 13// define trigger pin as 13:
#define echoPin 12// define echo pin as 12:
 float duration, distance;
 int angle;



// Define integer to remember toggle state
int togglestate = 0;

// Define IR Receiver and Results Objects
IRrecv irrecv(RECV_PIN);
decode_results results;
 
//[line_tracking]
//define sensor pins
#define PIN_R A0
#define PIN_M A1 
#define PIN_L A2
int RValue;
int LValue;
int MValue;

int mode = 3;// initialize mode when robot is not set to any mode:

void setup(){
  Serial.begin(9600);//initialize serial comunication at 9600 bits per second:
  // Enable the IR Receiver
  irrecv.enableIRIn();
 // set pins as outputs
    for (int i=5 ;i<9 ;i++){
pinMode(i, OUTPUT);}
 digitalWrite(3, HIGH);//initialize standby pin:

//obstacle
  pinMode(trigPin, OUTPUT);//set trigpin pin as output:
  pinMode(echoPin, INPUT);//set echopin pin as input:
  servo_tilt.attach(11);

  //[line_tracking]set pins as input:
pinMode(PIN_R, INPUT);
pinMode(PIN_M, INPUT);
pinMode(PIN_L, INPUT);
}


void loop(){
 //if buttons on the IR remote are pressed ,the robot will perform a certain function: 
    if (irrecv.decode(&results)){
 Serial.println(results.value, HEX);
        irrecv.resume(); 
 
        switch(results.value){
          //if mode==3 perform munual program:
          if(mode==3){
          case 0xFF629D: 
         forward();
          
        break;
   
          case 0xFFA857: 
       backward();
         break;
        
        case 0xFF02FD:
        stop();
         break;

        case 0xFF22DD:
        left();
        break;
          
        case 0xFFC23D:
        right();
        break; }       
        
       case 0xFF6897:
       mode =1;
       stop();
       
       break;
      
      case 0xFF9867:
       mode = 2;
       stop();
       break;
      
      case 0xFFB04F:
      mode= 3;
       stop();
       break;      
        }
       
        
    }
    // if mode ==1 perform obstacle program:
   if (mode==1){
     // write a pulse to the HC-SR04 Trigger pin:
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

//measure the response from the HC-SR04 echo pin
duration = pulseIn(echoPin, HIGH);

//determine distance from duration
//use 343 meters per sec as speed of sound

distance =(duration / 2) * 0.0343;

//send results to serialmonitor
 Serial.print("Distance = ");
if ( distance <= 2){
  Serial.println("Out of range");
}
else {
  Serial.print(distance);
  Serial.println(" cm ");
  delay(50);
  }
 
//moves forward 
  forward();
//stop if an obstacle is in range:
if(distance <= 15 || distance <= 2 ){
 digitalWrite(8, 1);
 digitalWrite(7, 1);
 analogWrite(5, 0);
 analogWrite(6, 0); 
delay(1000);
//move left for half a second:
left();
delay(500);
//move right for a second:
right();
delay(1000);
 } 
 


   }
//if mode==2 perform line tracking program:
  if(mode==2){
  int RValue= analogRead(A0); // read value from pin:
float Right = RValue ;//right sensor value
Serial.print("RValue =");//display message
Serial.print(Right);//display value

 int MValue= analogRead(A1); // read value from pin:
float Middle = MValue ;//middle sensor value
Serial.print("   MValue =");//display message
Serial.print(Middle);//display value

int LValue= analogRead(A2); // read value from pin:
float Left = LValue ;//left sensor value
Serial.print("  LValue =");//display message
Serial.println(Left);//display value


//moves forward if middle sensor is on the line:
if ( Middle >= 70 ){
 digitalWrite(8, 1);
 digitalWrite(7, 1);
 analogWrite(5, 70);
 analogWrite(6, 70);
}
// moves left if left sensor is on the lien:
else if (Left >= 70){
  digitalWrite(8, 0);
 digitalWrite(7, 1);
 analogWrite(5, 70);
 analogWrite(6, 70);
}
//moves right if right sensor is on the line:
else if (Right >= 70){
  digitalWrite(8, 1);
 digitalWrite(7, 0);
 analogWrite(5, 70);
 analogWrite(6, 70);
}
//stop when the robot picked up:
 if(Middle >800 && Left>800 && Right>800){
  stop();
}
  }
  
  }


/* funtions that make the robot move forward,backward,left,right,
forward left,forward right,backward left,backward right,  and stop respectively.*/
void forward(){
 digitalWrite(8, 1);
 digitalWrite(7, 1);
 analogWrite(5, 126);
 analogWrite(6, 126);
}
void backward(){
 digitalWrite(8, 0);
 digitalWrite(7, 0);
 analogWrite(5, 126);
 analogWrite(6, 126);
}
void left(){
 digitalWrite(8, 0);
 digitalWrite(7, 1);
 analogWrite(5, 126);
 analogWrite(6, 126);
}
 void right(){
 digitalWrite(8, 1);
 digitalWrite(7, 0);
 analogWrite(5, 126);
 analogWrite(6, 126);
}
 void forwardleft(){
 digitalWrite(8, 1);
 digitalWrite(7, 1);
 analogWrite(5, 126);
 analogWrite(6, 63);
}
 void forwardright(){
 digitalWrite(8, 1);
 digitalWrite(7, 1);
 analogWrite(5, 63);
 analogWrite(6, 126);
}
 void backwardleft(){
 digitalWrite(8, 0);
 digitalWrite(7, 0);
 analogWrite(5, 126);
 analogWrite(6, 63);
}
void backwardright(){
 digitalWrite(8, 0);
 digitalWrite(7, 0);
 analogWrite(5, 63);
 analogWrite(6, 126);
}
void stop(){
 digitalWrite(8, 1);
 digitalWrite(7, 1);
 analogWrite(5, 0);
 analogWrite(6, 0);
}
