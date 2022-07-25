#include <Wire.h>

#define s1 15
#define s2 2
#define s3  0
#define s4  4
#define s5  32
#define s6  33
#define s7  25
#define s8  26
#define s9  27
#define s10  14
#define s11  12
#define s12  13
#define E 35

int thresh_angle=10;
int d_max=10;   // values between 6 and 20
float base_dis=0.001; //added to vy


int angle=0;
int distance=9999;

long Timer=0;
long Timer1=2700;


long delay1=10;

float d_raw=0;

float data[13];
float d_cor[13];
double d[13];

float corr[13][2] = {{0,0},{0.8030,0.9066},{ 0.1404, 0.7590 },{ -0.0340, 0.7764 },{ 0.4265, 0.9064 },{ -0.2529, 0.8244 },{ -0.3177, 0.8007 },
                    { 0.4810, 0.7984 },{ 0.0086, 0.9278 },{ 0.2707, 0.8578 },{ 0.0165, 0.8551 },{ 0.6868, 0.8881 },{ -0.2392, 0.8045 }}; // [g][f]
boolean last=false;
int count=1;

void setup() {
 pinMode(s1, OUTPUT); pinMode(s2, OUTPUT); pinMode(s3, OUTPUT);
 pinMode(s4, OUTPUT); pinMode(s5, OUTPUT); pinMode(s6, OUTPUT);
 pinMode(s7, OUTPUT); pinMode(s8, OUTPUT); pinMode(s9, OUTPUT); 
 pinMode(s10, OUTPUT); pinMode(s11, OUTPUT); pinMode(s12, OUTPUT);
 pinMode(E,INPUT);
 Serial.begin(115200);
 Serial.println("Test");
 Wire.begin();
}



void loop() {
if(count==1 && millis()>=Timer){  
  digitalWrite(s1,HIGH); messure(); data[1]=d_raw; digitalWrite(s1,LOW); Timer=Timer+delay1; count++; };
if(count==2 && millis()>=Timer){  
  digitalWrite(s2,HIGH); messure(); data[2]=d_raw; digitalWrite(s2,LOW); Timer=Timer+delay1; count++; };
if(count==3 && millis()>=Timer){      
  digitalWrite(s3,HIGH); messure(); data[3]=d_raw; digitalWrite(s3,LOW); Timer=Timer+delay1;count++;};
if(count==4 && millis()>=Timer){     
  digitalWrite(s4,HIGH); messure(); data[4]=d_raw; digitalWrite(s4,LOW); Timer=Timer+delay1;count++;};
if(count==5 && millis()>=Timer){      
  digitalWrite(s5,HIGH); messure(); data[5]=d_raw; digitalWrite(s5,LOW); Timer=Timer+delay1;count++;};
if(count==6 && millis()>=Timer){      
  digitalWrite(s6,HIGH); messure(); data[6]=d_raw; digitalWrite(s6,LOW); Timer=Timer+delay1;count++;};
if(count==7 && millis()>=Timer){    
  digitalWrite(s7,HIGH); messure(); data[7]=d_raw; digitalWrite(s7,LOW); Timer=Timer+delay1;count++;};
if(count==8 && millis()>=Timer){     
  digitalWrite(s8,HIGH); messure(); data[8]=d_raw; digitalWrite(s8,LOW); Timer=Timer+delay1;count++;};
if(count==9 && millis()>=Timer){    
  digitalWrite(s9,HIGH); messure(); data[9]=d_raw; digitalWrite(s9,LOW); Timer=Timer+delay1;count++;};
if(count==10 && millis()>=Timer){    
  digitalWrite(s10,HIGH); messure(); data[10]=d_raw; digitalWrite(s10,LOW); Timer=Timer+delay1;count++;};
if(count==11 && millis()>=Timer){    
  digitalWrite(s11,HIGH); messure(); data[11]=d_raw; digitalWrite(s11,LOW); Timer=Timer+delay1;count++;};
if(count==12 && millis()>=Timer){    
  digitalWrite(s12,HIGH); messure(); data[12]=d_raw; digitalWrite(s12,LOW); Timer=Timer+delay1;count++;};

if(count==13){
//for(int i = 1; i < 13; i++){ 
// d_cor[i]=(corr[i][1])*(data[i][0])+(corr[i][0]);
//   if(d_cor[i]<5){d_cor[i]=5;};
//  if(d_cor[i]>20){d_cor[i]=20;};  };

for(int i = 1; i < 13; i++){ // keine korrektur
  if(data[i]<5){d_cor[i]=5;}
  else{if(data[i]>d_max){d_cor[i]=d_max;}
  else{d_cor[i]=data[i];};};};
  
for(int i = 1; i < 13; i++){
  d[i]=(d_cor[i]-5)/(d_max-5);};

calVec();

count=1;

//for(int i = 1; i < 13; i++){
//   Serial.print("d"); Serial.print(i); Serial.print("\t");Serial.print(d_cor[i]); Serial.print("\t"); Serial.println(millis());
//  d[i]=0;};

Serial.print("angle"); Serial.print("\t");Serial.println(angle);  
  
if(angle <= thresh_angle or 360-angle<= thresh_angle){ angle=0;};   

send_data();

};
   
};

