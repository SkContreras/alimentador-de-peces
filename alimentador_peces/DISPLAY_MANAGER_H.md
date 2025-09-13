# 🖥️ **DISPLAY_MANAGER.H - GESTIÓN PANTALLAS**

## 🎯 **PROPÓSITO**
Coordina todas las pantallas y gestiona estados de visualización.

## 📋 **ESTRUCTURA DE LA CLASE**

```cpp
class DisplayManager {
private:
  LCDDisplayAVR* lcdDisplay;
  unsigned long lastUpdate;
  bool needsUpdate;
  
public:
  // Constructor
  DisplayManager(RTCManager* rtc, ScheduleManager* schedule, RelayController* relay);
  
  // Inicialización
  void begin();
  
  // Gestión de pantallas
  void showClock();
  void showMainMenu(int selectedOption);
  void showSchedules();
  void showScheduleEditor(int schedule, int hour, int minute, bool enabled, int cursor);
  void showStatus();
  void showTimeAdjust(int hour, int minute, int day, int month, int year, int cursor);
  void showFeeding(int remainingTime);
  
  // Utilidades
  void clear();
  void showMessage(String title, String message, int duration = 2000);
  void showConfirmation(String message);
  void showError(String message);
  
  // Control de actualización
  void forceUpdate();
  void setUpdateNeeded(bool needed);
  bool isUpdateNeeded();
};
```

## 🔧 **FUNCIONES PRINCIPALES**

### **🚀 Inicialización:**
```cpp
void begin() {
  lcdDisplay->begin();
  lastUpdate = 0;
  needsUpdate = true;
}
```

### **⏰ Pantalla de Reloj:**
```cpp
void showClock() {
  if (needsUpdate || (millis() - lastUpdate > CLOCK_UPDATE_INTERVAL)) {
    lcdDisplay->showClock();
    lastUpdate = millis();
    needsUpdate = false;
  }
}
```

### **📋 Menú Principal:**
```cpp
void showMainMenu(int selectedOption) {
  if (needsUpdate) {
    lcdDisplay->showMainMenu(selectedOption);
    needsUpdate = false;
  }
}
```

### **📅 Horarios:**
```cpp
void showSchedules() {
  if (needsUpdate) {
    lcdDisplay->showSchedules();
    needsUpdate = false;
  }
}
```

### **✏️ Editor de Horarios:**
```cpp
void showScheduleEditor(int schedule, int hour, int minute, bool enabled, int cursor) {
  if (needsUpdate) {
    lcdDisplay->showScheduleEditor(schedule, hour, minute, enabled, cursor);
    needsUpdate = false;
  }
}
```

### **📊 Estado del Sistema:**
```cpp
void showStatus() {
  if (needsUpdate) {
    lcdDisplay->showStatus();
    needsUpdate = false;
  }
}
```

### **⏰ Ajuste de Hora:**
```cpp
void showTimeAdjust(int hour, int minute, int day, int month, int year, int cursor) {
  if (needsUpdate) {
    lcdDisplay->showTimeAdjust(hour, minute, day, month, year, cursor);
    needsUpdate = false;
  }
}
```

### **🍽️ Alimentación:**
```cpp
void showFeeding(int remainingTime) {
  if (needsUpdate) {
    lcdDisplay->showFeeding(remainingTime);
    needsUpdate = false;
  }
}
```

## 🔧 **CÓMO AJUSTAR**

### **⏱️ Cambiar Frecuencia de Actualización:**
```cpp
// En config.h:
const unsigned long CLOCK_UPDATE_INTERVAL = 5000;  // 5 segundos

// En display_manager.h:
void showClock() {
  if (needsUpdate || (millis() - lastUpdate > CLOCK_UPDATE_INTERVAL)) {
    lcdDisplay->showClock();
    lastUpdate = millis();
    needsUpdate = false;
  }
}
```

### **🔄 Control de Actualización:**
```cpp
void forceUpdate() {
  needsUpdate = true;
  lastUpdate = 0;
}

void setUpdateNeeded(bool needed) {
  needsUpdate = needed;
}

bool isUpdateNeeded() {
  return needsUpdate;
}
```

### **💬 Mensajes Personalizados:**
```cpp
void showMessage(String title, String message, int duration = 2000) {
  lcdDisplay->showMessage(title, message, duration);
}

void showConfirmation(String message) {
  lcdDisplay->showConfirmation(message);
}

void showError(String message) {
  lcdDisplay->showError(message);
}
```

