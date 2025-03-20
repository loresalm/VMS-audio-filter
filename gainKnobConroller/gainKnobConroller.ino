 #include <MIDIUSB.h>
 #include <MIDI.h>
// Define the pin where the potentiometer (knob) is connected
const int potPin = A0;
// led pins 
const int ledPin = 9; // PWM pin for the LED
/*
const int pin_red = 9;
const int pin_green = 3;
const int pin_blue = 5;
int ledValue = 0;
*/


// Define the MIDI channel and controller number
const byte midiChannel = 0;  // MIDI channels are 0-15 in code (shown as 1-16 to users)
const byte controllerNumber = 7;  // CC #7 is volume control - widely recognized by DAWs

// Variables to track potentiometer value
int currentPotValue = 0;
int lastPotValue = -1;
int midiValue = 0;

// For smoothing the potentiometer readings
const int numReadings = 5;
int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  // Initialize the smoothing array
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
  // setup pin led 
  pinMode(ledPin, OUTPUT); // Set the LED pin as an output
  /*
  pinMode(pin_red, OUTPUT);
  pinMode(pin_green, OUTPUT);
  pinMode(pin_blue, OUTPUT);
  */

  //setup midi in 
  MIDI.begin(MIDI_CHANNEL_OMNI);
  //MIDI.setHandleControlChange(MyCCFunction);
  
  // Start serial for debugging
  Serial.begin(115200);
  Serial.println("MIDI Controller starting...");
}

void loop() {

  if (MIDI.read()) { // Check if a MIDI message is received
    if (MIDI.getType() == midi::ControlChange) { // Check if it's a Control Change message
      if (MIDI.getData1() == 7) { // Check if it's CC#7
        int value = MIDI.getData2(); // Get the value (0-127)
        // Map the MIDI value (0-127) to a PWM value (0-255)
        int pwmValue = map(value, 0, 127, 0, 255);
        
        // Set the LED brightness using PWM
        analogWrite(ledPin, pwmValue);
      }
    }
  }

  // write to led
  //analogWrite(pin_red, 255);
  //analogWrite(pin_green, 0);
  //analogWrite(pin_blue, 0);

  // Read the potentiometer and smooth the value
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(potPin);
  total = total + readings[readIndex];
  readIndex = (readIndex + 1) % numReadings;
  
  // Calculate the average of readings
  average = total / numReadings;

/*
  ledValue = map(average, 0, 1023, 0, 1000);
  if(ledValue < 500){
    analogWrite(pin_red, 0);
    analogWrite(pin_green, 0);
    analogWrite(pin_blue, 255);
  }else{
    analogWrite(pin_red, 250);
    analogWrite(pin_green, 0);
    analogWrite(pin_blue, 0);
  }
  */
  
  
  // Map the value to MIDI range (0-127)
  midiValue = map(average, 0, 1023, 0, 127);
  
  // Only send MIDI if the value has changed significantly
  if (abs(midiValue - lastPotValue) > 0) {
    // Send the MIDI CC message
    sendMidiCC(midiChannel, controllerNumber, midiValue);
    
    // Update the last value
    lastPotValue = midiValue;
    
    // Debug output
    Serial.print("Sending CC#");
    Serial.print(controllerNumber);
    Serial.print(" value: ");
    Serial.print(midiValue);
    Serial.print(" (pot: ");
    Serial.print(average);
    Serial.println(")");
  }
  
  // Send a refresh message every 2 seconds to ensure connection
  static unsigned long lastRefreshTime = 0;
  if (millis() - lastRefreshTime > 2000) {
    // Send the current value again to refresh
    sendMidiCC(midiChannel, controllerNumber, midiValue);
    lastRefreshTime = millis();
    
    Serial.println("Sending refresh message");
  }
  
  // Small delay to stabilize readings and reduce USB bus traffic
  delay(5);
}

// Function to send a MIDI Control Change message
void sendMidiCC(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}
/*
void MyCCFunction(byte channel, byte number, byte value){
    Serial.print("got MIDI msg ");
    Serial.print(number);
    Serial.print(" value: ");
    Serial.print(value);
}
*/