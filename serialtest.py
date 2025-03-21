import serial
import time

# Set up serial communication (adjust the port if needed)
ser = serial.Serial("COM8", 115200, timeout=1)  # Change COM3 to your port (e.g., /dev/ttyUSB0 on Linux)
time.sleep(2)  # Give time for Arduino to initialize

def send_and_receive():
    while True:
        user_input = input("Enter message (type 'exit' to quit): ")
        if user_input.lower() == "exit":
            break

        # Format message with $ and #
        message = f"${user_input}#"
        ser.write(message.encode())  # Send data
        print(f"Sent: {message}")

        # Wait for Arduino's response
        response = ser.readline().decode().strip()
        if response:
            print(f"Received: {response}")

# Run the serial communication function
send_and_receive()

# Close the serial connection
ser.close()
