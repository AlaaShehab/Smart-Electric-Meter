#include <LiquidCrystal.h>
#include <Keypad.h>
unsigned long startMillis;
unsigned long totalTime;
const int sensorIn = A0;
char money[4]; 
int MaxMoney;
double MaxEnergy = 0.0;
//const int maxEnergyCons = A1;
double energy = 0;
//the sensor scale factor
int mVperAmp = 185;
int ledpin = 13;
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'#','0','*'}
};
byte rowPins[ROWS] = {3, 2, A5, A4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4,5,6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); // set Key pad 
//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
void setup() {
  Serial.begin(9600);
  pinMode(ledpin, OUTPUT);
  digitalWrite(ledpin, LOW);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
     // Print a message to the LCD.
     // take maximume money 
     lcd.println("WELCOME...");
     delay(500);
     lcd.clear();
     lcd.println("ENTER MAXIMIUMRE MONEY");
     delay(500);
     lcd.clear();
  startMillis = millis();
  for (int i=0; i < 3 ;){
    char key = keypad.getKey();
    if (key){
    lcd.print(key);
    money [i] = key;
    i++;
    }
  }
  MaxMoney = atoi(money);
  MaxEnergy = MaxMoney / 1.5;
  lcd.clear();
  lcd.print("START"); 
  delay(100);

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
  energy = energy + ((double)RMSPower * ((double)time/1000));
  lcd.setCursor(0, 0);
  lcd.print(energy, 6);
  lcd.print("KW");

  if (energy >= MaxEnergy) {
    digitalWrite(ledpin, HIGH);
  }
  if (totalTime >= 900000 - 1000 && totalTime <= 900000 + 1000 ) {
    MaxEnergy = energy;
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
