# 🎮 **CÓDIGO PRINCIPAL - alimentador_peces.ino**

## 🎯 **DESCRIPCIÓN GENERAL**

El archivo `alimentador_peces.ino` es el **controlador principal** del sistema. Coordina todos los módulos y gestiona el flujo principal del programa.

---

## 📋 **ESTRUCTURA DEL ARCHIVO**

### **📚 Inclusión de Librerías:**
```cpp
#include <Wire.h>              // Comunicación I2C
#include <RTClib.h>            // RTC DS3231
#include <LiquidCrystal_I2C.h> // LCD I2C
#include <EEPROM.h>            // Memoria persistente
```

### **🔧 Inclusión de Módulos:**
```cpp
#include "config.h"            // Configuración global
#include "button_manager.h"    // Gestión de botones
#include "lcd_display_avr.h"   // Control LCD
#include "rtc_manager.h"       // Gestión RTC
#include "schedule_manager.h"  // Gestión horarios
#include "relay_controller.h"  // Control relé
```

---

## 🏗️ **INSTANCIAS DE MÓDULOS**

### **🎮 Objetos Globales:**
```cpp
ButtonManager buttonManager;                    // Gestión de botones
RTCManager rtcManager;                          // Gestión RTC
ScheduleManager scheduleManager;                // Gestión horarios
RelayController relayController;                // Control relé
LCDDisplayAVR lcdDisplay(&rtcManager, &scheduleManager, &relayController);
```

### **📊 Variables Globales:**
```cpp
unsigned long lastLoopTime = 0;     // Control de timing
unsigned long lastLCDUpdate = 0;    // Control de actualización LCD
bool systemInitialized = false;     // Estado de inicialización
```

---

## 🎯 **ENUMERACIONES Y ESTADOS**

### **📋 Estados del Menú:**
```cpp
enum MenuState {
  MENU_CLOCK,           // Pantalla principal (reloj)
  MENU_MAIN,            // Menú principal
  MENU_VIEW_SCHEDULES,  // Ver horarios
  MENU_EDIT_SCHEDULE,   // Editar horario
  MENU_FEEDING,         // Alimentación activa
  MENU_STATUS,          // Estado del sistema
  MENU_TIME_ADJUST      // Ajuste de hora/fecha
};
```

### **✏️ Estados de Edición:**
```cpp
enum EditState {
  EDIT_HOUR,    // Editando hora
  EDIT_MINUTE,  // Editando minutos
  EDIT_ENABLED, // Editando estado (ON/OFF)
  EDIT_SAVE     // Guardar cambios
};

enum TimeEditState {
  TIME_EDIT_HOUR,   // Editando hora
  TIME_EDIT_MINUTE, // Editando minutos
  TIME_EDIT_DAY,    // Editando día
  TIME_EDIT_MONTH,  // Editando mes
  TIME_EDIT_YEAR,   // Editando año
  TIME_EDIT_SAVE    // Guardar cambios
};
```

---

## 🚀 **FUNCIÓN SETUP()**

### **🎯 Propósito:**
Inicializa todos los módulos y configura el sistema para funcionar.

### **📋 Secuencia de Inicialización:**
```cpp
void setup() {
  // 1. Inicializar I2C
  Wire.begin();
  delay(100);
  
  // 2. Inicializar RTC
  if (!rtcManager.begin()) {
    while(1) delay(1000);  // Bucle infinito si falla
  }
  
  // 3. Inicializar botones
  buttonManager.begin();
  
  // 4. Inicializar relay
  relayController.begin();
  
  // 5. Inicializar horarios
  scheduleManager.begin();
  
  // 6. Inicializar LCD
  lcdDisplay.begin();
  
  // 7. Configurar estado inicial
  systemInitialized = true;
  currentState = MENU_CLOCK;
  updateActivity();
  
  // 8. Mostrar pantalla inicial
  delay(1000);
  updateLCD();
}
```

---

## 🔄 **FUNCIÓN LOOP()**

### **🎯 Propósito:**
Ejecuta el ciclo principal del programa de forma continua.

### **📋 Secuencia del Loop:**
```cpp
void loop() {
  // 1. Verificar inicialización
  if (!systemInitialized) {
    delay(1000);
    return;
  }
  
  // 2. Control de timing
  unsigned long currentTime = millis();
  if (currentTime - lastLoopTime < LOOP_DELAY) {
    delay(LOOP_DELAY - (currentTime - lastLoopTime));
  }
  lastLoopTime = millis();
  
  // 3. Verificar horarios programados
  checkScheduledFeeding();
  
  // 4. Actualizar componentes
  relayController.update();
  buttonManager.update();
  
  // 5. Verificar timeout del menú
  checkMenuTimeout();
  
  // 6. Procesar menú
  processMenu();
  
  // 7. Actualizar LCD
  if (millis() - lastLCDUpdate > 200) {
    updateLCD();
    lastLCDUpdate = millis();
  }
}
```

