import matplotlib.pyplot as plt
from reportlab.platypus import SimpleDocTemplate, Image, Paragraph, Spacer
from reportlab.lib.styles import getSampleStyleSheet

data = []

with open("ecg_data.txt", "r") as f:
    for line in f:
        try:
            data.append(int(line.strip()))
        except:
            pass

print("Samples loaded:", len(data))

# Create ECG plot
plt.figure(figsize=(10,4))
plt.plot(data)
plt.title("ECG Signal")
plt.xlabel("Sample Number")
plt.ylabel("Amplitude")
plt.grid(True)

plt.savefig("ecg_plot.png")
plt.close()

print("Graph saved as ecg_plot.png")

# Build PDF
styles = getSampleStyleSheet()

pdf = SimpleDocTemplate("ecg_report.pdf")

elements = []

elements.append(Paragraph("ECG Monitoring Report", styles["Title"]))
elements.append(Spacer(1,20))

elements.append(Paragraph("Source: Arduino ECG sensor", styles["Normal"]))
elements.append(Spacer(1,20))

img = Image("ecg_plot.png", width=500, height=200)

elements.append(img)

pdf.build(elements)

print("PDF report created: ecg_report.pdf")