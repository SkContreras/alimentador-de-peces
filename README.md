# Alimentador de Peces Automático con Arduino - Versión 3.0 con LCD

Este proyecto implementa un sistema de alimentación automática para peces usando Arduino Uno, un reloj de tiempo real DS3231 y un relay para controlar el mecanismo de alimentación. 

**📺 NUEVA VERSIÓN 3.0**: ¡Ahora incluye pantalla LCD 20x4 con fondo azul y menús visuales interactivos!

**🎮 Incluye Botones**: 4 botones físicos para navegación completa sin computadora.

**🔧 Código Modular**: El proyecto está organizado en módulos independientes para facilitar el mantenimiento, personalización y comprensión del código.

## 🐠 Características

### 📺 **NUEVAS Características v3.0:**
- 🆕 **LCD 20x4 I2C** con fondo azul para menús visuales
- 🆕 **Menús interactivos** mostrados en pantalla LCD
- 🆕 **Reloj visual** con hora, fecha y próximo horario
- 🆕 **Editor visual** de horarios en pantalla
- 🆕 **Ajuste de hora/fecha** con botones (¡SIN COMPUTADORA!)
- 🆕 **Mensajes de estado** y confirmación en LCD
- 🆕 **Caracteres personalizados** (pez, reloj)

### 🎮 **Características v2.0 (Incluidas):**
- ✅ **4 botones físicos** para configurar sin computadora
- ✅ **Menú interactivo** navegable con botones
- ✅ **Edición de horarios** directamente en el dispositivo
- ✅ **Retroalimentación sonora** con buzzer (opcional)
- ✅ **Sistema anti-rebote** avanzado para botones
- ✅ **Pulsación larga** y repetición automática

### ✅ **Características Originales:**
- ✅ **4 horarios programables** independientes
- ✅ **Control por relay** para activar alimentador
- ✅ **Reloj de tiempo real** DS3231 con batería de respaldo
- ✅ **Duración configurable** de alimentación
- ✅ **LED indicador** de estado
- ✅ **Interfaz serial** para configuración y monitoreo (compatible)
- ✅ **Alimentación manual** mediante botón o comando
- ✅ **Horarios habilitables/deshabilitables**

## 🔧 Componentes Necesarios

| Componente | Cantidad | Descripción |
|------------|----------|-------------|
| Arduino Uno | 1 | Microcontrolador principal |
| DS3231 RTC | 1 | Reloj de tiempo real |
| Módulo Relay 5V | 1 | Para controlar el alimentador |
| **📺 LCD 20x4 I2C** | **1** | **Pantalla con fondo azul y módulo I2C** |
| **🎮 Botones pulsadores** | **4** | **Para navegación y configuración** |
| **🔊 Buzzer 5V** | **1** | **Retroalimentación sonora (opcional)** |
| LED | 1 | Indicador visual (opcional) |
| Resistencia 220Ω | 1 | Para el LED |
| Cables jumper | Varios | Para conexiones |
| Protoboard | 1 | Para montaje |

### 🎮 **Distribución de Botones:**
- **SELECT** (Pin 2): Navegar menús / Entrar al menú principal
- **UP** (Pin 3): Incrementar valores / Navegar hacia arriba
- **DOWN** (Pin 4): Decrementar valores / Navegar hacia abajo  
- **CONFIRM** (Pin 5): Confirmar selecciones / Alimentar manualmente

## 📁 Estructura del Proyecto (Modular)

El código está organizado en módulos para mejor mantenimiento:

```
alimentador-de-peces/
├── alimentador_peces.ino    # Archivo principal
├── config.h                 # Configuraciones globales
├── rtc_manager.h           # Gestión del reloj DS3231
├── relay_controller.h      # Control del relay y LED
├── schedule_manager.h      # Gestión de horarios
├── serial_commands.h       # Comandos por monitor serial
├── 🆕 button_manager.h     # 🆕 Gestión de botones físicos
├── 🆕 display_manager.h    # 🆕 Gestión de pantalla/display
├── 🆕 menu_system.h        # 🆕 Sistema de menús interactivos
├── conexiones.md           # Diagrama de conexiones
├── INSTALACION.md          # Guía de instalación detallada
└── README.md              # Esta documentación
```

### 📋 Descripción de Módulos