---

## ⏰ **GESTIÓN DE HORARIOS**

### **🔍 Verificación de Horarios:**
```cpp
void checkScheduledFeeding() {
  if (relayController.isFeedingActive()) return;  // Ya está alimentando
  
  DateTime now = rtcManager.now();
  int nextSchedule = scheduleManager.getNextSchedule(rtcManager);
  
  if (nextSchedule > 0) {
    FeedTime schedule = scheduleManager.getSchedule(nextSchedule);
    
    // Verificar si es hora de alimentar
    if (now.hour() == schedule.hour && 
        now.minute() == schedule.minute && 
        now.second() == 0) {
      
      relayController.startFeeding(nextSchedule);
      
      // Volver al reloj si está en menú
      if (currentState != MENU_CLOCK) {
        currentState = MENU_CLOCK;
      }
    }
  }
}
```

---

## 🎮 **GESTIÓN DEL MENÚ**

### **🔄 Procesamiento Principal:**
```cpp
void processMenu() {
  switch (currentState) {
    case MENU_CLOCK:
      handleClockMode();
      break;
    case MENU_MAIN:
      handleMainMenu();
      break;
    case MENU_VIEW_SCHEDULES:
      handleViewSchedules();
      break;
    case MENU_EDIT_SCHEDULE:
      handleEditSchedule();
      break;
    case MENU_FEEDING:
      handleFeeding();
      break;
    case MENU_STATUS:
      handleStatus();
      break;
    case MENU_TIME_ADJUST:
      handleTimeAdjust();
      break;
  }
}
```

### **🏠 Modo Reloj (Pantalla Principal):**
```cpp
void handleClockMode() {
  if (buttonManager.selectPressed()) {
    enterMainMenu();  // Entrar al menú
  }
  
  if (buttonManager.confirmPressed()) {
    if (!relayController.isFeedingActive()) {
      relayController.startFeeding(0);  // Alimentar ahora
      buttonManager.confirmBeep();
    }
  }
}
```

### **📋 Menú Principal:**
```cpp
void handleMainMenu() {
  updateActivity();
  
  // Navegación con UP/DOWN
  if (buttonManager.upPressed()) {
    selectedOption = (selectedOption > 1) ? selectedOption - 1 : 9;
    buttonManager.beep();
  }
  
  if (buttonManager.downPressed()) {
    selectedOption = (selectedOption < 9) ? selectedOption + 1 : 1;
    buttonManager.beep();
  }
  
  // Selección con CONFIRM
  if (buttonManager.confirmPressed()) {
    executeMainMenuOption();
  }
  
  // Salir con SELECT largo
  if (buttonManager.selectLongPressed()) {
    returnToClock();
  }
}
```

---

## ✏️ **EDITOR DE HORARIOS**

### **🎯 Iniciar Edición:**
```cpp
void startScheduleEdit() {
  currentState = MENU_EDIT_SCHEDULE;
  editState = EDIT_HOUR;
  
  // Cargar valores actuales
  FeedTime current = scheduleManager.getSchedule(editingSchedule);
  tempHour = current.hour;
  tempMinute = current.minute;
  tempEnabled = current.enabled;
}
```

### **🔄 Manejo de Edición:**
```cpp
void handleEditSchedule() {
  updateActivity();
  
  // Cambiar valores con UP/DOWN
  if (buttonManager.upPressed() || buttonManager.upRepeating()) {
    incrementEditValue();
    buttonManager.beep();
  }
  
  if (buttonManager.downPressed() || buttonManager.downRepeating()) {
    decrementEditValue();
    buttonManager.beep();
  }
  
  // Navegar campos con SELECT
  if (buttonManager.selectPressed()) {
    nextEditField();
    buttonManager.beep();
  }
  
  // Guardar con CONFIRM
  if (buttonManager.confirmPressed()) {
    if (editState == EDIT_SAVE) {
      saveSchedule();
    } else {
      nextEditField();
      buttonManager.beep();
    }
  }
  
  // Cancelar con SELECT largo
  if (buttonManager.selectLongPressed()) {
    currentState = MENU_MAIN;
    buttonManager.errorBeep();
  }
}
```

---

## 🕐 **AJUSTE DE HORA**

