# ⚡ **RELAY_CONTROLLER.H - CONTROL RELÉ**

## 🎯 **PROPÓSITO**
Controla el relé para el motor alimentador y el LED de estado.

## 📋 **ESTRUCTURA DE LA CLASE**

```cpp
class RelayController {
private:
  unsigned long feedStartTime;
  bool isFeeding;
  
public:
  // Constructor
  RelayController();
  
  // Inicialización
  void begin();
  
  // Control de alimentación
  void startFeeding(int scheduleNumber = 0);
  void stopFeeding();
  void emergencyStop();
  
  // Estado
  bool isFeedingActive();
  int getRemainingFeedTime();
  bool getRelayState();
  
  // Control directo
  void setRelayState(bool state);
  void setLEDState(bool state);
  
  // Actualización
  void update();
};
```

## 🔧 **FUNCIONES PRINCIPALES**

### **🚀 Inicialización:**
```cpp
void begin() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  
  isFeeding = false;
}
```

### **⚡ Iniciar Alimentación:**
```cpp
void startFeeding(int scheduleNumber = 0) {
  if (isFeeding) return;
  
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  
  feedStartTime = millis();
  isFeeding = true;
}
```

### **🛑 Detener Alimentación:**
```cpp
void stopFeeding() {
  if (!isFeeding) return;
  
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  
  isFeeding = false;
}
```

### **🚨 Parada de Emergencia:**
```cpp
void emergencyStop() {
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  
  isFeeding = false;
  feedStartTime = 0;
}
```

### **🔄 Actualización:**
```cpp
void update() {
  if (isFeeding && (millis() - feedStartTime >= FEED_DURATION * 1000)) {
    stopFeeding();
  }
}
```

### **📊 Estado:**
```cpp
bool isFeedingActive() {
  return isFeeding;
}

int getRemainingFeedTime() {
  if (!isFeeding) return 0;
  
  unsigned long elapsed = millis() - feedStartTime;
  unsigned long remaining = (FEED_DURATION * 1000) - elapsed;
  
  return max(0, (int)(remaining / 1000));
}

bool getRelayState() {
  return digitalRead(RELAY_PIN);
}
```

## 🔧 **CÓMO AJUSTAR**

### **⏱️ Cambiar Duración:**
```cpp
// En config.h:
const int FEED_DURATION = 10;  // Cambiar a 10 segundos

// En relay_controller.h:
void update() {
  if (isFeeding && (millis() - feedStartTime >= FEED_DURATION * 1000)) {
    stopFeeding();
  }
}
```

### **🔌 Cambiar Pines:**
```cpp
// En config.h:
const int RELAY_PIN = 8;  // Cambiar pin del relé
const int LED_PIN = 9;    // Cambiar pin del LED
```

### **⚡ Control Directo:**
```cpp
void setRelayState(bool state) {
  digitalWrite(RELAY_PIN, state ? HIGH : LOW);
}

void setLEDState(bool state) {
  digitalWrite(LED_PIN, state ? HIGH : LOW);
}
```

### **🔄 Modo Manual:**
```cpp
void manualFeed(int duration) {
  if (isFeeding) return;
  
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  
  feedStartTime = millis();
  isFeeding = true;
  
  // Usar duración personalizada
  if (duration > 0) {
    // Implementar lógica para duración personalizada
  }
}
```

## 📝 **EXPLICACIÓN DE MÉTODOS**

### **⚡ Control:**
- **startFeeding()**: Inicia alimentación automática
- **stopFeeding()**: Detiene alimentación
- **emergencyStop()**: Parada de emergencia
- **setRelayState()**: Control directo del relé
- **setLEDState()**: Control directo del LED

### **📊 Estado:**
- **isFeedingActive()**: Verifica si está alimentando
- **getRemainingFeedTime()**: Tiempo restante de alimentación
- **getRelayState()**: Estado actual del relé

### **🔄 Sistema:**
- **begin()**: Inicializa pines y estados
- **update()**: Actualiza lógica de alimentación

## ⚙️ **CONFIGURACIÓN AVANZADA**

### **🔊 Feedback Sonoro:**
```cpp
void startFeeding(int scheduleNumber = 0) {
  if (isFeeding) return;
  
  // Beep de inicio
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);
  
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  
  feedStartTime = millis();
  isFeeding = true;
}

void stopFeeding() {
  if (!isFeeding) return;
  
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  
  // Beep de fin
  digitalWrite(BUZZER_PIN, HIGH);
  delay(50);
  digitalWrite(BUZZER_PIN, LOW);
  
  isFeeding = false;
}
```

### **📊 Estadísticas:**
```cpp
class RelayController {
private:
  unsigned long totalFeedTime;
  int feedCount;
  
public:
  void startFeeding(int scheduleNumber = 0) {
    // ... código existente ...
    feedCount++;
  }
  
  void stopFeeding() {
    // ... código existente ...
    totalFeedTime += (millis() - feedStartTime);
  }
  
  unsigned long getTotalFeedTime() {
    return totalFeedTime;
  }
  
  int getFeedCount() {
    return feedCount;
  }
  
  void resetStats() {
    totalFeedTime = 0;
    feedCount = 0;
  }
};
```

### **🔄 Modo Programable:**
```cpp
void startFeeding(int scheduleNumber = 0) {
  if (isFeeding) return;
  
  // Duración según horario
  int duration = FEED_DURATION;
  if (scheduleNumber > 0) {
    // Duración personalizada por horario
    duration = getScheduleDuration(scheduleNumber);
  }
  
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  
  feedStartTime = millis();
  isFeeding = true;
}
```

### **🛡️ Protección:**
```cpp
void update() {
  if (isFeeding) {
    unsigned long elapsed = millis() - feedStartTime;
    
    // Protección contra sobrecarga
    if (elapsed > (FEED_DURATION * 1000)) {
      stopFeeding();
    }
    
    // Protección contra fallo de hardware
    if (elapsed > 30000) { // 30 segundos máximo
      emergencyStop();
    }
  }
}
```

### **🔍 Diagnóstico:**
```cpp
bool testRelay() {
  // Test del relé
  digitalWrite(RELAY_PIN, HIGH);
  delay(100);
  bool state1 = digitalRead(RELAY_PIN);
  
  digitalWrite(RELAY_PIN, LOW);
  delay(100);
  bool state2 = digitalRead(RELAY_PIN);
  
  return state1 && !state2;
}

bool testLED() {
  // Test del LED
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  bool state1 = digitalRead(LED_PIN);
  
  digitalWrite(LED_PIN, LOW);
  delay(100);
  bool state2 = digitalRead(LED_PIN);
  
  return state1 && !state2;
}
```

---

**📅 Fecha**: Diciembre 2024  
**🔧 Versión**: 3.8  
**✅ Estado**: Control completo del relé y LED
