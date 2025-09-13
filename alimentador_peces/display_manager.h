/*
  display_manager.h - Gestor de pantalla y visualización
  
  Este módulo maneja la visualización de información tanto en:
  - LCD 20x4 I2C (implementado)
  - Monitor Serial (compatible)
  - LED indicadores
*/

#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include "config.h"
#include "rtc_manager.h"
#include "schedule_manager.h"
#include "relay_controller.h"
#include "lcd_display_avr.h"

// Estados de pantalla
enum DisplayMode {
  DISPLAY_CLOCK,        // Mostrar reloj
  DISPLAY_STATUS,       // Mostrar estado
  DISPLAY_MENU,         // Mostrar menú
  DISPLAY_SCHEDULE_VIEW, // Ver horarios
  DISPLAY_SCHEDULE_EDIT, // Editar horario
  DISPLAY_TIME_ADJUST   // Ajustar hora/fecha
};

class DisplayManager {
private:
  DisplayMode currentMode;
  unsigned long lastUpdate;
  bool needsUpdate;
  
  // LCD Display
  LCDDisplayAVR lcdDisplay;
  
  // Referencias a otros módulos
  RTCManager* rtcManager;
  ScheduleManager* scheduleManager;
  RelayController* relayController;

public:
  // Constructor
  DisplayManager(RTCManager* rtc, ScheduleManager* schedule, RelayController* relay) 
    : currentMode(DISPLAY_CLOCK), lastUpdate(0), needsUpdate(true),
      lcdDisplay(rtc, schedule, relay),
      rtcManager(rtc), scheduleManager(schedule), relayController(relay) {}

  // Inicializar display
  void begin() {
    Serial.println("\n=== DISPLAY INICIALIZADO ===");
    
    // Inicializar LCD
    if (lcdDisplay.begin()) {
      Serial.println("Modo: LCD 20x4 I2C + Serial");
      lcdDisplay.createCustomChars();
    } else {
      Serial.println("Modo: Solo Monitor Serial");
      Serial.println("LCD no detectado o deshabilitado");
    }
    
    Serial.println("============================\n");
    
    // Mostrar pantalla inicial
    showWelcomeScreen();
  }

  // Actualizar display si es necesario
  void update() {
    if (needsUpdate || (millis() - lastUpdate > DISPLAY_UPDATE_INTERVAL)) {
      refreshDisplay();
      lastUpdate = millis();
      needsUpdate = false;
    }
  }

  // Forzar actualización
  void forceUpdate() {
    needsUpdate = true;
  }

  // Cambiar modo de pantalla
  void setMode(DisplayMode mode) {
    if (currentMode != mode) {
      currentMode = mode;
      forceUpdate();
    }
  }

  // Obtener modo actual
  DisplayMode getMode() {
    return currentMode;
  }

  // Mostrar pantalla de bienvenida
  void showWelcomeScreen() {
    Serial.println(MSG_SYSTEM_START);
    Serial.println("🐠 ALIMENTADOR DE PECES v2.0 🐠");
    Serial.println("Versión con botones físicos");
    Serial.println("");
    scheduleManager->displaySchedulesCompact();
    Serial.print("Duración: ");
    Serial.print(FEED_DURATION);
    Serial.println(" segundos");
    Serial.println(MSG_SYSTEM_END);
    Serial.println("");
  }

  // Mostrar reloj principal
  void showClock() {
    if (currentMode != DISPLAY_CLOCK) return;
    
    // Mostrar en LCD
    if (lcdDisplay.isReady()) {
      lcdDisplay.showClock();
    }
    
    // También mostrar en Serial (compatible)
    DateTime now = rtcManager->now();
    Serial.print("\r🕐 ");
    printTwoDigits(now.hour());
    Serial.print(":");
    printTwoDigits(now.minute());
    Serial.print(":");
    printTwoDigits(now.second());
    
    if (relayController->isFeedingActive()) {
      Serial.print(" [ALIMENTANDO ");
      Serial.print(relayController->getRemainingFeedTime());
      Serial.print("s]");
    }
    
    int nextSchedule = scheduleManager->getNextSchedule(*rtcManager);
    if (nextSchedule > 0) {
      FeedTime next = scheduleManager->getSchedule(nextSchedule);
      Serial.print(" | Próximo: ");
      printTwoDigits(next.hour);
      Serial.print(":");
      printTwoDigits(next.minute);
    }
    Serial.print("                    \r");
  }

