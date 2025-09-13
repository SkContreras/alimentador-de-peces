# 🔧 **GUÍA DE INSTALACIÓN - ALIMENTADOR DE PECES**

## 📋 **REQUISITOS PREVIOS**

### **🛠️ Hardware Necesario:**
- ✅ **Arduino Uno** o **Arduino Nano**
- ✅ **LCD 20x4 I2C** (dirección 0x27)
- ✅ **Módulo RTC DS3231** con batería
- ✅ **Relé 5V** (para motor alimentador)
- ✅ **4 Botones** (momentáneos)
- ✅ **LED** (indicador de estado)
- ✅ **Resistencias** (pull-up para botones)
- ✅ **Cables** de conexión
- ✅ **Protoboard** o PCB
- ✅ **Motor alimentador** 12V DC

### **💻 Software Necesario:**
- ✅ **Arduino IDE** (versión 1.8.x o superior)
- ✅ **Librerías**:
  - `RTClib` (para RTC DS3231)
  - `LiquidCrystal_I2C` (para LCD)
  - `Wire` (comunicación I2C)

---

## 📦 **PASO 1: INSTALACIÓN DE LIBRERÍAS**

### **🔽 Descargar Librerías:**

1. **Abrir Arduino IDE**
2. **Ir a**: `Herramientas` → `Administrar librerías...`
3. **Buscar e instalar**:
   ```
   RTClib by Adafruit
   LiquidCrystal I2C by Frank de Brabander
   ```

### **📚 Instalación Manual (si es necesario):**
```bash
# RTClib
https://github.com/adafruit/RTClib

# LiquidCrystal_I2C
https://github.com/johnrickman/LiquidCrystal_I2C
```

---

## 🔌 **PASO 2: CONEXIONES**

### **📋 Códigos de Colores:**
- **⚫ Negro**: GND (Tierra)
- **🔴 Rojo**: VCC (5V)
- **🟡 Amarillo**: SCL (I2C Clock)
- **🟠 Naranja**: SDA (I2C Data)
- **🟢 Verde**: Relé (Control)
- **⚪ Blanco**: Botón CONFIRM
- **🔘 Gris**: Botón DOWN
- **🟣 Morado**: Botón UP
- **🔵 Azul**: Botón SELECT

### **🔗 Conexiones Arduino:**

#### **LCD I2C (20x4):**
```
LCD VCC  → Arduino 5V (🔴)
LCD GND  → Arduino GND (⚫)
LCD SDA  → Arduino A4 (🟠)
LCD SCL  → Arduino A5 (🟡)
```

#### **RTC DS3231:**
```
RTC VCC  → Arduino 5V (🔴)
RTC GND  → Arduino GND (⚫)
RTC SDA  → Arduino A4 (🟠)
RTC SCL  → Arduino A5 (🟡)
```

#### **Botones:**
```
SELECT   → Arduino Pin 2 (🔵)
UP       → Arduino Pin 3 (🟣)
DOWN     → Arduino Pin 4 (🔘)
CONFIRM  → Arduino Pin 5 (⚪)
```

#### **Relé y LED:**
```
Relé IN  → Arduino Pin 6 (🟢)
LED +    → Arduino Pin 7 (🟢)
LED -    → Arduino GND (⚫)
```

#### **Motor Alimentador:**
```
Motor +  → Relé COM (12V)
Motor -  → Relé NO (12V)
Relé VCC → 12V DC
Relé GND → GND
```

---

## 💻 **PASO 3: CONFIGURACIÓN DEL CÓDIGO**

### **📁 Estructura de Archivos:**
```
alimentador_peces/
├── alimentador_peces.ino    # Archivo principal
├── config.h                 # Configuración
├── button_manager.h         # Gestión de botones
├── lcd_display_avr.h        # Control LCD
├── rtc_manager.h            # Gestión RTC
├── schedule_manager.h       # Gestión horarios
├── relay_controller.h       # Control relé
└── display_manager.h        # Gestión pantallas
```

### **⚙️ Configuración Básica:**

