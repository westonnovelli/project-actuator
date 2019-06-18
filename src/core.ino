#include <Keypad.h>
#include <hid_keys.h>

const byte ROWS = 7;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  // Matrix is mapped to keys, easier to make this matrix and figure out which keys it maps
  // to by pressing them, than to try to figure it out based on the wiring
  {'A', 'B', 'C', 'D'},
  {'E', 'F', 'G', 'H'},
  {'I', 'J', 'K', 'L'},
  {'M', 'N', 'O', 'P'},
  {'a', 'b', 'c', 'd'},
  {'e', 'f', 'g', 'h'},
  {'i', 'j', 'k', 'l'}
};
byte rowPins[ROWS] = {6, 7, 8, 9, 10, 11, 12}; // output
byte colPins[COLS] = {2, 3, 4, 5}; // input

const int signalOut = 14;
const int signalIn = 15;
const int rotaryOption1 = 16;
const int rotaryOption3 = 17;
int rotary1;
int rotary2;

Keypad kpd = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

bool pressed = false;
uint8_t idx = 2;
uint8_t buf[8] = { 0 }; // USB key buffer. byte 0 is a modifier (bitmap for shift, ctrl, etc.)
                        // byte 1 is reserverd. the remaining are the detected keys.

bool panelActive = true;
int jumpMode = 0;

void setup(){
  Serial.begin(9600);
  
  // setup alt circuit
  pinMode(signalIn, INPUT);
  pinMode(rotaryOption1, INPUT);
  pinMode(rotaryOption3, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // setup keypad
  kpd.setDebounceTime(100);
  kpd.setHoldTime(800); //without this we risk having multiple presses sent to quickly 
  delay(2000);
}

bool handleKeypress(char key) {
  if (key == 'A' || key == 'C') {
    return handleMotivator1(key);
  } else if (key == 'B' || key == 'D') {
    return handleMotivator2(key);
  } else if (key == 'l') {
    return handleJump(key);
  } else if (key == 'P' || key == 'O') {
    return handle0Throttle(key);
  } else if (key == 'M' || key == 'N') {
    return handle50Throttle(key);
  } else if (key == 'd' || key == 'c') {
    return handle75Throttle(key);
  } else if (key == 'a' || key == 'b') {
    return handle100Throttle(key);
  } else if (key == 'k' || key == 'j' || key == 'i') {
    return handleSafety(key);
  } else if (key == 'h' || key == 'g' || key == 'e' || key == 'f') {
    return handleShipPeripherals(key);
  } else if (key == 'E' || key == 'F' || key == 'G' || key == 'H') {
    return handleVisualAugmentation(key);
  } else if (key == 'I' || key == 'J' || key == 'K' || key == 'L') {
    return handleTravelAssist(key);
  }
  return false;
}

bool queueModifiers(uint8_t modifier) {
  buf[0] = modifier;
  return true;
}

bool queueKeystroke(uint8_t keystroke) {
  buf[idx] = keystroke;
  idx++;
  return true;
}

void serialWrite(uint8_t buf[8], int len) {
  if (panelActive) {
    Serial.write(buf, len);
  }
}

void sendKeystroke() {
  serialWrite(buf, 8);
  releaseKey();
  pressed = false;
  idx = 2;
}

void releaseKey() {
    for(int i = 0; i < 8; i++) {
        buf[i] = 0;
    }
    serialWrite(buf, 8);  // Release key  
}

int readJumpMode(int supercruise, int hyperspace) {
  if (supercruise == 1) {
    return 1;
  } else if (hyperspace == 1) {
    return 2;
  } else {
    return 0;
  }
}

void loop() {
  bool activity = kpd.getKeys();
  if (activity) {
    for (int i = 0; i < LIST_MAX; i++) {
      const Key key = kpd.key[i];
      switch (key.kstate) {
          case PRESSED: {
            bool resultInPress = handleKeypress(key.kchar);
            if (resultInPress) {
              pressed = true;
            }
            continue;
          }
          default:
            break;
      }
    }
  
    if (pressed) { // have we pressed any key
      sendKeystroke();
    }
  }

  // update active state based on on/off switch
  panelActive = digitalRead(signalIn) == HIGH;

  // if the panel is active and the motivators are primed, activate the light
  // keep it off otherwise
  if (panelActive) {
//    if (allPrimed()) {
//      digitalWrite(ledOutput, HIGH);
//    } else {
//      digitalWrite(ledOutput, LOW);
//    }
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
//    digitalWrite(ledOutput, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  }

  // Update jump mode based on rotary state
  jumpMode = readJumpMode(digitalRead(rotaryOption1), digitalRead(rotaryOption3));
}