| Módulo | Descripción |
|--------|-------------|
| `alimentador_peces.ino` | Archivo principal con setup() y loop() |
| `config.h` | Configuraciones: pines, tiempos, mensajes |
| `rtc_manager.h` | Manejo del reloj DS3231 |
| `relay_controller.h` | Control del relay y LED indicador |
| `schedule_manager.h` | Gestión de 4 horarios programables |
| `serial_commands.h` | Procesamiento de comandos seriales |
| **🆕 `button_manager.h`** | **Gestión avanzada de botones con anti-rebote** |
| **🆕 `display_manager.h`** | **Gestión de visualización (Serial/LCD)** |
| **🆕 `menu_system.h`** | **Sistema completo de menús navegables** |

## 📦 Librerías Necesarias

Instala estas librerías en el Arduino IDE:

```
RTClib by Adafruit
LiquidCrystal_I2C by Frank de Brabander
Wire (incluida en Arduino IDE)
```

**Instalación:**
1. Arduino IDE → Herramientas → Administrar librerías
2. Busca "RTClib" e instala la versión de **Adafruit**
3. Busca "LiquidCrystal I2C" e instala la versión de **Frank de Brabander**

## 🔌 Conexiones

Ver archivo `conexiones.md` para el diagrama detallado de conexiones.

### Resumen rápido:
- **DS3231**: VCC→5V, GND→GND, SDA→A4, SCL→A5
- **Relay**: VCC→5V, GND→GND, IN→Pin 7
- **LED**: Ánodo→Pin 13 (con resistencia 220Ω), Cátodo→GND

## 🚀 Instalación y Uso

### 1. Preparación del Hardware
1. Realiza todas las conexiones según el diagrama en `conexiones.md`
2. **📺 Conecta el LCD 20x4 I2C** a los pines SDA (A4) y SCL (A5)
3. **🎮 Conecta los 4 botones** a los pines 2, 3, 4, 5
4. **🔊 Conecta el buzzer** al pin 6 (opcional)
5. Verifica que el DS3231 tenga la batería CR2032 instalada
6. Conecta el mecanismo de alimentación al relay

### 2. Carga del Código
1. Descarga todos los archivos del proyecto en una carpeta
2. Abre `alimentador_peces.ino` en Arduino IDE
3. **Importante**: Todos los archivos .h deben estar en la misma carpeta
4. Selecciona la placa "Arduino Uno"
5. Selecciona el puerto COM correcto
6. Carga el código al Arduino

### 3. Configuración Inicial
El sistema arranca mostrando la hora actual y está listo para usar con botones.

**Horarios predeterminados:**
- Horario 1: 08:00 AM ✅
- Horario 2: 12:00 PM ✅  
- Horario 3: 18:00 PM ✅
- Horario 4: 22:00 PM ✅

## 🎮 Uso de los Botones

## 📺 Uso del LCD 20x4

### 🏠 **Pantalla Principal (Reloj)**
```
┌────────────────────┐
│    ALIMENTADOR     │
│     14:25:30       │
│   25/12/2024       │
│Proximo: 18:00 H3   │
└────────────────────┘
```
- **SELECT**: Entrar al menú principal
- **CONFIRM**: Alimentar manualmente
- **UP/DOWN**: (Sin función en esta pantalla)

### 📋 **Menú Principal**
```
┌────────────────────┐
│===== MENU =====    │
│>Ver Horarios       │
│ Editar Horario 1   │
│ Editar Horario 2   │
└────────────────────┘
```
- **UP/DOWN**: Navegar entre opciones
- **CONFIRM**: Seleccionar opción
- **SELECT (largo)**: Volver al reloj

### ⏰ **Editar Horario**
```
┌────────────────────┐
│=== EDITAR H1 ===   │
│Hora: [08]:00       │
│Estado: ON          │
│UP/DOWN SELECT CONF │
└────────────────────┘
```
- **UP/DOWN**: Cambiar valor (hora/minuto/estado)
- **SELECT**: Siguiente campo
- **CONFIRM**: Guardar cambios
- **SELECT (largo)**: Cancelar

### 📊 **Ver Horarios**
```
┌────────────────────┐
│==== HORARIOS ====  │
│H1: 08:00 ON   <    │
│H2: 12:00 ON        │
│H3: 18:00 ON        │
│H4: 22:00 ON        │
└────────────────────┘
```

