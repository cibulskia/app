// --- PODEŠAVANJA ---
const int LED_PIN = 13;       // Ugrađena LED lampica
const long BAUD_RATE = 115200; // OBAVEZNO: Mora biti isto kao u Android aplikaciji!

// Promenljive za prijem poruka
String inputString = "";      // Buffer za poruku
bool stringComplete = false;  // Da li je stigao kraj reda (\n)

void setup() {
  // Pokrećemo serijsku komunikaciju preko USB-a
  Serial.begin(BAUD_RATE);
  
  // Podešavamo LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Početno stanje: ugašeno

  // Opciono: Pošalji poruku da je spreman (vidi se u Serial Monitoru)
  // Serial.println("Arduino USB Ready");
}

void loop() {
  // 1. ČITANJE PODATAKA SA USB-a
  // Proveravamo da li ima novih bajtova u bufferu
  while (Serial.available()) {
    // Čitaj jedan po jedan karakter
    char inChar = (char)Serial.read();
    
    // Android šalje '\n' na kraju svake poruke
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      // Dodaj karakter u string
      inputString += inChar;
    }
  }

  // 2. OBRADA PORUKE
  if (stringComplete) {
    processMessage(inputString);

    // Očisti buffer za sledeću poruku
    inputString = "";
    stringComplete = false;
  }
}

// Funkcija koja odlučuje šta da radi na osnovu teksta
void processMessage(String msg) {
  // Ukloni prazna mesta (whitespace) sa početka i kraja
  msg.trim();

  // Ako je poruka prazna, ignoriši
  if (msg.length() == 0) return;

  // --- LOGIKA SLIČNA PYTHON KODU ---

  // Slučaj 1: Ništa nije detektovano
  if (msg.indexOf("NODETECT") >= 0) {
    digitalWrite(LED_PIN, LOW);
  } 
  // Slučaj 2: Detektovana je flaša (prilagodi "bottle" ili "flasa" prema svom modelu)
  else if (msg.indexOf("bottle") >= 0 || msg.indexOf("flasa") >= 0) {
    digitalWrite(LED_PIN, HIGH);
  }
  // Slučaj 3: Detektovana osoba (primer)
  else if (msg.indexOf("person") >= 0) {
    // Možeš dodati drugu logiku, npr blinkanje
    digitalWrite(LED_PIN, HIGH);
  }
  
  // Ovde možeš dodati kod za Servo motor, Relej, itd.
}
