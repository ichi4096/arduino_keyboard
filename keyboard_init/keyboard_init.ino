/*
  Arduino Sketch for figuring out the mapping of pins keyboard matrix
*/

// little ring buffer implementation
template <typename T, size_t maxSize>
struct RingBuffer{
  T data[maxSize];
  size_t begin, end;

  inline void push(const T& object){
    if(!isFull()){
      data[end] = object;
      end = (end + 1) % maxSize;
    }
    return;
  }

  inline T pop(){
    if(!isEmpty()){
      T ret = data[begin];
      begin = (begin + 1) % maxSize;
      return ret;
    }
  }

  inline bool isFull() const {
    return begin == end+1 || (begin == 0 && end == maxSize-1);
  }

  inline bool isEmpty() const {
    return begin == end;
  }
};

struct uint8_pair{
  uint8_t first, second;
};

constexpr size_t nrKeyPins = 11 + 8;
constexpr size_t nrKeys = 88;
RingBuffer<uint8_pair, 2*nrKeys> keyEventQueue;

// function mapping 0 ... nrKeyPins-1 to actual pin numbers
constexpr uint8_t keyPins(size_t i){
  return i + 35;
}

/* matrix of all pin combinations, storing
last seen state: (un-/)connected */
bool pinMatrix[nrKeyPins][nrKeyPins] = {false};

void scanKeys(){
  for(unsigned int i=0; i<nrKeyPins; i++){
    pinMode(keyPins(i), INPUT_PULLUP);
    for(unsigned int j=0; j<nrKeyPins; j++){
      if(i == j){
        continue;
      }
      digitalWrite(keyPins(j), LOW);
      bool curState = !digitalRead(keyPins(i));
      if(curState != pinMatrix[i][j]){
        keyEventQueue.push({i,j});
        pinMatrix[i][j] = curState;
      }
      digitalWrite(keyPins(j), HIGH);
    }
    pinMode(keyPins(i), OUTPUT);
    digitalWrite(keyPins(i), HIGH);
  }
}

void setup() {

  // initialize the key pins with default value
  for(size_t i=0; i<nrKeyPins; i++){
    pinMode(keyPins(i), OUTPUT);
    digitalWrite(keyPins(i), HIGH);
  }

  Serial.begin(9600);
}

void loop() {
  scanKeys();
  while(!keyEventQueue.isEmpty()){
    uint8_pair event = keyEventQueue.pop();
    if(pinMatrix[event.first][event.second]){
      Serial.println("{"+String(event.first)+", "+String(event.second)+"},");
    }
  }
}
