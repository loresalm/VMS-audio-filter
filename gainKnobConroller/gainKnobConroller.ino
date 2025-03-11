 #include <MIDIUSB.h>

// Define the pin where the potentiometer (knob) is connected
const int potPin = A0;

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

void setup() {
  // Initialize the smoothing array
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
  
  // Start serial for debugging
  Serial.begin(115200);
  Serial.println("MIDI Controller starting...");
}

void loop() {
  // Read the potentiometer and smooth the value
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(potPin);
  total = total + readings[readIndex];
  readIndex = (readIndex + 1) % numReadings;
  
  // Calculate the average of readings
  average = total / numReadings;
  
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