import serial
import keyboard
import time

AUTHORIZED_UIDS = ["6DC4C721", "27592D66"]

PORT = "COM8"
BAUD = 9600

ser = serial.Serial(PORT, BAUD)

print("Waiting for RFID...")

while True:
    uid = ser.readline().decode().strip().upper()

    if uid:
        print("Scanned:", uid)

        if uid in AUTHORIZED_UIDS:
            print("Input Success")

            time.sleep(1)

            keyboard.write("Hello World")
            keyboard.press_and_release("enter")

        else:
            print("Input Failed")  