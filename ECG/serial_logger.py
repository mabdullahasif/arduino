import serial
import time

PORT = "COM3"        # change if needed
BAUD_RATE = 115200

ser = serial.Serial(PORT, BAUD_RATE, timeout=1)

file = open("ecg_data.txt", "w")

print("ECG recording started")
print("Press CTRL+C to stop")

try:
    while True:
        line = ser.readline().decode("utf-8").strip()

        if line:
            print(line)
            file.write(line + "\n")

except KeyboardInterrupt:
    print("Recording stopped")

file.close()
ser.close()