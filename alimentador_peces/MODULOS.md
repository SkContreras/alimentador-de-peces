# 🔧 **DOCUMENTACIÓN DE MÓDULOS**

## 🎯 **DESCRIPCIÓN GENERAL**

El sistema está dividido en **7 módulos especializados**, cada uno con responsabilidades específicas y bien definidas.

---

## ⚙️ **MÓDULO: config.h**

### **🎯 Propósito:**
Configuración centralizada de todos los parámetros del sistema.

### **📋 Categorías de Configuración:**

#### **🔌 Pines de Hardware:**
```cpp
const int BUTTON_SELECT_PIN = 2;      // Botón SELECT
const int BUTTON_UP_PIN = 3;          // Botón UP
const int BUTTON_DOWN_PIN = 4;        // Botón DOWN
const int BUTTON_CONFIRM_PIN = 5;     // Botón CONFIRM
const int RELAY_PIN = 6;              // Relé
const int LED_PIN = 7;                // LED
```

#### **📱 Configuración LCD:**
```cpp
const int LCD_ADDRESS = 0x27;         // Dirección I2C
const int LCD_COLUMNS = 20;           // Columnas
const int LCD_ROWS = 4;               // Filas
const bool USE_LCD = true;            // Habilitar LCD
```

#### **⏱️ Tiempos y Delays:**
```cpp
const int FEED_DURATION = 5;          // Duración alimentación
const unsigned long LOOP_DELAY = 100; // Delay del loop
const unsigned long MENU_TIMEOUT = 30000; // Timeout menú
```

#### **🔧 Configuración Sistema:**
```cpp
const bool SERIAL_ENABLED = false;    // Mensajes seriales
const bool DEBUG_MODE = false;        // Modo debug
const int MAX_FEED_TIMES = 4;         // Máximo horarios
```

### **✅ Ventajas:**
- **Configuración centralizada**
- **Fácil personalización**
- **Sin modificar código principal**
- **Documentación clara**

---

## 🎮 **MÓDULO: button_manager.h**

### **🎯 Propósito:**
Gestión completa de los 4 botones del sistema con debounce y detección de pulsaciones largas.

### **🔧 Características Técnicas:**
- **Debounce software** (50ms)
- **Pull-up interno** del Arduino
- **Detección de flanco** (HIGH→LOW)
- **Pulsaciones largas** (500ms)
- **Repetición automática** para navegación

### **📋 Métodos Principales:**
```cpp
// Inicialización
void begin()

// Detección de pulsaciones
bool selectPressed()      // Pulsación corta SELECT
bool selectLongPressed()  // Pulsación larga SELECT
bool upPressed()          // Pulsación UP
bool downPressed()        // Pulsación DOWN
bool confirmPressed()     // Pulsación CONFIRM

// Repetición automática
bool upRepeating()        // Repetición UP
bool downRepeating()      // Repetición DOWN

// Feedback sonoro
void beep()               // Beep corto
void confirmBeep()        // Beep de confirmación
void errorBeep()          // Beep de error

// Actualización
void update()             // Actualizar estado
```

### **🎯 Estados Internos:**
- **Estado de cada botón** (presionado/liberado)
- **Timestamps** de última pulsación
- **Flags** de debounce
- **Contadores** de repetición

### **✅ Ventajas:**
- **Respuesta rápida** y confiable
- **Sin rebotes** de botones
- **Navegación fluida** con repetición
- **Feedback sonoro** integrado

---

## 📱 **MÓDULO: lcd_display_avr.h**

### **🎯 Propósito:**
Control completo del LCD 20x4 I2C con optimizaciones específicas para AVR.

### **🖥️ Pantallas Implementadas:**

#### **🚀 Pantalla de Inicio:**
```cpp
void showBootScreen()
// Muestra: "ALIMENTADOR DE PECES v3.0"
```

#### **⏰ Reloj Principal:**
```cpp
void showClock()
// Muestra: Hora, fecha, próximo horario, estado
```

#### **📋 Menú Principal:**
```cpp
void showMainMenu(int selectedOption)
// Muestra: Lista de opciones con cursor
```

