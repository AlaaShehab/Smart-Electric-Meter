#include <LiquidCrystal.h>

unsigned long startMillis;
unsigned long totalTime;
const int sensorIn = A0;
//const int maxEnergyCons = A1;
double preEnergy = 0.0003;
double energy = 0;
//the sensor scale factor
int mVperAmp = 185;

int ledpin = 13;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

void setup() {
  Serial.begin(9600);
  pinMode(ledpin, OUTPUT);
  digitalWrite(ledpin, LOW);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
     // Print a message to the LCD.
  startMillis = millis();
}

void loop() {
  double Voltage = 0;
  double VRMS = 0;
  double AmpsRMS = 0;
  //get the peak to peak voltage
  Voltage = getVPP();
 //get volt RMS value
  VRMS = (Voltage/2.0) *0.707;
 //get current RMS value
  AmpsRMS = (VRMS * 1000)/mVperAmp;
   Serial.println("Current");

   Serial.println(AmpsRMS);
  int RMSPower = 220 * AmpsRMS;
     Serial.println("Power");

     Serial.println(RMSPower);

  //end time.
  unsigned long endMillis = millis();
  //time within which the energy is calculated
  unsigned long time = endMillis - startMillis;
  //total time since beg of the month.
  totalTime = totalTime + time;

  //calculate energy consumed
  energy = energy + ((double)RMSPower * ((double)time/60/60/1000000));
  lcd.setCursor(0, 0);
  lcd.print(energy, 6);
  lcd.print("KW");

  if (energy >= preEnergy) {
    digitalWrite(ledpin, HIGH);
  }
  if (totalTime >= 900000 - 1000 && totalTime <= 900000 + 1000 ) {
    preEnergy = energy;
    //let 1 kilo watt is 1.5 L.E
    double money = energy * 1.5;
    lcd.setCursor(0, 1);
    lcd.print(money, 5);
    lcd.print("LE");

   totalTime = 0;
    energy = 0;
    digitalWrite(ledpin, LOW);

  }
    startMillis = millis();
    delay(1000);
    lcd.clear();

}

double getVPP()
{
  double result;
  
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
  int minValue = 1024;          // store min value here
  
   uint32_t start_time = millis();
   while((millis()-start_time) < 500){ //sample for 500 milliSec 
       readValue = analogRead(sensorIn);
       // see if you have a new maxValue
       if (readValue > maxValue){
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
       if (readValue < minValue){
           /*record the maximum sensor value*/
           minValue = readValue;
       }
   } 
   // Subtract min from max
   result = ((maxValue - minValue) * 5.0)/1024.0;
      
   return result;
 }
