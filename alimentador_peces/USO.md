# 📖 **MANUAL DE USUARIO - ALIMENTADOR DE PECES**

## 🎯 **INTRODUCCIÓN**

Bienvenido al **Alimentador Automático de Peces**. Este manual te guiará paso a paso para usar todas las funciones del sistema.

---

## 🎮 **CONTROLES BÁSICOS**

### **🔘 Botones:**
- **🔵 SELECT**: Navegación y menús
- **🟣 UP**: Subir opciones/valores
- **🔘 DOWN**: Bajar opciones/valores
- **⚪ CONFIRM**: Confirmar selección

### **🔊 Feedback Sonoro:**
- **Beep corto**: Acción exitosa
- **Beep largo**: Confirmación importante
- **Beep doble**: Error o cancelación

---

## 🏠 **PANTALLA PRINCIPAL (RELOJ)**

### **📱 Información Mostrada:**
```
    ALIMENTADOR     
    14:30:25    
   15/12/2024   
Proximo: 08:00 H1
```

### **🎮 Controles:**
- **🔵 SELECT**: Entrar al menú principal
- **⚪ CONFIRM**: Alimentar ahora (5 segundos)
- **⏰ Automático**: Vuelve al reloj después de 30 segundos

---

## 📋 **MENÚ PRINCIPAL**

### **🚀 Acceso:**
1. **Presionar** 🔵 SELECT en pantalla principal
2. **Verás** el menú con 9 opciones

### **📱 Opciones del Menú:**
```
===== MENU =====
> Ver Horarios
  Editar Horario 1
  Editar Horario 2
  Editar Horario 3
  Editar Horario 4
  Alimentar Ahora
  Ver Estado
  Ajustar Hora
  Salir
```

### **🎮 Navegación:**
- **🟣 UP**: Subir en la lista
- **🔘 DOWN**: Bajar en la lista
- **⚪ CONFIRM**: Seleccionar opción
- **🔵 SELECT largo**: Volver al reloj

---

## ⏰ **GESTIÓN DE HORARIOS**

### **👀 Ver Horarios:**
```
==== HORARIOS ====
H1: 08:00 ON <
H2: 12:00 ON  
H3: 18:00 ON  
H4: --:-- OFF
```

**Información:**
- **H1-H4**: Horarios 1-4
- **ON/OFF**: Estado activo/inactivo
- **<**: Próximo horario programado

### **✏️ Editar Horario:**

#### **📝 Campos Editables:**
```
=== EDITAR H1 ===
Hora: [08]:[00]
Estado: [ON ]
UP/DOWN SELECT CONF
```

#### **🎮 Controles:**
- **🟣 UP/🔘 DOWN**: Cambiar valor actual
- **🔵 SELECT**: Siguiente campo
- **⚪ CONFIRM**: Guardar cambios

#### **📋 Secuencia de Edición:**
1. **Hora**: 00-23
2. **Minutos**: 00-59
3. **Estado**: ON/OFF
4. **Guardar**: CONFIRM

---

## 🍽️ **ALIMENTACIÓN MANUAL**

### **⚡ Alimentar Ahora:**
1. **Menú** → "Alimentar Ahora" → **⚪ CONFIRM**
2. **Pantalla** muestra:
   ```
   ==== ALIMENTANDO ====
   
      Tiempo: 5s     
    SELECT: Parar    
   ```
3. **Duración**: 5 segundos (configurable)
4. **🔵 SELECT**: Parar alimentación

### **🎯 Desde Pantalla Principal:**
- **⚪ CONFIRM** directo = Alimentar ahora

---

## 📊 **ESTADO DEL SISTEMA**

### **📱 Información Mostrada:**
```
===== ESTADO =====
Hora: 14:30:25
Alimentando 3s
Activos: 3/4 H1:08:00
```

**Detalles:**
- **Hora actual** del RTC
- **Estado del relay** (ON/OFF)
- **Tiempo restante** si está alimentando
- **Horarios activos** (3 de 4)
- **Próximo horario** programado

---

## 🕐 **AJUSTE DE HORA Y FECHA**

