#include <PIDController.h>
#include <Wire.h>
volatile long int encoder1_pos = 0;
volatile long int encoder2_pos = 0;
PIDController pos1_pid; 
PIDController pos2_pid; 
int mode = 0; // 0 - Motionless/Stop , 1 - Clockwise rotation, 2 - Counterclockwise rotation
int motor1_value = 0;
int motor2_value = 0;
long int timer1=0 ;
long int timer2=0 ;
long int timer3=0;

String dataString;
boolean next=false; //to differentiate rotation and movement Phase
long int Phi=0;  //recieved angle in degree as natual Number
long int Dis=0;  //recieved angle in degree as natual Number
long int angle=0;  //inkrements to rotate Robot by angle0 degree
long int distance=0;  // inkrements of a distance to move
long int wheel_dia = 54;  // diameter of a Wheel in mm
long int wheel_dis = 94;  // distance between the Wheels in mm

#define MOTOR1A 4 //IN
#define MOTOR1B 5 //IN
#define MOTOR2A 6 //IN
#define MOTOR2B 7 //IN
#define ENCODER1 3   // PhaseA of Motor
#define ENCODER1d 9  // PhaseB of Motor
#define ENCODER2 2   // PhaseA of Motor
#define ENCODER2d 8  // PhaseB of Motor


boolean next1=false;
boolean rotation=true;

void setup() {
  Wire.begin(8);
  Wire.onReceive(dataReceive);
  attachInterrupt(digitalPinToInterrupt(ENCODER1), encoder1 , RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER2), encoder2 , RISING);
  pinMode(MOTOR1A, OUTPUT);
  pinMode(MOTOR1B, OUTPUT);
  pinMode(MOTOR2A, OUTPUT);
  pinMode(MOTOR2B, OUTPUT);
  pinMode(ENCODER1d, INPUT);
  pinMode(ENCODER2d, INPUT);
  pinMode(ENCODER1, INPUT);
  pinMode(ENCODER2, INPUT);

  pos1_pid.begin();         
  pos1_pid.tune(100,0,5000); // PID constants   
  pos1_pid.limit(-255, 255); // limit of the output-funktion
  pos1_pid.setpoint(0);      // current Ought value
  
  pos2_pid.begin();    
  pos2_pid.tune(100,0,5000); //500,0,15000)  
  pos2_pid.limit(-255, 255);     
  pos2_pid.setpoint(0);
  
  Serial.begin(9600);}

void loop() {

//if(millis()>200 && next1==false){pos2_pid.setpoint(300); next1=true;}
//if(millis()>200 && millis()<1700)
 // {motor2_value = pos2_pid.compute(encoder2_pos);  if(motor2_value > 0){  Motor2Clockwise(motor2_value); }else{  Motor2CounterClockwise(abs(motor2_value)); };};
//if(millis()>1700){MotorStop();};  
//if(millis()<1700) {  Serial.print(millis()); Serial.print("\t"); Serial.print(motor2_value); Serial.print("\t"); Serial.println(encoder2_pos); };


//if(millis()>200 && next1==false){pos1_pid.setpoint(300); next1=true;}if(millis()>200 && millis()<1700)  {motor1_value = pos1_pid.compute(encoder1_pos);  
//if(motor1_value > 0){  Motor1Clockwise(motor1_value); }else{  Motor1CounterClockwise(abs(motor1_value)); };};if(millis()>1700){MotorStop();};  
//if(millis()<1700) {  Serial.print(millis()); Serial.print("\t"); Serial.print(motor1_value); Serial.print("\t"); Serial.println(encoder1_pos); };

  
if(mode == 1 && timer2<50){
   rotation=false;
   pos1_pid.setpoint(angle);
  // pos2_pid.setpoint(angle);
   motor1_value = pos1_pid.compute(encoder1_pos);
   motor2_value = motor1_value;
   //motor2_value = pos2_pid.compute(encoder2_pos) ;                
   if(motor1_value > 0){
   Motor1Clockwise(motor1_value); }else{
   Motor1CounterClockwise(abs(motor1_value)); };
   if(motor2_value > 0){
   Motor2CounterClockwise(motor2_value); }else{
   Motor2Clockwise(abs(motor2_value));};
  if(abs(angle-encoder1_pos)<=12 && next==false){next=true; timer1=millis();}; 
  if(next==true){timer2=millis()-timer1;};}
if(mode == 2 && timer2<50){
   rotation=false;
   pos1_pid.setpoint(-angle);
   //pos2_pid.setpoint(-angle);
   motor1_value = pos1_pid.compute(encoder1_pos);
   motor2_value = motor1_value; 
   if(motor1_value > 0){
   Motor1Clockwise(motor1_value); }else{
   Motor1CounterClockwise(abs(motor1_value)); };
   if(motor2_value > 0){
   Motor2CounterClockwise(motor2_value); }else{
   Motor2Clockwise(abs(motor2_value)); };
  if(abs(angle+encoder1_pos)<=12 && next==false){next=true; timer1=millis();}; 
  if(next==true){timer2=millis()-timer1;};}
if(timer2>=50){
   if(next==true){
   encoder1_pos=0;
   encoder2_pos=0;
   pos1_pid.setpoint(distance);
   pos2_pid.setpoint(distance);
   next=false;
   rotation=true;
   };
   motor1_value = pos1_pid.compute(encoder1_pos);
   motor2_value = motor1_value ; 
   if(motor1_value > 0){
   Motor1Clockwise(motor1_value); }else{
   Motor1CounterClockwise(abs(motor1_value)); }
   if(motor2_value > 0){
   Motor2Clockwise(motor2_value); }else{
   Motor2CounterClockwise(abs(motor2_value));} }

 Serial.print(millis());Serial.print("\t"); 
 Serial.print(encoder1_pos);Serial.print("\t"); 
// Serial.println(encoder2_pos);Serial.print("\t"); 
 Serial.print(angle);Serial.print("\t"); 
 Serial.println(timer2);Serial.print("\t"); 
// Serial.println(angle-encoder1_pos);
 delay(1) ;}