### **🎨 Pantallas Personalizadas:**
```cpp
void showCustomScreen(String line1, String line2, String line3, String line4) {
  if (needsUpdate) {
    lcdDisplay->clear();
    lcdDisplay->setCursor(0, 0);
    lcdDisplay->print(line1);
    lcdDisplay->setCursor(0, 1);
    lcdDisplay->print(line2);
    lcdDisplay->setCursor(0, 2);
    lcdDisplay->print(line3);
    lcdDisplay->setCursor(0, 3);
    lcdDisplay->print(line4);
    needsUpdate = false;
  }
}
```

## 📝 **EXPLICACIÓN DE MÉTODOS**

### **🖥️ Pantallas:**
- **showClock()**: Pantalla principal con hora
- **showMainMenu()**: Menú principal
- **showSchedules()**: Lista de horarios
- **showScheduleEditor()**: Editor de horarios
- **showStatus()**: Estado del sistema
- **showTimeAdjust()**: Ajuste de hora
- **showFeeding()**: Pantalla de alimentación

### **💬 Mensajes:**
- **showMessage()**: Mensaje genérico
- **showConfirmation()**: Mensaje de confirmación
- **showError()**: Mensaje de error

### **🔧 Control:**
- **begin()**: Inicializar display
- **clear()**: Limpiar pantalla
- **forceUpdate()**: Forzar actualización
- **setUpdateNeeded()**: Marcar necesidad de actualización
- **isUpdateNeeded()**: Verificar si necesita actualización

## ⚙️ **CONFIGURACIÓN AVANZADA**

### **🔄 Actualización Inteligente:**
```cpp
class DisplayManager {
private:
  unsigned long lastUpdate;
  bool needsUpdate;
  int lastSelectedOption;
  int lastCursor;
  
public:
  void showMainMenu(int selectedOption) {
    if (needsUpdate || selectedOption != lastSelectedOption) {
      lcdDisplay->showMainMenu(selectedOption);
      lastSelectedOption = selectedOption;
      needsUpdate = false;
    }
  }
  
  void showScheduleEditor(int schedule, int hour, int minute, bool enabled, int cursor) {
    if (needsUpdate || cursor != lastCursor) {
      lcdDisplay->showScheduleEditor(schedule, hour, minute, enabled, cursor);
      lastCursor = cursor;
      needsUpdate = false;
    }
  }
};
```

### **📊 Estadísticas de Pantalla:**
```cpp
class DisplayManager {
private:
  unsigned long totalUpdateTime;
  int updateCount;
  
public:
  void showClock() {
    unsigned long startTime = millis();
    
    if (needsUpdate || (millis() - lastUpdate > CLOCK_UPDATE_INTERVAL)) {
      lcdDisplay->showClock();
      lastUpdate = millis();
      needsUpdate = false;
      
      totalUpdateTime += (millis() - startTime);
      updateCount++;
    }
  }
  
  float getAverageUpdateTime() {
    return updateCount > 0 ? (float)totalUpdateTime / updateCount : 0;
  }
  
  int getUpdateCount() {
    return updateCount;
  }
};
```

### **🎭 Modo Demo:**
```cpp
void showDemo() {
  String demoScreens[] = {
    "Pantalla 1",
    "Pantalla 2",
    "Pantalla 3",
    "Pantalla 4"
  };
  
  for (int i = 0; i < 4; i++) {
    showCustomScreen(demoScreens[i], "Demo", "Funcionando", "OK");
    delay(2000);
  }
}
```

### **🔍 Diagnóstico:**
```cpp
void showDiagnostic() {
  String diagnostic = "Diagnostico:";
  String status = "OK";
  String memory = "RAM: ";
  String uptime = "Uptime: ";
  
  // Verificar memoria
  memory += String(freeMemory());
  
  // Verificar uptime
  uptime += String(millis() / 1000);
  
  showCustomScreen(diagnostic, status, memory, uptime);
}
```

### **🎨 Temas:**
```cpp
enum DisplayTheme {
  THEME_DEFAULT,
  THEME_DARK,
  THEME_BRIGHT
};

class DisplayManager {
private:
  DisplayTheme currentTheme;
  
public:
  void setTheme(DisplayTheme theme) {
    currentTheme = theme;
    needsUpdate = true;
  }
  
  void showClock() {
    if (needsUpdate || (millis() - lastUpdate > CLOCK_UPDATE_INTERVAL)) {
      switch (currentTheme) {
        case THEME_DEFAULT:
          lcdDisplay->showClock();
          break;
        case THEME_DARK:
          showDarkClock();
          break;
        case THEME_BRIGHT:
          showBrightClock();
          break;
      }
      lastUpdate = millis();
      needsUpdate = false;
    }
  }
};
```

---

**📅 Fecha**: Diciembre 2024  
**🔧 Versión**: 3.8  
**✅ Estado**: Gestión completa de pantallas