### **📅 Acceso:**
1. **Menú** → "Ajustar Hora" → **⚪ CONFIRM**

### **📝 Pantalla de Edición:**
```
=== AJUSTAR HORA ===
Hora: [14]:[30]
Fecha: [15]/[12]/[2024]
CONFIRM: Guardar
```

### **🎮 Controles:**
- **🟣 UP/🔘 DOWN**: Cambiar valor actual
- **🔵 SELECT**: Siguiente campo
- **⚪ CONFIRM**: Guardar cambios

### **📋 Secuencia de Edición:**
1. **Hora**: 00-23
2. **Minutos**: 00-59
3. **Día**: 1-31
4. **Mes**: 1-12
5. **Año**: 2024-2099
6. **Guardar**: CONFIRM

---

## ⚙️ **CONFIGURACIÓN AVANZADA**

### **🕐 Horarios Automáticos:**
- **Máximo**: 4 horarios programables
- **Duración**: 5 segundos cada uno
- **Precisión**: ±1 segundo
- **Memoria**: Guardado en EEPROM

### **⏱️ Timeouts:**
- **Menú**: 30 segundos sin actividad
- **Alimentación**: 5 segundos (configurable)
- **Loop**: 100ms (optimización)

### **🔊 Sonidos:**
- **Beep corto**: Navegación
- **Beep largo**: Confirmación
- **Beep doble**: Error

---

## 🎯 **FLUJO DE TRABAJO TÍPICO**

### **🚀 Configuración Inicial:**
1. **Ajustar hora** → Menú → "Ajustar Hora"
2. **Configurar horarios** → Menú → "Editar Horario X"
3. **Verificar estado** → Menú → "Ver Estado"

### **📅 Uso Diario:**
1. **Verificar** pantalla principal
2. **Alimentar manual** si es necesario
3. **Revisar** próximos horarios

### **🔧 Mantenimiento:**
1. **Limpiar** motor alimentador (mensual)
2. **Verificar** batería RTC (anual)
3. **Revisar** conexiones (trimestral)

---

## 🆘 **SOLUCIÓN DE PROBLEMAS**

### **🔴 LCD no responde:**
- **Verificar** conexiones I2C
- **Reiniciar** Arduino
- **Probar** diagnóstico LCD

### **🔴 Botones no funcionan:**
- **Verificar** conexiones de pines
- **Limpiar** contactos de botones
- **Revisar** configuración

### **🔴 Horarios no se ejecutan:**
- **Verificar** hora del RTC
- **Activar** horarios en menú
- **Revisar** configuración de horarios

### **🔴 Motor no funciona:**
- **Verificar** conexiones del relé
- **Probar** alimentación 12V
- **Revisar** motor alimentador

---

## 💡 **CONSEJOS Y TRUCOS**

### **🎯 Uso Eficiente:**
- **Configura** horarios según hábitos de tus peces
- **Usa** alimentación manual para ajustes
- **Revisa** estado del sistema regularmente

### **🔧 Mantenimiento:**
- **Limpia** el motor mensualmente
- **Verifica** la batería del RTC anualmente
- **Mantén** las conexiones limpias

### **⚡ Optimización:**
- **Desactiva** horarios no utilizados
- **Ajusta** duración según necesidad
- **Monitorea** el consumo de comida

---

## 🎊 **¡DISFRUTA TU ALIMENTADOR!**

### **✅ Beneficios:**
- **Alimentación automática** precisa
- **Control manual** cuando necesites
- **Monitoreo** del estado del sistema
- **Configuración** flexible y fácil

### **🐠 Para Tus Peces:**
- **Horarios regulares** y consistentes
- **Cantidad controlada** de comida
- **Menos estrés** por alimentación irregular
- **Mejor salud** y crecimiento

---

**🎉 ¡Tu alimentador automático está listo para cuidar de tus peces!** 🐠✨

---

**📅 Última actualización**: Diciembre 2024  
**📖 Versión del manual**: 1.0  
**🐠 Para**: Alimentador Automático de Peces v3.0
