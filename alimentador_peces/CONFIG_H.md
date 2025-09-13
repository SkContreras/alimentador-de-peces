# ⚙️ **CONFIG.H - CONFIGURACIÓN GLOBAL**

## 🎯 **PROPÓSITO**
Archivo central de configuración que define todas las constantes y parámetros del sistema.

## 📋 **ESTRUCTURA COMPLETA**

```cpp
#ifndef CONFIG_H
#define CONFIG_H

// === CONFIGURACIÓN DE PINES ===
const int RELAY_PIN = 6;              // Pin del relay
const int LED_PIN = 7;                // Pin del LED
const int BUZZER_PIN = 8;             // Pin del buzzer

// Pines de botones
const int BUTTON_SELECT_PIN = 2;      // Botón SELECT
const int BUTTON_UP_PIN = 3;          // Botón UP
const int BUTTON_DOWN_PIN = 4;        // Botón DOWN
const int BUTTON_CONFIRM_PIN = 5;     // Botón CONFIRM

// === CONFIGURACIÓN DEL LCD ===
const int LCD_ADDRESS = 0x27;         // Dirección I2C del LCD
const int LCD_COLUMNS = 20;           // Columnas del LCD
const int LCD_ROWS = 4;               // Filas del LCD
const bool USE_LCD = true;            // Habilitar LCD

// === CONFIGURACIÓN DE TIEMPOS ===
const int FEED_DURATION = 5;          // Duración de alimentación en segundos
const unsigned long LOOP_DELAY = 100; // Delay del loop principal (ms)
const unsigned long TIME_DISPLAY_INTERVAL = 30000; // Mostrar hora cada 30s
const unsigned long MENU_TIMEOUT = 30000; // Timeout del menú (ms)

// === CONFIGURACIÓN DE SISTEMA ===
const bool SERIAL_ENABLED = false;    // Habilitar mensajes seriales
const bool DEBUG_MODE = false;        // Modo debug
const int MAX_FEED_TIMES = 4;         // Máximo número de horarios

// === CONFIGURACIÓN DE BOTONES ===
const unsigned long BUTTON_DEBOUNCE_TIME = 50;     // Tiempo de debounce (ms)
const unsigned long BUTTON_LONG_PRESS_TIME = 500;  // Tiempo para pulsación larga (ms)
const unsigned long BUTTON_REPEAT_DELAY = 200;     // Delay para repetición (ms)
const unsigned long BUTTON_REPEAT_INTERVAL = 100;  // Intervalo de repetición (ms)

// === CONFIGURACIÓN DE RTC ===
const bool RTC_AUTO_ADJUST = true;    // Ajuste automático del RTC
const int RTC_BACKUP_BATTERY_LIFE = 3; // Vida de batería en años

// === CONFIGURACIÓN DE EEPROM ===
const int EEPROM_SCHEDULE_START = 0;  // Dirección inicial de horarios
const int EEPROM_SCHEDULE_SIZE = 4;   // Tamaño de cada horario en bytes

// === MENSAJES DEL SISTEMA ===
const char* BOOT_MESSAGE = "ALIMENTADOR DE PECES v3.0";
const char* ERROR_MESSAGE = "ERROR";
const char* SUCCESS_MESSAGE = "EXITO";

#endif
```

## 🔧 **CÓMO AJUSTAR**

### **🔌 Cambiar Pines:**
```cpp
// Si quieres cambiar el relé al pin 8:
const int RELAY_PIN = 8;

// Si quieres cambiar botones:
const int BUTTON_SELECT_PIN = 10;
```

### **⏱️ Ajustar Tiempos:**
```cpp
// Cambiar duración de alimentación a 10 segundos:
const int FEED_DURATION = 10;

// Cambiar timeout del menú a 60 segundos:
const unsigned long MENU_TIMEOUT = 60000;
```

### **📱 Configurar LCD:**
```cpp
// Si tu LCD tiene dirección diferente:
const int LCD_ADDRESS = 0x3F;  // o 0x20

// Si tienes LCD 16x2:
const int LCD_COLUMNS = 16;
const int LCD_ROWS = 2;
```

### **🔊 Habilitar Serial:**
```cpp
// Para debugging:
const bool SERIAL_ENABLED = true;
const bool DEBUG_MODE = true;
```

### **📅 Cambiar Horarios:**
```cpp
// Aumentar a 6 horarios:
const int MAX_FEED_TIMES = 6;
```

## 📝 **EXPLICACIÓN DE CONSTANTES**

### **🔌 Pines:**
- **RELAY_PIN**: Pin digital que controla el relé del motor
- **LED_PIN**: Pin digital para el LED de estado
- **BUZZER_PIN**: Pin digital para el buzzer (no usado actualmente)
- **BUTTON_*_PIN**: Pines digitales para los botones

### **⏱️ Tiempos:**
- **FEED_DURATION**: Cuánto tiempo se activa el relé
- **LOOP_DELAY**: Pausa entre iteraciones del loop principal
- **TIME_DISPLAY_INTERVAL**: Cada cuánto se actualiza la hora
- **MENU_TIMEOUT**: Tiempo antes de volver al reloj

### **📱 LCD:**
- **LCD_ADDRESS**: Dirección I2C del display
- **LCD_COLUMNS/ROWS**: Dimensiones del display
- **USE_LCD**: Habilitar/deshabilitar LCD

### **🎮 Botones:**
- **BUTTON_DEBOUNCE_TIME**: Tiempo para evitar rebotes
- **BUTTON_LONG_PRESS_TIME**: Tiempo para pulsación larga
- **BUTTON_REPEAT_***: Configuración de repetición

---

**📅 Fecha**: Diciembre 2024  
**🔧 Versión**: 3.8  
**✅ Estado**: Configuración central del sistema
