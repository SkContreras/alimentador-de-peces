# 🔌 **DIAGRAMA DE CONEXIONES - ALIMENTADOR DE PECES**

## 🎨 **CÓDIGOS DE COLORES**

### **📋 Especificación de Colores:**
- **⚫ NEGRO**: GND (Tierra)
- **🔴 ROJO**: VCC (5V)
- **🟡 AMARILLO**: SCL (I2C Clock)
- **🟠 NARANJA**: SDA (I2C Data)
- **🟢 VERDE**: Relé (Control)
- **⚪ BLANCO**: Botón CONFIRM
- **🔘 GRIS**: Botón DOWN
- **🟣 MORADO**: Botón UP
- **🔵 AZUL**: Botón SELECT

---

## 🔗 **CONEXIONES ARDUINO UNO**

### **📱 LCD 20x4 I2C:**
```
LCD VCC  → Arduino 5V     (🔴 ROJO)
LCD GND  → Arduino GND    (⚫ NEGRO)
LCD SDA  → Arduino A4     (🟠 NARANJA)
LCD SCL  → Arduino A5     (🟡 AMARILLO)
```

### **⏰ RTC DS3231:**
```
RTC VCC  → Arduino 5V     (🔴 ROJO)
RTC GND  → Arduino GND    (⚫ NEGRO)
RTC SDA  → Arduino A4     (🟠 NARANJA)
RTC SCL  → Arduino A5     (🟡 AMARILLO)
```

### **🎮 Botones (con pull-up interno):**
```
SELECT   → Arduino Pin 2  (🔵 AZUL)
UP       → Arduino Pin 3  (🟣 MORADO)
DOWN     → Arduino Pin 4  (🔘 GRIS)
CONFIRM  → Arduino Pin 5  (⚪ BLANCO)
```

### **⚡ Relé y LED:**
```
Relé IN  → Arduino Pin 6  (🟢 VERDE)
LED +    → Arduino Pin 7  (🟢 VERDE)
LED -    → Arduino GND    (⚫ NEGRO)
```

---

## 🔌 **CONEXIONES EXTERNAS**

### **🍽️ Motor Alimentador (12V):**
```
Motor +  → Relé COM       (12V DC)
Motor -  → Relé NO        (12V DC)
Relé VCC → Fuente 12V     (12V DC)
Relé GND → Fuente GND     (GND)
```

### **🔋 Alimentación:**
```
Arduino VIN → Fuente 5V   (5V DC)
Arduino GND → Fuente GND  (GND)
```

---

## 📊 **DIAGRAMA VISUAL**

```
┌─────────────────────────────────────────────────────────────┐
│                    ARDUINO UNO                              │
├─────────────────────────────────────────────────────────────┤
│  Pin 2  ←──🔵 AZUL──→  SELECT                              │
│  Pin 3  ←──🟣 MORADO→  UP                                  │
│  Pin 4  ←──🔘 GRIS──→  DOWN                                │
│  Pin 5  ←──⚪ BLANCO→  CONFIRM                             │
│  Pin 6  ←──🟢 VERDE→  RELÉ                                 │
│  Pin 7  ←──🟢 VERDE→  LED                                  │
│  A4     ←──🟠 NARANJA→ SDA (I2C)                          │
│  A5     ←──🟡 AMARILLO→ SCL (I2C)                         │
│  5V     ←──🔴 ROJO──→  VCC                                 │
│  GND    ←──⚫ NEGRO→  GND                                  │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                    COMPONENTES I2C                          │
├─────────────────────────────────────────────────────────────┤
│  LCD 20x4 I2C (0x27)                                       │
│  ├── VCC  ←──🔴 ROJO──→  Arduino 5V                        │
│  ├── GND  ←──⚫ NEGRO→  Arduino GND                        │
│  ├── SDA  ←──🟠 NARANJA→ Arduino A4                        │
│  └── SCL  ←──🟡 AMARILLO→ Arduino A5                       │
│                                                             │
│  RTC DS3231                                                 │
│  ├── VCC  ←──🔴 ROJO──→  Arduino 5V                        │
│  ├── GND  ←──⚫ NEGRO→  Arduino GND                        │
│  ├── SDA  ←──🟠 NARANJA→ Arduino A4                        │
│  └── SCL  ←──🟡 AMARILLO→ Arduino A5                       │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                    SISTEMA DE ALIMENTACIÓN                  │
├─────────────────────────────────────────────────────────────┤
│  Relé 5V                                                    │
│  ├── IN   ←──🟢 VERDE→  Arduino Pin 6                      │
│  ├── VCC  ←──🔴 ROJO──→  Fuente 12V                        │
│  ├── GND  ←──⚫ NEGRO→  GND                                │
│  ├── COM  ←──🟢 VERDE→  Motor +                            │
│  └── NO   ←──🟢 VERDE→  Motor -                            │
│                                                             │
│  Motor Alimentador 12V                                      │
│  ├── +    ←──🟢 VERDE→  Relé COM                           │
│  └── -    ←──🟢 VERDE→  Relé NO                            │
└─────────────────────────────────────────────────────────────┘
```

