#include <SoftwareSerial.h>

// --- KONFIGURACIJA PINOVA ---
const int LED_PIN = 13;       // Ugrađena LED lampica
const int BT_RX_PIN = 10;     // Poveži TX Bluetooth modula ovde
const int BT_TX_PIN = 11;     // Poveži RX Bluetooth modula ovde
const int BUTTON_PIN = 2;     // Opciono: Dugme za promenu modela

// --- KOMUNIKACIJA ---
// Baud rate mora biti ISTI kao u Android aplikaciji (BAUD_RATE = 115200)
const long USB_BAUD_RATE = 115200; 
const long BT_BAUD_RATE = 9600;    // Standardno za HC-05

SoftwareSerial BTSerial(BT_RX_PIN, BT_TX_PIN); // Kreiramo softverski serijski port

String inputString = "";      // Ovde čuvamo poruku koja pristiže
bool stringComplete = false;  // Zastavica da li je poruka cela

void setup() {
  // 1. Inicijalizacija USB komunikacije (Hardware Serial)
  Serial.begin(USB_BAUD_RATE);
  
  // 2. Inicijalizacija Bluetooth komunikacije (Software Serial)
  BTSerial.begin(BT_BAUD_RATE);

  // 3. Podešavanje pinova
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Dugme sa internim pull-up otpornikom
  
  digitalWrite(LED_PIN, LOW); // Početno stanje: ugaseno

  Serial.println("Arduino spreman. Cekam konekciju...");
}

void loop() {
  // --- A) ČITANJE SA USB-a (Ako je telefon povezan kablom) ---
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }

  // --- B) ČITANJE SA BLUETOOTH-a (Ako je telefon povezan bežično) ---
  while (BTSerial.available()) {
    char inChar = (char)BTSerial.read();
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }

  // --- C) OBRADA PORUKE ---
  if (stringComplete) {
    processMessage(inputString);
    // Resetuj buffer
    inputString = "";
    stringComplete = false;
  }

  // --- D) SLANJE KOMANDE TELEFONU (OPCIONO) ---
  // Ako pritisneš dugme, šaljemo telefonu komandu da učita drugi model
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50); // Debounce
    if (digitalRead(BUTTON_PIN) == LOW) {
      sendLoadModelCommand("model_flase.tflite", "labels.txt", 60);
      delay(500); // Sacekaj malo da ne salje sto puta
    }
  }
}

// Funkcija koja analizira šta je telefon poslao
void processMessage(String msg) {
  msg.trim(); // Ukloni prazna mesta sa početka i kraja

  // Ignoriši prazne poruke
  if (msg.length() == 0) return;

  // Debug ispis na Serial Monitor (ako je zakacen na PC)
  // Serial.print("Primljeno: "); Serial.println(msg);

  // LOGIKA DETEKCIJE
  if (msg.indexOf("NODETECT") >= 0) {
    // Telefon ne vidi ništa -> Gasi LED
    digitalWrite(LED_PIN, LOW);
  } 
  else if (msg.indexOf("bottle") >= 0 || msg.indexOf("flasa") >= 0) {
    // Telefon vidi flašu -> Pali LED
    digitalWrite(LED_PIN, HIGH);
  }
  else if (msg.indexOf("person") >= 0) {
     // Primer: Blikaj brzo ako vidi osobu
     digitalWrite(LED_PIN, HIGH);
     delay(100);
     digitalWrite(LED_PIN, LOW);
     delay(100);
     digitalWrite(LED_PIN, HIGH);
  }
  
  // Ako stigne ERROR od telefona
  if (msg.indexOf("ERROR") >= 0) {
    // Uradi nešto specifično, npr. blinkaj sporo
  }
}

// Funkcija za slanje komande telefonu da promeni model
void sendLoadModelCommand(String modelName, String labelName, int confidence) {
  // Format komande: "ime_modela.tflite,ime_labela.txt,conf60"
  String command = modelName + "," + labelName + ",conf" + String(confidence);
  
  // Šaljemo na oba porta (nije bitno koji je aktivan, telefon će primiti)
  Serial.println(command);   // Šalji na USB
  BTSerial.println(command); // Šalji na Bluetooth
}
