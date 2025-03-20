#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

const int ledPin = 9; // PWM pin for the LED

void setup() {
  pinMode(ledPin, OUTPUT); // Set the LED pin as an output
  MIDI.begin(MIDI_CHANNEL_OMNI); // Listen to all MIDI channels
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
}