### 🍽️ **Alimentando**
```
┌────────────────────┐
│==== ALIMENTANDO ===│
│                    │
│   Tiempo: 3s       │
│  SELECT: Parar     │
└────────────────────┘
```

### ⏰ **Ajustar Hora/Fecha**
```
┌────────────────────┐
│=== AJUSTAR HORA ===│
│Hora: [14]:25       │
│Fecha: 25/12/2024   │
│UP/DOWN SELECT CONF │
└────────────────────┘
```
- **UP/DOWN**: Cambiar valor del campo seleccionado
- **SELECT**: Siguiente campo (hora→minuto→día→mes→año→guardar)
- **CONFIRM**: Guardar cambios o siguiente campo
- **SELECT (largo)**: Cancelar y volver al menú

### 🎯 **Campos Editables:**
1. **Hora** (00-23)
2. **Minuto** (00-59)  
3. **Día** (01-31)
4. **Mes** (01-12)
5. **Año** (2000-2099)
6. **Guardar** (confirmar cambios)

### 🔧 **Funciones Avanzadas**
- **Pulsación larga UP/DOWN**: Repetición automática para cambios rápidos
- **Buzzer**: Confirma cada pulsación (si está conectado)
- **Timeout**: El menú vuelve al reloj automáticamente tras 30 segundos

## 💻 Comandos Seriales (Compatibilidad)

**🎮 ¡Ahora puedes usar los botones físicos!** Pero los comandos seriales siguen funcionando para usuarios avanzados.

Usa el Monitor Serial para interactuar con el sistema:

| Comando | Descripción | Ejemplo |
|---------|-------------|---------|
| `help` o `?` | Mostrar ayuda completa | `help` |
| `time` | Mostrar hora actual | `time` |
| `schedules` | Ver horarios programados | `schedules` |
| `status` | Ver estado completo del sistema | `status` |
| `feed` | Alimentar manualmente | `feed` |
| `stop` | Parada de emergencia | `stop` |
| `next` | Ver próximo horario | `next` |
| `set X HH:MM` | Configurar horario X | `set 1 07:30` |
| `set X off` | Deshabilitar horario X | `set 2 off` |
| `enable X` | Habilitar horario X | `enable 3` |
| `disable X` | Deshabilitar horario X | `disable 4` |
| `enable all` | Habilitar todos | `enable all` |
| `disable all` | Deshabilitar todos | `disable all` |
| `test relay` | Probar relay 3 segundos | `test relay` |
| `test led` | Probar LED (parpadeo) | `test led` |

### Ejemplos de Uso:

```
// Ver horarios actuales
schedules

// Cambiar el primer horario a 7:30 AM
set 1 07:30

// Deshabilitar el cuarto horario
set 4 off

// Alimentar manualmente
feed

// Ver la hora actual
time
```

## ⏰ Uso del Ajuste de Hora con Botones

### 🎯 **Cómo Ajustar la Hora Correcta:**

1. **Acceder al Ajuste:**
   - Desde la pantalla principal, presiona **SELECT**
   - Navega con **UP/DOWN** hasta "Ajustar Hora"
   - Presiona **CONFIRM** para entrar

2. **Navegar por los Campos:**
   - **Hora**: Usa UP/DOWN para cambiar (00-23)
   - **SELECT**: Pasar a minutos
   - **Minuto**: Usa UP/DOWN para cambiar (00-59)
   - **SELECT**: Pasar a día
   - **Día**: Usa UP/DOWN para cambiar (01-31)
   - **SELECT**: Pasar a mes
   - **Mes**: Usa UP/DOWN para cambiar (01-12)
   - **SELECT**: Pasar a año
   - **Año**: Usa UP/DOWN para cambiar (2000-2099)
   - **SELECT**: Ir a "Guardar"

3. **Guardar Cambios:**
   - Cuando llegues a "CONFIRM: Guardar"
   - Presiona **CONFIRM** para aplicar los cambios
   - El sistema validará la fecha automáticamente

4. **Cancelar (si es necesario):**
   - Mantén presionado **SELECT** (pulsación larga)
   - Volverás al menú sin guardar cambios