  // Mostrar estado del sistema
  void showStatus() {
    if (currentMode != DISPLAY_STATUS) return;
    
    Serial.println("\n=== ESTADO DEL SISTEMA ===");
    
    // Hora actual
    DateTime now = rtcManager->now();
    Serial.print("Hora: ");
    printTwoDigits(now.hour());
    Serial.print(":");
    printTwoDigits(now.minute());
    Serial.print(":");
    printTwoDigits(now.second());
    Serial.print(" - ");
    Serial.print(now.day());
    Serial.print("/");
    Serial.print(now.month());
    Serial.print("/");
    Serial.println(now.year());
    
    // Estado del relay
    Serial.print("Relay: ");
    Serial.println(relayController->getRelayState() ? "🔴 ACTIVO" : "⚫ INACTIVO");
    
    // Estado de alimentación
    if (relayController->isFeedingActive()) {
      Serial.print("🍽️  Alimentando - Restante: ");
      Serial.print(relayController->getRemainingFeedTime());
      Serial.println(" segundos");
    } else {
      Serial.println("⏸️  No alimentando");
    }
    
    // Horarios habilitados
    Serial.print("Horarios activos: ");
    Serial.print(scheduleManager->getEnabledSchedulesCount());
    Serial.print("/");
    Serial.println(MAX_FEED_TIMES);
    
    // Próximo horario
    int nextSchedule = scheduleManager->getNextSchedule(*rtcManager);
    if (nextSchedule > 0) {
      FeedTime next = scheduleManager->getSchedule(nextSchedule);
      Serial.print("⏰ Próximo: Horario #");
      Serial.print(nextSchedule);
      Serial.print(" a las ");
      printTwoDigits(next.hour);
      Serial.print(":");
      printTwoDigits(next.minute);
      Serial.println();
    } else {
      Serial.println("❌ No hay horarios habilitados");
    }
    
    Serial.println("=========================\n");
  }

  // Mostrar menú principal
  void showMainMenu() {
    if (currentMode != DISPLAY_MENU) return;
    
    Serial.println("\n=== MENÚ PRINCIPAL ===");
    Serial.println("SELECT: Navegar opciones");
    Serial.println("UP/DOWN: Cambiar selección");
    Serial.println("CONFIRM: Confirmar");
    Serial.println("");
    Serial.println("1. Ver Horarios");
    Serial.println("2. Editar Horario 1");
    Serial.println("3. Editar Horario 2");
    Serial.println("4. Editar Horario 3");
    Serial.println("5. Editar Horario 4");
    Serial.println("6. Alimentar Ahora");
    Serial.println("7. Ver Estado");
    Serial.println("8. Salir");
    Serial.println("=====================\n");
  }

  // Mostrar horarios
  void showSchedules() {
    if (currentMode != DISPLAY_SCHEDULE_VIEW) return;
    
    Serial.println("\n=== HORARIOS PROGRAMADOS ===");
    for (int i = 0; i < MAX_FEED_TIMES; i++) {
      FeedTime schedule = scheduleManager->getSchedule(i + 1);
      
      Serial.print("Horario ");
      Serial.print(i + 1);
      Serial.print(": ");
      
      if (schedule.enabled) {
        printTwoDigits(schedule.hour);
        Serial.print(":");
        printTwoDigits(schedule.minute);
        Serial.println(" ✅");
      } else {
        Serial.println("DESHABILITADO ❌");
      }
    }
    Serial.print("Duración: ");
    Serial.print(FEED_DURATION);
    Serial.println(" segundos");
    Serial.println("============================\n");
  }

  // Mostrar editor de horario
  void showScheduleEditor(int scheduleNumber, int hour, int minute, bool enabled, int cursorPos) {
    if (currentMode != DISPLAY_SCHEDULE_EDIT) return;
    
    Serial.println("\n=== EDITAR HORARIO ===");
    Serial.print("Editando: Horario ");
    Serial.println(scheduleNumber);
    Serial.println("");
    
    // Mostrar hora con cursor
    Serial.print("Hora: ");
    if (cursorPos == 0) Serial.print(">");
    printTwoDigits(hour);
    if (cursorPos == 0) Serial.print("<");
    
    Serial.print(":");
    
    if (cursorPos == 1) Serial.print(">");
    printTwoDigits(minute);
    if (cursorPos == 1) Serial.print("<");
    
    Serial.println("");
    
    // Mostrar estado
    Serial.print("Estado: ");
    if (cursorPos == 2) Serial.print(">");
    Serial.print(enabled ? "HABILITADO" : "DESHABILITADO");
    if (cursorPos == 2) Serial.print("<");
    Serial.println("");
    
    Serial.println("");
    Serial.println("UP/DOWN: Cambiar valor");
    Serial.println("SELECT: Siguiente campo");
    Serial.println("CONFIRM: Guardar");
    Serial.println("=====================\n");
  }