1. **Abrir** `alimentador_peces.ino` en Arduino IDE
2. **Verificar** configuración en `config.h`:
   ```cpp
   const int LCD_ADDRESS = 0x27;    // Dirección I2C LCD
   const int LCD_COLUMNS = 20;      // Columnas LCD
   const int LCD_ROWS = 4;          // Filas LCD
   const bool USE_LCD = true;       // Habilitar LCD
   ```

3. **Ajustar pines** si es necesario:
   ```cpp
   const int BUTTON_SELECT_PIN = 2;   // Botón SELECT
   const int BUTTON_UP_PIN = 3;       // Botón UP
   const int BUTTON_DOWN_PIN = 4;     // Botón DOWN
   const int BUTTON_CONFIRM_PIN = 5;  // Botón CONFIRM
   const int RELAY_PIN = 6;           // Relé
   const int LED_PIN = 7;             // LED
   ```

---

## 🚀 **PASO 4: COMPILACIÓN Y SUBIDA**

### **🔧 Configuración Arduino IDE:**

1. **Seleccionar placa**:
   - `Herramientas` → `Placa` → `Arduino Uno` o `Arduino Nano`

2. **Seleccionar puerto**:
   - `Herramientas` → `Puerto` → `COM3` (o el que corresponda)

3. **Verificar código**:
   - `Sketch` → `Verificar/Compilar`
   - Debería compilar sin errores

### **📤 Subir Código:**

1. **Conectar Arduino** por USB
2. **Subir código**:
   - `Sketch` → `Subir`
   - Esperar compilación y subida

3. **Verificar subida**:
   - LED integrado debería parpadear
   - LCD debería mostrar pantalla de inicio

---

## 🧪 **PASO 5: PRUEBAS INICIALES**

### **🔍 Verificación de Componentes:**

#### **LCD:**
- ✅ **Pantalla de inicio** visible
- ✅ **Texto claro** y legible
- ✅ **Sin parpadeo** excesivo

#### **Botones:**
- ✅ **SELECT**: Entra al menú
- ✅ **UP/DOWN**: Navega opciones
- ✅ **CONFIRM**: Confirma selección
- ✅ **Beeps** audibles

#### **RTC:**
- ✅ **Hora correcta** en pantalla
- ✅ **Fecha actual** visible
- ✅ **Sin reseteo** al reiniciar

#### **Relé:**
- ✅ **Click** audible al activar
- ✅ **LED** se enciende
- ✅ **Motor** se activa

---

## ⚠️ **SOLUCIÓN DE PROBLEMAS**

### **🔴 LCD no funciona:**
1. **Verificar conexiones** I2C
2. **Probar dirección** 0x3F o 0x20
3. **Usar** `diagnostico_lcd.ino`

### **🔴 Botones no responden:**
1. **Verificar conexiones** de pines
2. **Revisar** configuración en `config.h`
3. **Probar** con multímetro

### **🔴 RTC no funciona:**
1. **Verificar** batería del RTC
2. **Comprobar** conexiones I2C
3. **Revisar** librería RTClib

### **🔴 Compilación falla:**
1. **Instalar** librerías faltantes
2. **Verificar** sintaxis del código
3. **Revisar** configuración de placa

---

## ✅ **INSTALACIÓN COMPLETADA**

### **🎉 ¡Felicidades!**

Tu alimentador automático de peces está **completamente instalado** y listo para usar.

### **📋 Próximos Pasos:**

1. **Leer** [USO.md](USO.md) para aprender a usar el sistema
2. **Revisar** [CONEXIONES.md](CONEXIONES.md) para verificar conexiones
3. **Configurar** horarios de alimentación
4. **¡Disfrutar** de tu alimentador automático!

### **🆘 Si Necesitas Ayuda:**

- **Problemas técnicos**: [SOLUCION_PROBLEMAS.md](SOLUCION_PROBLEMAS.md)
- **Uso del sistema**: [USO.md](USO.md)
- **Configuración**: [CONFIGURACION.md](CONFIGURACION.md)

---

**🎊 ¡Tu alimentador está listo para alimentar a tus peces automáticamente!** 🐠✨