### **🎯 Iniciar Ajuste:**
```cpp
void startTimeAdjust() {
  currentState = MENU_TIME_ADJUST;
  timeEditState = TIME_EDIT_HOUR;
  
  // Cargar tiempo actual del RTC
  DateTime now = rtcManager.now();
  tempTimeHour = now.hour();
  tempTimeMinute = now.minute();
  tempTimeDay = now.day();
  tempTimeMonth = now.month();
  tempTimeYear = now.year();
  
  updateLCD();
}
```

### **🔄 Manejo de Ajuste:**
```cpp
void handleTimeAdjust() {
  updateActivity();
  
  // Cambiar valores
  if (buttonManager.upPressed() || buttonManager.upRepeating()) {
    incrementTimeValue();
    buttonManager.beep();
  }
  
  if (buttonManager.downPressed() || buttonManager.downRepeating()) {
    decrementTimeValue();
    buttonManager.beep();
  }
  
  // Navegar campos
  if (buttonManager.selectPressed()) {
    nextTimeField();
    buttonManager.beep();
  }
  
  // Guardar
  if (buttonManager.confirmPressed()) {
    if (timeEditState == TIME_EDIT_SAVE) {
      saveTime();
    } else {
      nextTimeField();
      buttonManager.beep();
    }
  }
  
  // Cancelar
  if (buttonManager.selectLongPressed()) {
    currentState = MENU_MAIN;
    buttonManager.errorBeep();
  }
}
```

---

## 📱 **ACTUALIZACIÓN DEL LCD**

### **🔄 Función Principal:**
```cpp
void updateLCD() {
  static MenuState lastState = MENU_CLOCK;
  static int lastSelectedOption = 1;
  // ... más variables estáticas para detección de cambios
  
  bool needsUpdate = false;
  
  // Detectar cambios
  if (currentState != lastState) {
    needsUpdate = true;
    lastState = currentState;
  }
  
  // Actualizar según estado
  if (needsUpdate) {
    switch (currentState) {
      case MENU_CLOCK:
        lcdDisplay.showClock();
        break;
      case MENU_MAIN:
        lcdDisplay.showMainMenu(selectedOption);
        break;
      // ... más casos
    }
  }
}
```

---

## ⏱️ **GESTIÓN DE TIMEOUTS**

### **🔄 Verificación de Timeout:**
```cpp
void checkMenuTimeout() {
  if (inMenu && (millis() - lastActivity > MENU_TIMEOUT)) {
    returnToClock();  // Volver al reloj automáticamente
  }
}

void updateActivity() {
  lastActivity = millis();  // Actualizar timestamp de actividad
}
```

---

## 🎯 **FUNCIONES AUXILIARES**

### **💾 Guardar Horario:**
```cpp
void saveSchedule() {
  if (scheduleManager.setSchedule(editingSchedule, tempHour, tempMinute)) {
    scheduleManager.enableSchedule(editingSchedule, tempEnabled);
    buttonManager.confirmBeep();
    currentState = MENU_MAIN;
  } else {
    buttonManager.errorBeep();
  }
}
```

### **💾 Guardar Tiempo:**
```cpp
void saveTime() {
  // Validar fecha
  if (tempTimeDay > 31 || tempTimeDay < 1 ||
      tempTimeMonth > 12 || tempTimeMonth < 1 ||
      tempTimeYear < 2024 || tempTimeYear > 2099) {
    buttonManager.errorBeep();
    return;
  }
  
  // Crear nueva fecha/hora
  DateTime newTime(tempTimeYear, tempTimeMonth, tempTimeDay, 
                   tempTimeHour, tempTimeMinute, 0);
  
  // Ajustar RTC
  if (rtcManager.adjust(newTime)) {
    buttonManager.confirmBeep();
    currentState = MENU_MAIN;
  } else {
    buttonManager.errorBeep();
  }
}
```

---

## 🎊 **CARACTERÍSTICAS DESTACADAS**

### **✅ Optimización:**
- **Control de timing** preciso
- **Actualización inteligente** del LCD
- **Gestión eficiente** de memoria
- **Timeouts configurables**

### **✅ Robustez:**
- **Validación** de datos de entrada
- **Manejo de errores** con feedback
- **Estados consistentes** del sistema
- **Recuperación** automática de errores

### **✅ Usabilidad:**
- **Navegación intuitiva** con botones
- **Feedback sonoro** para todas las acciones
- **Timeouts automáticos** para comodidad
- **Estados visuales** claros

---

**🎮 El código principal es el corazón del sistema, coordinando todos los módulos para crear una experiencia de usuario fluida y confiable.** 🐠✨
