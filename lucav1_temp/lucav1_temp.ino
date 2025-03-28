// 1. Core Libraries (Order Corrected)
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// 2. Define display FIRST
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 3. Undef conflicting DEFAULT before RoboEyes
#undef DEFAULT
#define DEFAULT 0

// 4. Include RoboEyes and immediately fix 'S' conflict
#include <FluxGarage_RoboEyes.h>
#undef S  // Fix conflict with DHT library's parameter name

// 5. Now include other device libraries
#include <DHT.h>
#include <Pushbutton.h>

// 6. Pin Definitions
#define DHT_PIN      D3
#define BUTTON1_PIN  D5
#define BUTTON2_PIN  D6
#define BUTTON3_PIN  D7
#define OLED_SDA     D2
#define OLED_SCL     D1

// 7. Constants
const float COLD_THRESHOLD = 26.0;
const float HOT_THRESHOLD = 31.0;
const uint8_t MOOD_COUNT = 4;
const uint8_t MAX_PARTICLES = 8;

// 8. Structures & Globals
typedef struct {
  float x, y, speed;
} Particle;

roboEyes eyes;
DHT dht(DHT_PIN, DHT11);
Pushbutton btnMood(BUTTON1_PIN), btnFlicker(BUTTON2_PIN), btnTempEffect(BUTTON3_PIN);

Particle snow[MAX_PARTICLES], sweat[MAX_PARTICLES];
bool showSnow = false, showSweat = false, tempEffects = false, flicker = false;
int8_t currentMood = 0;
const int8_t MOODS[MOOD_COUNT] = {DEFAULT, TIRED, ANGRY, HAPPY};

// 9. Particle Functions (same as before)
void initParticles(Particle* p, uint8_t count) {
  for(uint8_t i=0; i<count; i++) {
    p[i].x = random(0, SCREEN_WIDTH);
    p[i].y = random(-SCREEN_HEIGHT, 0);
    p[i].speed = random(10, 30)/10.0f;
  }
}

void updateDrawParticles(Particle* p, uint8_t count, char glyph) {
  for(uint8_t i=0; i<count; i++) {
    p[i].y += p[i].speed;
    if(p[i].y > SCREEN_HEIGHT) {
      p[i].y = random(-SCREEN_HEIGHT, 0);
      p[i].x = random(0, SCREEN_WIDTH);
    }
    display.drawChar(p[i].x, p[i].y, glyph, WHITE, BLACK, 1);
  }
}

// 10. Modified Temperature Handling
void updateEyesForTemperature(float temp) {
  if(temp < COLD_THRESHOLD) {
    float scale = 0.5 + (temp - 15.0) * (0.5 / 13.0);
    scale = constrain(scale, 0.5, 1.0);
    
    // Directly set eye dimensions
    eyes.eyeLwidthNext = eyes.eyeLwidthDefault * scale;
    eyes.eyeRwidthNext = eyes.eyeRwidthDefault * scale;
    eyes.eyeLheightNext = eyes.eyeLheightDefault * scale;
    eyes.eyeRheightNext = eyes.eyeRheightDefault * scale;
    
    showSnow = true;
    showSweat = false;
    eyes.setHFlicker(flicker ? ON : OFF, 1);
  }
  else if(temp > HOT_THRESHOLD) {
    eyes.setMood(TIRED);
    showSnow = false;
    showSweat = true;
  }
  else {
    // Reset to default dimensions
    eyes.eyeLwidthNext = eyes.eyeLwidthDefault;
    eyes.eyeRwidthNext = eyes.eyeRwidthDefault;
    eyes.eyeLheightNext = eyes.eyeLheightDefault;
    eyes.eyeRheightNext = eyes.eyeRheightDefault;
    showSnow = showSweat = false;
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(OLED_SDA, OLED_SCL);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED FAIL"));
    while(1);
  }

  dht.begin();
  eyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 60);
  eyes.close();
  eyes.setPosition(DEFAULT);
  eyes.setAutoblinker(ON, 3, 2); // Auto blinker: on, 3s interval, 2s variation
  eyes.setIdleMode(ON, 3, 1);    // Idle mode: on, 3s interval, 1s variation
  initParticles(snow, MAX_PARTICLES);
  initParticles(sweat, MAX_PARTICLES);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);
}

void loop() {
  // Button handling (same as before)
  if(btnMood.getSingleDebouncedPress()) {
    currentMood = (currentMood + 1) % MOOD_COUNT;
    eyes.setMood(MOODS[currentMood]);
  }

  if(btnFlicker.getSingleDebouncedPress()) {
    flicker = !flicker;
    eyes.setHFlicker(flicker ? ON : OFF, 2);
  }

  if(btnTempEffect.getSingleDebouncedPress()) {
    tempEffects = !tempEffects;
    digitalWrite(LED_BUILTIN, tempEffects ? LOW : HIGH); // LED ON when effect active
    if(!tempEffects) {
      eyes.setMood(DEFAULT);
      updateEyesForTemperature(28.0); // Reset to neutral state
    }
  }

  // Temperature reading (fixed S conflict)
  float temp = dht.readTemperature(false); // Explicit false for Celsius
  if(isnan(temp)) {
    Serial.println(F("DHT ERROR"));
    temp = 29.0;
  }

  if(tempEffects) {
    updateEyesForTemperature(temp);
  }

  // Display update
  display.clearDisplay();
  eyes.update();
  
  if(showSnow) updateDrawParticles(snow, MAX_PARTICLES, '*');
  if(showSweat) updateDrawParticles(sweat, MAX_PARTICLES, 'o');
  
  display.display();
}
