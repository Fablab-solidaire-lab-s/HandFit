
#include "Keyboard.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

///// LCD
LiquidCrystal_I2C lcd(0x3F, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

byte customChar[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b00000,
  0b00000
};



//// Keyboard
char ctrlKey = KEY_LEFT_GUI;
char haut = KEY_UP_ARROW;
char bas = KEY_DOWN_ARROW;
char gauche = KEY_LEFT_ARROW;
char droite = KEY_RIGHT_ARROW;
char echap = KEY_ESC;
char entre = KEY_RETURN;


//Capteur vitesse
const int capteurPin = 10;  // Broche du capteur magnétique
unsigned long previousMillis = 0;
const long interval = 1000;              // Intervalle de temps pour le calcul de la vitesse (en millisecondes)
int aimantState = HIGH;                  // État actuel du capteur magnétique
int previousAimantState = HIGH;          // État précédent du capteur magnétique
unsigned int rotations = 0;              // Compteur de rotations
float circonferenceEnKm = 0.0007539823;  // Circonférence de la roue en km

//pin bouton
#define PinGauche 5
#define PinDroite 4
#define PinEntre 8
#define PinEchap 9


// Setting up the counter
int reading = 0;
int changeamnt = 1;

// Timing for polling the encoder
unsigned long currentTime;
unsigned long lastTime;

// Pin definitions for encoder
const int pinA = 6;
const int pinB = 7;

boolean encA;
boolean encB;
boolean lastA = false;

void setup() {
  // make pin 2 an input and turn on the pull-up resistor so it goes high unless
  // set the two pins as inputs with internal pullups
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);

  // connected to ground:
  pinMode(PinGauche, INPUT_PULLUP);
  pinMode(PinDroite, INPUT_PULLUP);
  pinMode(PinEntre, INPUT_PULLUP);
  pinMode(PinEchap, INPUT_PULLUP);

  //capteur Vitesse
  pinMode(capteurPin, INPUT_PULLUP);

  lcd.createChar(1, customChar);
  lcd.init();  // initialize the lcd
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("DEMARRAGE");
  for (int i = 0; i < 16; i++) {
    lcd.setCursor(i, 1);
    lcd.write(byte(1));
    delay(100);
  }
  delay(1500);

  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Bonjour");
  lcd.setCursor(4, 1);
  lcd.print("Bienvenue");
  delay(2000);

  // initialize control over the keyboard:
  Keyboard.begin();

  // Set up the timing of the polling
  currentTime = millis();
  lastTime = currentTime;

  Serial.begin(9600);
}


void loop() {

  currentTime = millis();

  int aimantState = digitalRead(capteurPin);

  // Check if it's time to read
  if (currentTime >= (lastTime + 0.1)) {
    // read the two pins
    encA = digitalRead(pinA);
    encB = digitalRead(pinB);
    // check if A has gone from high to low
    if ((!encA) && (lastA)) {
      // check if B is high
      if (encB) {

        reading = reading + changeamnt;
        ///
        Keyboard.press(haut);
      } else {
        // anti-clockwise
        reading = reading - changeamnt;

        Keyboard.press(bas);
      }
      // Output reading for debugging
      Serial.println(reading);
    }
    Keyboard.release(haut);
    Keyboard.release(bas);
    // store reading of A and millis for next loop
    lastA = encA;
    lastTime = currentTime;
  }

  if (digitalRead(PinEchap) == LOW) {

    Keyboard.press(echap);
  } else {
    Keyboard.release(echap);
  }

  if (digitalRead(PinEntre) == LOW) {

    Keyboard.press(entre);
  } else {
    Keyboard.release(entre);
  }


  ///
  if (digitalRead(PinDroite) == LOW) {

    Keyboard.press(droite);
  } else {
    Keyboard.release(droite);
  }

  ///
  if (digitalRead(PinGauche) == LOW) {

    Keyboard.press(gauche);
  } else {
    Keyboard.release(gauche);
  }

  //////////////////////////////////////VITESSE RPM////////////////////////////

  if (aimantState != previousAimantState) {
    // Si l'état du capteur magnétique a changé, incrémenter le compteur de rotations
    rotations++;
  }

  if (currentTime - previousMillis >= interval) {
    // Si l'intervalle de temps s'est écoulé, calculer et afficher la vitesse de rotation en tours par minute (RPM)
    float vitesseRotation = (float)rotations / (float)(interval / 1000) * 60;
    Serial.print("Vitesse de rotation: ");
    Serial.print(vitesseRotation);
    Serial.println(" RPM");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Vitesse RPM :");
    lcd.setCursor(0, 1);
    lcd.print(vitesseRotation);

    // Réinitialiser les compteurs pour le prochain intervalle
    rotations = 0; 
    previousMillis = currentTime;
  }

  previousAimantState = aimantState;
}
