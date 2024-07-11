/*
  Arduino Sketch for keyboard controller
  Given the appropriate pins in rowA and rowB
  ordered by keys, generates MIDI on/off events
  by multiplexing. pitches are ordered first by
  rowB, then by rowA, and the diodes in the key
  matrix are such that rowA must be positive voltage
  (via input_pullup) and rowB tied to ground when
  scanning.
*/

// requires the "Control Surface" library and MIDI USB firmware
#include <Control_Surface.h>

constexpr size_t nrKeyPinsA = 8, nrKeyPinsB = 11;
constexpr uint8_t rowA[nrKeyPinsA] = {41, 39, 42, 40, 38, 37, 36, 35};
constexpr uint8_t rowB[nrKeyPinsB] = {49, 51, 50, 53, 52, 43, 48, 46, 47, 45, 44};
constexpr size_t nrKeys = 88;
constexpr unsigned long debounceMillis = 40;
USBMIDI_Interface midi;

/* matrix of all pin combinations, storing
last seen state: (un-/)connected */
bool pinMatrix[nrKeyPinsA][nrKeyPinsB] = {false};
/* corresponding to pinMatrix, stores when
the element at a given index was last accessed in
terms of millis(), which will overflow in 50 days, which
is not handled by this code */
unsigned long lastUpdated[nrKeyPinsA][nrKeyPinsB] = {0};

void scanKeys(){
  for(unsigned int i=0; i<nrKeyPinsA; i++){
    for(unsigned int j=0; j<nrKeyPinsB; j++){
      digitalWrite(rowB[j], LOW);
      bool curState = !digitalRead(rowA[i]);
      if(curState != pinMatrix[i][j] &&
          millis() - lastUpdated[i][j] > debounceMillis){ // software debounce
        uint8_t pitch = 21 + i + nrKeyPinsA * j;
        MIDIAddress note(pitch);
        if(curState){
          midi.sendNoteOn(note, 127);
        }else{
          midi.sendNoteOff(note, 127);
        }
        pinMatrix[i][j] = curState;
        lastUpdated[i][j] = millis();
      }
      digitalWrite(rowB[j], HIGH);
    }
  }/
}

void setup() {
  for(size_t i=0; i<nrKeyPinsA; i++){
    pinMode(rowA[i], INPUT_PULLUP);
  }
  for(size_t i=0; i<nrKeyPinsB; i++){
    pinMode(rowB[i], OUTPUT);
    digitalWrite(rowB[i], HIGH);
  }

  midi.begin();
}

void loop() {
  scanKeys();
}
