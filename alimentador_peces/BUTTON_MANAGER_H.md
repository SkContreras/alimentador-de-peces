# 🎮 **BUTTON_MANAGER.H - GESTIÓN DE BOTONES**

## 🎯 **PROPÓSITO**
Maneja todos los botones del sistema con debounce, pulsaciones largas y repetición.

## 📋 **ESTRUCTURA DE LA CLASE**

```cpp
class ButtonManager {
private:
  // Estados de botones
  bool buttonStates[4];
  bool lastButtonStates[4];
  unsigned long lastPressTime[4];
  unsigned long lastReleaseTime[4];
  bool longPressTriggered[4];
  unsigned long repeatTime[4];
  
  // Pines de botones
  int buttonPins[4];
  
  // Métodos privados
  void updateButton(int buttonIndex);
  bool isButtonPressed(int buttonIndex);
  bool isButtonReleased(int buttonIndex);
  bool isLongPress(int buttonIndex);
  bool isRepeating(int buttonIndex);
  
public:
  // Constructor
  ButtonManager();
  
  // Inicialización
  void begin();
  
  // Detección de pulsaciones
  bool selectPressed();
  bool selectLongPressed();
  bool upPressed();
  bool upRepeating();
  bool downPressed();
  bool downRepeating();
  bool confirmPressed();
  
  // Feedback sonoro
  void beep();
  void confirmBeep();
  void errorBeep();
  
  // Actualización
  void update();
};
```

## 🔧 **FUNCIONES PRINCIPALES**

### **🚀 Inicialización:**
```cpp
void begin() {
  // Configurar pines como entrada con pull-up
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    buttonStates[i] = HIGH;
    lastButtonStates[i] = HIGH;
  }
}
```

### **🔄 Actualización:**
```cpp
void update() {
  for (int i = 0; i < 4; i++) {
    updateButton(i);
  }
}
```

### **🔍 Detección de Pulsaciones:**
```cpp
bool selectPressed() {
  return isButtonPressed(0);
}

bool selectLongPressed() {
  return isLongPress(0);
}

bool upPressed() {
  return isButtonPressed(1);
}

bool upRepeating() {
  return isRepeating(1);
}
```

### **🔊 Feedback Sonoro:**
```cpp
void beep() {
  // Beep corto
  digitalWrite(BUZZER_PIN, HIGH);
  delay(50);
  digitalWrite(BUZZER_PIN, LOW);
}

void confirmBeep() {
  // Beep de confirmación
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);
  delay(50);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);
}
```

## 🔧 **CÓMO AJUSTAR**

### **⏱️ Cambiar Tiempos de Debounce:**
```cpp
// En config.h:
const unsigned long BUTTON_DEBOUNCE_TIME = 100;  // Aumentar a 100ms
const unsigned long BUTTON_LONG_PRESS_TIME = 1000; // Aumentar a 1 segundo
```

### **🔄 Ajustar Repetición:**
```cpp
// En config.h:
const unsigned long BUTTON_REPEAT_DELAY = 300;     // Delay más largo
const unsigned long BUTTON_REPEAT_INTERVAL = 50;   // Repetición más rápida
```

### **🔌 Cambiar Pines:**
```cpp
// En config.h:
const int BUTTON_SELECT_PIN = 10;
const int BUTTON_UP_PIN = 11;
const int BUTTON_DOWN_PIN = 12;
const int BUTTON_CONFIRM_PIN = 13;
```

### **🔊 Personalizar Beeps:**
```cpp
void customBeep(int duration, int repetitions) {
  for (int i = 0; i < repetitions; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
    delay(duration);
  }
}
```

## 📝 **EXPLICACIÓN DE MÉTODOS**

### **🔍 Detección:**
- **selectPressed()**: Detecta pulsación corta del botón SELECT
- **selectLongPressed()**: Detecta pulsación larga del botón SELECT
- **upPressed()**: Detecta pulsación del botón UP
- **upRepeating()**: Detecta repetición del botón UP
- **downPressed()**: Detecta pulsación del botón DOWN
- **downRepeating()**: Detecta repetición del botón DOWN
- **confirmPressed()**: Detecta pulsación del botón CONFIRM

### **🔊 Sonido:**
- **beep()**: Beep corto
- **confirmBeep()**: Beep de confirmación (doble)
- **errorBeep()**: Beep de error (triple)

### **🔄 Control:**
- **update()**: Actualiza estado de todos los botones
- **begin()**: Inicializa pines y estados

## 🎮 **ÍNDICES DE BOTONES**

```cpp
// Orden de botones en el array:
0 = SELECT
1 = UP
2 = DOWN
3 = CONFIRM
```

## ⚙️ **CONFIGURACIÓN AVANZADA**

### **🎯 Sensibilidad:**
```cpp
// Ajustar sensibilidad de pulsación larga:
const unsigned long BUTTON_LONG_PRESS_TIME = 800;  // 800ms
```

### **🔄 Velocidad de Repetición:**
```cpp
// Repetición más rápida:
const unsigned long BUTTON_REPEAT_DELAY = 150;     // 150ms delay
const unsigned long BUTTON_REPEAT_INTERVAL = 80;   // 80ms intervalo
```

### **🔊 Volumen de Beep:**
```cpp
// Beep más largo:
void beep() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);  // Aumentar duración
  digitalWrite(BUZZER_PIN, LOW);
}
```

---

**📅 Fecha**: Diciembre 2024  
**🔧 Versión**: 3.8  
**✅ Estado**: Gestión completa de botones
