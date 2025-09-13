# 💻 **EXPLICACIÓN GENERAL DEL CÓDIGO**

## 🎯 **ARQUITECTURA DEL SISTEMA**

El **Alimentador Automático de Peces** está diseñado con una arquitectura modular que separa las responsabilidades en diferentes módulos especializados.

### **📁 Estructura Modular:**
```
alimentador_peces/
├── alimentador_peces.ino    # 🎮 Controlador principal
├── config.h                 # ⚙️ Configuración global
├── button_manager.h         # 🎮 Gestión de botones
├── lcd_display_avr.h        # 📱 Control LCD
├── rtc_manager.h            # ⏰ Gestión RTC
├── schedule_manager.h       # 📅 Gestión horarios
├── relay_controller.h       # ⚡ Control relé
└── display_manager.h        # 🖥️ Gestión pantallas
```

---

## 🏗️ **PRINCIPIOS DE DISEÑO**

### **🎯 Modularidad:**
- **Separación de responsabilidades** clara
- **Módulos independientes** y reutilizables
- **Interfaces bien definidas** entre módulos

### **⚡ Optimización:**
- **Memoria optimizada** para Arduino Uno (32KB)
- **Sin mensajes seriales** para ahorrar RAM
- **Código eficiente** y sin desperdicios

### **🔧 Mantenibilidad:**
- **Código limpio** y bien documentado
- **Configuración centralizada** en `config.h`
- **Fácil modificación** y extensión

---

## 🎮 **MÓDULO PRINCIPAL (alimentador_peces.ino)**

### **🎯 Responsabilidades:**
- **Coordinación** de todos los módulos
- **Gestión del menú** y navegación
- **Control del flujo** principal del programa
- **Comunicación** entre módulos

### **🔄 Flujo Principal:**
```cpp
void setup() {
  // Inicialización de módulos
  // Configuración inicial
  // Pantalla de inicio
}

void loop() {
  // Verificación de horarios
  // Actualización de componentes
  // Procesamiento de menú
  // Actualización de LCD
}
```

### **📋 Estados del Menú:**
- **MENU_CLOCK**: Pantalla principal
- **MENU_MAIN**: Menú principal
- **MENU_VIEW_SCHEDULES**: Ver horarios
- **MENU_EDIT_SCHEDULE**: Editar horario
- **MENU_FEEDING**: Alimentación activa
- **MENU_STATUS**: Estado del sistema
- **MENU_TIME_ADJUST**: Ajuste de hora

---

## ⚙️ **MÓDULO DE CONFIGURACIÓN (config.h)**

### **🎯 Propósito:**
- **Configuración centralizada** de todos los parámetros
- **Fácil personalización** sin modificar código
- **Constantes globales** para todo el sistema

### **📋 Categorías:**
```cpp
// Pines de hardware
const int BUTTON_SELECT_PIN = 2;
const int RELAY_PIN = 6;

// Configuración LCD
const int LCD_ADDRESS = 0x27;
const int LCD_COLUMNS = 20;

// Tiempos y delays
const int FEED_DURATION = 5;
const unsigned long MENU_TIMEOUT = 30000;

// Configuración de sistema
const bool USE_LCD = true;
const bool SERIAL_ENABLED = false;
```

---

## 🎮 **MÓDULO DE BOTONES (button_manager.h)**

### **🎯 Funcionalidades:**
- **Detección de pulsaciones** con debounce
- **Detección de pulsaciones largas**
- **Repetición automática** para navegación
- **Feedback sonoro** con beeps

### **🔧 Características Técnicas:**
- **Debounce software** (50ms)
- **Pull-up interno** del Arduino
- **Detección de flanco** (HIGH→LOW)
- **Timeout configurable** para pulsaciones largas

### **📋 Métodos Principales:**
```cpp
bool selectPressed()      // Pulsación corta SELECT
bool selectLongPressed()  // Pulsación larga SELECT
bool upPressed()          // Pulsación UP
bool upRepeating()        // Repetición UP
void beep()               // Beep de confirmación
```

---

## 📱 **MÓDULO LCD (lcd_display_avr.h)**

### **🎯 Funcionalidades:**
- **Control completo** del LCD 20x4 I2C
- **Pantallas especializadas** para cada función
- **Caracteres personalizados** (pez, reloj)
- **Optimización para AVR**

### **🖥️ Pantallas Implementadas:**
- **Pantalla de inicio** (boot screen)
- **Reloj principal** con hora, fecha y estado
- **Menú principal** con navegación
- **Editor de horarios** con cursor
- **Estado del sistema** detallado
- **Ajuste de hora/fecha** completo

### **⚡ Optimizaciones:**
- **Sin mensajes seriales** para ahorrar memoria
- **Actualización inteligente** (solo cuando cambia)
- **Caracteres personalizados** para iconos
- **Centrado automático** de texto

---

