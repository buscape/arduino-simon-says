#include <TimedAction.h>
#include <SoftwareSerial.h>

int leds[] = {3, 5, 6, 9};
boolean ledStates[] = {false, false, false, false};

int sws[] = {A5, A4, A3, A2};
int swStates[] = {LOW, LOW, LOW, LOW};
TimedAction blinkAction = TimedAction(1000, blink);

String displayText = "";

int TX = 11;
int RX = 12;
SoftwareSerial display = SoftwareSerial(12, 11); // RX, TX

int treshold = 50;

void setup(){
  for (int i=0; i<4; i++){
    pinMode(leds[i], OUTPUT);
    pinMode(swStates[i], INPUT);
  }
  display.begin(9600); 
  
  // enable display
  display.write(0xFE);
  display.write(0x41);
  delay(10);

  // clear Display
  display.write(0xFE);
  display.write(0x51);
  delay(10);

  // disbale underline Cursor
  display.write(0xFE);
  display.write(0x48);

  // disable blinking Cursor
  display.write(0xFE);
  display.write(0x4C);

  writeText("Welcome to Simon says");
}

void loop() {

  for (int i=0; i<4; i++){
    swStates[i] = analogRead(sws[i]) > treshold;
    if (swStates[i] == HIGH){
      ledStates[i] = true;
      blink();
      blinkAction.reset();
    }
  }
  blinkAction.check();
}

void blink(){  
 //digitalWrite(LED1, LED1_On ? HIGH : LOW);
  for (int i=0; i<4; i++){
    if (ledStates[i]){
      //tone(leds[i], 191);
      digitalWrite(leds[i], HIGH);
      ledStates[i] = false;
    } else {
      //noTone(leds[i]);
     digitalWrite(leds[i], LOW);
    }
  }
}

void clearText() {
  //clear display
  display.write(0xFE);
  display.write(0x51);
  delay(10);
}

void writeText(String text) {
  displayText = text;
  clearText();
  display.print(displayText.substring(0, 16));
  
  if (displayText.length() > 16){
    // set cursor to second line
    display.write(0xFE);
    display.write(0x45);
    display.write(0x40);
    delay(10);
    String secondLine = displayText.substring(16, displayText.length());
    secondLine.trim();
    display.print(secondLine);
  }

}
