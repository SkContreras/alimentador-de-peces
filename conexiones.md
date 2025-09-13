# Diagrama de Conexiones - Alimentador de Peces v3.0 (Con LCD y Botones)

## Componentes Necesarios
- Arduino Uno
- Módulo RTC DS3231
- Módulo Relay (5V)
- **🆕 LCD 20x4 2004 con módulo I2C (fondo azul)**
- 4 Botones pulsadores (Normalmente Abiertos)
- LED indicador (opcional)
- Buzzer 5V (opcional)
- Resistencia 220Ω (para LED)
- Resistencias 10kΩ x4 (pull-up para botones, opcional si usas INPUT_PULLUP)
- Cables jumper
- Protoboard o PCB

## Conexiones I2C (DS3231 y LCD) → Arduino Uno
```
Dispositivo I2C    Arduino Uno
---------------    -----------
DS3231 VCC     →   5V
DS3231 GND     →   GND  
DS3231 SDA     →   A4 (pin analógico 4)
DS3231 SCL     →   A5 (pin analógico 5)

LCD I2C VCC    →   5V
LCD I2C GND    →   GND
LCD I2C SDA    →   A4 (pin analógico 4) [COMPARTIDO]
LCD I2C SCL    →   A5 (pin analógico 5) [COMPARTIDO]
```

     **Nota**: El DS3231 y el LCD comparten el bus I2C (SDA/SCL).

## Conexiones Relay → Arduino Uno
```
Relay     Arduino Uno
-----     -----------
VCC   →   5V
GND   →   GND
IN    →   Pin 7 (digital)
```

## Conexiones Botones → Arduino Uno
```
Botón SELECT    Arduino Uno
------------    -----------
Terminal 1  →   Pin 2 (digital)
Terminal 2  →   GND

Botón UP        Arduino Uno  
--------        -----------
Terminal 1  →   Pin 3 (digital)
Terminal 2  →   GND

Botón DOWN      Arduino Uno
----------      -----------
Terminal 1  →   Pin 4 (digital)
Terminal 2  →   GND

Botón CONFIRM   Arduino Uno
-------------   -----------
Terminal 1  →   Pin 5 (digital)
Terminal 2  →   GND
```

**Nota**: Se usa INPUT_PULLUP interno, no necesitas resistencias externas.

## Conexiones LED Indicador → Arduino Uno (Opcional)
```
LED       Arduino Uno
---       -----------
Ánodo (+) → Pin 13 (digital) → Resistencia 220Ω
Cátodo(-) → GND
```

## Conexiones Buzzer → Arduino Uno (Opcional)
```
Buzzer    Arduino Uno
------    -----------
Positivo  → Pin 6 (digital)
Negativo  → GND
```

## Conexiones del Relay a la Carga
```
Relay Terminals:
- COM (Común): Conectar a la fuente de alimentación del motor/bomba
- NO (Normalmente Abierto): Conectar al motor/bomba del alimentador
- NC (Normalmente Cerrado): No usar (dejar libre)
```

## Diagrama Visual

```
                         Arduino Uno
                    ┌─────────────────────┐
                    │                     │
             5V ────│ 5V              A4  │──── SDA (DS3231 + LCD I2C)
            GND ────│ GND             A5  │──── SCL (DS3231 + LCD I2C)
                    │                     │
 BTN_SELECT ────────│ 2                7  │──── Relay IN
 BTN_UP ────────────│ 3               13  │──── LED (+ resistencia)
 BTN_DOWN ──────────│ 4                6  │──── Buzzer (opcional)
 BTN_CONFIRM ───────│ 5                   │
                    │                     │
                    └─────────────────────┘
                            │
                        Bus I2C
                    ┌───────┴───────┐
                    │               │
                    ▼               ▼
            DS3231 RTC          LCD 20x4 I2C
           ┌───────────┐       ┌─────────────┐
           │  VCC  SDA │       │ VCC     SDA │
           │  GND  SCL │       │ GND     SCL │
           └───────────┘       └─────────────┘

    Botones (x4)                              Relay Module
   ┌─────────────┐                           ┌─────────────┐
   │ SELECT  UP  │                           │ VCC  IN  COM│
   │ DOWN  CONF  │                           │ GND      NO │
   │    (GND)    │                           │          NC │
   └─────────────┘                           └─────────────┘
                                                     │
                                                 Motor/Bomba
                                                 Alimentador
```

## Notas Importantes

1. **Alimentación**: El DS3231 puede funcionar con 3.3V o 5V, pero se recomienda 5V para mayor estabilidad.

2. **Relay**: Asegúrate de que el relay pueda manejar la corriente de tu motor/bomba alimentadora.

3. **Batería RTC**: El DS3231 incluye una batería CR2032 para mantener la hora cuando se desconecta la alimentación.

4. **Botones**: Se usa INPUT_PULLUP interno del Arduino, no necesitas resistencias pull-up externas.

5. **Buzzer**: Es opcional, proporciona retroalimentación sonora al presionar botones.

6. **Resistencia Pull-up**: Los pines SDA y SCL del DS3231 ya incluyen resistencias pull-up internas.

7. **Protección**: Considera agregar un diodo flyback si controlas motores DC para proteger el relay.

8. **LCD I2C**: La dirección I2C por defecto es 0x27, pero puede ser 0x3F. Si no funciona, prueba cambiando en config.h.

9. **Bus I2C**: El DS3231 y LCD comparten el mismo bus I2C (SDA/SCL), esto es normal y funciona perfectamente.

10. **Pines Libres**: Quedan libres los pines 8, 9, 10, 11, 12, A0, A1, A2, A3 para expansiones futuras.

## Verificación de Conexiones

Antes de cargar el código:
1. Verifica que todas las conexiones estén firmes
2. Confirma la polaridad del LED y la alimentación
3. Asegúrate de que el relay esté correctamente conectado
4. Verifica que el DS3231 esté bien conectado a los pines I2C (A4, A5)
5. **NUEVO**: Prueba que cada botón funcione correctamente
6. **NUEVO**: Verifica la polaridad del buzzer (si lo usas)
7. **NUEVO**: Asegúrate de que los botones estén conectados a los pines correctos:
   - SELECT → Pin 2
   - UP → Pin 3  
   - DOWN → Pin 4
   - CONFIRM → Pin 5
8. **NUEVO**: Verifica las conexiones I2C del LCD:
   - VCC → 5V
   - GND → GND
   - SDA → A4 (compartido con DS3231)
   - SCL → A5 (compartido con DS3231)

## Pruebas del Sistema

Una vez cargado el código, puedes probar:

### 📺 **Pruebas del LCD:**
1. **Pantalla de inicio**: Debería mostrar "ALIMENTADOR DE PECES v3.0"
2. **Reloj principal**: Hora, fecha y próximo horario
3. **Retroiluminación**: Fondo azul encendido

### 🎮 **Pruebas de Botones:**
1. **SELECT**: Debería abrir el menú desde la pantalla del reloj
2. **UP/DOWN**: Deberían navegar por las opciones del menú
3. **CONFIRM**: Debería confirmar selecciones
4. **Pulsación larga**: SELECT mantenido debería salir del menú
5. **Buzzer**: Debería sonar al presionar botones (si está conectado)

### 🔧 **Solución de Problemas LCD:**
- **LCD en blanco**: Verifica alimentación 5V y GND
- **Caracteres extraños**: Verifica conexiones SDA/SCL
- **No responde**: Cambia dirección I2C en config.h (0x27 → 0x3F)
- **Sin retroiluminación**: Verifica módulo I2C del LCD
