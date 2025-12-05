📱 Detect — Android Client Application

## Description

Detect is an Android application designed as a client component of a larger object detection system.
The application serves as an interface between an Android phone and:

- Arduino devices (via USB)
- Computer (via Bluetooth communication)

The application is not standalone and does not function without the accompanying scripts and processing system on a computer or microcontroller.

---

## ⚙️ How the System Works

The Android application connects to:

- Arduino device via USB cable
- or a computer via Bluetooth

In the phone’s Downloads folder, the following files must be placed:

- model files YOLO8 with .tflite extension
- files with defined object classes (text file with .txt extension)

The user grants the application permission to access these files on the phone.

Upon receiving a "detect" command from the computer or Arduino, the application:

- processes the data
- returns the recognized object name
- returns the object coordinates on the phone screen

The result is sent back via:

- USB or Bluetooth to the computer / Arduino

---

## 📂 Permissions and Privacy

The application is designed to be completely offline.

✅ It does not use:

- GPS
- Mobile network (GSM)
- Wi-Fi
- Internet connection
- Video recording
- Audio recording

✅ It only uses:

- USB communication
- Bluetooth communication
- Files manually selected by the user

❗ The application does not collect, store, or send personal data.

---

## 📥 Download

The latest APK version can be downloaded here:

👉 [https://github.com/cibulskia/app/releases/tag/v1.0](https://github.com/cibulskia/app/releases/tag/v1.0)

### Installation:

1. Download the APK to your phone
2. Allow installation from unknown sources if needed
3. Install the application

---

## 🔌 Connection

### USB (Arduino)

1. Connect the phone to the Arduino device via USB cable
2. Run the script on the Arduino
3. Launch the application on the phone

### Bluetooth (Computer)

1. Pair the phone with the computer
2. Run the script on the PC
3. Launch the application and select the Bluetooth device

---

## 📜 Limitations

- The application is meaningless without external communication
- It cannot operate without the model and class files
- It is not intended for end users without technical setup

---

## 🛡 Security Note

This application only accesses files manually selected by the user.
It does not automatically scan the system or behave in a hidden manner.

---

## 📄 License

MIT License

