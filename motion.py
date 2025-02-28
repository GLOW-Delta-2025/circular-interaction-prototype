import cv2
import numpy as np
import time
import serial

arduino = serial.Serial(port='COM16', baudrate=9600, timeout=1)
time.sleep(2)
# Initialize the video capture object
cap = cv2.VideoCapture(1)  # Use '0' for the default camera, or replace with a video file path

# Initialize variables for motion detection
ret, frame1 = cap.read()
prev_gray = cv2.cvtColor(frame1, cv2.COLOR_BGR2GRAY)
prev_gray = cv2.GaussianBlur(prev_gray, (21, 21), 0)

motion_counter = 0  # Counter to track the motion duration
max_duration = 10  # Seconds for max motion duration
motion_start_time = None

while cap.isOpened():
    ret, frame2 = cap.read()
    if not ret:
        break

    # Convert to grayscale
    gray = cv2.cvtColor(frame2, cv2.COLOR_BGR2GRAY)

    # Apply Gaussian blur to reduce noise and improve accuracy
    gray = cv2.GaussianBlur(gray, (21, 21), 0)

    # Compute the absolute difference between the current and previous frame
    frame_diff = cv2.absdiff(prev_gray, gray)

    # Threshold the difference to highlight areas of motion
    _, thresh = cv2.threshold(frame_diff, 25, 255, cv2.THRESH_BINARY)

    # Find contours of the moving objects
    contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Check if any significant motion was detected
    motion_detected = False
    for contour in contours:
        if cv2.contourArea(contour) < 500:
            continue  # Ignore small contours (noise)
        motion_detected = True
        (x, y, w, h) = cv2.boundingRect(contour)
        cv2.rectangle(frame2, (x, y), (x + w, y + h), (0, 255, 0), 2)

    # If motion is detected, update motion counter
    if motion_detected:
        if motion_start_time is None:  # Start timing when motion is first detected
            motion_start_time = time.time()
        motion_duration = time.time() - motion_start_time
        motion_counter = min(255, int((motion_duration / max_duration) * 255))
    else:
        # No motion detected, decrease the motion counter gradually
        motion_counter = max(0, motion_counter - 5)  # Decrease by a small value, tweak as needed
        motion_start_time = None  # Reset the motion start time

    # Display the motion intensity value (0-255)
    cv2.putText(frame2, f"Motion Intensity: {motion_counter}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
    print(motion_counter)

    arduino.write((str(motion_counter) + '\n').encode())
    # Show the processed frame
    # cv2.imshow('Motion Detection', frame2)

    # Update the previous frame for the next loop
    prev_gray = gray

    # Break loop if 'q' is pressed
    # if cv2.waitKey(1) & 0xFF == ord('q'):
    #     break

# Release the video capture object and close all windows
cap.release()
cv2.destroyAllWindows()
