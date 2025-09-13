# ⏰ **RTC_MANAGER.H - GESTIÓN RTC**

## 🎯 **PROPÓSITO**
Maneja el RTC DS3231 para obtener y ajustar fecha/hora.

## 📋 **ESTRUCTURA DE LA CLASE**

```cpp
class RTCManager {
private:
  RTC_DS3231 rtc;
  bool isInitialized;
  
public:
  // Constructor
  RTCManager();
  
  // Inicialización
  bool begin();
  
  // Obtener tiempo
  DateTime now();
  int getCurrentHour();
  int getCurrentMinute();
  int getCurrentSecond();
  int getCurrentDay();
  int getCurrentMonth();
  int getCurrentYear();
  
  // Ajustar tiempo
  void setDateTime(int year, int month, int day, int hour, int minute, int second);
  bool setTime(int hour, int minute, int second = 0);
  bool setDate(int day, int month, int year);
  
  // Validación
  bool isValidTime(int hour, int minute, int second);
  bool isValidDate(int day, int month, int year);
  
  // Utilidades
  bool isInMinute(int hour, int minute);
};
```

## 🔧 **FUNCIONES PRINCIPALES**

### **🚀 Inicialización:**
```cpp
bool begin() {
  if (!rtc.begin()) {
    return false;
  }
  
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  isInitialized = true;
  return true;
}
```

### **⏰ Obtener Tiempo:**
```cpp
DateTime now() {
  if (!isInitialized) {
    return DateTime(2024, 1, 1, 0, 0, 0);
  }
  return rtc.now();
}

int getCurrentHour() {
  return now().hour();
}

int getCurrentMinute() {
  return now().minute();
}

int getCurrentSecond() {
  return now().second();
}
```

### **🔧 Ajustar Tiempo:**
```cpp
void setDateTime(int year, int month, int day, int hour, int minute, int second) {
  if (!isInitialized) return;
  
  DateTime newTime(year, month, day, hour, minute, second);
  rtc.adjust(newTime);
}

bool setTime(int hour, int minute, int second = 0) {
  if (!isValidTime(hour, minute, second)) return false;
  
  DateTime current = now();
  DateTime newTime(current.year(), current.month(), current.day(), 
                   hour, minute, second);
  rtc.adjust(newTime);
  return true;
}
```

### **✅ Validación:**
```cpp
bool isValidTime(int hour, int minute, int second) {
  return (hour >= 0 && hour <= 23) &&
         (minute >= 0 && minute <= 59) &&
         (second >= 0 && second <= 59);
}

bool isValidDate(int day, int month, int year) {
  if (year < 2024 || year > 2099) return false;
  if (month < 1 || month > 12) return false;
  if (day < 1 || day > 31) return false;
  
  // Validar días por mes
  int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  
  // Año bisiesto
  if (month == 2 && year % 4 == 0) {
    daysInMonth[1] = 29;
  }
  
  return day <= daysInMonth[month - 1];
}
```

## 🔧 **CÓMO AJUSTAR**

### **⏰ Cambiar Zona Horaria:**
```cpp
DateTime now() {
  DateTime utc = rtc.now();
  // Añadir offset de zona horaria (ej: +2 horas)
  return DateTime(utc.unixtime() + (2 * 3600));
}
```

### **🔋 Configurar Batería:**
```cpp
bool begin() {
  if (!rtc.begin()) {
    return false;
  }
  
  // Verificar batería
  if (rtc.lostPower()) {
    // Ajustar con fecha de compilación
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  // Verificar temperatura
  float temp = rtc.getTemperature();
  if (temp < -40 || temp > 85) {
    // Temperatura fuera de rango
    return false;
  }
  
  return true;
}
```

### **📅 Cambiar Rango de Años:**
```cpp
bool isValidDate(int day, int month, int year) {
  if (year < 2020 || year > 2100) return false;  // Cambiar rango
  // ... resto del código
}
```

### **🔄 Sincronización Automática:**
```cpp
void autoSync() {
  // Sincronizar cada hora
  static unsigned long lastSync = 0;
  if (millis() - lastSync > 3600000) {  // 1 hora
    // Lógica de sincronización
    lastSync = millis();
  }
}
```

## 📝 **EXPLICACIÓN DE MÉTODOS**

### **⏰ Obtener Tiempo:**
- **now()**: Obtiene fecha y hora completa
- **getCurrentHour()**: Obtiene hora actual (0-23)
- **getCurrentMinute()**: Obtiene minuto actual (0-59)
- **getCurrentSecond()**: Obtiene segundo actual (0-59)
- **getCurrentDay()**: Obtiene día actual (1-31)
- **getCurrentMonth()**: Obtiene mes actual (1-12)
- **getCurrentYear()**: Obtiene año actual

### **🔧 Ajustar Tiempo:**
- **setDateTime()**: Ajusta fecha y hora completa
- **setTime()**: Ajusta solo hora
- **setDate()**: Ajusta solo fecha

### **✅ Validación:**
- **isValidTime()**: Valida hora, minuto, segundo
- **isValidDate()**: Valida día, mes, año
- **isInMinute()**: Verifica si estamos en un minuto específico

## ⚙️ **CONFIGURACIÓN AVANZADA**

### **🌡️ Monitoreo de Temperatura:**
```cpp
float getTemperature() {
  if (!isInitialized) return 0.0;
  return rtc.getTemperature();
}

bool isTemperatureOK() {
  float temp = getTemperature();
  return (temp >= -40 && temp <= 85);
}
```

### **🔋 Estado de Batería:**
```cpp
bool isBatteryOK() {
  if (!isInitialized) return false;
  return !rtc.lostPower();
}

void checkBattery() {
  if (rtc.lostPower()) {
    // Batería baja o sin batería
    // Implementar lógica de alerta
  }
}
```

### **📅 Formato de Fecha:**
```cpp
String getFormattedDate() {
  DateTime now = this->now();
  String date = "";
  if (now.day() < 10) date += "0";
  date += String(now.day());
  date += "/";
  if (now.month() < 10) date += "0";
  date += String(now.month());
  date += "/";
  date += String(now.year());
  return date;
}

String getFormattedTime() {
  DateTime now = this->now();
  String time = "";
  if (now.hour() < 10) time += "0";
  time += String(now.hour());
  time += ":";
  if (now.minute() < 10) time += "0";
  time += String(now.minute());
  time += ":";
  if (now.second() < 10) time += "0";
  time += String(now.second());
  return time;
}
```

### **🔄 Backup y Restore:**
```cpp
void backupToEEPROM() {
  DateTime now = this->now();
  EEPROM.write(0, now.year() - 2000);
  EEPROM.write(1, now.month());
  EEPROM.write(2, now.day());
  EEPROM.write(3, now.hour());
  EEPROM.write(4, now.minute());
  EEPROM.write(5, now.second());
}

void restoreFromEEPROM() {
  int year = EEPROM.read(0) + 2000;
  int month = EEPROM.read(1);
  int day = EEPROM.read(2);
  int hour = EEPROM.read(3);
  int minute = EEPROM.read(4);
  int second = EEPROM.read(5);
  
  if (isValidDate(day, month, year) && isValidTime(hour, minute, second)) {
    setDateTime(year, month, day, hour, minute, second);
  }
}
```

---

**📅 Fecha**: Diciembre 2024  
**🔧 Versión**: 3.8  
**✅ Estado**: Gestión completa del RTC DS3231
