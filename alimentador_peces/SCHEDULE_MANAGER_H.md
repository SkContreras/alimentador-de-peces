# 📅 **SCHEDULE_MANAGER.H - GESTIÓN HORARIOS**

## 🎯 **PROPÓSITO**
Maneja los 4 horarios programables con persistencia en EEPROM.

## 📋 **ESTRUCTURA DE LA CLASE**

```cpp
struct FeedTime {
  int hour;      // 0-23
  int minute;    // 0-59
  bool enabled;  // true/false
};

class ScheduleManager {
private:
  FeedTime schedules[MAX_FEED_TIMES];
  bool isInitialized;
  
  // EEPROM
  void saveToEEPROM();
  void loadFromEEPROM();
  int getEEPROMAddress(int scheduleIndex);
  
  // Validación
  bool isValidSchedule(int hour, int minute);
  
public:
  // Constructor
  ScheduleManager();
  
  // Inicialización
  void begin();
  
  // Configurar horarios
  bool setSchedule(int index, int hour, int minute);
  bool enableSchedule(int index, bool enabled);
  bool disableSchedule(int index);
  
  // Obtener horarios
  FeedTime getSchedule(int index);
  int getNextSchedule(RTCManager& rtc);
  int getEnabledSchedulesCount();
  
  // Validación
  bool isScheduleEnabled(int index);
  bool isValidSchedule(int hour, int minute);
};
```

## 🔧 **FUNCIONES PRINCIPALES**

### **🚀 Inicialización:**
```cpp
void begin() {
  loadFromEEPROM();
  isInitialized = true;
}
```

### **💾 Cargar desde EEPROM:**
```cpp
void loadFromEEPROM() {
  for (int i = 0; i < MAX_FEED_TIMES; i++) {
    int address = getEEPROMAddress(i);
    schedules[i].hour = EEPROM.read(address);
    schedules[i].minute = EEPROM.read(address + 1);
    schedules[i].enabled = EEPROM.read(address + 2);
  }
}
```

### **💾 Guardar en EEPROM:**
```cpp
void saveToEEPROM() {
  for (int i = 0; i < MAX_FEED_TIMES; i++) {
    int address = getEEPROMAddress(i);
    EEPROM.write(address, schedules[i].hour);
    EEPROM.write(address + 1, schedules[i].minute);
    EEPROM.write(address + 2, schedules[i].enabled ? 1 : 0);
  }
}
```

### **🔧 Configurar Horario:**
```cpp
bool setSchedule(int index, int hour, int minute) {
  if (index < 1 || index > MAX_FEED_TIMES) return false;
  if (!isValidSchedule(hour, minute)) return false;
  
  schedules[index - 1].hour = hour;
  schedules[index - 1].minute = minute;
  
  saveToEEPROM();
  return true;
}
```

### **✅ Habilitar/Deshabilitar:**
```cpp
bool enableSchedule(int index, bool enabled) {
  if (index < 1 || index > MAX_FEED_TIMES) return false;
  
  schedules[index - 1].enabled = enabled;
  saveToEEPROM();
  return true;
}

bool disableSchedule(int index) {
  return enableSchedule(index, false);
}
```

### **🔍 Obtener Horario:**
```cpp
FeedTime getSchedule(int index) {
  if (index < 1 || index > MAX_FEED_TIMES) {
    return {0, 0, false};
  }
  return schedules[index - 1];
}
```

### **⏰ Próximo Horario:**
```cpp
int getNextSchedule(RTCManager& rtc) {
  DateTime now = rtc.now();
  int currentTime = now.hour() * 60 + now.minute();
  int nextSchedule = -1;
  int minTime = 24 * 60; // 24 horas en minutos
  
  for (int i = 0; i < MAX_FEED_TIMES; i++) {
    if (!schedules[i].enabled) continue;
    
    int scheduleTime = schedules[i].hour * 60 + schedules[i].minute;
    int timeDiff = scheduleTime - currentTime;
    
    if (timeDiff > 0 && timeDiff < minTime) {
      minTime = timeDiff;
      nextSchedule = i + 1;
    }
  }
  
  return nextSchedule;
}
```

## 🔧 **CÓMO AJUSTAR**

### **📅 Cambiar Número de Horarios:**
```cpp
// En config.h:
const int MAX_FEED_TIMES = 6;  // Cambiar a 6 horarios

// En schedule_manager.h:
FeedTime schedules[6];  // Cambiar array size
```

