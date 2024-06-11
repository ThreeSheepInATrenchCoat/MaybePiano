//This code is for use with a MIDI chime contraption
//MIDI data is sent from a keyboard to the serial input of Arduino
//Arduino decides what solenoid MOSFETs to energize based on what note is pressed
//The solenoids strike steel tubes that are tuned for a specific note

 
const byte noteOn = 144; //note on message
const int hold = 50;  //solenoid sustain time

const int numKeys = 4;

struct key {
  int pin;                  // Arduino pin number
  int command;              // Serial command
  unsigned long pressTime;  // Time key was pressed, or 0 if not pressed
};

struct key keys[numKeys];
 
void setup() {
  Serial.begin(115200);

  pinMode(0, INPUT);   //MIDI Input

  // C4
  keys[0].pin = 5;
  keys[0].command = 60;

  // C4s
  keys[1].pin = 4;
  keys[1].command = 61;

  // D4s
  keys[2].pin = 3;
  keys[2].command = 62;

  // D4
  keys[3].pin = 2;
  keys[3].command = 63;

  // Set initial state for all keys
  for (int i = 0; i < numKeys; i++) {
    pinMode(keys[i].pin, OUTPUT);
    digitalWrite(keys[i].pin, LOW);
    keys[i].pressTime = 0;
  }
}

void checkMIDI() {
  // Exit function if there are no bytes waiting
  if (!Serial.available()) {
    return;
  }

  // Recive byte buffers
  // Will be -1 if invalid
  static int commandByte = -1;
  static int noteByte = -1;
  static int velocityByte = -1;

  // Check if first byte is command
  byte rxByte = Serial.read();
  if (commandByte == -1) {
    if (rxByte == noteOn) {
      commandByte = rxByte;
    }
  }

  // Only store the next 2 bytes if we get a valid noteOn command
  if (commandByte != -1) {
    if (noteByte == -1) {
      // Note not yet populated, so second byte
      noteByte = rxByte;
    } else {
      // Note populated, so third byte
      velocityByte = rxByte;
    }
  }

  // Full 3 bytes received
  if (velocityByte != -1) {
    if (velocityByte > 0) {
      // Action note byte
      for (int i = 0; i < numKeys; i++) {
        if (noteByte == keys[i].command) {
          digitalWrite(keys[i].pin, HIGH); 
          keys[i].pressTime = millis();
        }
      }
    }
    commandByte = noteByte = velocityByte = -1;
  }
}
 
void loop() {
  int currentTime;

  // Check serial comms and update keys
  checkMIDI();

  // Check all keys for expired timers
  currentTime = millis();
  for (int i = 0; i < numKeys; i++) {
    if (keys[i].pressTime != 0) {
      if ((currentTime - keys[i].pressTime) >= hold) {
        digitalWrite(keys[i].pin, LOW);
        keys[i].pressTime = 0;
      }
    }
  }
}