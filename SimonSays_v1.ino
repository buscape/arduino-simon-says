#define MAX_SEQUENCE 255

int input[] = {6,13,3,12};
int output[] = {9,11,7,8};
int sequence[MAX_SEQUENCE] = {0};
int sequenceLength = 0;
int buttonState = 0;

void setup() {
 Serial.begin(9600);
  // initialize the LED pin as an output:
  for (int i = 0; i < 4; i++) {
    pinMode(output[i], OUTPUT);      
    pinMode(input[i], INPUT);     

  }
}

void loop(){  
  Serial.println("New round ....");
  generateNextSequenceStep();
  displaySequence();
  int success = verifyUserInput();
  if(success == 1){
    Serial.println("Good.");
    blinkWin();
  }else{
    Serial.println("Error! Resetting sequence ...");
    blinkLoss();
    resetSequence();
  }
  delay(2000);
}

void generateNextSequenceStep(){
    if(sequenceLength >= MAX_SEQUENCE){
      return;
    }
    long nextSequenceNumber = random(4);
    Serial.print("Next expected number: ");
    Serial.println(nextSequenceNumber);
    sequence[sequenceLength] = nextSequenceNumber;
    sequenceLength++;
}

int verifyUserInput(){
  for(int i = 0; i < sequenceLength; i++){
    int expectedBottle = sequence[i];
    if(verifyNextUserInput(expectedBottle) == 0){
      return 0;
    }
    
    digitalWrite(output[expectedBottle], HIGH);
    delay(200);
    digit alWrite(output[expectedBottle], LOW);
  }
  return 1;
}

int verifyNextUserInput(int expectedBottle){
  Serial.println("Waiting for next user input ...");
  Serial.print("Expecting: ");
  Serial.println(expectedBottle);
  Serial.flush();
  unsigned long startTime = millis();
  int timeout = 5000;
  while(1){
    if(millis()-startTime > timeout){
      return 0;
    }
    for(int i = 0; i < 4; i++){
      int state = digitalRead(input[i]);
      if(state == HIGH){
        Serial.print(i);
        Serial.println(" was pressed!");
        if (i == expectedBottle){
          return 1;
        }else{
          return 0;
        }
      }
    }
  }
}

void displaySequence(){
   for(int i = 0; i < sequenceLength; i++){
     int bottle = sequence[i];
     digitalWrite(output[bottle], HIGH);
     delay(1000);
     digitalWrite(output[bottle], LOW);
     delay(200);
   }
}

void resetSequence(){
  sequenceLength = 0;
}

void blinkWin(){
  digitalWrite(output[2], HIGH);
  digitalWrite(output[3], HIGH);
  delay(500);
  
  digitalWrite(output[2], LOW);
  digitalWrite(output[3], LOW);
  delay(500);
  
  digitalWrite(output[2], HIGH);
  digitalWrite(output[3], HIGH);
  delay(500);
  
  digitalWrite(output[2], LOW);
  digitalWrite(output[3], LOW);
  delay(500);
}

void blinkLoss(){
  digitalWrite(output[0], HIGH);
  digitalWrite(output[1], HIGH);
  delay(500);
  
  digitalWrite(output[0], LOW);
  digitalWrite(output[1], LOW);
  delay(500);
  
  digitalWrite(output[0], HIGH);
  digitalWrite(output[1], HIGH);
  delay(500);
  
  digitalWrite(output[0], LOW);
  digitalWrite(output[1], LOW);
  delay(500);
}
