import processing.serial.*;  // Import the Serial library for communication

Serial myPort;               // The Serial port
String incomingData = "";    // To store incoming data from Arduino
String[] emotions = new String[100];  // Array to store emotion data (adjust size as needed)
int emotionIndex = 0;        // Current index in the emotions array

void setup() {
  size(800, 400);            // Set up the canvas size
  background(255);           // Set background color to white
  println(Serial.list());    // List all available serial ports for debugging
  myPort = new Serial(this, Serial.list()[7], 115200); // Open the first available port
  myPort.bufferUntil('\n');  // Trigger serialEvent() when a newline is received
  println("Setup complete. Waiting for data...");
}

void draw() {
  background(255);           // Clear the canvas
  textAlign(CENTER);
  textSize(16);
  fill(0);
  text("Emotion Over Time", width / 2, 20);

  // Draw the emotion graph
  for (int i = 0; i < emotionIndex; i++) {
    int x = (int) map(i, 0, emotions.length - 1, 50, width - 50);
    int barHeight = 100;
    int y = height / 2;

    // Set color based on emotion
    if (emotions[i].equals("aggressive")) {
      fill(255, 0, 0);  // Red for aggressive
    } else if (emotions[i].equals("calm")) {
      fill(0, 0, 255);  // Blue for calm
    } else if (emotions[i].equals("neutral")) {
      fill(200);        // White/Gray for neutral
    } else {
      fill(0);          // Default to black for unknown
    }

    // Draw the bar
    rect(x, y - barHeight / 2, 10, barHeight);
  }
}

void serialEvent(Serial myPort) {
  // Read the incoming string
  incomingData = myPort.readStringUntil('\n');
  if (incomingData != null) {
    incomingData = trim(incomingData); // Remove whitespace

    // Parse the emotion from the line (e.g., "Determined Emotion: aggressive")
    if (incomingData.startsWith("Determined Emotion:")) {
      String emotion = incomingData.split(": ")[1]; // Extract emotion
      println("Received emotion: " + emotion);

      // Add the emotion to the array
      if (emotionIndex < emotions.length) {
        emotions[emotionIndex++] = emotion;
      } else {
        // Shift the array to make room for new data
        for (int i = 0; i < emotions.length - 1; i++) {
          emotions[i] = emotions[i + 1];
        }
        emotions[emotions.length - 1] = emotion;
      }
    }
  }
}
