// --- PODEŠAVANJA ---
const int LED_PIN = 13;       // LED lampica
const int BUTTON_PIN = 2;     // Dugme za promenu modela (spojiti na GND)
const long BAUD_RATE = 115200; // Mora biti isto kao u Androidu

// Promenljive za prijem
String inputString = "";
bool stringComplete = false;

// Podaci o objektu
String detectedClass = "";
float detectedX = 0.0;
float detectedY = 0.0;
float detectedAzimuth = 0.0;

void setup() {
  Serial.begin(BAUD_RATE);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Aktiviramo interni pull-up otpornik za dugme
  // To znači: kada dugme NIJE pritisnuto, pin je HIGH. Kad se pritisne, ide na LOW.
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
}

void loop() {
  // --- 1. ČITANJE PORUKA OD ANDROIDA ---
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }

  if (stringComplete) {
    parseData(inputString); // Analiziraj šta je stiglo
    inputString = "";
    stringComplete = false;
  }

  // --- 2. SLANJE KOMANDE ANDROIDU (Promena Modela) ---
  // Primer: Ako pritisneš dugme na Pinu 2
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50); // Debounce (zaštita od lažnog pritiska)
    if (digitalRead(BUTTON_PIN) == LOW) {
      
      // OVO JE KOMANDA KOJU TRAŽIŠ:
      // Format: "ime_modela.tflite, ime_labela.txt, confVrednost"
      // Pazi: Fajlovi moraju biti već uvezeni u aplikaciju!
      
      changeModel("model_flase.tflite", "labels.txt", 60);
      
      // Blinkni LED da potvrdiš slanje
      digitalWrite(LED_PIN, LOW); delay(100); digitalWrite(LED_PIN, HIGH);
      delay(1000); // Sačekaj malo da ne pošalje 10 puta odjednom
    }
  }
}

// --- FUNKCIJA ZA SLANJE KOMANDE ---
void changeModel(String model, String labels, int confidence) {
  // Pravimo string: "model.tflite,labels.txt,conf60"
  String cmd = model + "," + labels + ",conf" + String(confidence);
  
  // Šaljemo preko USB-a
  Serial.println(cmd);
}

// --- FUNKCIJA ZA ANALIZU PRIMLJENIH PODATAKA ---
void parseData(String msg) {
  msg.trim();
  if (msg.length() == 0) return;

  // Slučaj: Ništa nije detektovano
  if (msg.indexOf("NODETECT") >= 0) {
    digitalWrite(LED_PIN, LOW);
    return;
  }

  // Slučaj: Stigli su podaci (klasa,x,y,azimut;)
  // Primer: "bottle,0.55,0.42,125;"
  
  int firstComma = msg.indexOf(',');
  int secondComma = msg.indexOf(',', firstComma + 1);
  int thirdComma = msg.indexOf(',', secondComma + 1);
  int endSemi = msg.indexOf(';', thirdComma + 1);

  if (firstComma == -1 || secondComma == -1 || thirdComma == -1) return;

  String classStr = msg.substring(0, firstComma);
  String xStr = msg.substring(firstComma + 1, secondComma);
  String yStr = msg.substring(secondComma + 1, thirdComma);
  String azStr;
  
  if (endSemi != -1) azStr = msg.substring(thirdComma + 1, endSemi);
  else azStr = msg.substring(thirdComma + 1);

  // Konverzija
  detectedClass = classStr;
  detectedX = xStr.toFloat();
  detectedY = yStr.toFloat();
  detectedAzimuth = azStr.toFloat();

  // --- LOGIKA UPRAVLJANJA ---
  // Primer: Reaguj na flašu ako je ispred nas (Azimut oko 0 ili 360)
  
  if (detectedClass == "bottle" || detectedClass == "flasa") {
    digitalWrite(LED_PIN, HIGH); // Vidim flašu!
    
    // Možeš dodati i proveru ugla:
    // if (detectedAzimuth > 350 || detectedAzimuth < 10) { ... }
  }
}
