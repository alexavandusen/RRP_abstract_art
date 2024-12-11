// ===============================
// Globals
// ===============================
#include <Servo.h>

#define Trig1 4
#define Echo1 5
#define Trig2 6
#define Echo2 7
#define AMicPin A0

Servo servoAggressive;
Servo servoCalm;
Servo servoNeutral;

String currentEmotion = "";

// Emotion tracking
#define MAX_EMOTION_SAMPLES 100 // Array size for emotion tracking
String emotionArray[MAX_EMOTION_SAMPLES];
int emotionIndex = 0;

// Amplitude and distance arrays
#define SAMPLE_SIZE 5
int amplitudeArray[SAMPLE_SIZE];
float distanceArray[SAMPLE_SIZE];
int sampleIndex = 0;

// Pin assignment for servos
#define SERVO_AGGRESSIVE_PIN 11
#define SERVO_CALM_PIN 2
#define SERVO_NEUTRAL_PIN 3

void setup() {
  Serial.begin(115200);

  pinMode(Echo1, INPUT);
  pinMode(Trig1, OUTPUT);
  pinMode(Echo2, INPUT);
  pinMode(Trig2, OUTPUT);
  digitalWrite(Trig1, LOW);
  digitalWrite(Trig2, LOW);

  servoAggressive.attach(SERVO_AGGRESSIVE_PIN);
  servoCalm.attach(SERVO_CALM_PIN);
  servoNeutral.attach(SERVO_NEUTRAL_PIN);

  servoAggressive.write(0); // Initial position
  servoCalm.write(0);       // Initial position
  servoNeutral.write(0);    // Initial position

  Serial.println("Setup complete.");
}

void loop() {
  // Collect amplitude and distance readings
  amplitudeArray[sampleIndex] = analogRead(AMicPin);
  distanceArray[sampleIndex] = measureDistance(Trig1, Echo1);

  sampleIndex++;
  delay(100); // Sampling delay

  // Process data when array is full
  if (sampleIndex == SAMPLE_SIZE) {
    Serial.println("Sample array full. Analyzing data...");

    // Analyze amplitude and distance arrays
    int amplitudeChange = analyzeAmplitudeChange(amplitudeArray, SAMPLE_SIZE);
    float distanceChange = analyzeMaxDistanceChange(distanceArray, SAMPLE_SIZE);

    Serial.print("Amplitude Change: ");
    Serial.println(amplitudeChange);
    Serial.print("Max Distance Change: ");
    Serial.println(distanceChange);

    // Determine emotion
    String newEmotion = determineEmotion(distanceChange, amplitudeChange);
    Serial.print("Determined Emotion: ");
    Serial.println(newEmotion);

    // Store the emotion in the emotion array
    if (emotionIndex < MAX_EMOTION_SAMPLES) {
      emotionArray[emotionIndex++] = newEmotion;
    }

    // Transmit emotion for plotting
    Serial.print("Emotion Log: ");
    for (int i = 0; i < emotionIndex; i++) {
      Serial.print(emotionArray[i]);
      if (i < emotionIndex - 1) Serial.print(", ");
    }
    Serial.println();

    // Perform servo action
    handleEmotion(newEmotion);

    // Reset sample index for new collection
    sampleIndex = 0;
  }
}

// Function to measure distance using ultrasonic sensor
float measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2; // Convert to cm
}

// Function to analyze amplitude change
int analyzeAmplitudeChange(int data[], int size) {
  int minVal = data[0];
  int maxVal = data[0];
  
  for (int i = 1; i < size; i++) {
    if (data[i] < minVal) minVal = data[i];
    if (data[i] > maxVal) maxVal = data[i];
  }

  return maxVal - minVal; // Return the range
}

// Function to analyze max distance change
float analyzeMaxDistanceChange(float data[], int size) {
  float minDistance = data[0];
  float maxDistance = data[0];
  
  for (int i = 1; i < size; i++) {
    if (data[i] < minDistance) minDistance = data[i];
    if (data[i] > maxDistance) maxDistance = data[i];
  }

  return maxDistance - minDistance; // Return the range
}

// Function to determine emotion based on distance and amplitude changes
String determineEmotion(float distanceChange, int amplitudeChange) {
  if (distanceChange > 20 && amplitudeChange >= 3) {
    return "aggressive";
  } else if (distanceChange < 5 && amplitudeChange <= 1) {
    return "calm";
  } else {
    return "neutral";
  }
}

// Function to handle servo action based on emotion
void handleEmotion(String newEmotion) {
  if (newEmotion != currentEmotion) {
    Serial.print("Emotion changed to: ");
    Serial.println(newEmotion);

    // Close previous servo if it was active
    if (currentEmotion == "aggressive") servoAggressive.write(0);
    else if (currentEmotion == "calm") servoCalm.write(0);
    else if (currentEmotion == "neutral") servoNeutral.write(0);

    delay(500); // Allow time for servo to close

    // Open new servo
    if (newEmotion == "aggressive") {
      servoAggressive.write(90);
      Serial.println("Aggressive servo activated.");
    } else if (newEmotion == "calm") {
      servoCalm.write(90);
      Serial.println("Calm servo activated.");
    } else if (newEmotion == "neutral") {
      servoNeutral.write(90);
      Serial.println("Neutral servo activated.");
    }
    
    currentEmotion = newEmotion; // Update current emotion
  } else {
    Serial.println("Emotion unchanged. No servo action.");
  }
}