#### **📅 Ver Horarios:**
```cpp
void showSchedules()
// Muestra: Los 4 horarios con estado
```

#### **✏️ Editor de Horarios:**
```cpp
void showScheduleEditor(int schedule, int hour, int minute, bool enabled, int cursor)
// Muestra: Formulario de edición con cursor
```

#### **📊 Estado del Sistema:**
```cpp
void showStatus()
// Muestra: Hora, estado relay, horarios activos
```

#### **🕐 Ajuste de Hora:**
```cpp
void showTimeAdjust(int hour, int minute, int day, int month, int year, int cursor)
// Muestra: Formulario de ajuste de tiempo
```

### **🎨 Caracteres Personalizados:**
```cpp
void createCustomChars()
// Crea: Pez (posición 0), Reloj (posición 1)
```

### **⚡ Optimizaciones:**
- **Sin mensajes seriales** para ahorrar memoria
- **Actualización inteligente** (solo cuando cambia)
- **Centrado automático** de texto
- **Compatibilidad AVR** específica

### **✅ Ventajas:**
- **Interfaz visual** clara y profesional
- **Optimizado** para memoria limitada
- **Fácil mantenimiento** y extensión
- **Compatible** con Arduino Uno

---

## ⏰ **MÓDULO: rtc_manager.h**

### **🎯 Propósito:**
Gestión completa del RTC DS3231 con alta precisión y persistencia.

### **🔧 Características Técnicas:**
- **Precisión**: ±2ppm (muy alta)
- **Batería**: CR2032 (2-3 años)
- **Comunicación**: I2C
- **Validación**: Rangos de fecha/hora
- **Persistencia**: Mantiene tiempo sin alimentación

### **📋 Métodos Principales:**
```cpp
// Inicialización
bool begin()

// Obtener tiempo
DateTime now()                    // Tiempo actual
int getHour()                     // Hora actual
int getMinute()                   // Minuto actual
int getSecond()                   // Segundo actual

// Ajustar tiempo
bool adjust(DateTime newTime)     // Ajustar fecha/hora completa
bool setTime(int h, int m, int s) // Ajustar solo hora
bool setDate(int d, int m, int y) // Ajustar solo fecha

// Validación
bool isValidTime(int h, int m, int s)  // Validar hora
bool isValidDate(int d, int m, int y)  // Validar fecha
```

### **🎯 Funcionalidades:**
- **Inicialización automática** con fecha de compilación
- **Validación** de rangos de tiempo
- **Ajuste preciso** de hora y fecha
- **Persistencia** con batería

### **✅ Ventajas:**
- **Alta precisión** temporal
- **Persistencia** sin alimentación
- **Validación** de datos
- **Interfaz simple** y confiable

---

## 📅 **MÓDULO: schedule_manager.h**

### **🎯 Propósito:**
Gestión de 4 horarios programables con persistencia en EEPROM.

### **💾 Almacenamiento:**
- **EEPROM**: 512 bytes para horarios
- **Estructura**: Hora, minuto, estado activo
- **Validación**: Rangos de tiempo válidos
- **Backup**: Valores por defecto

### **📋 Métodos Principales:**
```cpp
// Inicialización
void begin()

// Configurar horarios
bool setSchedule(int index, int hour, int minute)
bool enableSchedule(int index, bool enabled)
bool disableSchedule(int index)

// Obtener horarios
FeedTime getSchedule(int index)
int getNextSchedule(RTCManager& rtc)
int getEnabledSchedulesCount()

// Validación
bool isValidSchedule(int hour, int minute)
bool isScheduleEnabled(int index)
```

### **🎯 Estructura de Datos:**
```cpp
struct FeedTime {
  int hour;      // 0-23
  int minute;    // 0-59
  bool enabled;  // true/false
};
```

### **🔄 Funcionalidades:**
- **4 horarios** programables
- **Activación/desactivación** individual
- **Cálculo** del próximo horario
- **Persistencia** en EEPROM
- **Validación** de datos

### **✅ Ventajas:**
- **Persistencia** de configuraciones
- **Flexibilidad** en horarios
- **Validación** de datos
- **Fácil gestión** de horarios

