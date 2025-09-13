# 🆘 **SOLUCIÓN DE PROBLEMAS - ALIMENTADOR DE PECES**

## 🎯 **GUÍA DE DIAGNÓSTICO**

Esta guía te ayudará a identificar y solucionar los problemas más comunes del alimentador automático.

---

## 🔴 **PROBLEMAS DE HARDWARE**

### **📱 LCD NO FUNCIONA**

#### **🔍 Síntomas:**
- Pantalla completamente negra
- Caracteres extraños o ilegibles
- Parpadeo excesivo
- No responde a cambios

#### **✅ Soluciones:**

**1. Verificar Conexiones I2C:**
```
LCD VCC  → Arduino 5V (🔴 ROJO)
LCD GND  → Arduino GND (⚫ NEGRO)
LCD SDA  → Arduino A4 (🟠 NARANJA)
LCD SCL  → Arduino A5 (🟡 AMARILLO)
```

**2. Probar Dirección I2C:**
- Cambiar en `config.h`:
```cpp
const int LCD_ADDRESS = 0x3F;  // Prueba esta
// o
const int LCD_ADDRESS = 0x20;  // O esta
```

**3. Usar Diagnóstico:**
- Subir `diagnostico_lcd.ino`
- Verificar que muestre "LCD FUNCIONANDO"

**4. Verificar Alimentación:**
- Medir 5V en VCC del LCD
- Verificar GND común
- Revisar conexiones sueltas

---

### **🎮 BOTONES NO RESPONDEN**

#### **🔍 Síntomas:**
- No hay beeps al presionar
- Menú no cambia
- Navegación no funciona

#### **✅ Soluciones:**

**1. Verificar Conexiones:**
```
SELECT   → Arduino Pin 2 (🔵 AZUL)
UP       → Arduino Pin 3 (🟣 MORADO)
DOWN     → Arduino Pin 4 (🔘 GRIS)
CONFIRM  → Arduino Pin 5 (⚪ BLANCO)
```

**2. Probar con Multímetro:**
- Medir continuidad en botones
- Verificar que cierren correctamente
- Revisar soldaduras

**3. Verificar Configuración:**
- Revisar pines en `config.h`
- Confirmar que coincidan con conexiones

**4. Limpiar Contactos:**
- Limpiar botones con alcohol
- Verificar que no estén pegados

---

### **⏰ RTC NO FUNCIONA**

#### **🔍 Síntomas:**
- Hora incorrecta o reseteada
- Fecha incorrecta
- No mantiene tiempo al reiniciar

#### **✅ Soluciones:**

**1. Verificar Conexiones I2C:**
```
RTC VCC  → Arduino 5V (🔴 ROJO)
RTC GND  → Arduino GND (⚫ NEGRO)
RTC SDA  → Arduino A4 (🟠 NARANJA)
RTC SCL  → Arduino A5 (🟡 AMARILLO)
```

**2. Verificar Batería:**
- Medir voltaje de batería CR2032
- Debe ser > 3V
- Cambiar si es necesario

**3. Ajustar Hora:**
- Usar menú "Ajustar Hora"
- Configurar fecha y hora correctas
- Verificar que se guarde

**4. Verificar Librería:**
- Instalar RTClib correctamente
- Verificar versión compatible

---

### **⚡ RELÉ NO FUNCIONA**

#### **🔍 Síntomas:**
- No hay click al activar
- Motor no se mueve
- LED no se enciende

#### **✅ Soluciones:**

**1. Verificar Conexiones:**
```
Relé IN  → Arduino Pin 6 (🟢 VERDE)
Relé VCC → Fuente 12V (🔴 ROJO)
Relé GND → GND (⚫ NEGRO)
```

**2. Verificar Alimentación:**
- Medir 12V en relé
- Verificar GND común
- Revisar fusibles

**3. Probar Relé:**
- Medir continuidad en contactos
- Verificar que active con 5V
- Revisar diodo flyback

**4. Verificar Motor:**
- Probar motor directamente con 12V
- Verificar que gire libremente
- Revisar conexiones del motor

---

## 🔴 **PROBLEMAS DE SOFTWARE**

### **💻 COMPILACIÓN FALLA**

#### **🔍 Síntomas:**
- Errores de compilación
- Librerías faltantes
- Memoria insuficiente

#### **✅ Soluciones:**

**1. Instalar Librerías:**
```
RTClib by Adafruit
LiquidCrystal I2C by Frank de Brabander
```

**2. Verificar Sintaxis:**
- Revisar llaves y paréntesis
- Verificar puntos y comas
- Revisar nombres de variables

**3. Optimizar Memoria:**
- Desactivar mensajes seriales
- Reducir strings largos
- Usar F() macro para strings

