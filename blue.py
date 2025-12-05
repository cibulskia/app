import serial
import time
import threading
import sys

# --- PODEŠAVANJA ---
PORT = 'COM4'       # Proveri svoj port
BAUD_RATE = 9600

# Globalna promenljiva za kontrolu gašenja niti
running = True

def read_from_serial(ser):
    """Ova funkcija trči u pozadini i sluša šta telefon šalje."""
    global running
    print("[INFO] Slušanje aktivno...\n")
    
    while running:
        if ser.in_waiting > 0:
            try:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                if not line:
                    continue

                # Filtriranje i parsiranje (identično kao pre)
                if "NODETECT" in line:
                    parts = line.split(',')
                    if len(parts) >= 4:
                        azimuth = float(parts[3].replace(';', ''))
                        # \r vraća kursor na početak reda (lepši ispis)
                        sys.stdout.write(f"\rStatus: Skeniram... | Azimut: {azimuth:.0f}°   ")
                        sys.stdout.flush()
                else:
                    # Detekcija objekta
                    sys.stdout.write("\n") # Novi red da ne pregazi prethodni ispis
                    print(f"[PRIJEM] {line}")
                    print("-" * 30)
            
            except Exception as e:
                print(f"\n[Greška pri čitanju] {e}")
        
        time.sleep(0.01)

def main():
    global running
    try:
        print(f"Povezivanje na {PORT}...")
        ser = serial.Serial(PORT, BAUD_RATE, timeout=1)
        print("Uspešno povezano!")
        time.sleep(2)
        ser.reset_input_buffer()

        # Pokrećemo nit za slušanje (background thread)
        thread = threading.Thread(target=read_from_serial, args=(ser,))
        thread.daemon = True # Gasi se kad se ugasi glavni program
        thread.start()

        print("="*50)
        print("DINAMIČKA KONTROLA MODELA")
        print("Unesi komandu bilo kada u formatu:")
        print("fajl_modela, fajl_klasa, confVrednost")
        print("Primer: model2.tflite, classes2.txt, conf60")
        print("Za izlaz ukucaj: exit")
        print("="*50)

        while True:
            # Glavna petlja čeka tvoj unos
            command = input() # Ovde program čeka dok ne pritisneš Enter
            
            if command.lower() == 'exit':
                print("Gašenje...")
                running = False
                break
            
            if command.strip():
                print(f"[SLANJE] >> {command}")
                ser.write(command.encode('utf-8'))
            
            time.sleep(0.5)

    except serial.SerialException as e:
        print(f"\n[CRITICAL] Greška sa portom {PORT}: {e}")
    except KeyboardInterrupt:
        print("\nPrekinuto.")
        running = False
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()

if __name__ == "__main__":
    main()
