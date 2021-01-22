// BitsyBoard Petting Zoo
// Zane Cochran
// ATTineous Vibratous

// While ATTineous Territorious is generally a territorial creature and can become quite temperamental 
// when encroached upon, it does occasionally welcome contact from more keenly observant caretakers.


#include <CapacitiveSensor.h>                       // Include the Capacitive Touch Sensor Library
CapacitiveSensor feeler = CapacitiveSensor(3,4);    // Capacitive "Feeler" attached to Pins 3 and 4

int redLED = 2;       // Red Anode Connected to Pin 2
int greenLED = 1;     // Green Anode Connected to Pin 2
int blueLED = 0;      // Blue Anode Connected to Pin 2

int feelThresh = 65;  // Capacitive Threshold when Animal is Touched
int happyChance = 5;  // Likelihood that a Touch will make the Animal Happy
int mood = 0;         // The current mood of the animal

int standbyBright = 128;      // How bright the animal should be in standby mood
long standbyTimer = 0;        // Timer to keep track of standby animation cycle
long standbySpeed = 10;       // How quickly should the breathing information change
boolean standbyDir = false;   // Direction of fading for standby breathing

int happyMax = 255;           // How bright the animal should be in happy mood
int happyTimes = 5;           // Number of cycles the animal should be happy
int happySpeed = 3;           // How quicky the should cycle when it's happy

void setup() {
  // Set LEDs to OUTPUT
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);

  // Initialize Random Seed
  randomSeed(analogRead(3));
}

void loop() {
  // Monitor the Animal's Mood
  switch(mood){
    case 0: standby();  break;
    case 1: angry();    break;
    case 2: happy();    break;
  }
}

// Nominal State of the Animal (steady green breathing)
void standby(){
  if(millis() - standbyTimer > standbySpeed){   // Check to see the standbyTimer to update the Green LED
    if(standbyDir){standbyBright++;}            // If direction is true, increase brightness
    else{standbyBright--;}                      // Otherwise, decrease brightness
    
    if(standbyBright <= 100 || standbyBright >= 200){standbyDir = !standbyDir;} // If brightness is at either extreme, reverse direction
    analogWrite(greenLED, standbyBright);                                       // Update the Green LED
    standbyTimer = millis();                                                    // Update the Timer
  }

  long sense = feeler.capacitiveSensor(30);     // Get the current value of the capacitive sensor
  if(sense > feelThresh){                       // If that value exceeds the predetermined threshold, change the mood
    analogWrite(greenLED, 0);                   // Shut off the standby LED
    if(random(happyChance) == 0){mood = 2;}     // If the random value chosen is zero, the animal mood is happy!
    else{mood = 1;}                             // Otherwise animal mood is angry
  }
}

// Angry State of the Animal (red flashing)
void angry(){
  // Long then Short Flashes of Red LED
  digitalWrite(redLED, HIGH); delay(1000);
  digitalWrite(redLED, LOW); delay(100);
  digitalWrite(redLED, HIGH); delay(100);
  digitalWrite(redLED, LOW); delay(100);
  digitalWrite(redLED, HIGH); delay(100);
  digitalWrite(redLED, LOW); delay(100);

  // Animal is Done being angry, return to standby mood
  mood = 0;
}

// Happy State of the Animal (quick blue fading)
void happy(){

  // Fade Blue LED up and down several times
  for (int i = 0; i < happyTimes; i++){
    for (int j = 0; j < happyMax; j++){analogWrite(blueLED, j); delay(happySpeed);} // Fade LED Up
    for (int j = happyMax; j > 0; j--){analogWrite(blueLED, j); delay(happySpeed);} // Fade LED Down
  }
  
  analogWrite(blueLED, 0);  // Turn the LED off

  // Animal is Done being happy, return to standby mood
  mood = 0;
}
