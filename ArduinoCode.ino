
const int buzzer = 8;
const int LightSensor1 = A1;
const int LightSensor2 = A2;
const int LED1 = 3;
const int LED2 = 4;
const int LED3 = 5;
const int ButtonPin = 2;

const int ThresholdSensor1 = 750;
const int ThresholdSensor2 = 800;
const int DifferenceThreshold = 250;

int Sensor1Read;
int Sensor2Read;
long currentMillis;
long Gate1OpenMillis;
long Gate2OpenMillis;
int FirstOpen;

bool Gate1Open;
bool Gate2Open;
bool Intruder;

void setup() {
  Serial.begin(115200);
  pinMode(A2, INPUT);
  pinMode(A1, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(ButtonPin, INPUT);

}

// maybe reset both when one is over 3000?!?!?!?!?!?!?!??!??!!?!   : line 174
void siren(int iterations) {
  for(int i = 0; i < iterations; i++) {
    // Whoop up
    for(int hz = 440; hz < 1000; hz+=25){
      tone(buzzer, hz, 50);
      delay(5);
    }

    // Whoop down
    for(int hz = 1000; hz > 440; hz-=25){
      tone(buzzer, hz, 50);
      delay(5);
    }
  }
}


void loop() {
  currentMillis = millis();
  Sensor1Read = analogRead(LightSensor1);
  Sensor2Read = analogRead(LightSensor2);

//  Serial.print("Sensor1: ");Serial.println(Sensor1Read);
//  Serial.print("Sensor2: ");Serial.println(Sensor2Read);

  if(Sensor1Read > ThresholdSensor1) {
    Gate1Open = false;
  }
  else {
    Gate1Open = true;
  }
  if(Sensor2Read > ThresholdSensor2) {
    Gate2Open = false;
  }
  else {
    Gate2Open = true;
  }

  if(Gate1Open && Gate1OpenMillis == 0) {
    Gate1OpenMillis = millis(); // Durchgangstimestamp setzen wenn noch nicht gesetzt
    digitalWrite(LED1, HIGH);
  }
  else {
    if(currentMillis - Gate1OpenMillis > 3000) {
      Gate1OpenMillis = 0; // nach 3 sekunden timestamp wieder zurücksetzen
      FirstOpen = 0;
    }
    digitalWrite(LED1, LOW);
  }
  if(Gate2Open && Gate2OpenMillis == 0) {
    Gate2OpenMillis = millis(); // Durchgangstimestamp setzen wenn noch nicht gesetzt
    digitalWrite(LED2, HIGH);
  }
  else {
    if(currentMillis - Gate2OpenMillis > 3000) {
      Gate2OpenMillis = 0; // nach 3 sekunden timestamp wieder zurücksetzen
      FirstOpen = 0;
    }
    digitalWrite(LED2, LOW);
  }

  if(Gate1OpenMillis > 0 || Gate2OpenMillis > 0) { // a led to indicate the reset status
    digitalWrite(LED3, HIGH);
  }
  else {
    digitalWrite(LED3, LOW);
  }
  delay(50);

  if(Gate1OpenMillis < Gate2OpenMillis && Gate1OpenMillis > 0 && Gate2OpenMillis > 0) {
    FirstOpen = 1;
  }
  else if (Gate1OpenMillis > Gate2OpenMillis && Gate1OpenMillis > 0 && Gate2OpenMillis > 0){
    FirstOpen = 0;
  }
//  Serial.print("currentMillis: ");Serial.println(currentMillis);
//  Serial.print("FirstOpen: ");Serial.println(FirstOpen);
//  Serial.print("Gate1OpenMillis: ");Serial.println(Gate1OpenMillis);
//  Serial.print("Gate2openMillis: ");Serial.println(Gate2OpenMillis);
//  Serial.println();
  if(FirstOpen == 1 && Gate2OpenMillis - Gate1OpenMillis > DifferenceThreshold && Gate2OpenMillis > 0) { // adapt DifferenceThreshold according to parallel distance between lasers
    Intruder = false;
  }

  while(!Intruder) {
    siren(1);
    Intruder = digitalRead(ButtonPin);
    Serial.println(Intruder);
  }
}

