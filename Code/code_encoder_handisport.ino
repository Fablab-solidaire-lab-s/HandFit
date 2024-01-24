// Setting up the counter
int reading = 0;
int lowest = -12;
int highest = 12;
int changeamnt = 1;

// Timing for polling the encoder
unsigned long currentTime;
unsigned long lastTime;


// Pin definitions
const int pinA = 16;
const int pinB = 10;

// Storing the readings

boolean encA;
boolean encB;
boolean lastA = false;

void setup() {
  // set the two pins as inputs with internal pullups
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  // Set up the timing of the polling
  currentTime = millis();
  lastTime = currentTime;
  // Start the serial monitor for debugging
  Serial.begin(9600);
}


void loop()
{
  // Read elapsed time
  currentTime = millis();
  // Check if it's time to read
  if (currentTime >= (lastTime + 1))
  {
    // read the two pins
    encA = digitalRead(pinA);
    encB = digitalRead(pinB);
    // check if A has gone from high to low
    if ((!encA) && (lastA))
    {
      // check if B is high
      if (encB)
      {

        reading = reading + changeamnt;

      }
      else
      {
        // anti-clockwise

        reading = reading - changeamnt;

      }
      // Output reading for debugging
      Serial.println(reading);
    }
    // store reading of A and millis for next loop
    lastA = encA;
    lastTime = currentTime;





  }

}
