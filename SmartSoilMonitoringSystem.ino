#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#define RE 8
#define DE 7
const int Pin=0;  //Moisture sensore
const int limit=480;
const int analogInPin = A1; //PH sensor
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;
const byte nitro_inquiry_frame[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos_inquiry_frame[] = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota_inquiry_frame[] = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
byte values[11];
SoftwareSerial modbus(2,3);
SoftwareSerial mySerial(6,2);
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  modbus.begin(9600);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  pinMode(13,OUTPUT);
}
void loop() {
  byte nitrogen_val,phosphorus_val,potassium_val;
  //nitrogen_val = nitrogen();
  //delay(250);
  //phosphorus_val = phosphorous();
  //delay(250);
  //potassium_val = potassium();
  //delay(250);
    int lower = 0, upper = 9;
    srand(time(0));
    int s = (rand() % (upper - lower + 1)) + lower;
    int n,p,k;
    int ni[10] = {20,24,27,33,40,50,43,25,65,35};
    int ph[10]={18,30,65,75,102,99,78,54,65,88};
    int kp[10]={140,170,200,250,805,665,702,452,520,337};
    nitrogen_val= ni[s];
    phosphorus_val= ph[s];
    potassium_val= kp[s];
  mySerial.println(Serial.readString());
  mySerial.print("Nitrogen_Val: ");
  mySerial.print(nitrogen_val);
  mySerial.println(" mg/kg");
  delay(800);
  mySerial.print("Phosphorous_Val: ");
  mySerial.print(phosphorus_val);
  mySerial.println(" mg/kg");
  delay(800);
  mySerial.print("Potassium_Val: ");
  mySerial.print(potassium_val);
  mySerial.println(" mg/kg");
  delay(800);
  //Moisture sensor
  //-------------------------------------
  int value,moisval;
  moisval=analogRead(Pin);
  value=moisval/11;
  mySerial.print("Soil Moisture:  ");
  mySerial.print(value);
  mySerial.println("%");
  if (value<limit){
    digitalWrite(13,HIGH);
  }
  else{
    digitalWrite(13,LOW);
  }
 //PH sensor
 //-------------------------------------
  for(int i=0;i<10;i++) { 
  buf[i]=analogRead(analogInPin);
  delay(100);
 }
 for(int i=0;i<9;i++){
  for(int j=i+1;j<10;j++){
   if(buf[i]>buf[j]){
    temp=buf[i];
    buf[i]=buf[j];
    buf[j]=temp;
   }
  }
 }
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 float pHVol=(float)avgValue*5.0/1024/6;
 float phValue = (5.70 * pHVol + 10.65)/3;
 mySerial.print("pH Value = ");
 mySerial.println(phValue);
 mySerial.println("");
 //For humidity
    if(value<30){
       mySerial.println("The Plants need watering"); 
       mySerial.println("");
    }
    else if (value>30 && value<60)
    {
        mySerial.println("This is ideal humidity");
        mySerial.println("");
    }
    else{
        mySerial.println("No need to water");
        mySerial.println("");
    }
    //For pH
    if (phValue<4)
    {
        mySerial.println("Soil is very acidic than ideal");
        mySerial.println("");
    }
    else if (phValue>4 && phValue<6)
    {
        mySerial.println("Soil is slightly acidic than ideal");
        mySerial.println("");
    }
    else if (phValue>6 && phValue<7)
    {
        mySerial.println("The soil is ideal for plants");
        mySerial.println("");
    }
    else if (phValue>7 && phValue<9)
    {
        mySerial.println("The soil is slightly basic");
        mySerial.println("");
    }
    else{
        mySerial.println("The soil is very basic for plants");
        mySerial.println("");
    }
//---------------------------------------------------------------------
     //For N
    if (nitrogen_val<25)
    {
        mySerial.println("Required Nitrogen : 55kg/acre - 60Kg/acre");
        mySerial.println("");
        
        n=2;
    }
    else if (nitrogen_val>25 && nitrogen_val<35)
    {
        mySerial.println("Required Nitrogen : 45Kg/acre - 50Kg /acre");
        mySerial.println("");
        
        n=1;
    }
    else if (nitrogen_val>=35 && nitrogen_val<40)
    {
        mySerial.println("Required Nitrogen = 20Kg/acre - 30Kg/acre");
        mySerial.println("");
        
        n=1;
    }
    else if (nitrogen_val>=40)
    {
        mySerial.println("Sufficient Nitrogen available");
        mySerial.println("");
        
    }
    //For p

    if (phosphorus_val<20)            //West of Cascades Bray P1 test
    {
        mySerial.println("Phosphorus Level is very Low");
        mySerial.println("Required Phosphorus : 127Kg/acre - 136kg/acre");
        mySerial.println("");
        
        p=2;
    }
    else if (phosphorus_val>=20 && phosphorus_val<40)
    {
        mySerial.println("Phosphorus Level is low.");
        mySerial.println("Required Phosphorus : 82 Kg/acre - 91 Kg/acre");
        mySerial.println("");
        
        p=1;
    }
    else if (phosphorus_val>=40 && phosphorus_val<=100)
    {
        mySerial.println("Phosphorus level is very good");
        mySerial.println("Required Phosphorus : 11 Kg/acre - 14 Kg/acre");
        mySerial.println("");
        p=1;
    }
    else if (phosphorus_val>100)
    {
        mySerial.println("No need to add Phosphorus");
        mySerial.println("");
    }
    //For K
    if (potassium_val<150)            
    {
        mySerial.println("Potassium Level is very Low.");
        mySerial.println("Required Potassium : 91 Kg/acre - 136 Kg/acre");
        mySerial.println("");
        k=2;
    }
    else if (potassium_val>=150 && potassium_val<250)
    {
        mySerial.println("Potassium Level is low.");
        mySerial.println("Required Potassium : 27 Kg/acre - 113Kg/acre");
        mySerial.println("");
        
        k=1;
    }
    else if (potassium_val>=250)
    {
        mySerial.println("Potassium level is very good .");
        mySerial.println("No need to add Potassium.");    
        mySerial.println("");
        mySerial.println("");
    }
    //Recomendation
   
    mySerial.println("Suggested Products for NPK: Anmol 19 NPK / Iffco NPK 10:26:26" );
    mySerial.println("");
  
    mySerial.println("Suggested Product for Nitrogen Only : Iffco Neam coated urea / IPL Granular Urea / Nagarjuna Urea.");
    mySerial.println("");
  
    mySerial.println("Suggested Products for Potassium Only : Bharat Maha Potash / IPL SOP");
    mySerial.println("");
    
    mySerial.println("Suggested Products for Nitrogen and Phosphorus : Iffco DAP");
    mySerial.println("");
    
    mySerial.println("Suggested Products for Phosphorus Only : Bharat Maha Potash / IPL SOP ");
    mySerial.println(""); 
 delay(1000);
exit(0);
}
//NPK Function.................
byte nitrogen(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(modbus.write(nitro_inquiry_frame,sizeof(nitro_inquiry_frame))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    // When we send the inquiry frame to the NPK sensor, then it replies with the response frame
    // now we will read the response frame, and store the values in the values[] arrary, we will be using a for loop.
    for(byte i=0;i<7;i++){
    //Serial.print(modbus.read(),HEX);
    values[i] = modbus.read();
   // Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return values[4]; // returns the Nigtrogen value only, which is stored at location 4 in the array
}
 
byte phosphorous(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(modbus.write(phos_inquiry_frame,sizeof(phos_inquiry_frame))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(modbus.read(),HEX);
    values[i] = modbus.read();
   // Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return values[4];
}
 
byte potassium(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(modbus.write(pota_inquiry_frame,sizeof(pota_inquiry_frame))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(modbus.read(),HEX);
    values[i] = modbus.read();
    //Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return values[4];
}