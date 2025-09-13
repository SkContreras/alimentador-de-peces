/*
  lcd_display.h - Controlador para LCD 20x4 I2C
  
  Este módulo maneja específicamente la pantalla LCD 20x4 con interfaz I2C
  para mostrar menús, hora, estado y configuraciones del alimentador.
*/

#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <LiquidCrystal_I2C.h>
#include "config.h"
#include "rtc_manager.h"
#include "schedule_manager.h"
#include "relay_controller.h"

// Verificar compatibilidad de la librería LCD
#if defined(ARDUINO_ARCH_AVR)
  // Para Arduino AVR, usar implementación compatible
  #define LCD_COMPATIBLE_MODE true
#else
  #define LCD_COMPATIBLE_MODE false
#endif

class LCDDisplay {
private:
  LiquidCrystal_I2C lcd;
  bool isInitialized;
  unsigned long lastUpdate;
  
  // Referencias a otros módulos
  RTCManager* rtcManager;
  ScheduleManager* scheduleManager;
  RelayController* relayController;

public:
  // Constructor
  LCDDisplay(RTCManager* rtc, ScheduleManager* schedule, RelayController* relay) 
    : lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS), isInitialized(false), lastUpdate(0),
      rtcManager(rtc), scheduleManager(schedule), relayController(relay) {
    // Inicialización adicional para compatibilidad AVR
    #if defined(ARDUINO_ARCH_AVR)
      // Configuración específica para AVR
    #endif
  }

  // Inicializar LCD
  bool begin() {
    if (!USE_LCD) return false;
    
    #if defined(ARDUINO_ARCH_AVR)
      // Inicialización específica para AVR
      Wire.begin();
      delay(100);
    #endif
    
    lcd.init();
    lcd.backlight();
    lcd.clear();
    
    // Mostrar pantalla de inicio
    showBootScreen();
    delay(2000);
    
    isInitialized = true;
    Serial.println("LCD 20x4 I2C inicializado correctamente");
    return true;
  }

  // Verificar si el LCD está inicializado
  bool isReady() {
    return isInitialized && USE_LCD;
  }

  // Limpiar pantalla
  void clear() {
    if (!isReady()) return;
    lcd.clear();
  }

  // Mostrar pantalla de inicio
  void showBootScreen() {
    if (!isReady()) return;
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   ALIMENTADOR DE   ");
    lcd.setCursor(0, 1);
    lcd.print("    PECES v2.0      ");
    lcd.setCursor(0, 2);
    lcd.print("  Con LCD y Botones ");
    lcd.setCursor(0, 3);
    lcd.print("   Iniciando...     ");
  }

  // Mostrar reloj principal
  void showClock() {
    if (!isReady()) return;
    
    DateTime now = rtcManager->now();
    
    // Línea 1: Título
    lcd.setCursor(0, 0);
    lcd.print("    ALIMENTADOR     ");
    
    // Línea 2: Hora grande
    lcd.setCursor(0, 1);
    lcd.print("    ");
    printTwoDigits(now.hour());
    lcd.print(":");
    printTwoDigits(now.minute());
    lcd.print(":");
    printTwoDigits(now.second());
    lcd.print("    ");
    
    // Línea 3: Fecha
    lcd.setCursor(0, 2);
    lcd.print("   ");
    printTwoDigits(now.day());
    lcd.print("/");
    printTwoDigits(now.month());
    lcd.print("/");
    lcd.print(now.year());
    lcd.print("   ");
    
    // Línea 4: Estado o próximo horario
    lcd.setCursor(0, 3);
    if (relayController->isFeedingActive()) {
      lcd.print("  ALIMENTANDO ");
      lcd.print(relayController->getRemainingFeedTime());
      lcd.print("s  ");
    } else {
      int nextSchedule = scheduleManager->getNextSchedule(*rtcManager);
      if (nextSchedule > 0) {
        FeedTime next = scheduleManager->getSchedule(nextSchedule);
        lcd.print("Proximo: ");
        printTwoDigits(next.hour);
        lcd.print(":");
        printTwoDigits(next.minute);
        lcd.print(" H");
        lcd.print(nextSchedule);
      } else {
        lcd.print("  Sin horarios      ");
      }
    }
  }

  // Mostrar menú principal
  void showMainMenu(int selectedOption) {
    if (!isReady()) return;
    
    String options[] = {"", "Ver Horarios", "Editar Horario 1", "Editar Horario 2", 
                       "Editar Horario 3", "Editar Horario 4", "Alimentar Ahora", 
                       "Ver Estado", "Ajustar Hora", "Salir"};
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("===== MENU =====");
    
    // Mostrar 3 opciones centradas alrededor de la seleccionada
    int startOption = max(1, min(selectedOption - 1, 7));
    
    for (int i = 0; i < 3 && (startOption + i) <= 9; i++) {
      lcd.setCursor(0, i + 1);
      
      if (startOption + i == selectedOption) {
        lcd.print(">");
      } else {
        lcd.print(" ");
      }
      
      String option = options[startOption + i];
      if (option.length() > 18) {
        option = option.substring(0, 18);
      }
      lcd.print(option);
      
      // Limpiar resto de la línea
      for (int j = option.length() + 1; j < 20; j++) {
        lcd.print(" ");
      }
    }
  }

  // Mostrar horarios
  void showSchedules() {
    if (!isReady()) return;
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("==== HORARIOS ====");
    
    for (int i = 0; i < min(3, MAX_FEED_TIMES); i++) {
      FeedTime schedule = scheduleManager->getSchedule(i + 1);
      
      lcd.setCursor(0, i + 1);
      lcd.print("H");
      lcd.print(i + 1);
      lcd.print(": ");
      
      if (schedule.enabled) {
        printTwoDigits(schedule.hour);
        lcd.print(":");
        printTwoDigits(schedule.minute);
        lcd.print(" ON ");
      } else {
        lcd.print("--:-- OFF");
      }
      
      // Mostrar próximo indicador si es el siguiente
      int nextSchedule = scheduleManager->getNextSchedule(*rtcManager);
      if (nextSchedule == i + 1) {
        lcd.print(" <");
      } else {
        lcd.print("  ");
      }
    }
    
    // Mostrar horario 4 en la línea 4 si existe
    if (MAX_FEED_TIMES > 3) {
      FeedTime schedule = scheduleManager->getSchedule(4);
      lcd.setCursor(0, 3);
      lcd.print("H4: ");
      
      if (schedule.enabled) {
        printTwoDigits(schedule.hour);
        lcd.print(":");
        printTwoDigits(schedule.minute);
        lcd.print(" ON ");
      } else {
        lcd.print("--:-- OFF");
      }
      
      int nextSchedule = scheduleManager->getNextSchedule(*rtcManager);
      if (nextSchedule == 4) {
        lcd.print(" <");
      }
    }
  }

  // Mostrar editor de horario
  void showScheduleEditor(int scheduleNumber, int hour, int minute, bool enabled, int cursorPos) {
    if (!isReady()) return;
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("=== EDITAR H");
    lcd.print(scheduleNumber);
    lcd.print(" ===");
    
    // Línea 2: Hora
    lcd.setCursor(0, 1);
    lcd.print("Hora: ");
    if (cursorPos == 0) lcd.print("[");
    printTwoDigits(hour);
    if (cursorPos == 0) lcd.print("]"); else lcd.print(" ");
    lcd.print(":");
    if (cursorPos == 1) lcd.print("[");
    printTwoDigits(minute);
    if (cursorPos == 1) lcd.print("]"); else lcd.print(" ");
    
    // Línea 3: Estado
    lcd.setCursor(0, 2);
    lcd.print("Estado: ");
    if (cursorPos == 2) lcd.print("[");
    lcd.print(enabled ? "ON " : "OFF");
    if (cursorPos == 2) lcd.print("]"); else lcd.print(" ");
    
    // Línea 4: Instrucciones
    lcd.setCursor(0, 3);
    if (cursorPos == 3) {
      lcd.print("CONFIRM: Guardar    ");
    } else {
      lcd.print("UP/DOWN SELECT CONF ");
    }
  }

  // Mostrar estado del sistema
  void showStatus() {
    if (!isReady()) return;
    
    DateTime now = rtcManager->now();
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("===== ESTADO =====");
    
    // Línea 2: Hora actual
    lcd.setCursor(0, 1);
    lcd.print("Hora: ");
    printTwoDigits(now.hour());
    lcd.print(":");
    printTwoDigits(now.minute());
    lcd.print(":");
    printTwoDigits(now.second());
    
    // Línea 3: Estado del relay
    lcd.setCursor(0, 2);
    if (relayController->isFeedingActive()) {
      lcd.print("Alimentando ");
      lcd.print(relayController->getRemainingFeedTime());
      lcd.print("s");
    } else {
      lcd.print("Relay: ");
      lcd.print(relayController->getRelayState() ? "ON " : "OFF");
    }
    
    // Línea 4: Horarios activos
    lcd.setCursor(0, 3);
    lcd.print("Activos: ");
    lcd.print(scheduleManager->getEnabledSchedulesCount());
    lcd.print("/");
    lcd.print(MAX_FEED_TIMES);
    
    int nextSchedule = scheduleManager->getNextSchedule(*rtcManager);
    if (nextSchedule > 0) {
      FeedTime next = scheduleManager->getSchedule(nextSchedule);
      lcd.print(" H");
      lcd.print(nextSchedule);
      lcd.print(":");
      printTwoDigits(next.hour);
      printTwoDigits(next.minute);
    }
  }

  // Mostrar mensaje temporal
  void showMessage(String title, String message, int duration = 2000) {
    if (!isReady()) return;
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(centerText(title, 20));
    
    lcd.setCursor(0, 2);
    lcd.print(centerText(message, 20));
    
    delay(duration);
  }

  // Mostrar mensaje de confirmación
  void showConfirmation(String message) {
    showMessage("*** EXITO ***", message, 1500);
  }

  // Mostrar mensaje de error
  void showError(String message) {
    showMessage("*** ERROR ***", message, 2000);
  }

  // Mostrar pantalla de alimentación
  void showFeeding(int remainingTime) {
    if (!isReady()) return;
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("==== ALIMENTANDO ===");
    
    lcd.setCursor(0, 1);
    lcd.print("                    ");
    
    lcd.setCursor(0, 2);
    lcd.print("   Tiempo: ");
    lcd.print(remainingTime);
    lcd.print("s     ");
    
    lcd.setCursor(0, 3);
    lcd.print("  SELECT: Parar     ");
  }

  // Mostrar pantalla de ajuste de hora/fecha
  void showTimeAdjust(int hour, int minute, int day, int month, int year, int cursorPos) {
    if (!isReady()) return;
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("=== AJUSTAR HORA ===");
    
    // Línea 2: Hora
    lcd.setCursor(0, 1);
    lcd.print("Hora: ");
    if (cursorPos == 0) lcd.print("[");
    printTwoDigits(hour);
    if (cursorPos == 0) lcd.print("]"); else lcd.print(" ");
    lcd.print(":");
    if (cursorPos == 1) lcd.print("[");
    printTwoDigits(minute);
    if (cursorPos == 1) lcd.print("]"); else lcd.print(" ");
    
    // Línea 3: Fecha
    lcd.setCursor(0, 2);
    lcd.print("Fecha: ");
    if (cursorPos == 2) lcd.print("[");
    printTwoDigits(day);
    if (cursorPos == 2) lcd.print("]"); else lcd.print(" ");
    lcd.print("/");
    if (cursorPos == 3) lcd.print("[");
    printTwoDigits(month);
    if (cursorPos == 3) lcd.print("]"); else lcd.print(" ");
    lcd.print("/");
    if (cursorPos == 4) lcd.print("[");
    lcd.print(year);
    if (cursorPos == 4) lcd.print("]"); else lcd.print(" ");
    
    // Línea 4: Instrucciones
    lcd.setCursor(0, 3);
    if (cursorPos == 5) {
      lcd.print("CONFIRM: Guardar    ");
    } else {
      lcd.print("UP/DOWN SELECT CONF ");
    }
  }

  // Mostrar cargando
  void showLoading(String message = "Procesando") {
    if (!isReady()) return;
    
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(centerText(message, 20));
    
    lcd.setCursor(0, 2);
    lcd.print("    ");
    
    for (int i = 0; i < 3; i++) {
      lcd.print(".");
      delay(300);
    }
    lcd.print("    ");
  }

  // Crear caracteres personalizados
  void createCustomChars() {
    if (!isReady()) return;
    
    // Carácter de pez (posición 0)
    byte fish[8] = {
      0b00000,
      0b00110,
      0b01001,
      0b11111,
      0b01001,
      0b00110,
      0b00000,
      0b00000
    };
    
    // Carácter de reloj (posición 1)
    byte clock[8] = {
      0b00000,
      0b01110,
      0b10001,
      0b10101,
      0b10011,
      0b01110,
      0b00000,
      0b00000
    };
    
    lcd.createChar(0, fish);
    lcd.createChar(1, clock);
  }

  // Mostrar caracteres personalizados
  void showCustomChar(int charIndex, int col, int row) {
    if (!isReady()) return;
    lcd.setCursor(col, row);
    lcd.write(charIndex);
  }

private:
  // Función auxiliar para imprimir números con dos dígitos
  void printTwoDigits(int number) {
    if (number < 10) {
      lcd.print("0");
    }
    lcd.print(number);
  }

  // Centrar texto en una línea
  String centerText(String text, int width) {
    if (text.length() >= width) {
      return text.substring(0, width);
    }
    
    int padding = (width - text.length()) / 2;
    String result = "";
    
    for (int i = 0; i < padding; i++) {
      result += " ";
    }
    result += text;
    
    while (result.length() < width) {
      result += " ";
    }
    
    return result;
  }
};

#endif // LCD_DISPLAY_H
