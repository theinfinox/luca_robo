# luca_robo
 The desktop pet project
---

# 🤖✨ RoboEyes with Moods & Weather Effects

A cute OLED robot face that reacts to temperature and button presses! Features animated eyes with different moods, temperature-sensitive effects (snowflakes when cold, sweat drops when hot), and interactive controls.

![RoboEyes Demo](https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExcDF5Z2R5b3BmY2Z4d3R3d3V6eGZ2eGJ4Y2RqZzV1ZzB1bmZ6eGJ6ZyZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/xT5LMHxhOfscxPfIfm/giphy.gif)

## 📌 Pin Mapping
| Component      | Pin  |
|---------------|------|
| OLED SDA      | D2   |
| OLED SCL      | D1   |
| DHT11 Sensor  | D3   |
| Mood Button   | D5   |
| Flicker Toggle| D6   |
| Temp Effects  | D7   |

## 📚 Required Libraries
1. `Wire.h` (I2C communication)
2. `Adafruit_GFX.h` (Graphics core)
3. `Adafruit_SSD1306.h` (OLED display)
4. `FluxGarage_RoboEyes.h` (Animated eyes)
5. `DHT.h` (Temperature sensor)
6. `Pushbutton.h` (Debounced buttons)

## 🎮 Features
- **4 Expressive Moods**: 
  - 😊 Default | 😪 Tired | 😠 Angry | 😃 Happy
- **Temperature Reactions**:
  - ❄️ Snowflakes below 26°C
  - 💧 Sweat drops above 31°C
  - 👀 Eye size changes with cold
- **Interactive Controls**:
  - Button 1: Cycle through moods
  - Button 2: Toggle eye flickering
  - Button 3: Enable/disable temp effects

## 🛠️ Setup Guide
1. Connect components as per pin mapping
2. Install all required libraries
3. Upload the sketch
4. Watch your robot come to life!

## 🌡️ Temperature Thresholds
| Effect       | Temperature |
|-------------|------------|
| Snow Mode   | < 26°C     |
| Normal Mode | 26-31°C    |
| Sweat Mode  | > 31°C     |

Pro Tip: The LED lights up when temperature effects are active! 💡

---

Made with ❤️ by @theinfinox | GPLv3
