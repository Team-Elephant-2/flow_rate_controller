const int sampleTimeinMs = 200;
const int baudRate = 9600;

// not used yet
const int wlanLedPin = 19;

const int blueButtonPin = 17;
const int whiteButtonPin = 16;
const int greenButtonPin = 4;
const int redButtonPin = 13;

const int buttonPins[] = {
    blueButtonPin,
    whiteButtonPin,
    greenButtonPin,
    redButtonPin,
};
const int numButtonPins = 4;

const int valveTriggerPin = 26;
const int pumpTriggerPin = 27;

void setup(){
    Serial.begin(baudRate);

    for (int i = 0; i < numButtonPins; i++){
        pinMode(buttonPins[i], INPUT_PULLDOWN);
    };

    pinMode(wlanLedPin, OUTPUT);
    pinMode(valveTriggerPin, OUTPUT);
    pinMode(pumpTriggerPin, OUTPUT);
}

void loop(){
    if (digitalRead(blueButtonPin) == HIGH) {
        digitalWrite(pumpTriggerPin, HIGH);
    }

    if (digitalRead(whiteButtonPin) == HIGH) {
        digitalWrite(pumpTriggerPin, LOW);
    }

    if (digitalRead(greenButtonPin) == HIGH) {
        digitalWrite(valveTriggerPin, HIGH);
    }

    if (digitalRead(redButtonPin) == HIGH) {
        digitalWrite(valveTriggerPin, LOW);
    }

    delay(sampleTimeinMs);
}
