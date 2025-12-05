// --- PODEŠAVANJA ---
const int LED_PIN = 13;
const long BAUD_RATE = 115200; // Mora biti isto kao na telefonu!

String inputString = "";
bool stringComplete = false;

// Promenljive u koje ćemo sačuvati podatke
String detectedClass = "";
float detectedX = 0.0;
float detectedY = 0.0;
float detectedAzimuth = 0.0;

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  // 1. ČITANJE SA USB-a
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }

  // 2. OBRADA
  if (stringComplete) {
    parseData(inputString);
    
    // Očisti za sledeći put
    inputString = "";
    stringComplete = false;
  }
}

void parseData(String msg) {
  msg.trim(); // Skloni razmake
  if (msg.length() == 0) return;

  // Format poruke sa telefona: "klasa,x,y,azimut;" 
  // Primer: "bottle,0.55,0.42,125;"
  
  // Provera da li je NODETECT
  if (msg.indexOf("NODETECT") >= 0) {
    digitalWrite(LED_PIN, LOW);
    // Serial.println("Nista ne vidim.");
    return;
  }

  // --- PARSIRANJE (Razdvajanje po zarezima) ---
  
  // 1. Nađi pozicije zareza
  int firstComma = msg.indexOf(',');
  int secondComma = msg.indexOf(',', firstComma + 1);
  int thirdComma = msg.indexOf(',', secondComma + 1);
  int endSemi = msg.indexOf(';', thirdComma + 1); // Kraj podataka za taj objekat

  // Ako format nije dobar (fale zarezi), prekini
  if (firstComma == -1 || secondComma == -1 || thirdComma == -1) return;

  // 2. Izdvajanje podataka u Stringove
  String classStr = msg.substring(0, firstComma);
  String xStr = msg.substring(firstComma + 1, secondComma);
  String yStr = msg.substring(secondComma + 1, thirdComma);
  
  // Za Azimut uzimamo od treceg zareza do tacka-zareza (;) ili kraja stringa
  String azStr;
  if (endSemi != -1) {
    azStr = msg.substring(thirdComma + 1, endSemi);
  } else {
    azStr = msg.substring(thirdComma + 1);
  }

  // 3. Konverzija u brojeve
  detectedClass = classStr;
  detectedX = xStr.toFloat();
  detectedY = yStr.toFloat();
  detectedAzimuth = azStr.toFloat();

  // --- TVOJA LOGIKA ---
  // Sada imaš podatke u promenljivima! Evo primera šta možeš s njima:

  // PRIMER 1: Pali LED samo ako je 'bottle' I ako je Azimut veći od 100
  if (detectedClass == "bottle" || detectedClass == "flasa") {
    
    if (detectedAzimuth > 100 && detectedAzimuth < 200) {
       digitalWrite(LED_PIN, HIGH); // Pali
    } else {
       digitalWrite(LED_PIN, LOW);  // Gasi ako ugao nije dobar
    }
  }

  // PRIMER 2: Servo logika (ako imaš servo)
  // Ako je X < 0.4 (objekat levo) -> okreni servo levo
  // Ako je X > 0.6 (objekat desno) -> okreni servo desno
  /*
  if (detectedX < 0.4) {
    // servo.write(0);
  } else if (detectedX > 0.6) {
    // servo.write(180);
  }
  */

  // Debug ispis (da vidiš u Serial Monitoru šta se dešava)
  /*
  Serial.print("Objekat: "); Serial.print(detectedClass);
  Serial.print(" | X: "); Serial.print(detectedX);
  Serial.print(" | Y: "); Serial.print(detectedY);
  Serial.print(" | Azimuth: "); Serial.println(detectedAzimuth);
  */
}
