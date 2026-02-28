from flask import Flask, jsonify, render_template_string
import serial
import re
import time
import sys
from datetime import datetime

app = Flask(__name__)

try:
    ser = serial.Serial('COM8', 9600, timeout=1)
    time.sleep(2)
    ser.reset_input_buffer()
    print("Connected to COM8")
except Exception as e:
    print("Serial Error:", e)
    sys.exit()

temperature = "--"
humidity = "--"
last_updated = "--"

@app.route("/")
def index():
    return render_template_string("""
<!DOCTYPE html>
<html>
<head>
    <title>Temperature Dashboard</title>
    <script src="https://cdn.tailwindcss.com"></script>
    <script>
        async function fetchData() {
            const response = await fetch('/data');
            const data = await response.json();

            if (data.temperature !== "--") {

                const tempValue = parseFloat(data.temperature).toFixed(2);
                const humValue = parseFloat(data.humidity).toFixed(2);

                document.getElementById('temp').innerText = tempValue;
                document.getElementById('hum').innerText = humValue;
                document.getElementById('time').innerText = data.time;

                const tempBox = document.getElementById('temp');

                if (data.temperature >= 30) {
                    tempBox.className = "text-7xl font-bold text-red-500 leading-none tabular-nums inline";
                } else if (data.temperature >= 20) {
                    tempBox.className = "text-7xl font-bold text-yellow-400 leading-none tabular-nums inline";
                } else {
                    tempBox.className = "text-7xl font-bold text-blue-400 leading-none tabular-nums inline";
                }
            }
        }

        setInterval(fetchData, 500);
        window.onload = fetchData;
    </script>
</head>

<body class="bg-gray-900 flex items-center justify-center min-h-screen">

    <div class="bg-gray-800 rounded-3xl shadow-2xl p-14 w-full max-w-xl text-center">

        <h1 class="text-4xl font-semibold text-white mb-14">
            Arduino Temperature Monitor
        </h1>

        <!-- Temperature -->
        <div class="mb-14">
            <p class="text-gray-400 text-sm tracking-widest uppercase mb-5">
                Temperature
            </p>

            <div class="text-center">
                <span id="temp"
                      class="text-7xl font-bold text-yellow-400 leading-none tabular-nums inline">
                    --
                </span>
                <span class="text-4xl text-white leading-none ml-3">
                    °C
                </span>
            </div>
        </div>

        <!-- Humidity -->
        <div class="mb-12">
            <p class="text-gray-400 text-sm tracking-widest uppercase mb-5">
                Humidity
            </p>

            <div class="text-center">
                <span id="hum"
                      class="text-5xl font-semibold text-blue-400 leading-none tabular-nums inline">
                    --
                </span>
                <span class="text-3xl text-white leading-none ml-3">
                    %
                </span>
            </div>
        </div>

        <!-- Time -->
        <div class="text-gray-500 text-sm">
            Last updated:
            <span id="time" class="ml-2 text-gray-300 tabular-nums">
                --
            </span>
        </div>

    </div>

</body>
</html>
    """)

@app.route("/data")
def data():
    global temperature, humidity, last_updated

    try:
        while ser.in_waiting:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            match = re.search(r"Temp:([\d.]+),Hum:([\d.]+)", line)
            if match:
                temperature = float(match.group(1))
                humidity = float(match.group(2))
                last_updated = datetime.now().strftime("%H:%M:%S")
                print("Updated:", temperature, humidity)
    except Exception as e:
        print("Read Error:", e)

    return jsonify({
        "temperature": temperature,
        "humidity": humidity,
        "time": last_updated
    })

if __name__ == "__main__":
    app.run(debug=False, use_reloader=False)