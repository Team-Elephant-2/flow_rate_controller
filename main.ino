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

void setup(){
    Serial.begin(baudRate);

    for (int i = 0; i < numButtonPins; i++){
        pinMode(buttonPins[i], INPUT_PULLDOWN);
    };
}

void loop(){
    Serial.print("Button States (Blue White Green Red): ");

    for (int i = 0; i < numButtonPins; i++) {
        int buttonState = digitalRead(buttonPins[i]);
        Serial.print(buttonState);
        Serial.print(" ");
    }
    Serial.println();

    delay(sampleTimeinMs);

}