## ⏰ **MÓDULO RTC (rtc_manager.h)**

### **🎯 Funcionalidades:**
- **Gestión completa** del RTC DS3231
- **Ajuste de hora/fecha** programático
- **Validación de datos** de tiempo
- **Persistencia** con batería

### **🔧 Características Técnicas:**
- **Precisión**: ±2ppm (muy alta)
- **Batería**: CR2032 (2-3 años)
- **Comunicación**: I2C
- **Validación**: Rangos de fecha/hora

### **📋 Métodos Principales:**
```cpp
bool begin()                    // Inicialización
DateTime now()                  // Obtener tiempo actual
bool adjust(DateTime newTime)   // Ajustar tiempo
bool setTime(int h, int m, int s)  // Ajustar hora
```

---

## 📅 **MÓDULO DE HORARIOS (schedule_manager.h)**

### **🎯 Funcionalidades:**
- **Gestión de 4 horarios** programables
- **Activación/desactivación** individual
- **Persistencia en EEPROM**
- **Cálculo de próximo horario**

### **💾 Almacenamiento:**
- **EEPROM**: 512 bytes para horarios
- **Estructura**: Hora, minuto, estado activo
- **Validación**: Rangos de tiempo válidos
- **Backup**: Valores por defecto

### **📋 Métodos Principales:**
```cpp
bool setSchedule(int index, int hour, int minute)
bool enableSchedule(int index, bool enabled)
FeedTime getSchedule(int index)
int getNextSchedule(RTCManager& rtc)
```

---

## ⚡ **MÓDULO DE RELÉ (relay_controller.h)**

### **🎯 Funcionalidades:**
- **Control del relé** para motor alimentador
- **Control del LED** de estado
- **Temporización** de alimentación
- **Parada de emergencia**

### **🔧 Características Técnicas:**
- **Duración configurable** (5 segundos por defecto)
- **Control de estado** (activo/inactivo)
- **LED indicador** de estado
- **Protección** contra activación múltiple

### **📋 Métodos Principales:**
```cpp
void startFeeding(int scheduleIndex)  // Iniciar alimentación
void emergencyStop()                  // Parada de emergencia
bool isFeedingActive()                // Verificar estado
int getRemainingFeedTime()            // Tiempo restante
```

---

## 🖥️ **MÓDULO DE PANTALLAS (display_manager.h)**

### **🎯 Funcionalidades:**
- **Coordinación** de todas las pantallas
- **Gestión de estados** de visualización
- **Optimización** de actualizaciones
- **Interfaz unificada** para LCD

### **🔄 Flujo de Actualización:**
1. **Verificar** si necesita actualización
2. **Determinar** pantalla actual
3. **Llamar** método específico del LCD
4. **Actualizar** timestamp de última actualización

---

## 🔄 **FLUJO DE COMUNICACIÓN**

### **📊 Diagrama de Interacción:**
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

### **🔄 Ciclo Principal:**
1. **Lectura** de botones
2. **Procesamiento** de menú
3. **Verificación** de horarios
4. **Actualización** de componentes
5. **Renderizado** de pantalla
6. **Repetición** del ciclo

---

## ⚡ **OPTIMIZACIONES IMPLEMENTADAS**

### **💾 Memoria:**
- **Sin mensajes seriales** (ahorro de RAM)
- **Strings optimizados** (F() macro)
- **Variables estáticas** donde es posible
- **Código modular** (reutilización)

### **🔄 Rendimiento:**
- **Actualización inteligente** del LCD
- **Debounce optimizado** para botones
- **Timeouts configurables**
- **Loop principal** con delay controlado

### **🔧 Mantenibilidad:**
- **Configuración centralizada**
- **Código bien documentado**
- **Separación de responsabilidades**
- **Interfaces claras** entre módulos

---

## 🎯 **VENTAJAS DEL DISEÑO**

### **✅ Modularidad:**
- **Fácil mantenimiento** y modificación
- **Reutilización** de componentes
- **Testing independiente** de módulos
- **Escalabilidad** del sistema

### **✅ Optimización:**
- **Memoria eficiente** para Arduino Uno
- **Rendimiento optimizado**
- **Consumo de energía** mínimo
- **Respuesta rápida** del sistema

### **✅ Usabilidad:**
- **Interfaz intuitiva** con botones
- **Feedback visual** y sonoro
- **Configuración fácil** de horarios
- **Monitoreo** del estado del sistema

---

## 🎊 **CONCLUSIÓN**

El código del **Alimentador Automático de Peces** está diseñado con los más altos estándares de ingeniería de software, combinando:

- **🏗️ Arquitectura modular** y escalable
- **⚡ Optimización** para hardware limitado
- **🔧 Mantenibilidad** y extensibilidad
- **🎯 Funcionalidad completa** y robusta

¡Un sistema profesional y confiable para el cuidado automático de tus peces! 🐠✨
