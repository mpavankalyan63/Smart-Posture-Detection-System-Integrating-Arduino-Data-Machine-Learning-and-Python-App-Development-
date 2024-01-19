import tkinter as tk
from tkinter import ttk
import serial  # For communication with Arduino

# Establish serial communication with Arduino
arduino = serial.Serial('COM3', 9600)  # Replace 'COM3' with the appropriate port and baud rate


# Function to handle button clicks (initiate prediction request)
def button_click(sensor_type):
    sensor_names = {
        "1": "Accelerometer",
        "2": "Gyroscope",
        "3": "Magnetometer"
    }

    update_text_box("Detecting posture using " + sensor_names.get(sensor_type))

    # After 100 ms it will send request to arduino so that text box gets updated by Detecting... text
    root.after(100, lambda: send_request_read_response(sensor_type))


# Function to send prediction request to Arduino
def send_request_read_response(sensor_type):
    # Send a request to the Arduino with the selected sensor type
    arduino.write(sensor_type.encode())

    # Read the response from the Arduino
    prediction = arduino.readline().decode().strip()
    update_text_box(prediction)


# Function to update the 'Detected Postures' display with real-time predictions
def update_text_box(message):
    detected_postures_text.config(state="normal")
    detected_postures_text.delete(0.001, "end")  # Clear previous content
    detected_postures_text.insert("insert", message)


# Create the main application window
root = tk.Tk()
root.title("Posture Detection")

# Create a frame for the buttons (sensor selection)
button_frame = ttk.Frame(root)
button_frame.pack(pady=20)

# Create and customize the buttons with larger font size
style = ttk.Style()
style.configure(
    "Large.TButton",
    width=20,
    padding=(15, 5),
    font=("Arial", 18)
)

# Create buttons for selecting sensor types
accelerometer_button = ttk.Button(button_frame, text="Accelerometer", command=lambda: button_click("1"), style="Large.TButton")
gyroscope_button = ttk.Button(button_frame, text="Gyroscope", command=lambda: button_click("2"), style="Large.TButton")
magnetometer_button = ttk.Button(button_frame, text="Magnetometer", command=lambda: button_click("3"), style="Large.TButton")

# Pack the buttons with increased spacing
accelerometer_button.pack(side="left", padx=20)
gyroscope_button.pack(side="left", padx=20)
magnetometer_button.pack(side="left", padx=20)

# Create a frame for the 'Detected Postures' display
detected_postures_frame = ttk.Frame(root)
detected_postures_frame.pack(side="bottom", pady=20)

# Heading for the 'Detected Postures' display 
detected_postures_heading = ttk.Label(detected_postures_frame, text="Detected Postures", font=("Arial", 18))
detected_postures_heading.pack()

# Create a Text widget to display real-time predictions
detected_postures_text = tk.Text(detected_postures_frame, height=1, width=40)
detected_postures_text.config(state="disabled", font=("Arial", 18))
detected_postures_text.pack()

# Starts Tkinter loop
root.mainloop()