**4. Verificar Configuración:**
- Seleccionar placa correcta
- Verificar puerto COM
- Revisar configuración de placa

---

### **🔄 SISTEMA NO RESPONDE**

#### **🔍 Síntomas:**
- Arduino se cuelga
- LCD no se actualiza
- Botones no responden

#### **✅ Soluciones:**

**1. Reiniciar Arduino:**
- Desconectar y reconectar USB
- Presionar botón reset
- Verificar alimentación

**2. Verificar Loop Principal:**
- Revisar que no haya bucles infinitos
- Verificar delays excesivos
- Revisar manejo de errores

**3. Verificar Memoria:**
- Monitorear uso de RAM
- Evitar variables globales grandes
- Usar PROGMEM para constantes

**4. Debugging:**
- Activar mensajes seriales temporalmente
- Verificar flujo del programa
- Revisar estados de módulos

---

### **⏰ HORARIOS NO SE EJECUTAN**

#### **🔍 Síntomas:**
- No alimenta automáticamente
- Horarios no se activan
- Tiempo incorrecto

#### **✅ Soluciones:**

**1. Verificar Hora del RTC:**
- Ajustar hora correctamente
- Verificar que se mantenga
- Revisar batería del RTC

**2. Activar Horarios:**
- Verificar que estén ON
- Configurar horarios válidos
- Revisar en menú "Ver Horarios"

**3. Verificar Lógica:**
- Revisar función `checkScheduledFeeding()`
- Verificar comparación de tiempo
- Revisar estado del relé

**4. Probar Manualmente:**
- Usar "Alimentar Ahora"
- Verificar que funcione
- Revisar configuración de duración

---

## 🔴 **PROBLEMAS DE CONFIGURACIÓN**

### **⚙️ CONFIGURACIÓN INCORRECTA**

#### **🔍 Síntomas:**
- Comportamiento inesperado
- Valores incorrectos
- Funciones no disponibles

#### **✅ Soluciones:**

**1. Verificar config.h:**
- Revisar pines de hardware
- Verificar direcciones I2C
- Confirmar timeouts

**2. Ajustar Parámetros:**
- Modificar duración de alimentación
- Ajustar timeouts de menú
- Configurar delays apropiados

**3. Reiniciar Sistema:**
- Subir código actualizado
- Reiniciar Arduino
- Verificar cambios

---

## 🧪 **HERRAMIENTAS DE DIAGNÓSTICO**

### **📱 Diagnóstico LCD:**
```cpp
// Usar diagnostico_lcd.ino
// Verifica funcionamiento básico del LCD
// Prueba diferentes direcciones I2C
```

### **🔍 Scanner I2C:**
```cpp
// Buscar dispositivos I2C conectados
// Verificar direcciones correctas
// Detectar problemas de comunicación
```

### **📊 Monitor Serial:**
```cpp
// Activar temporalmente SERIAL_ENABLED = true
// Verificar mensajes de debug
// Monitorear flujo del programa
```

---

## 🎯 **PROCESO DE DIAGNÓSTICO**

### **📋 Pasos Generales:**

1. **🔍 Identificar Síntoma:**
   - ¿Qué no funciona exactamente?
   - ¿Cuándo empezó el problema?
   - ¿Qué cambió recientemente?

2. **🔧 Verificar Hardware:**
   - Conexiones físicas
   - Alimentación
   - Componentes

3. **💻 Verificar Software:**
   - Configuración
   - Código
   - Librerías

4. **🧪 Probar Soluciones:**
   - Una a la vez
   - Documentar resultados
   - Verificar funcionamiento

5. **✅ Confirmar Reparación:**
   - Probar todas las funciones
   - Verificar estabilidad
   - Documentar solución

---

## 🆘 **CUANDO BUSCAR AYUDA**

### **🔴 Problemas Complejos:**
- Múltiples componentes fallando
- Problemas intermitentes
- Errores de hardware

### **🔴 Problemas de Diseño:**
- Modificaciones importantes
- Nuevas funcionalidades
- Optimizaciones avanzadas

### **🔴 Problemas de Fabricación:**
- Componentes defectuosos
- Problemas de soldadura
- Fallas de alimentación

---

## 🎊 **MANTENIMIENTO PREVENTIVO**

### **📅 Tareas Regulares:**

#### **🔄 Mensual:**
- Limpiar motor alimentador
- Verificar conexiones
- Probar todas las funciones

#### **📅 Trimestral:**
- Revisar batería RTC
- Limpiar contactos de botones
- Verificar alimentación

#### **📅 Anual:**
- Cambiar batería RTC
- Revisar soldaduras
- Actualizar software

---

**🛠️ Con esta guía podrás diagnosticar y solucionar la mayoría de problemas del alimentador automático.** 🐠✨
