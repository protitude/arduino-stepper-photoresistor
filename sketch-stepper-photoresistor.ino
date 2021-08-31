#include <Stepper.h>
#include <EEPROM.h>

int in1Pin = 12;
int in2Pin = 11;
int in3Pin = 10;
int in4Pin = 9;

// Retrieve the location of the wheel on power up.
int currentStep = EEPROM.read(0);
// Set clock to 0 for 60 second counter.
int resistorClock = 0;
// Name each pupil.
String currentEye[] = {"tiny", "pin", "pinShift", "small", "large", "star", "heart", "center", "downLeft", "left", "lefter"};
// Reset to tiny pupil.
String setEye = "tiny";

Stepper motor(512, in1Pin, in2Pin, in3Pin, in4Pin);

void setup()
{
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);

  //while (!Serial);

  Serial.begin(9600);
  motor.setSpeed(20);
}

void loop()
{
  // Every 30 seconds check resistor and set pupil accordingly.
  if (resistorClock++ == 30) {
    int photoValue = analogRead(A0); // PhotoResistor readings
    Serial.println("Analog Value :");
    Serial.println(photoValue);
    setEye = photoSetEye(photoValue);
    Serial.println("Photon Eye :");
    Serial.println(setEye);
    resistorClock = 0; // Reset clock
  }
  // Run every second.
  delay(1000);

  // Check if current eye is set to the same as the photo resistor, if not increment up one eye.
  if(setEye != currentEye[currentStep]) {
    motor.step(186.55);
    currentStep++;
    if (currentStep > 10) {
      int amountOver = currentStep - 11;
      currentStep = 0 + amountOver;
    }
    // Persist current step after unplugging.
    EEPROM.write(0, currentStep);
    Serial.println("Wheel set to: ");
    Serial.println(currentEye[currentStep]);
    Serial.println(currentStep);
    Serial.println("Eprom set to: ");
    Serial.println(EEPROM.read(0));
  }

  if (Serial.available()) {
    int steps = Serial.parseInt();
    //motor.step(steps*186.5);
    motor.step(steps);
    //Serial.println(steps);
    //currentStep = currentStep + steps;
    //if (currentStep > 10) {
    //  int amountOver = currentStep - 11;
    //  currentStep = 0 + amountOver;
    //}

    //EEPROM.write(0, currentStep);
    delay(120000); // Delay 2 minutes when sent
  }
}

// Take the photoresistor value and line that up with different eyes.
String photoSetEye(int photoValue){
  String setEye;
  if (photoValue < 500) {
    setEye = "tiny";
  } else if (photoValue > 501 && photoValue < 550) {
    setEye = "pin";
  } else if (photoValue > 551 && photoValue < 600) {
    setEye = "pinShift";
  } else if (photoValue > 601 && photoValue < 650) {
    setEye = "small";
  } else if (photoValue > 651 && photoValue < 700) {
    setEye = "large";
  } else if (photoValue > 701 && photoValue < 750) {
    setEye = "star";
  } else if (photoValue > 751 && photoValue < 800) {
    setEye = "heart";
  } else if (photoValue > 801 && photoValue < 850) {
    setEye = "center";
  } else if (photoValue > 851 && photoValue < 900) {
    setEye = "downLeft";
  } else if (photoValue > 901 && photoValue < 950) {
    setEye = "left";
  } else if (photoValue > 951 && photoValue < 1000) {
    setEye = "lefter";
  }
  return setEye;
}