  // Mostrar mensaje temporal
  void showMessage(String message, int duration = 2000) {
    Serial.println("");
    Serial.println("*** " + message + " ***");
    Serial.println("");
    
    // En una implementación con LCD, aquí se mostraría el mensaje
    // y se restauraría la pantalla anterior después del tiempo
  }

  // Mostrar mensaje temporal con título
  void showMessage(String title, String message, int duration = 2000) {
    Serial.println("");
    Serial.println("*** " + title + " ***");
    Serial.println(message);
    Serial.println("");
  }

  // Mostrar mensaje de error
  void showError(String error) {
    Serial.println("");
    Serial.println("❌ ERROR: " + error);
    Serial.println("");
  }

  // Mostrar mensaje de confirmación
  void showConfirmation(String message) {
    // Mostrar en LCD
    if (lcdDisplay.isReady()) {
      lcdDisplay.showConfirmation(message);
    }
    
    // También en Serial
    Serial.println("");
    Serial.println("✅ " + message);
    Serial.println("");
  }

  // === MÉTODOS ESPECÍFICOS PARA LCD ===
  
  // Mostrar menú principal en LCD
  void showMainMenuLCD(int selectedOption) {
    if (lcdDisplay.isReady()) {
      lcdDisplay.showMainMenu(selectedOption);
    }
  }
  
  // Mostrar horarios en LCD
  void showSchedulesLCD() {
    if (lcdDisplay.isReady()) {
      lcdDisplay.showSchedules();
    }
  }
  
  // Mostrar editor de horario en LCD
  void showScheduleEditorLCD(int scheduleNumber, int hour, int minute, bool enabled, int cursorPos) {
    if (lcdDisplay.isReady()) {
      lcdDisplay.showScheduleEditor(scheduleNumber, hour, minute, enabled, cursorPos);
    }
  }
  
  // Mostrar estado en LCD
  void showStatusLCD() {
    if (lcdDisplay.isReady()) {
      lcdDisplay.showStatus();
    }
  }
  
  // Mostrar pantalla de alimentación
  void showFeedingLCD(int remainingTime) {
    if (lcdDisplay.isReady()) {
      lcdDisplay.showFeeding(remainingTime);
    }
  }
  
  // Mostrar mensaje de error en LCD
  void showErrorLCD(String message) {
    if (lcdDisplay.isReady()) {
      lcdDisplay.showError(message);
    }
    
    Serial.println("");
    Serial.println("❌ ERROR: " + message);
    Serial.println("");
  }
  
  // Mostrar ajuste de hora en LCD
  void showTimeAdjustLCD(int hour, int minute, int day, int month, int year, int cursorPos) {
    if (lcdDisplay.isReady()) {
      lcdDisplay.showTimeAdjust(hour, minute, day, month, year, cursorPos);
    }
  }

  // Limpiar LCD
  void clearLCD() {
    if (lcdDisplay.isReady()) {
      lcdDisplay.clear();
    }
  }

  // Limpiar pantalla (simular para Serial)
  void clear() {
    // En Serial, simplemente añadimos líneas
    Serial.println("");
    Serial.println("");
    Serial.println("");
  }

  // Mostrar indicador de carga
  void showLoading(String message = "Procesando...") {
    Serial.print(message);
    for (int i = 0; i < 3; i++) {
      delay(300);
      Serial.print(".");
    }
    Serial.println(" ✅");
  }

private:
  // Actualizar display según el modo actual
  void refreshDisplay() {
    switch (currentMode) {
      case DISPLAY_CLOCK:
        showClock();
        break;
      case DISPLAY_STATUS:
        showStatus();
        break;
      case DISPLAY_MENU:
        showMainMenu();
        break;
      case DISPLAY_SCHEDULE_VIEW:
        showSchedules();
        break;
      case DISPLAY_SCHEDULE_EDIT:
        // Se actualiza desde el menú system
        break;
    }
  }

  // Función auxiliar para imprimir números con dos dígitos
  void printTwoDigits(int number) {
    if (number < 10) {
      Serial.print("0");
    }
    Serial.print(number);
  }
};

#endif // DISPLAY_MANAGER_H
