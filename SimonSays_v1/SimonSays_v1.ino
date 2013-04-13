#define MAX_SEQUENCE 255

// For each buttle, there must be an input AND an output pin!
#define MAX_BOTTLES 4

String gameMode = "simon";
int input[] = {A5,A4,A3,A2};
int output[] = {3,5,6,9};
int keksPin = 13;

int sequence[MAX_SEQUENCE] = {0};
int highscores[MAX_SEQUENCE] = {0};
int sequenceLength = 0;
int buttonState = 0;
int maxReactionTime = 1000;
int points = 0;

int treshold = 25;

int inputState[MAX_BOTTLES] = {LOW};
int outputState[MAX_BOTTLES] = {LOW};

void setup() {
 Serial.begin(9600);
 for (int i = 0; i < MAX_BOTTLES; i++) {
    pinMode(output[i], OUTPUT);      
    pinMode(input[i], INPUT);     
  }
  
  pinMode(keksPin, OUTPUT);
  
  digitalWrite(keksPin, HIGH);
}

void loop() {  
  // blinkAction.check();
  
  Serial.println("New round ....");
  if(gameMode == "simon") {
    simonLoop();
  }
  if(gameMode == "reaction") {
    reactionLoop();
  }
}

void reactionLoop() {
  long randomLed = random(MAX_BOTTLES);
  digitalWrite(output[randomLed], HIGH);

  Serial.println(maxReactionTime);
  maxReactionTime = maxReactionTime-10;
  int success = verifyNextUserInput(randomLed, maxReactionTime);
  if (success == 1) {
    points++;
    resetLeds();
  } else {
    Serial.println("Error! Resetting sequence ...");
    resetLeds();
    blinkLoss();
    storeHighscore(points);
    resetReactionLoop();
  }
}

void simonLoop() {
  generateNextSequenceStep();
  displaySequence();
  int success = verifyUserInput();
  if(success == 1) {
    Serial.println("Good.");
    points++;
    blinkWin();
  } else {
    Serial.println("Error! Resetting sequence ...");
    blinkLoss();
    storeHighscore(points);
    resetSequence();
  }
  delay(2000);
}

void generateNextSequenceStep() {
    if(sequenceLength >= MAX_SEQUENCE) {
      return;
    }
    long nextSequenceNumber = random(MAX_BOTTLES);
    Serial.print("Next expected number: ");
    Serial.println(nextSequenceNumber);
    sequence[sequenceLength] = nextSequenceNumber;
    sequenceLength++;
}

int verifyUserInput() {
  for(int i = 0; i < sequenceLength; i++) {
    int expectedBottle = sequence[i];
    if(verifyNextUserInput(expectedBottle, 5000) == 0) {
      return 0;
    }
    digitalWrite(output[expectedBottle], HIGH);
    delay(200);
    digitalWrite(output[expectedBottle], LOW);
  }
  return 1;
}

int verifyNextUserInput(int expectedBottle, int maxReactionTime) {
  Serial.println("Waiting for next user input ...");
  Serial.print("Expecting: ");
  Serial.println(expectedBottle);
  Serial.flush();
  unsigned long startTime = millis();
  while(1) {
    if(millis()-startTime > maxReactionTime) {
      return 0;
    }
    for(int i = 0; i < MAX_BOTTLES; i++) {
      inputState[i] = analogRead(input[i]) > treshold ? HIGH : LOW;
      if (inputState[i] == HIGH){
        Serial.print(i);
        Serial.println(" was pressed!");
        if (i == expectedBottle) {
          return 1;
        } else {
          return 0;
        }
      }
    }
  }
}

void displaySequence() {
   for(int i = 0; i < sequenceLength; i++) {
     int bottle = sequence[i];
     digitalWrite(output[bottle], HIGH);
     delay(1000);
     digitalWrite(output[bottle], LOW);
     delay(200);
   }
}

void resetSequence() {
  sequenceLength = 0;
  points = 0;
}

void resetLeds() {
  for(int i; i < MAX_BOTTLES; i++) {
    digitalWrite(output[i], LOW);
  }
  delay(100);
}

void resetReactionLoop() {
  maxReactionTime = 1000;
  Serial.println("YOUR POINTS:");
  Serial.println(points);
  points = 0;
}

void storeHighscore(int highscore){
  int new_highscore = highscore;
  
  for(int i = 0; i < MAX_SEQUENCE; i++){
    int tempscore = highscores[i];
    if(highscore > tempscore){
      highscores[i] = highscore;
      tempscore = highscore;
    }
    Serial.println(highscores[i]);
  }
  if(new_highscore >= highscores[0]){
        Serial.println("NEW HIGHSCORE : " + new_highscore);
    keksOut();
  }
}

void blinkWin() {
  for(int i = 0; i < 2; i++) {
    digitalWrite(output[1], HIGH);
    digitalWrite(output[3], HIGH);
    delay(500);
    digitalWrite(output[1], LOW);
    digitalWrite(output[3], LOW);
    delay(500);
  }
}

void blinkLoss() {
  for(int i = 0; i < 2; i++) {
    digitalWrite(output[0], HIGH);
    digitalWrite(output[2], HIGH);
    delay(500);
    digitalWrite(output[0], LOW);
    digitalWrite(output[2], LOW);
    delay(500);
  }
}

void keksOut(){
  digitalWrite(output[0], HIGH);
  digitalWrite(output[1], HIGH);
  digitalWrite(output[2], HIGH);
  digitalWrite(output[3], HIGH);
  
  digitalWrite(keksPin, LOW);
  
  delay(500);
  digitalWrite(output[0], LOW);
  digitalWrite(output[1], LOW);
  digitalWrite(output[2], LOW);
  digitalWrite(output[3], LOW);
  
  digitalWrite(keksPin, HIGH);
}
  
  