### **💾 Cambiar Dirección EEPROM:**
```cpp
int getEEPROMAddress(int scheduleIndex) {
  return EEPROM_SCHEDULE_START + (scheduleIndex * 3); // 3 bytes por horario
}

// Para empezar en dirección 100:
const int EEPROM_SCHEDULE_START = 100;
```

### **⏰ Cambiar Validación:**
```cpp
bool isValidSchedule(int hour, int minute) {
  // Permitir solo horarios de 6 AM a 10 PM
  if (hour < 6 || hour > 22) return false;
  if (minute < 0 || minute > 59) return false;
  return true;
}
```

### **🔄 Cambiar Formato de Almacenamiento:**
```cpp
void saveToEEPROM() {
  for (int i = 0; i < MAX_FEED_TIMES; i++) {
    int address = getEEPROMAddress(i);
    // Almacenar como minutos desde medianoche
    int totalMinutes = schedules[i].hour * 60 + schedules[i].minute;
    EEPROM.write(address, totalMinutes & 0xFF);        // Byte bajo
    EEPROM.write(address + 1, (totalMinutes >> 8) & 0xFF); // Byte alto
    EEPROM.write(address + 2, schedules[i].enabled ? 1 : 0);
  }
}
```

## 📝 **EXPLICACIÓN DE MÉTODOS**

### **🔧 Configuración:**
- **setSchedule()**: Configura hora y minuto de un horario
- **enableSchedule()**: Habilita/deshabilita un horario
- **disableSchedule()**: Deshabilita un horario

### **🔍 Consulta:**
- **getSchedule()**: Obtiene datos de un horario
- **getNextSchedule()**: Encuentra el próximo horario activo
- **getEnabledSchedulesCount()**: Cuenta horarios habilitados
- **isScheduleEnabled()**: Verifica si un horario está habilitado

### **💾 Persistencia:**
- **saveToEEPROM()**: Guarda horarios en EEPROM
- **loadFromEEPROM()**: Carga horarios desde EEPROM
- **getEEPROMAddress()**: Calcula dirección EEPROM

### **✅ Validación:**
- **isValidSchedule()**: Valida hora y minuto

## ⚙️ **CONFIGURACIÓN AVANZADA**

### **📊 Estadísticas:**
```cpp
int getEnabledSchedulesCount() {
  int count = 0;
  for (int i = 0; i < MAX_FEED_TIMES; i++) {
    if (schedules[i].enabled) count++;
  }
  return count;
}

String getScheduleSummary() {
  String summary = "Horarios: ";
  summary += String(getEnabledSchedulesCount());
  summary += "/";
  summary += String(MAX_FEED_TIMES);
  return summary;
}
```

### **🔄 Backup y Restore:**
```cpp
void backupSchedules() {
  // Backup a EEPROM secundaria
  for (int i = 0; i < MAX_FEED_TIMES; i++) {
    int address = 200 + (i * 3); // Dirección de backup
    EEPROM.write(address, schedules[i].hour);
    EEPROM.write(address + 1, schedules[i].minute);
    EEPROM.write(address + 2, schedules[i].enabled ? 1 : 0);
  }
}

void restoreSchedules() {
  // Restaurar desde backup
  for (int i = 0; i < MAX_FEED_TIMES; i++) {
    int address = 200 + (i * 3);
    schedules[i].hour = EEPROM.read(address);
    schedules[i].minute = EEPROM.read(address + 1);
    schedules[i].enabled = EEPROM.read(address + 2);
  }
  saveToEEPROM();
}
```

### **📅 Horarios Predefinidos:**
```cpp
void setDefaultSchedules() {
  // Horarios por defecto
  setSchedule(1, 8, 0);   // 8:00 AM
  setSchedule(2, 14, 0);  // 2:00 PM
  setSchedule(3, 20, 0);  // 8:00 PM
  setSchedule(4, 0, 0);   // Deshabilitado
}
```

### **🔍 Búsqueda Avanzada:**
```cpp
int findSchedule(int hour, int minute) {
  for (int i = 0; i < MAX_FEED_TIMES; i++) {
    if (schedules[i].hour == hour && schedules[i].minute == minute) {
      return i + 1;
    }
  }
  return -1; // No encontrado
}

bool hasConflict(int hour, int minute, int excludeIndex = -1) {
  for (int i = 0; i < MAX_FEED_TIMES; i++) {
    if (i == excludeIndex - 1) continue;
    if (schedules[i].hour == hour && schedules[i].minute == minute) {
      return true;
    }
  }
  return false;
}
```

---

**📅 Fecha**: Diciembre 2024  
**🔧 Versión**: 3.8  
**✅ Estado**: Gestión completa de horarios con EEPROM
