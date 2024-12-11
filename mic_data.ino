#define MIC_PIN A0       // Define the analog pin connected to the microphone
#define SAMPLE_SIZE 100  // Number of samples to store

int amplitudeArray[SAMPLE_SIZE]; // Array to store amplitude readings
int currentIndex = 0;            // Index to keep track of array position
int LQArray [SAMPLE_SIZE];

void setup() {
  Serial.begin(9600); // Start serial communication
  Serial.println("Microphone Amplitude Recorder");

}

void loop() {
  // Record amplitude from the microphone
  int amplitude = analogRead(MIC_PIN);
  Serial.println(amplitude); // Print amplitude to Serial Monitor

  // Store amplitude in the array
  if (currentIndex < SAMPLE_SIZE) {
    amplitudeArray[currentIndex++] = amplitude;
  }

  // If the array is full, calculate and print the lower quartile
  if (currentIndex == SAMPLE_SIZE) {
    Serial.println("Array is full. Calculating lower quartile...");
    int lowerQuartile = calculateLowerQuartile(amplitudeArray, SAMPLE_SIZE);
    Serial.print("Lower Quartile (Q1): ");
    Serial.println(lowerQuartile);

    // Reset index for fresh data collection
    currentIndex = 0;
  }

  delay(100); // Sampling interval
}

// Function to calculate the lower quartile (Q1)
int calculateLowerQuartile(int data[], int size) {
  // Sort the data
  for (int i = 0; i < size - 1; i++) {
    for (int j = i + 1; j < size; j++) {
      if (data[i] > data[j]) {
        int temp = data[i];
        data[i] = data[j];
        data[j] = temp;
      }
    }
  }

  // Calculate the lower quartile
  int lowerQuartileIndex = size / 4;
  Serial.print("Lower Quartile:  ");
  return data[lowerQuartileIndex];
  if (currentIndex < SAMPLE_SIZE) {
    LQArray[currentIndex++] = amplitude;
  }
  memset(amplitudeArray, 0, sizeof(amplitudeArray));

}