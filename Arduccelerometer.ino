#include <Wire.h>

// Display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Contains 
#include "logo.h"

// Sensor
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Display dimensions
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Square logo dimensions
#define LOGO_WIDTH  64
#define LOGO_HEIGHT 64

// Wide logo dimensions
#define LOGO_W_WIDTH 128
#define LOGO_W_HEIGHT 64

// Accelerometer and Gyro
Adafruit_MPU6050 mpu;

void setup(void) {
  Serial.begin(115200);
 
  // Starting communication with the display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed!"));
    for(;;); // Don't proceed, loop forever
  }

  // Display startup
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  
  // Show initial logos for the start-up 'routine' - 4s total
  drawInitialLogo();

  // Try to initialize
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip!");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // Settings
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_10_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(1000);
}

void loop() {
  // Get new sensor events with the readings
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Printing the values to the screen
  display.clearDisplay();
  const float gBase = 10.1;
  const float maxCarAcc = 1;
  int nbOfLines = (a.acceleration.z * 64) / gBase / maxCarAcc;  
  int x = 0;
  int y;
  for (int i = 0; i < nbOfLines; ++i, x += 2) {
    y = 2 + i * i / 64;
    display.drawLine(x, 62, x, 62 - y, SSD1306_WHITE);
  }
  display.setCursor(0, 0);
  display.print(a.acceleration.z / gBase);
  display.display();

  delay(50);
}

// Draws the logos
void drawInitialLogo(void) {
  // TUCN Logo
  display.clearDisplay();
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    tucn_logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(2000);

  // TUCN Text Logo
  display.clearDisplay();
  display.drawBitmap(
    (display.width()  - LOGO_W_WIDTH ) / 2,
    (display.height() - LOGO_W_HEIGHT) / 2,
    text_logo_bmp, LOGO_W_WIDTH, LOGO_W_HEIGHT, 1);
  display.display();
  delay(2000);
}