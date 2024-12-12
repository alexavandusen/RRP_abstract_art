// Pin definitions
const int TRIG_PIN = 4; // Trigger pin for sonar
const int ECHO_PIN = 5; // Echo pin for sonar
const int SAMPLE_SIZE = 256; // Number of samples to record
const int CATEGORIES = 3; // Number of categories: stationary, moving, fast

// Arrays to store training data
float storedVariations[CATEGORIES] = {0.0, 0.0, 0.0}; // Average variations for categories
const char* categoryLabels[CATEGORIES] = {"stationary", "moving", "fast"};

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Manually define average variations for training (or collect real training data)
  storedVariations[0] = 0.5;   // Stationary
  storedVariations[1] = 5.0;   // Moving
  storedVariations[2] = 15.0;  // Fast
}

void loop() {
  // Record a snippet of distance data
  float distances[SAMPLE_SIZE];
  for (int i = 0; i < SAMPLE_SIZE; i++) {
    distances[i] = measureDistance();
    delay(50); // Adjust sampling rate as needed
  }

  // Calculate variation in the recorded data
  float variation = calculateVariation(distances, SAMPLE_SIZE);

  // Categorize the snippet based on the variation
  const char* category = categorizeSnippet(variation);

  // Output results
  Serial.print("Variation: ");
  Serial.println(variation);
  Serial.print("Category: ");
  Serial.println(category);

  delay(2000); // Wait before the next recording
}

// Function to measure distance using sonar
float measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2; // Convert to centimeters
}

// Function to calculate variation in distance
float calculateVariation(float* distances, int size) {
  float sum = 0.0;
  float sumSquares = 0.0;

  // Compute mean
  for (int i = 0; i < size; i++) {
    sum += distances[i];
  }
  float mean = sum / size;

  // Compute variance
  for (int i = 0; i < size; i++) {
    sumSquares += pow(distances[i] - mean, 2);
  }
  float variance = sumSquares / size;

  // Variation is the square root of variance (standard deviation)
  return sqrt(variance);
}

// Function to categorize snippet based on variation
const char* categorizeSnippet(float variation) {
  float minDifference = 999999; // Large initial value
  int closestCategory = -1;

  // Compare variation to stored category averages
  for (int i = 0; i < CATEGORIES; i++) {
    float difference = abs(variation - storedVariations[i]);
    if (difference < minDifference) {
      minDifference = difference;
      closestCategory = i;
    }
  }

  // Return the label of the closest category
  return categoryLabels[closestCategory];
}