### 💡 **Consejos de Uso:**
- ✅ **Pulsación larga**: UP/DOWN mantenidos cambian rápidamente
- ✅ **Validación automática**: El sistema verifica fechas válidas
- ✅ **Visual**: Los corchetes `[14]` indican el campo activo
- ✅ **Sonido**: El buzzer confirma cada cambio
- ✅ **Seguro**: Siempre puedes cancelar con SELECT largo

### 📅 **Ejemplo de Ajuste:**
```
Hora actual incorrecta: 01:00  1/01/2024
Hora correcta deseada:  14:25  25/12/2024

Pasos:
1. SELECT → "Ajustar Hora" → CONFIRM
2. UP hasta 14 → SELECT (ir a minutos)
3. UP hasta 25 → SELECT (ir a día)  
4. UP hasta 25 → SELECT (ir a mes)
5. UP hasta 12 → SELECT (ir a año)
6. UP hasta 2024 → SELECT (ir a guardar)
7. CONFIRM → ¡Hora ajustada!
```

## ⚙️ Configuración Personalizada (Código Modular)

### Cambiar Configuraciones Básicas
Modifica el archivo `config.h`:

```cpp
// Cambiar pines
const int RELAY_PIN = 7;        // Pin del relay
const int LED_PIN = 13;         // Pin del LED

// Cambiar tiempos
const int FEED_DURATION = 5;    // Duración en segundos
const unsigned long TIME_DISPLAY_INTERVAL = 30000; // Mostrar hora cada 30s

// Cambiar horarios predeterminados
#define DEFAULT_SCHEDULE_1_HOUR   8   // 8:00 AM
#define DEFAULT_SCHEDULE_1_MINUTE 0
// ... etc para los otros horarios
```

### Personalizar Mensajes
También en `config.h`:
```cpp
#define MSG_FEED_START   "¡Hora de alimentar! Horario #"
#define MSG_FEED_COMPLETED "Alimentación completada"
// ... otros mensajes
```

### Ventajas del Código Modular:
- ✅ **Fácil personalización**: Solo modifica `config.h`
- ✅ **Código organizado**: Cada funcionalidad en su módulo
- ✅ **Fácil mantenimiento**: Cambios localizados
- ✅ **Reutilizable**: Los módulos se pueden usar en otros proyectos

## 🔍 Solución de Problemas

### El RTC no mantiene la hora
- Verifica que la batería CR2032 esté instalada y tenga carga
- Confirma las conexiones SDA (A4) y SCL (A5)

### El relay no activa
- Verifica la conexión del pin 7 al relay
- Confirma que el relay sea compatible con 5V
- Revisa las conexiones de alimentación del relay

### Error "No se pudo encontrar el RTC DS3231"
- Verifica las conexiones I2C (SDA y SCL)
- Confirma que el DS3231 esté alimentado correctamente
- Revisa que no haya cables sueltos

### El alimentador no funciona a tiempo
- Verifica que la hora del RTC esté correctamente configurada
- Confirma que los horarios estén habilitados (`schedules`)
- Revisa las conexiones del relay al mecanismo alimentador

## 📝 Notas Importantes

1. **Batería de Respaldo**: El DS3231 mantiene la hora incluso sin alimentación gracias a la batería CR2032.

2. **Precisión**: El DS3231 es muy preciso y compensa automáticamente la temperatura.

3. **Seguridad**: El relay se desactiva automáticamente después del tiempo programado para evitar sobrealimentación.

4. **Monitoreo**: El sistema muestra la hora actual cada 30 segundos en el Monitor Serial.

5. **Alimentación Manual**: Puedes activar el alimentador manualmente con el comando `feed`.

## 🐟 Recomendaciones de Uso

- **Frecuencia**: 2-4 veces al día es suficiente para la mayoría de peces
- **Cantidad**: Ajusta la duración según la cantidad de comida que dispense tu mecanismo
- **Mantenimiento**: Limpia regularmente el mecanismo alimentador
- **Backup**: Anota los horarios configurados por si necesitas reprogramar

## 📄 Licencia

Este proyecto es de código abierto. Puedes modificarlo y distribuirlo libremente.

---

**¡Disfruta de tu alimentador automático y tus peces estarán siempre bien alimentados! 🐠🕐**
