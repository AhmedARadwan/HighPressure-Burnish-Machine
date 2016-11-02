#define cont 39
#define motorA1 40
#define motorA2 41
#define motorB1 42
#define motorB2 43

#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>
LiquidCrystal lcd(12, 11, 14, 15, 16, 17);
Servo firstESC;
Servo Serv;
int counter = 0;
int counterA = 0;
int counterB = 0;
int counter11 = -1;
int counter22 = -1;
int pos=0;
const byte numRows= 4; //number of rows on the keypad
const byte numCols= 4; //number of columns on the keypad
char seconds1;
char seconds2;
char min1;
char min2;
int Min;
int Sec;
int hrs;
char enter_key;
char MotorNum;
int x;
char speed_key;
boolean switcher=true;
//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols]= 
{
{'1', '2', '3', 'A'}, 
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

//Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {9,8,7,6}; //Rows 0 to 3
byte colPins[numCols]= {5,4,3,2}; //Columns 0 to 3

//initializes an instance of the Keypad class
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void system_init();
void ContActivation();
void MA_run();
void MB_run();
void trigger();
void stepDown();
void Get_time();
void Servo_sweep(int Ang_from, int Ang_to, int delay_);
int ServoSweepForward(int angle_from, int angle_to, int delay_);
int ServoSweepBackward(int angle_from, int angle_to, int delay_);

void setup() {
  system_init();
  Serv.attach(1);
  Serv.write(150);
  lcd.print("System is ready!");  
  delay(1000);
}

void loop(){
  if (counter == 0){
    lcd.clear();
    Serv.write(150);
    lcd.setCursor(0, 0);
    lcd.print("<A>:MotorA");
    lcd.setCursor(0, 1);
    lcd.print("<B>:MotorB");
    counter++;
  }
  MotorNum = myKeypad.getKey();
  switch(MotorNum){
    
    
    /*--------------------------------------Motor A----------------------------*/
    case 'A':
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Set Time");
      lcd.setCursor(0, 1);
      lcd.print("00:00");
      lcd.setCursor(7, 1);
      lcd.print("<#>:Entr");
      Get_time();
      lcd.noBlink();
      while(1){
        enter_key = myKeypad.getKey();
        if(enter_key == '#'){ 
          lcd.clear();
          break;
        }
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Set Speed:");
      lcd.setCursor(0, 1);
      lcd.print("<1>");
      lcd.setCursor(6, 1);
      lcd.print("<2>");
      lcd.setCursor(12, 1);
      lcd.print("<3>");
      while(1){
        speed_key = myKeypad.getKey();
        if(speed_key == '1' || speed_key == '2' || speed_key == '3'){ 
          lcd.clear();
          break;
        }
      }
      MA_run();
      while (hrs > 0 || Min > 0 || Sec >= 0) {
        if (counterA == 0){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("MotorA ON");
          counterA++;
        }
        if (speed_key == '1') firstESC.writeMicroseconds(1200);
        if (speed_key == '2') firstESC.writeMicroseconds(1600);
        if (speed_key == '3') firstESC.writeMicroseconds(2000);
        
        char inturr_key = myKeypad.getKey();
        if (inturr_key == 'D'){
          counter = 0;
          counterA = 0;
          counterB = 0;
          digitalWrite(cont, HIGH);
          digitalWrite(motorB1, LOW);
          digitalWrite(motorB2, LOW);
          digitalWrite(motorA1, LOW);
          digitalWrite(motorA2, LOW);
          break;
        }
         lcd.setCursor(4, 2);
         (hrs < 10) ? lcd.print("0") : NULL;
         lcd.print(hrs);
         lcd.print(":");
         (Min < 10) ? lcd.print("0") : NULL;
         lcd.print(Min);
         lcd.print(":");
         (Sec < 10) ? lcd.print("0") : NULL;
         lcd.print(Sec);
         lcd.display();
         stepDown();
         counter11++;
         if (counter11 == 0) ServoSweepForward(45, 80, 29);     //Big:   45     115
         if (counter11 == 1) ServoSweepForward(80, 115, 29);
         if (counter11 == 2) ServoSweepBackward(80, 115, 29);
         if (counter11 == 3) {
           ServoSweepBackward(45, 80, 29);
           counter11 = -1;
         }
         
         if (counterA == 0) break;
      }
      MotorNum = 0;
      break;
      
      
      /*---------------------------------Motor B-----------------------------*/
      
      
    case 'B':
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Set Time");
      lcd.setCursor(0, 1);
      lcd.print("00:00");
      lcd.setCursor(7, 1);
      lcd.print("<#>:Entr");
      Get_time();
      lcd.noBlink();
      while(1){
        enter_key = myKeypad.getKey();
        if(enter_key == '#'){ 
          lcd.clear();
          break;
        }
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Set Speed:");
      lcd.setCursor(0, 1);
      lcd.print("<1>");
      lcd.setCursor(6, 1);
      lcd.print("<2>");
      lcd.setCursor(12, 1);
      lcd.print("<3>");
      while(1){
        speed_key = myKeypad.getKey();
        if(speed_key == '1' || speed_key == '2' || speed_key == '3'){ 
          lcd.clear();
          break;
        }
      }
      MB_run();
      while (hrs > 0 || Min > 0 || Sec >= 0) {
        if (counterB == 0){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("MotorB ON");
          counterB++;
        }
        if (speed_key == '1') firstESC.writeMicroseconds(1200);
        if (speed_key == '2') firstESC.writeMicroseconds(1600);
        if (speed_key == '3') firstESC.writeMicroseconds(2000);
        char inturr_key = myKeypad.getKey();
        if (inturr_key == 'D') {
          counter = 0;
          counterA = 0;
          counterB = 0;
          digitalWrite(cont, HIGH);
          digitalWrite(motorB1, LOW);
          digitalWrite(motorB2, LOW);
          digitalWrite(motorA1, LOW);
          digitalWrite(motorA2, LOW);
          break;
        }
         lcd.setCursor(4, 2);
         (hrs < 10) ? lcd.print("0") : NULL;
         lcd.print(hrs);
         lcd.print(":");
         (Min < 10) ? lcd.print("0") : NULL;
         lcd.print(Min);
         lcd.print(":");
         (Sec < 10) ? lcd.print("0") : NULL;
         lcd.print(Sec);
         lcd.display();
         stepDown();
         counter22++;
         if (counter22 == 0) ServoSweepForward(100, 125, 40);  //100 to 150
         if (counter22 == 1) ServoSweepForward(125, 150, 40);
         if (counter22 == 2) ServoSweepBackward(125, 150, 40);
         if (counter22 == 3) {
           ServoSweepBackward(100, 125, 40);
           counter22 = -1;
         }
         //delay(16);
         //delay(1000);
         if (counterB == 0) break;
      }
      MotorNum = 0;
      break;
    default:
      break;
  }
  
}
void system_init(){
  pinMode(cont, OUTPUT);
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  digitalWrite(cont, HIGH);
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
  firstESC.attach(10);
  lcd.begin(16, 2);
}


void MA_run(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Activating:");
  lcd.setCursor(0, 1);
  digitalWrite(cont, LOW);
  delay(500);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, HIGH);
  firstESC.writeMicroseconds(1000);
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
}

void MB_run(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Activating:");
  lcd.setCursor(0, 1);
  digitalWrite(cont, LOW);
  delay(500);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, HIGH);
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  firstESC.writeMicroseconds(1000);
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
}

void stepDown() {
 if (Sec > 0) {
 Sec -= 1;
 } else {
 if (Min > 0) {
 Sec = 59;
 Min -= 1;
 } else {
 if (hrs > 0) {
 Sec = 59;
 Min = 59;
 hrs -= 1;
 } else {
 trigger();
 }
 }
 }
}

void trigger() {
   lcd.clear(); // clears the screen and buffer
   lcd.setCursor(5, 1); // set timer position on lcd for end.
   lcd.print("END ");
   counter = 0;
   counterA = 0;
   counterB = 0;
   digitalWrite(cont, HIGH);
   digitalWrite(motorB1, LOW);
   digitalWrite(motorB2, LOW);
   digitalWrite(motorA1, LOW);
   digitalWrite(motorA2, LOW);
   Serv.write(150);
}

void Get_time(){
      lcd.setCursor(0, 1);
      lcd.blink();
      while(1){
        min1 = myKeypad.getKey();
        if(min1 == '1' || min1 == '2' || min1 == '3' || min1 == '4' || min1 == '5' || min1 == '6' || min1 == '7' || min1 == '8' || min1 == '9' || min1 == '0') break;
      }
      lcd.setCursor(0, 1);
      lcd.print(min1);
      while(1){
        min2 = myKeypad.getKey();
        if(min2 == '1' || min2 == '2' || min2 == '3' || min2 == '4' || min2 == '5' || min2 == '6' || min2 == '7' || min2 == '8' || min2 == '9' || min2 == '0') break;
      }
      lcd.setCursor(1, 1);
      lcd.print(min2);
      
      lcd.setCursor(3, 1);
      
      while(1){
        seconds1 = myKeypad.getKey();
        if(seconds1 == '1' || seconds1 == '2' || seconds1 == '3' || seconds1 == '4' || seconds1 == '5' || seconds1 == '6' || seconds1 == '7' || seconds1 == '8' || seconds1 == '9' || seconds1 == '0') break;
      }
      lcd.setCursor(3, 1);
      lcd.print(seconds1);
      while(1){
        seconds2 = myKeypad.getKey();
        if(seconds2 == '1' || seconds2 == '2' || seconds2 == '3' || seconds2 == '4' || seconds2 == '5' || seconds2 == '6' || seconds2 == '7' || seconds2 == '8' || seconds2 == '9' || seconds2 == '0') break;
      }
      lcd.setCursor(4, 1);
      lcd.print(seconds2);
      Sec = (seconds1-48)*10+(seconds2-48);
      Min = (min1-48)*10+(min2-48);

}



void Servo_sweep(int Ang_from, int Ang_to, int delay_){
  for(int pos = Ang_from; pos <= Ang_to; pos += 1){
    Serv.write(pos);
    delay(delay_);
  }
  for(int pos = Ang_to; pos>=Ang_from; pos-=1){                                
    Serv.write(pos);
    delay(delay_);
  }
}


int ServoSweepForward(int angle_from, int angle_to, int delay_){
  for(int pos = angle_from; pos <= angle_to; pos += 1){
    Serv.write(pos);
    delay(delay_);
  }
  return x;
}

int ServoSweepBackward(int angle_from, int angle_to, int delay_){
  for(int pos = angle_to; pos>=angle_from; pos-=1){                                
    Serv.write(pos);
    delay(delay_);
  }
  return x;
}
