# 📥 Guía de Instalación - Alimentador de Peces Modular

Esta guía te ayudará a instalar y configurar el sistema de alimentador de peces con código modular.

## 📋 Lista de Verificación Pre-Instalación

- [ ] Arduino Uno disponible
- [ ] Módulo DS3231 RTC
- [ ] Módulo Relay 5V
- [ ] LED y resistencia 220Ω (opcional)
- [ ] Cables jumper
- [ ] Arduino IDE instalado

## 🔽 Paso 1: Descargar Archivos

Asegúrate de tener todos estos archivos en la misma carpeta:

```
📁 alimentador-de-peces/
├── alimentador_peces.ino    ⭐ PRINCIPAL
├── config.h                 🔧 CONFIGURACIÓN
├── rtc_manager.h           🕐 RELOJ
├── relay_controller.h      🔌 RELAY
├── schedule_manager.h      ⏰ HORARIOS
├── serial_commands.h       💬 COMANDOS
├── conexiones.md           📊 DIAGRAMA
├── README.md              📖 DOCUMENTACIÓN
└── INSTALACION.md         📥 ESTA GUÍA
```

## 🔧 Paso 2: Instalar Librerías

1. **Abrir Arduino IDE**
2. **Ir a**: Herramientas → Administrar librerías
3. **Buscar**: "RTClib"
4. **Instalar**: RTClib by Adafruit (versión más reciente)

```
✅ RTClib by Adafruit - INSTALADA
✅ Wire - Ya incluida en Arduino IDE
```

## 🔌 Paso 3: Realizar Conexiones

Sigue el diagrama en `conexiones.md`:

### Conexiones Rápidas:
```
DS3231 → Arduino Uno
VCC → 5V
GND → GND  
SDA → A4
SCL → A5

Relay → Arduino Uno
VCC → 5V
GND → GND
IN  → Pin 7

LED (opcional) → Arduino Uno
Ánodo (+) → Pin 13 (con resistencia 220Ω)
Cátodo(-) → GND
```

## 💾 Paso 4: Cargar el Código

1. **Abrir Arduino IDE**
2. **Archivo → Abrir**: Selecciona `alimentador_peces.ino`
3. **Verificar**: Todos los archivos .h aparecen como pestañas
4. **Herramientas → Placa**: Arduino Uno
5. **Herramientas → Puerto**: Selecciona el puerto correcto
6. **Subir** (botón →)

### ⚠️ Errores Comunes:
- **"No such file"**: Los archivos .h no están en la misma carpeta
- **"Port not found"**: Arduino no conectado o driver faltante
- **"Compilation error"**: Librería RTClib no instalada

## 🧪 Paso 5: Verificar Funcionamiento

1. **Abrir Monitor Serial** (9600 baudios)
2. **Debería mostrar**:
   ```
   === Alimentador de Peces Iniciado ===
   Horarios programados:
   Horario 1: 08:00
   Horario 2: 12:00
   Horario 3: 18:00
   Horario 4: 22:00
   Duración de alimentación: 5 segundos
   Escribe 'help' para ver comandos disponibles
   =====================================
   ```

3. **Probar comandos básicos**:
   ```
   time      ← Ver hora actual
   status    ← Ver estado del sistema
   feed      ← Alimentar manualmente (prueba)
   ```

## ⚙️ Paso 6: Personalización Inicial

### Configurar Hora (si es necesario):
El sistema configura automáticamente la hora de compilación, pero puedes verificar con:
```
time
```

### Cambiar Horarios:
```
set 1 07:30    ← Cambiar primer horario a 7:30 AM
set 2 12:30    ← Cambiar segundo horario a 12:30 PM
schedules      ← Ver todos los horarios
```

### Cambiar Duración:
Edita `config.h` y cambia:
```cpp
const int FEED_DURATION = 5; // Cambiar a los segundos deseados
```

## 🔧 Personalización Avanzada

### Cambiar Pines:
En `config.h`:
```cpp
const int RELAY_PIN = 7;  // Cambiar pin del relay
const int LED_PIN = 13;   // Cambiar pin del LED
```

### Cambiar Horarios Predeterminados:
En `config.h`:
```cpp
#define DEFAULT_SCHEDULE_1_HOUR   6    // 6:00 AM
#define DEFAULT_SCHEDULE_1_MINUTE 0
// ... cambiar otros horarios
```

### Personalizar Mensajes:
En `config.h`:
```cpp
#define MSG_FEED_START "¡A comer pescaditos! Horario #"
#define MSG_FEED_COMPLETED "Alimentación terminada"
```

## 🚨 Solución de Problemas

### Problema: "RTC no encontrado"
**Solución:**
- Verificar conexiones SDA (A4) y SCL (A5)
- Verificar alimentación del DS3231 (5V y GND)
- Probar con otro DS3231

### Problema: "Relay no funciona"
**Solución:**
- Verificar conexión del pin 7 al relay
- Probar comando: `test relay`
- Verificar alimentación del relay (5V)

### Problema: "Horarios no funcionan"
**Solución:**
- Verificar hora actual: `time`
- Verificar horarios: `schedules`
- Verificar que los horarios estén habilitados

### Problema: "LED no parpadea"
**Solución:**
- Verificar resistencia 220Ω en serie
- Verificar polaridad del LED
- Probar comando: `test led`

## 📞 Comandos de Diagnóstico

```
status      ← Estado completo del sistema
time        ← Hora actual del RTC
schedules   ← Todos los horarios programados
next        ← Próximo horario de alimentación
test relay  ← Probar relay por 3 segundos
test led    ← Probar LED (parpadeo)
help        ← Lista completa de comandos
```

## ✅ Lista de Verificación Final

- [ ] Código cargado sin errores
- [ ] Monitor serial muestra mensaje de inicio
- [ ] Comando `time` muestra hora correcta
- [ ] Comando `feed` activa relay y LED
- [ ] Comando `schedules` muestra horarios
- [ ] LED parpadea con `test led`
- [ ] Relay funciona con `test relay`

## 🎉 ¡Listo!

Tu alimentador de peces modular está funcionando. El sistema:

- ⏰ Alimentará automáticamente en los horarios programados
- 🔧 Se puede configurar completamente via comandos seriales
- 📊 Muestra estado y próximos horarios
- 🛡️ Incluye funciones de seguridad y prueba

**¡Tus peces estarán siempre bien alimentados!** 🐠🍽️

---

*Para más información detallada, consulta el archivo `README.md`*