void encoder1(){
  if(digitalRead(ENCODER1d) == HIGH){
    encoder1_pos--;
  }else{
    encoder1_pos++; }}
    
void encoder2(){
  if(digitalRead(ENCODER2d) == HIGH){
    encoder2_pos++;
  }else{
    encoder2_pos--; }}  

void Motor1Clockwise(int power){
  if(power > 100){
  digitalWrite(MOTOR1A, LOW);
  analogWrite(MOTOR1B, power);
  }else{
    digitalWrite(MOTOR1A, LOW);
    digitalWrite(MOTOR1B, LOW);  }}
    
void Motor1CounterClockwise(int power){
  if(power > 100){
  digitalWrite(MOTOR1B, LOW);
  analogWrite(MOTOR1A, power);
  }else{
    digitalWrite(MOTOR1B, LOW);
    digitalWrite(MOTOR1A, LOW);  }}

void Motor2Clockwise(int power){
  if(power > 100){
  digitalWrite(MOTOR2B, LOW);
  analogWrite(MOTOR2A, power);
  }else{
    digitalWrite(MOTOR2A, LOW);
    digitalWrite(MOTOR2B, LOW);  }}   
     
void Motor2CounterClockwise(int power){
  if(power > 100){
  digitalWrite(MOTOR2A, LOW);
  analogWrite(MOTOR2B, power);
  }else{
    digitalWrite(MOTOR2B, LOW);
    digitalWrite(MOTOR2A, LOW);  }}   
         
void MotorStop(){                  //this funktion disables both motors
  digitalWrite(MOTOR1B, LOW);
  digitalWrite(MOTOR1A, LOW);
  digitalWrite(MOTOR2B, LOW);
  digitalWrite(MOTOR2A, LOW);
  mode = 0;
}

void dataReceive(int howMany){
  int i = 0;
  char data[7]={};
  while(Wire.available()){
   data[i] = Wire.read();
   i++;}
  String dataString = String(data);
  if(dataString.equals("stop")){
  MotorStop();};
  
  Phi = dataString.substring(0,3).toInt();
  Dis = dataString.substring(3).toInt();
   
  if(Phi<360 and (Phi!=0) and rotation==true){
  timer1=0;
  timer2=0;
  angle=0;
  distance=0;
  encoder1_pos=0;
  encoder2_pos=0;
  if(Phi>180){Phi=360-Phi;mode=2;}else{mode=1;};
  angle = Phi*894*wheel_dis/(360*wheel_dia); 
  distance = Dis*89400/(314*wheel_dia);
  
 // Serial.print("Phi: "); Serial.print(Phi); Serial.print("\t");
 // Serial.print("angle: "); Serial.print(angle); Serial.print("\t");
 // Serial.print("distance: "); Serial.println(distance); //Serial.print("\t");
  
   };
}
