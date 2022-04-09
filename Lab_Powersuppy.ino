#include <LiquidCrystal.h>
/*
cv=A0
cc=A1
voltage A2
current A3
buzzer=6;
 */
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//Voltage
int Read_Voltage  = A2;
double Voltage = 0.0;
double Voltage_offset=0.00;

//Current
int Read_Current  = A3;
double Current = 0.0;
double current_sample=0.0;
int cv=A0;
int cc=A1;
int sensitivity = 66;
float adcValue= 0;
float offsetVoltage = 2502.5;
double adcVoltage = 0.00;
double milliAmps=0.0;

//Function
int buzzer=6;

//Power
double Power = 0.0;

//Energy
double Energy=0.0;

//Symbols
byte thunder[] = { //icon for thunder
  B00011,
  B00110,
  B01100,
  B11111,
  B11111,
  B00110,
  B01100,
  B11000
};

//====================================================================Setup
void setup() { 
  //==============================LCD
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(cc,INPUT);
  pinMode(cv,INPUT);
  pinMode(Read_Voltage,INPUT);
  pinMode(Read_Current,INPUT);
  pinMode(buzzer,OUTPUT);
  lcd.createChar(2,thunder);
//=====================================Booting function call
booting();
}

//======================================================================Loop
  void loop()
{
 Read_data();                                 
 lcd_display();      
}
//=====================================================================Booting
void booting(){
  lcd.home();
  lcd.begin(16, 2);
  lcd.print("Lab Powersupply");
  lcd.setCursor(2,1);
  lcd.print("Version 1.0");
  delay(1500);
  lcd.home();
  lcd.begin(16, 2);
  lcd.print("Booting....");
  digitalWrite(buzzer,HIGH);
  delay(100);
  digitalWrite(buzzer,LOW);
  delay(100);
  digitalWrite(buzzer,HIGH);
  delay(100);
  digitalWrite(buzzer,LOW);
  delay(1000);
  lcd.clear();
  lcd.home();

}


//=================================================================LCD Display
void lcd_display()
{
  lcd.home();lcd.clear();
  lcd.print(Voltage);  
  lcd.print("V ");
  lcd.print(Current);  
  lcd.print("A");
  lcd.setCursor(14,0);
 if(analogRead(cv)==HIGH || analogRead(cc)==LOW)
{
 lcd.print("CV"); 
}else if(analogRead(cc)==HIGH || analogRead(cv)==LOW)
{
 lcd.setCursor(14,0);
 lcd.print("CC"); 
}
  lcd.setCursor(0, 1);
  if(Power<1000){
  lcd.print(Power);    
  lcd.print("mW ");
  }else{
  lcd.print(Power/1000);    
  lcd.print("W ");
  }
  if(Energy<1000){
  lcd.print(Energy);
  lcd.print("mWh");
  }else{
  lcd.print(Energy/1000);
  lcd.print("Wh");
  }
  Current=Voltage=Power=0.00;
}  
//===================================================================Reading Data
void Read_data() 
{
  
for(int i=0;i<=1000;i++)
{
  Voltage=Voltage+analogRead(Read_Voltage);
  current_sample=current_sample+analogRead(Read_Current);
}
Voltage/=1000;
Voltage=Voltage*(30.0/1024.0);
current_sample/=1000;
adcVoltage = (current_sample / 1024.0) * 5000;
Current = ((adcVoltage - offsetVoltage) / sensitivity);

if(Current<0)
  Current=0.00;
  
Serial.print("Current=");
Serial.print(Current);
Serial.print(" Voltage=");
Serial.print(Voltage);
Serial.print(" Analog read=");
Serial.print(analogRead(current_sample));

 //Power
double milliAmps=Current*1000;
Power=Voltage*milliAmps;

 //Energy
 if(Power>0)
 Energy=Energy+((Voltage*milliAmps)/3600);
 else
 Energy=0;
}