void messure(){
   long w=0;
   long value=0;  
   delay(5);      //delay to ensure the IR Led is operational at messurement time
   for(int i = 0; i < 100; i++){ 
   w+= analogRead(E);};
   value = w/100; 
   d_raw=-0.1+8.346*pow(0.2209,log10(0.0008113*value+0.149));};

void calVec(){
  double vy= 1*d[1] + 0.866*d[2] + 0.5*d[3] - 0.5*d[5] - 0.866*d[6] - 1*d[7] - 0.866*d[8] - 0.5*d[9] + 0.5*d[11] + 0.866*d[12] ; //projektion der Sensoren auf x bzw. y Achse
  double vx= + 0.5*d[2] + 0.866*d[3] + 1*d[4] + 0.866*d[5] + 0.5*d[6] - 0.5*d[8] - 0.866*d[9]- 1*d[10] - 0.866*d[11] - 0.5*d[12] ;
   vy= vy + base_dis;
  
   if(abs(vy)==0){vy=1;}; // obsolete if base_dis != 0 
  //Serial.print("vx"); Serial.print("\t");Serial.print(vx);  Serial.print("vy"); Serial.print("\t");Serial.println(vy); 
  
  if(vy>0){    // 
    if(vx>=0){ angle = round( atan(vx/vy) * 180/3.14159265 );} // radians to degrees and rounding
    else{ angle = 360 + round( atan(vx/vy) * 180/3.14159265 );};
  }
  else{
    if(vx>=0){ angle = 180 - round( atan(vx/(-vy)) * 180/3.14159265 );} // radians to degrees and rounding
    else{ angle = 270 - round( atan(vx/vy) * 180/3.14159265 );};
  };
  //Serial.println(angle);
  }
  
void send_data(){
 char a[3]; 
 char d[4]; 
 sprintf(a,"%ld", angle);
 sprintf(d,"%ld", distance);
 if(angle<100){
   if(angle<10){
      if(angle=0){
  Wire.beginTransmission(8); // for angle=0 send 000
    Wire.write("0"); Serial.print("0");
    Wire.write("0"); Serial.print("0");
    Wire.write("0"); Serial.print("0");
    for (int i=0; i<4; i++){Wire.write(d[i]); Serial.println(d[i]);
    };
  Wire.endTransmission();
      }else{
    Wire.beginTransmission(8); // for angle<10 send 00"angle"
    Wire.write("0"); Serial.print("0");
    Wire.write("0"); Serial.print("0");
    Wire.write(angle); Serial.print(angle);
    for (int i=0; i<4; i++){Wire.write(d[i]); Serial.print(d[i]);} ;
    Wire.endTransmission();}
    }else{
  Wire.beginTransmission(8); 
    Wire.write("0");
    Wire.write(a[0]);
    Wire.write(a[1]);
    for (int i=0; i<4; i++){Wire.write(d[i]);};
  Wire.endTransmission();
   };}
 else{ 
 Wire.beginTransmission(8); 
  for (int i=0; i<3; i++)
  {Wire.write(a[i]);};
  for (int i=0; i<4; i++)
  {Wire.write(d[i]);};
 Wire.endTransmission();
 }
 
  //for(int i = 0; i < 3; i++){Serial.print(a[i]);};
  //for(int i = 0; i < 4; i++){Serial.print(d[i]);};
  //Serial.println();

}
