#include <LiquidCrystal.h>


int mVperAmp = 185; 
int RawValue= 0;
int ACSoffset = 2500; 
double Voltage = 0;
double Amps = 0;
double energy = 0;
unsigned long startMillis;
unsigned long endMillis;
double preState = 0;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int led = 13;
int analogIn = A0;

void setup() {
  lcd.begin(16, 2);
  pinMode(led, OUTPUT);
  pinMode(analogIn, INPUT);
  startMillis = millis();

}

void loop() {
  RawValue = digitalRead(analogIn);
  Voltage = (RawValue / 1024.0) * 5000; // Gets you mV
  if (Voltage != 0) {
     Amps = ((Voltage - ACSoffset) / mVperAmp);
  }
  lcd.println("current:");
  lcd.println(Amps);
  delay(1000);
  lcd.clear();
 
  double power = Amps * 5;
  lcd.println("power");
  lcd.println(power);
  delay(1000);
  lcd.clear();
 
  endMillis = millis();
  unsigned long totalTime = totalTime+(endMillis - startMillis);

  energy =(totalTime/1000)*power;
  lcd.println("Energy: ");
  lcd.println(energy);
   if(energy  >= 5){
   digitalWrite(led, HIGH);
  }
  if (totalTime >= (25000-100) && totalTime <= (25000+100)) {
    lcd.print("Energy: ");
    lcd.print(" Money:");
    lcd.setCursor(0, 1);
    lcd.println(energy);
    double money = energy * 1.5;   
    lcd.print(money);
    energy = 0;
    totalTime = 0;
     digitalWrite(led, LOW);
  }
    startMillis = millis();
   delay (1000);

}
