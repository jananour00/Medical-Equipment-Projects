import cv2
import mediapipe as mp
import numpy as np
import time
from collections import deque

# Initialize MediaPipe Pose
mp_pose = mp.solutions.pose
pose = mp_pose.Pose(static_image_mode=False, min_detection_confidence=0.7, min_tracking_confidence=0.7)

# Initialize video capture
cap = cv2.VideoCapture(0)

# Configuration parameters
MOVEMENT_THRESHOLD = 15  # pixels for movement detection
ELBOW_ANGLE_THRESHOLD = 100  # Warning if angle < 110°
SLEEP_PRESSURE_THRESHOLD = 0.25  # ratio for compression detection
STRAIGHTNESS_THRESHOLD = 0.75  # cosine similarity for straight arm
MOVEMENT_AVERAGE_WINDOW = 5  # frames to average movement
LEAN_THRESHOLD = 0.15  # ratio of shoulder difference to frame height for lean detection

# State variables
prev_wrist_pos = None
movement_history = deque(maxlen=MOVEMENT_AVERAGE_WINDOW)
alert_messages = []

def get_landmark_coords(landmark, frame_shape):
    """Convert normalized landmark to pixel coordinates"""
    height, width = frame_shape
    return int(landmark.x * width), int(landmark.y * height)

def calculate_angle(a, b, c):
    """Calculate angle between three points in degrees"""
    a, b, c = np.array(a), np.array(b), np.array(c)
    ba = a - b
    bc = c - b
    cosine_angle = np.dot(ba, bc) / (np.linalg.norm(ba) * np.linalg.norm(bc))
    angle = np.degrees(np.arccos(cosine_angle))
    return angle

def is_arm_straight(shoulder, elbow, wrist):
    """Check if arm is straight using vector similarity"""
    upper_arm_vec = np.array(elbow) - np.array(shoulder)
    lower_arm_vec = np.array(wrist) - np.array(elbow)
    cosine_sim = np.dot(upper_arm_vec, lower_arm_vec) / (np.linalg.norm(upper_arm_vec) * np.linalg.norm(lower_arm_vec))
    return cosine_sim > STRAIGHTNESS_THRESHOLD

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break

    # Flip and process frame
    frame = cv2.flip(frame, 1)
    h, w = frame.shape[:2]
    image_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    results = pose.process(image_rgb)

    alert_messages = []  # Reset alerts each frame

    if results.pose_landmarks:
        landmarks = results.pose_landmarks.landmark
        
        try:
            # Get landmarks for right arm and left shoulder
            r_shoulder = get_landmark_coords(landmarks[mp_pose.PoseLandmark.RIGHT_SHOULDER], (h, w))
            r_elbow = get_landmark_coords(landmarks[mp_pose.PoseLandmark.RIGHT_ELBOW], (h, w))
            r_wrist = get_landmark_coords(landmarks[mp_pose.PoseLandmark.RIGHT_WRIST], (h, w))
            l_shoulder = get_landmark_coords(landmarks[mp_pose.PoseLandmark.LEFT_SHOULDER], (h, w))
        except:
            continue

        # Draw right arm connections
        cv2.line(frame, r_shoulder, r_elbow, (255, 255, 255), 2)
        cv2.line(frame, r_elbow, r_wrist, (255, 255, 255), 2)
        
        # Draw left shoulder point (blue)
        cv2.circle(frame, l_shoulder, 8, (255, 0, 0), -1)
        
        # Calculate metrics
        elbow_angle = calculate_angle(r_shoulder, r_elbow, r_wrist)
        vertical_distance = abs(r_wrist[1] - r_shoulder[1])
        relative_vertical = vertical_distance / h
        
        # Calculate shoulder positions for lean detection
        shoulder_diff = r_shoulder[1] - l_shoulder[1]  # Positive if right shoulder is lower (leaning left)
        relative_shoulder_diff = abs(shoulder_diff) / h
        
        # Movement detection (continuous warning)
        if prev_wrist_pos:
            movement = np.linalg.norm(np.array(r_wrist) - np.array(prev_wrist_pos))
            movement_history.append(movement)
            avg_movement = sum(movement_history) / len(movement_history)
            
            if avg_movement > MOVEMENT_THRESHOLD:
                alert_messages.append("WARNING: HEMODIALYSIS ARM MOVING!")
        
        # Arm angle detection (continuous warning)
        if elbow_angle < ELBOW_ANGLE_THRESHOLD:
            alert_messages.append(f"WARNING: ARM BENT ({int(elbow_angle)}° < {ELBOW_ANGLE_THRESHOLD}°)")
        
        # Arm straightness check (continuous warning)
        if not is_arm_straight(r_shoulder, r_elbow, r_wrist):
            alert_messages.append("WARNING: KEEP ARM STRAIGHT!")
        
        # Left lean detection (continuous warning)
        if shoulder_diff > h * LEAN_THRESHOLD:  # Positive means left side is higher (leaning left)
            alert_messages.append("WARNING: LEANING ON LEFT SIDE DETECTED!")
        
        prev_wrist_pos = r_wrist
        
        # Draw right arm landmarks
        cv2.circle(frame, r_shoulder, 8, (0, 255, 0), -1)  # Shoulder (green)
        cv2.circle(frame, r_elbow, 8, (0, 165, 255), -1)    # Elbow (orange)
        cv2.circle(frame, r_wrist, 8, (0, 0, 255), -1)      # Wrist (red)
        
        # Display angle
        cv2.putText(frame, f"Angle: {int(elbow_angle)}°", 
                   (r_elbow[0] + 20, r_elbow[1]), 
                   cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1, cv2.LINE_AA)
        
        # Display shoulder difference
        cv2.putText(frame, f"Shoulder Diff: {int(shoulder_diff)}px", 
                   (20, h - 70), 
                   cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1, cv2.LINE_AA)
    
    # Display alerts (more visible)
    alert_color = (0, 0, 255)  # Red
    for i, msg in enumerate(alert_messages):
        cv2.putText(frame, msg, (20, 40 + i*30), 
                   cv2.FONT_HERSHEY_SIMPLEX, 0.8, alert_color, 2, cv2.LINE_AA)
    
    # Status indicator
    status = "WARNING!" if alert_messages else "NORMAL"
    status_color = (0, 0, 255) if alert_messages else (0, 255, 0)
    cv2.putText(frame, f"Status: {status}", (20, h - 40), 
               cv2.FONT_HERSHEY_SIMPLEX, 0.7, status_color, 2, cv2.LINE_AA)
    
    cv2.imshow("Dialysis Arm Monitor", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()