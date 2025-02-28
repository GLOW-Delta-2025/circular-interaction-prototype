import cv2
import numpy as np

# Initialize the video capture object
cap = cv2.VideoCapture(0)  # Use '0' for the default camera, or replace with a video file path

# Initialize variables for motion detection
ret, frame1 = cap.read()
prev_gray = cv2.cvtColor(frame1, cv2.COLOR_BGR2GRAY)
prev_gray = cv2.GaussianBlur(prev_gray, (21, 21), 0)

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

    # Draw bounding boxes around moving objects
    for contour in contours:
        if cv2.contourArea(contour) < 500:
            continue  # Ignore small contours (noise)

        (x, y, w, h) = cv2.boundingRect(contour)
        cv2.rectangle(frame2, (x, y), (x + w, y + h), (0, 255, 0), 2)

    # Show the processed frame
    cv2.imshow('Motion Detection', frame2)

    # Update the previous frame for the next loop
    prev_gray = gray

    # Break loop if 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the video capture object and close all windows
cap.release()
cv2.destroyAllWindows()
