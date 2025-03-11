#include <MIDIUSB.h>

// Define the pin where the button is connected
const int buttonPin = 3;

// Define the MIDI channel and note to send
const int midiChannel = 1;  // MIDI channels are 1-16
const int midiNote = 60;     // Middle C

// Variables to keep track of the button state
int buttonState = 0;
int lastButtonState = 0;

void setup() {
  // Set the button pin as an input with the internal pull-up resistor enabled
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // Read the current state of the button
  buttonState = digitalRead(buttonPin);

  // Check if the button state has changed
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      // Button is pressed (LOW because of pull-up), send MIDI Note On message
      sendMidiNoteOn(midiChannel, midiNote, 127);  // 127 is the velocity (max)
    } else {
      // Button is released, send MIDI Note Off message
      sendMidiNoteOff(midiChannel, midiNote, 0);
    }
    // Delay to avoid bouncing
    delay(50);
  }

  // Save the current state as the last state for the next loop
  lastButtonState = buttonState;
}

// Function to send a MIDI Note On message
void sendMidiNoteOn(byte channel, byte note, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, note, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}

// Function to send a MIDI Note Off message
void sendMidiNoteOff(byte channel, byte note, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, note, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}