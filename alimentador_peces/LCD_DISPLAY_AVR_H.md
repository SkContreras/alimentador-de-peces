# 📱 **LCD_DISPLAY_AVR.H - CONTROL LCD**

## 🎯 **PROPÓSITO**
Controla el LCD 20x4 I2C con optimizaciones específicas para AVR.

## 📋 **ESTRUCTURA DE LA CLASE**

```cpp
class LCDDisplayAVR {
private:
  LiquidCrystal_I2C lcd;
  bool isInitialized;
  unsigned long lastUpdate;
  
  // Referencias a otros módulos
  RTCManager* rtcManager;
  ScheduleManager* scheduleManager;
  RelayController* relayController;
  
  // Métodos privados
  void printTwoDigits(int number);
  String centerText(String text, int width);
  
public:
  // Constructor
  LCDDisplayAVR(RTCManager* rtc, ScheduleManager* schedule, RelayController* relay);
  
  // Inicialización
  bool begin();
  bool isReady();
  
  // Control básico
  void clear();
  
  // Pantallas principales
  void showBootScreen();
  void showClock();
  void showMainMenu(int selectedOption);
  void showSchedules();
  void showScheduleEditor(int schedule, int hour, int minute, bool enabled, int cursor);
  void showStatus();
  void showTimeAdjust(int hour, int minute, int day, int month, int year, int cursor);
  void showFeeding(int remainingTime);
  
  // Mensajes
  void showMessage(String title, String message, int duration = 2000);
  void showConfirmation(String message);
  void showError(String message);
  
  // Caracteres personalizados
  void createCustomChars();
  void showCustomChar(int charIndex, int col, int row);
};
```

## 🔧 **FUNCIONES PRINCIPALES**

### **🚀 Inicialización:**
```cpp
bool begin() {
  if (!USE_LCD) return false;
  
  Wire.begin();
  delay(100);
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  showBootScreen();
  delay(2000);
  
  isInitialized = true;
  return true;
}
```

### **⏰ Pantalla de Reloj:**
```cpp
void showClock() {
  if (!isReady()) return;
  
  DateTime now = rtcManager->now();
  
  lcd.setCursor(0, 0);
  lcd.print("    ALIMENTADOR     ");
  
  lcd.setCursor(0, 1);
  lcd.print("    ");
  printTwoDigits(now.hour());
  lcd.print(":");
  printTwoDigits(now.minute());
  lcd.print(":");
  printTwoDigits(now.second());
  lcd.print("    ");
  
  lcd.setCursor(0, 2);
  lcd.print("  ");
  printTwoDigits(now.day());
  lcd.print("/");
  printTwoDigits(now.month());
  lcd.print("/");
  lcd.print(now.year());
  lcd.print("  ");
  
  lcd.setCursor(0, 3);
  lcd.print("  SELECT: Menu      ");
}
```

### **📋 Menú Principal:**
```cpp
void showMainMenu(int selectedOption) {
  if (!isReady()) return;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   MENU PRINCIPAL   ");
  
  String options[] = {
    "1. Ver Horarios",
    "2. Editar Horario",
    "3. Estado Sistema",
    "4. Ajustar Hora"
  };
  
  for (int i = 0; i < 4; i++) {
    lcd.setCursor(0, i + 1);
    if (i == selectedOption) {
      lcd.print(">");
    } else {
      lcd.print(" ");
    }
    lcd.print(options[i]);
  }
}
```

### **📅 Editor de Horarios:**
```cpp
void showScheduleEditor(int schedule, int hour, int minute, bool enabled, int cursor) {
  if (!isReady()) return;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("EDITAR HORARIO ");
  lcd.print(schedule);
  
  lcd.setCursor(0, 1);
  lcd.print("Hora: ");
  if (cursor == 0) lcd.print("[");
  printTwoDigits(hour);
  if (cursor == 0) lcd.print("]");
  else lcd.print(" ");
  
  lcd.setCursor(0, 2);
  lcd.print("Min:  ");
  if (cursor == 1) lcd.print("[");
  printTwoDigits(minute);
  if (cursor == 1) lcd.print("]");
  else lcd.print(" ");
  
  lcd.setCursor(0, 3);
  lcd.print("Activo: ");
  lcd.print(enabled ? "SI" : "NO");
}
```

## 🔧 **CÓMO AJUSTAR**

### **📱 Cambiar Dirección I2C:**
```cpp
// En el constructor:
LCDDisplayAVR(RTCManager* rtc, ScheduleManager* schedule, RelayController* relay) 
  : lcd(0x3F, LCD_COLUMNS, LCD_ROWS), // Cambiar 0x27 por 0x3F
    isInitialized(false), lastUpdate(0),
    rtcManager(rtc), scheduleManager(schedule), relayController(relay) {}
```

### **🎨 Personalizar Pantallas:**
```cpp
void showBootScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   MI ALIMENTADOR   ");  // Cambiar mensaje
  lcd.setCursor(0, 1);
  lcd.print("    PERSONALIZADO   ");  // Cambiar mensaje
  lcd.setCursor(0, 2);
  lcd.print("   Iniciando...     ");
  lcd.setCursor(0, 3);
  lcd.print("   v3.8             ");
}
```

### **📏 Cambiar Tamaño de LCD:**
```cpp
// En config.h:
const int LCD_COLUMNS = 16;  // Para LCD 16x2
const int LCD_ROWS = 2;

// Ajustar pantallas para 16x2:
void showClock() {
  lcd.setCursor(0, 0);
  lcd.print("  ALIMENTADOR  ");
  lcd.setCursor(0, 1);
  lcd.print("  ");
  printTwoDigits(now.hour());
  lcd.print(":");
  printTwoDigits(now.minute());
  lcd.print("  ");
}
```

### **🎭 Caracteres Personalizados:**
```cpp
void createCustomChars() {
  // Crear caracteres personalizados
  byte customChar1[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111
  };
  
  lcd.createChar(0, customChar1);
}
```

## 📝 **EXPLICACIÓN DE MÉTODOS**

### **🖥️ Pantallas:**
- **showBootScreen()**: Pantalla de inicio
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
- **begin()**: Inicializar LCD
- **clear()**: Limpiar pantalla
- **isReady()**: Verificar si está listo

## ⚙️ **CONFIGURACIÓN AVANZADA**

### **🎨 Personalizar Colores:**
```cpp
// Cambiar color de fondo (si tu LCD lo soporta):
void setBacklightColor(int color) {
  // Implementar según tu LCD
}
```

### **📱 Optimización de Memoria:**
```cpp
// Usar PROGMEM para strings largos:
const char menu1[] PROGMEM = "1. Ver Horarios";
const char menu2[] PROGMEM = "2. Editar Horario";

void showMainMenu(int selectedOption) {
  lcd.setCursor(0, 1);
  lcd.print(FPSTR(menu1));
}
```

### **🔄 Actualización Inteligente:**
```cpp
void showClock() {
  static unsigned long lastUpdate = 0;
  static int lastSecond = -1;
  
  DateTime now = rtcManager->now();
  
  // Solo actualizar si cambió el segundo
  if (now.second() != lastSecond) {
    // Actualizar pantalla
    lastSecond = now.second();
    lastUpdate = millis();
  }
}
```

---

**📅 Fecha**: Diciembre 2024  
**🔧 Versión**: 3.8  
**✅ Estado**: Control LCD optimizado para AVR
