// Sketch for Adafruit Trinket-based Grade Crossing Blinker

int rightLED = 0;
int leftLED = 1;
int sensor = 2;

void setup() {
  pinMode(sensor, INPUT_PULLUP);
  setupLEDs();
}

void loop() {
  boolean trainDetected = (digitalRead(sensor) == LOW);
  animateLEDs(trainDetected);
  delay(3);
}


// LED Animation Routines

enum {
  kLEDsOff = 0,
  kLEDStartRightRamp,
  kLEDRightRamp,
  kLEDRightOn,
  kLEDStartLeftRamp,
  kLEDLeftRamp,
  kLEDLeftOn,
  kLEDStartFadeAll,
  kLEDFadeAll,
};

enum {
  kLEDAttack = 8,
  kLEDDecay = 6,
};

enum {
  kMaxBrightness = 255
};

int ledState;
int ledTimer;
int leftLEDValue = 0;
int rightLEDValue = 0;

void setupLEDs() {
  pinMode(leftLED, OUTPUT);     
  pinMode(rightLED, OUTPUT);     
  turnOffLEDs();
}

void turnOffLEDs(void) {
  ledState = kLEDsOff;
  ledTimer = 0;
  leftLEDValue = 0;
  rightLEDValue = 0;
}

void animateLEDs(boolean active) {

  analogWrite(rightLED,255-rightLEDValue);
  analogWrite(leftLED,255-leftLEDValue);

  switch (ledState) {

  case kLEDsOff:
    if (active) {
      ledState = kLEDStartRightRamp;
    }
    break;

  case kLEDStartRightRamp:
    ledTimer = 128;
    ledState = kLEDRightRamp;

  case kLEDRightRamp:
    if (ledTimer != 0) {
      rightLEDValue = min((rightLEDValue + kLEDAttack),kMaxBrightness);
      leftLEDValue = max((leftLEDValue-kLEDDecay),0);
    }
    else {
      ledState = kLEDRightOn;
    }
    break;

  case kLEDLeftOn:
  case kLEDRightOn:
    if (!active) {
      ledState = kLEDStartFadeAll;
    } 
    else if (ledTimer == 0) {
      ledState = (ledState == kLEDRightOn) ? kLEDStartLeftRamp : kLEDStartRightRamp;
    }
    break;

  case kLEDStartLeftRamp:
    ledTimer = 128;
    ledState = kLEDLeftRamp;

  case kLEDLeftRamp:
    if (ledTimer != 0) {
      leftLEDValue = min((leftLEDValue + kLEDAttack),kMaxBrightness);
      rightLEDValue = max((rightLEDValue-kLEDDecay),0);
    }
    else {
      ledState = kLEDLeftOn;
    }
    break;

  case kLEDStartFadeAll:
    ledTimer = 255;
    ledState = kLEDFadeAll;

  case kLEDFadeAll:
    rightLEDValue = max((rightLEDValue-kLEDDecay),0);
    leftLEDValue = max((leftLEDValue-kLEDDecay),0);
    if (ledTimer == 0) {
      ledState = kLEDsOff;
    }
    break;
  }

  if (ledTimer > 0)
    ledTimer--;
}