---

## ⚡ **MÓDULO: relay_controller.h**

### **🎯 Propósito:**
Control del relé para el motor alimentador y LED de estado.

### **🔧 Características Técnicas:**
- **Duración configurable** (5 segundos por defecto)
- **Control de estado** (activo/inactivo)
- **LED indicador** de estado
- **Protección** contra activación múltiple

### **📋 Métodos Principales:**
```cpp
// Inicialización
void begin()

// Control de alimentación
void startFeeding(int scheduleIndex)  // Iniciar alimentación
void emergencyStop()                  // Parada de emergencia
void stopFeeding()                    // Parar alimentación

// Estado
bool isFeedingActive()                // Verificar si está alimentando
int getRemainingFeedTime()            // Tiempo restante
bool getRelayState()                  // Estado del relé

// Actualización
void update()                         // Actualizar estado
```

### **🎯 Funcionalidades:**
- **Control del relé** para motor
- **Control del LED** de estado
- **Temporización** de alimentación
- **Parada de emergencia**
- **Protección** contra activación múltiple

### **✅ Ventajas:**
- **Control preciso** del motor
- **Indicación visual** del estado
- **Protección** del sistema
- **Fácil integración**

---

## 🖥️ **MÓDULO: display_manager.h**

### **🎯 Propósito:**
Coordinación de todas las pantallas y gestión de estados de visualización.

### **🔄 Funcionalidades:**
- **Coordinación** de todas las pantallas
- **Gestión de estados** de visualización
- **Optimización** de actualizaciones
- **Interfaz unificada** para LCD

### **📋 Métodos Principales:**
```cpp
// Inicialización
void begin()

// Gestión de pantallas
void showClock()
void showMainMenu(int selectedOption)
void showSchedules()
void showScheduleEditor(int schedule, int hour, int minute, bool enabled, int cursor)
void showStatus()
void showTimeAdjust(int hour, int minute, int day, int month, int year, int cursor)

// Utilidades
void clear()
void showMessage(String title, String message, int duration)
void showConfirmation(String message)
void showError(String message)
```

### **🎯 Flujo de Actualización:**
1. **Verificar** si necesita actualización
2. **Determinar** pantalla actual
3. **Llamar** método específico del LCD
4. **Actualizar** timestamp de última actualización

### **✅ Ventajas:**
- **Interfaz unificada** para pantallas
- **Optimización** de actualizaciones
- **Gestión centralizada** de estados
- **Fácil mantenimiento**

---

## 🔄 **COMUNICACIÓN ENTRE MÓDULOS**

### **📊 Diagrama de Dependencias:**
```
alimentador_peces.ino (Controlador Principal)
    ↓
button_manager.h (Entrada de usuario)
    ↓
display_manager.h (Coordinación de pantallas)
    ↓
lcd_display_avr.h (Renderizado LCD)
    ↓
rtc_manager.h (Tiempo actual)
    ↓
schedule_manager.h (Horarios programados)
    ↓
relay_controller.h (Control de motor)
```

### **🔗 Interfaces:**
- **Configuración**: `config.h` usado por todos
- **Comunicación**: Referencias entre objetos
- **Estados**: Variables globales compartidas
- **Timing**: `millis()` para sincronización

---

## 🎊 **VENTAJAS DEL DISEÑO MODULAR**

### **✅ Mantenibilidad:**
- **Separación clara** de responsabilidades
- **Código organizado** y fácil de entender
- **Modificaciones** sin afectar otros módulos
- **Testing independiente** de componentes

### **✅ Reutilización:**
- **Módulos independientes** y reutilizables
- **Interfaces bien definidas**
- **Fácil integración** en otros proyectos
- **Escalabilidad** del sistema

### **✅ Optimización:**
- **Memoria eficiente** para Arduino Uno
- **Rendimiento optimizado**
- **Código limpio** y sin desperdicios
- **Fácil debugging** y mantenimiento

---

**🔧 Cada módulo es una pieza esencial del sistema, trabajando en conjunto para crear un alimentador automático robusto y confiable.** 🐠✨