---

## 🔧 **DETALLES TÉCNICOS**

### **📱 LCD 20x4 I2C:**
- **Dirección**: 0x27 (común), 0x3F o 0x20 (alternativas)
- **Voltaje**: 5V
- **Comunicación**: I2C (SDA/SCL)
- **Backlight**: Incluido

### **⏰ RTC DS3231:**
- **Precisión**: ±2ppm (muy alta)
- **Batería**: CR2032 (2-3 años)
- **Voltaje**: 3.3V-5V
- **Comunicación**: I2C (SDA/SCL)

### **🎮 Botones:**
- **Tipo**: Momentáneos (NO)
- **Pull-up**: Interno del Arduino
- **Voltaje**: 5V
- **Debounce**: Software

### **⚡ Relé:**
- **Tipo**: 5V SPDT
- **Corriente**: 10A (suficiente para motor)
- **Voltaje**: 5V (coil), 12V (contactos)
- **Aislamiento**: Óptico

### **🍽️ Motor:**
- **Tipo**: DC 12V
- **Potencia**: 1-5W (típico)
- **Control**: Relé (ON/OFF)
- **Duración**: 5 segundos (configurable)

---

## ⚠️ **CONSIDERACIONES IMPORTANTES**

### **🔌 Conexiones I2C:**
- **SDA y SCL** deben estar conectados a **A4 y A5**
- **Pull-up resistors** incluidos en módulos
- **Máximo 2 dispositivos** I2C (LCD + RTC)

### **⚡ Alimentación:**
- **Arduino**: 5V DC (USB o externa)
- **Motor**: 12V DC (fuente separada)
- **Consumo total**: ~200mA (sin motor)

### **🛡️ Protecciones:**
- **Diodo flyback** en relé (recomendado)
- **Fusible** en línea de motor (opcional)
- **Condensador** de filtro en alimentación (opcional)

---

## 🧪 **VERIFICACIÓN DE CONEXIONES**

### **📋 Lista de Verificación:**

#### **✅ Alimentación:**
- [ ] Arduino conectado a 5V
- [ ] Motor conectado a 12V
- [ ] GND común entre fuentes

#### **✅ I2C:**
- [ ] LCD SDA → Arduino A4
- [ ] LCD SCL → Arduino A5
- [ ] RTC SDA → Arduino A4
- [ ] RTC SCL → Arduino A5
- [ ] VCC y GND conectados

#### **✅ Botones:**
- [ ] SELECT → Pin 2
- [ ] UP → Pin 3
- [ ] DOWN → Pin 4
- [ ] CONFIRM → Pin 5

#### **✅ Relé:**
- [ ] IN → Pin 6
- [ ] VCC → 12V
- [ ] GND → GND
- [ ] COM → Motor +
- [ ] NO → Motor -

---

## 🎯 **RESULTADO ESPERADO**

### **✅ Al Encender:**
- **LCD** muestra pantalla de inicio
- **RTC** mantiene hora correcta
- **Botones** responden con beeps
- **Relé** hace click al activar
- **Motor** se activa por 5 segundos

### **🐠 Sistema Funcional:**
- **Horarios automáticos** programables
- **Alimentación manual** instantánea
- **Menús interactivos** en LCD
- **Control completo** con botones

---

**🎊 ¡Conexiones completadas! Tu alimentador está listo para funcionar.** 🐠✨
