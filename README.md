# 📱 Detect — Android Client Application

## Description
**Detect** is an Android application designed as a client component of a larger object detection system. The application serves as an interface between an Android phone and:

- Arduino devices (via USB)
- Computer (via Bluetooth communication)

> ⚠️ The application is **not standalone** and does not function without the accompanying scripts and processing system on a computer or microcontroller.

The main purpose of this application is to perform **object detection** and return the coordinates of detected objects on the phone screen.  
As an additional feature, it can also provide **azimuth information for drones**, enabling navigation and orientation without additional hardware.  

This design allows the system to use an **Android device as a low-cost substitute for GPU-enabled platforms** (like NVIDIA Jetson) for object detection. The actual robot/drone control logic runs on a microcontroller or CPU-based system, which does **not require GPU capabilities**, making the setup cost-effective and accessible.

---

## ⚙️ How the System Works
The Android application connects to:

- Arduino device via USB cable  
- Computer via Bluetooth

Files required in the phone’s **Downloads** folder:

- YOLO8 model files (`.tflite` extension)  
- Object class definitions (`.txt` files)

The user grants the application permission to access these files on the phone.

Upon receiving a "detect" command from the computer or Arduino, the application:

- Processes the data locally on the Android device using the CPU  
- Returns the recognized object name  
- Returns the object coordinates on the phone screen  
- Optionally, provides azimuth data for drones

The result is sent back via:

- USB or Bluetooth to the computer / Arduino

> ⚡ **Key design note:** The Android device is used **exclusively for object detection and coordinate calculation**, avoiding the need for expensive GPU platforms. The controlling processor handles all navigation, motor control, or other robot/drone functions.

---

## 🏆 Key Advantage: High-Resolution Detection
Unlike typical mobile detection systems that rely on HD video, **Detect** works with **high-resolution photographs (e.g., 6000x8000 px)**.  
- Models like **YOLO8n** or **YOLO8s** are sufficient for stable detection even at these high resolutions.  
- This approach allows **detecting objects from much greater distances** compared to using HD video with lighter models.  
- By processing a single high-res image per command instead of streaming video, the system remains **efficient on CPU** while maintaining **accuracy and reliability**, making it ideal for robotics and drone applications.  

---

## 📂 Permissions and Privacy
The application is designed to be completely offline.

**It does NOT use:**

- GPS  
- Mobile network (GSM)  
- Wi-Fi  
- Internet connection  
- Video recording  
- Audio recording  

**It only uses:**

- USB communication  
- Bluetooth communication  
- Files manually selected by the user  

> ❗ The application does **not** collect, store, or send personal data.

---

## 📥 Download
The latest APK version can be downloaded here:

[👉 Download APK v1.0](https://github.com/cibulskia/app/releases/tag/v1.0)

**Installation:**

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
- Cannot operate without the model and class files  
- Not intended for end users without technical setup  
- Azimuth functionality is only relevant when used with drones  
- CPU-only detection may be slower than GPU-based platforms, but is sufficient for real-time operation in most robotics applications

---

## 🛡 Security Note
This application only accesses files **manually selected by the user**. It does not automatically scan the system or behave in a hidden manner.

---

## 📄 License
MIT License
