
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 11, en = 10, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int echo = 8, trig = 7, but = 6;
const int buz = 9;
const int rLED = 12, gLED = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int sceneManager = 0;
int t = 15;
long lowDist = 20, highDist = 40;
long deltaDist = 3;
int score = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  
  pinMode(but, INPUT);
  pinMode(buz, OUTPUT);
  pinMode(rLED, OUTPUT);
  pinMode(gLED, OUTPUT);

  digitalWrite(rLED, LOW);
  digitalWrite(gLED, LOW);
}

void loop(){
   switchScene();
}

void switchScene(){
  switch(sceneManager){
    case 0:
      openingFrame();
      break;
    case 1:
      mainMenu();
      break;
    case 2:
      mainGame();
      break;
    default:
      lcd.clear();
      break;
  }
}

void openingFrame(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance game!");
  delay(2000);
  sceneManager = 1;
}

void mainMenu(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Match the dist!");
  lcd.setCursor(0, 1);
  lcd.print("But to start");
  Serial.println(digitalRead(but));
  if(digitalRead(but)){
    sceneManager = 2;
    delay(200);
  }
  delay(500);
}

void mainGame(){
  int localT = t;
  score = 0;
  while(1){
    long trigDist = newrandom(lowDist, highDist);
    long dist = -1;
   for(int i = 0;i <= localT * 10;i++){
     delay(100);
     lcd.clear();
     dist  = readDistanceSenzorData(trig, echo);
     lcd.setCursor(0, 0);
     lcd.print("Your dist:");lcd.print(dist);lcd.print(" cm");
     lcd.setCursor(0, 1);
     lcd.print("Thr dist:");lcd.print(trigDist);lcd.print(" cm");
     Serial.println(digitalRead(but));
     
   }
   if((trigDist - deltaDist <= dist) && (dist < trigDist + deltaDist)){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Good job!");
    tone(buz, 1000);
    digitalWrite(gLED, HIGH);
    score++;
    localT -= 2;
    delay(1000);
    noTone(buz);
    digitalWrite(gLED, LOW);
    if(t <= 7){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You win");
      lcd.setCursor(0, 1);
      lcd.print("Score: "); lcd.print(score);
      for(int i = 0;i < 3;i++){
        digitalWrite(rLED, HIGH);
        digitalWrite(gLED, LOW);
        delay(250);
        digitalWrite(rLED, LOW);
        digitalWrite(gLED, HIGH);
        delay(250);
      }
      digitalWrite(rLED, LOW);
      digitalWrite(gLED, LOW);
      sceneManager = 1;
      break;
    }
   } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fail :(");
    lcd.setCursor(0, 1);
    lcd.print("Score: "); lcd.print(score);
    sceneManager = 1;
    digitalWrite(rLED, HIGH);
    tone(buz, 500);
    delay(1000);
    noTone(buz);
    digitalWrite(rLED, LOW);
    break;
   }
  }
}

long readDistanceSenzorData(int ping, int echo){
   long duration, inches, cm, distance;
   pinMode(ping, OUTPUT);
   digitalWrite(ping, LOW);
   delayMicroseconds(2);
   digitalWrite(ping, HIGH);
   delayMicroseconds(10);
   digitalWrite(ping, LOW);
   pinMode(echo, INPUT);
   duration = pulseIn(echo, HIGH);
   return duration / 29 / 2;
}

unsigned long newrandom(unsigned long howsmall, unsigned long howbig)
{
  return howsmall + random() % (howbig - howsmall);
}
