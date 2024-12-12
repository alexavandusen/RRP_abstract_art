// Pin definitions
const int MIC_PIN = A0; // Microphone analog pin
const int SAMPLE_SIZE = 256; // Number of samples to record
const int CATEGORIES = 3; // Number of categories: calm, neutral, expressive

// Arrays to store training data
float storedVariations[CATEGORIES] = {0.0, 0.0, 0.0}; // Average variations for categories
const char* categoryLabels[CATEGORIES] = {"calm", "neutral", "expressive"};

void setup() {
  Serial.begin(115200);
  pinMode(MIC_PIN, INPUT);

  // Manually define average variations for training (or collect real training data)
  storedVariations[0] = 5.0;   // Calm
  storedVariations[1] = 15.0;  // Neutral
  storedVariations[2] = 30.0;  // Expressive
}

void loop() {
  // Record a snippet of amplitude data
  int amplitudes[SAMPLE_SIZE];
  for (int i = 0; i < SAMPLE_SIZE; i++) {
    amplitudes[i] = analogRead(MIC_PIN);
    delayMicroseconds(500); // Adjust sampling rate as needed
  }

  // Calculate variation in the recorded data
  float variation = calculateVariation(amplitudes, SAMPLE_SIZE);

  // Categorize the snippet based on the variation
  const char* category = categorizeSnippet(variation);

  // Output results
  Serial.print("Variation: ");
  Serial.println(variation);
  Serial.print("Category: ");
  Serial.println(category);

  delay(2000); // Wait before the next recording
}

// Function to calculate variation in amplitude
float calculateVariation(int* amplitudes, int size) {
  float sum = 0.0;
  float sumSquares = 0.0;

  // Compute mean
  for (int i = 0; i < size; i++) {
    sum += amplitudes[i];
  }
  float mean = sum / size;

  // Compute variance
  for (int i = 0; i < size; i++) {
    sumSquares += pow(amplitudes[i] - mean, 2);
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
