/*
  alimentador_peces.ino - Versión optimizada solo LCD
  Sistema completo con menús en LCD, sin mensajes seriales
  Optimizado para Arduino Uno con memoria limitada
*/

#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

// Incluir módulos optimizados
#include "config.h"
#include "button_manager.h"
#include "lcd_display_avr.h"
#include "rtc_manager.h"
#include "schedule_manager.h"
#include "relay_controller.h"

// === INSTANCIAS DE MÓDULOS ===
ButtonManager buttonManager;
RTCManager rtcManager;
ScheduleManager scheduleManager;
RelayController relayController;
LCDDisplayAVR lcdDisplay(&rtcManager, &scheduleManager, &relayController);

// === VARIABLES GLOBALES ===
unsigned long lastLoopTime = 0;
unsigned long lastLCDUpdate = 0;
bool systemInitialized = false;

// === VARIABLES DEL MENÚ ===
enum MenuState {
  MENU_CLOCK,
  MENU_MAIN,
  MENU_VIEW_SCHEDULES,
  MENU_EDIT_SCHEDULE,
  MENU_FEEDING,
  MENU_STATUS,
  MENU_TIME_ADJUST
};

enum EditState {
  EDIT_HOUR,
  EDIT_MINUTE,
  EDIT_ENABLED,
  EDIT_SAVE
};

enum TimeEditState {
  TIME_EDIT_HOUR,
  TIME_EDIT_MINUTE,
  TIME_EDIT_DAY,
  TIME_EDIT_MONTH,
  TIME_EDIT_YEAR,
  TIME_EDIT_SAVE
};

MenuState currentState = MENU_CLOCK;
int selectedOption = 1;
int editingSchedule = 1;
EditState editState = EDIT_HOUR;
int tempHour = 8;
int tempMinute = 0;
bool tempEnabled = true;
unsigned long lastActivity = 0;
bool inMenu = false;

// Variables para ajuste de tiempo
TimeEditState timeEditState = TIME_EDIT_HOUR;
int tempTimeHour = 12;
int tempTimeMinute = 0;
int tempTimeDay = 1;
int tempTimeMonth = 1;
int tempTimeYear = 2024;

void setup() {
  // Inicializar Serial si está habilitado
  if (SERIAL_ENABLED) {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(1000);
    Serial.println("=== ALIMENTADOR DE PECES v3.8 ===");
  }
  
  // Inicializar I2C
  Wire.begin();
  delay(100);
  
  // Inicializar RTC
  if (!rtcManager.begin()) {
    while(1) delay(1000);
  }
  
  // Inicializar botones
  buttonManager.begin();
  
  // Inicializar relay
  relayController.begin();
  
  // Inicializar horarios
  scheduleManager.begin();
  
  // Inicializar LCD
  lcdDisplay.begin();
  
  systemInitialized = true;
  
  // Mostrar pantalla inicial
  currentState = MENU_CLOCK;
  updateActivity();
  
  // Mostrar reloj después de la pantalla de inicio
  delay(1000);
  updateLCD();
  
  // Debug: Verificar horarios configurados
  debugSchedules();
}

void loop() {
  if (!systemInitialized) {
    delay(1000);
    return;
  }
  
  // Control de timing
  unsigned long currentTime = millis();
  if (currentTime - lastLoopTime < LOOP_DELAY) {
    delay(LOOP_DELAY - (currentTime - lastLoopTime));
  }
  lastLoopTime = millis();
  
  // Verificar horarios
  checkScheduledFeeding();
  
  // Actualizar relay
  relayController.update();
  
  // Actualizar botones
  buttonManager.update();
  
  // Verificar timeout
  checkMenuTimeout();
  
  // Procesar menú
  processMenu();
  
  // Actualizar LCD con control de tiempo para evitar parpadeo
  if (millis() - lastLCDUpdate > 200) {  // Actualizar cada 200ms
    updateLCD();
    lastLCDUpdate = millis();
  }
}

// Verificar horarios programados
void checkScheduledFeeding() {
  if (relayController.isFeedingActive()) return;
  
  // Usar la función correcta del ScheduleManager
  int scheduleToFeed = scheduleManager.checkFeedTime(rtcManager);
  
  if (scheduleToFeed > 0) {
    if (DEBUG_MODE && SERIAL_ENABLED) {
      Serial.print("ACTIVANDO ALIMENTACIÓN - Horario ");
      Serial.println(scheduleToFeed);
    }
    
    relayController.startFeeding(scheduleToFeed);
    
    if (currentState != MENU_CLOCK) {
      currentState = MENU_CLOCK;
    }
  }
}

// Procesar menú
void processMenu() {
  switch (currentState) {
    case MENU_CLOCK:
      handleClockMode();
      break;
    case MENU_MAIN:
      handleMainMenu();
      break;
    case MENU_VIEW_SCHEDULES:
      handleViewSchedules();
      break;
    case MENU_EDIT_SCHEDULE:
      handleEditSchedule();
      break;
    case MENU_FEEDING:
      handleFeeding();
      break;
    case MENU_STATUS:
      handleStatus();
      break;
    case MENU_TIME_ADJUST:
      handleTimeAdjust();
      break;
  }
}

// Manejar modo reloj
void handleClockMode() {
  if (buttonManager.selectPressed()) {
    enterMainMenu();
  }
  
  // Solo permitir alimentación manual en pantalla principal
  if (buttonManager.confirmPressed() && currentState == MENU_CLOCK) {
    if (!relayController.isFeedingActive()) {
      relayController.startFeeding(0);
      buttonManager.confirmBeep();
    }
  }
}

// Entrar al menú principal
void enterMainMenu() {
  currentState = MENU_MAIN;
  selectedOption = 1;
  inMenu = true;
  updateActivity();
  buttonManager.confirmBeep();
}

// Manejar menú principal
void handleMainMenu() {
  updateActivity();
  
  if (buttonManager.upPressed()) {
    selectedOption = (selectedOption > 1) ? selectedOption - 1 : 9;
    buttonManager.beep();
  }
  
  if (buttonManager.downPressed()) {
    selectedOption = (selectedOption < 9) ? selectedOption + 1 : 1;
    buttonManager.beep();
  }
  
  if (buttonManager.confirmPressed()) {
    executeMainMenuOption();
  }
  
  if (buttonManager.selectLongPressed()) {
    returnToClock();
  }
}

// Ejecutar opción del menú principal
void executeMainMenuOption() {
  buttonManager.confirmBeep();
  
  switch (selectedOption) {
    case 1: // Ver Horarios
      currentState = MENU_VIEW_SCHEDULES;
      break;
      
    case 2: // Editar Horario 1
    case 3: // Editar Horario 2
    case 4: // Editar Horario 3
    case 5: // Editar Horario 4
      editingSchedule = selectedOption - 1;
      startScheduleEdit();
      break;
      
    case 6: // Alimentar Ahora
      if (!relayController.isFeedingActive()) {
        relayController.startFeeding(0);
        currentState = MENU_FEEDING;
      }
      break;
      
    case 7: // Ver Estado
      currentState = MENU_STATUS;
      break;
      
    case 8: // Ajustar Hora
      startTimeAdjust();
      break;
      
    case 9: // Salir
      returnToClock();
      break;
  }
}

// Manejar vista de horarios
void handleViewSchedules() {
  updateActivity();
  
  if (buttonManager.selectPressed() || buttonManager.confirmPressed()) {
    currentState = MENU_MAIN;
    buttonManager.beep();
  }
}

// Iniciar edición de horario
void startScheduleEdit() {
  currentState = MENU_EDIT_SCHEDULE;
  editState = EDIT_HOUR;
  
  FeedTime current = scheduleManager.getSchedule(editingSchedule);
  tempHour = current.hour;
  tempMinute = current.minute;
  tempEnabled = current.enabled;
}

// Manejar edición de horario
void handleEditSchedule() {
  updateActivity();
  
  if (buttonManager.upPressed() || buttonManager.upRepeating()) {
    incrementEditValue();
    buttonManager.beep();
  }
  
  if (buttonManager.downPressed() || buttonManager.downRepeating()) {
    decrementEditValue();
    buttonManager.beep();
  }
  
  if (buttonManager.selectPressed()) {
    nextEditField();
    buttonManager.beep();
  }
  
  if (buttonManager.confirmPressed()) {
    if (editState == EDIT_SAVE) {
      saveSchedule();
    } else {
      nextEditField();
      buttonManager.beep();
    }
  }
  
  if (buttonManager.selectLongPressed()) {
    currentState = MENU_MAIN;
    buttonManager.errorBeep();
  }
}

// Incrementar valor en edición
void incrementEditValue() {
  switch (editState) {
    case EDIT_HOUR:
      tempHour = (tempHour < 23) ? tempHour + 1 : 0;
      break;
    case EDIT_MINUTE:
      tempMinute = (tempMinute < 59) ? tempMinute + 1 : 0;
      break;
    case EDIT_ENABLED:
      tempEnabled = !tempEnabled;
      break;
  }
}

// Decrementar valor en edición
void decrementEditValue() {
  switch (editState) {
    case EDIT_HOUR:
      tempHour = (tempHour > 0) ? tempHour - 1 : 23;
      break;
    case EDIT_MINUTE:
      tempMinute = (tempMinute > 0) ? tempMinute - 1 : 59;
      break;
    case EDIT_ENABLED:
      tempEnabled = !tempEnabled;
      break;
  }
}

// Siguiente campo de edición
void nextEditField() {
  switch (editState) {
    case EDIT_HOUR:
      editState = EDIT_MINUTE;
      break;
    case EDIT_MINUTE:
      editState = EDIT_ENABLED;
      break;
    case EDIT_ENABLED:
      editState = EDIT_SAVE;
      break;
    case EDIT_SAVE:
      editState = EDIT_HOUR;
      break;
  }
}

// Guardar horario editado
void saveSchedule() {
  if (scheduleManager.setSchedule(editingSchedule, tempHour, tempMinute)) {
    scheduleManager.enableSchedule(editingSchedule, tempEnabled);
    buttonManager.confirmBeep();
    currentState = MENU_MAIN;
  } else {
    buttonManager.errorBeep();
  }
}

// Manejar alimentación manual
void handleFeeding() {
  updateActivity();
  
  if (!relayController.isFeedingActive()) {
    currentState = MENU_MAIN;
  }
  
  if (buttonManager.selectPressed()) {
    relayController.emergencyStop();
    currentState = MENU_MAIN;
    buttonManager.beep();
  }
}

// Manejar estado del sistema
void handleStatus() {
  updateActivity();
  
  if (buttonManager.selectPressed() || buttonManager.confirmPressed()) {
    currentState = MENU_MAIN;
    buttonManager.beep();
  }
}

// Manejar ajuste de hora
void handleTimeAdjust() {
  updateActivity();
  
  if (buttonManager.upPressed() || buttonManager.upRepeating()) {
    incrementTimeValue();
    buttonManager.beep();
  }
  
  if (buttonManager.downPressed() || buttonManager.downRepeating()) {
    decrementTimeValue();
    buttonManager.beep();
  }
  
  if (buttonManager.selectPressed()) {
    nextTimeField();
    buttonManager.beep();
  }
  
  if (buttonManager.confirmPressed()) {
    if (timeEditState == TIME_EDIT_SAVE) {
      saveTime();
    } else {
      nextTimeField();
      buttonManager.beep();
    }
  }
  
  if (buttonManager.selectLongPressed()) {
    currentState = MENU_MAIN;
    buttonManager.errorBeep();
  }
}

// Volver al reloj principal
void returnToClock() {
  currentState = MENU_CLOCK;
  inMenu = false;
  buttonManager.beep();
}

// Actualizar LCD
void updateLCD() {
  static MenuState lastState = MENU_CLOCK;
  static int lastSelectedOption = 1;
  static int lastEditingSchedule = 1;
  static EditState lastEditState = EDIT_HOUR;
  static int lastTempHour = 8;
  static int lastTempMinute = 0;
  static bool lastTempEnabled = true;
  static int lastRemainingTime = 0;
  static TimeEditState lastTimeEditState = TIME_EDIT_HOUR;
  static int lastTempTimeHour = 12;
  static int lastTempTimeMinute = 0;
  static int lastTempTimeDay = 1;
  static int lastTempTimeMonth = 1;
  static int lastTempTimeYear = 2024;
  
  // Solo actualizar si algo cambió o es el reloj
  bool needsUpdate = false;
  
  if (currentState != lastState) {
    needsUpdate = true;
    lastState = currentState;
  }
  
  if (currentState == MENU_MAIN && selectedOption != lastSelectedOption) {
    needsUpdate = true;
    lastSelectedOption = selectedOption;
  }
  
  if (currentState == MENU_EDIT_SCHEDULE) {
    if (editingSchedule != lastEditingSchedule || 
        editState != lastEditState ||
        tempHour != lastTempHour ||
        tempMinute != lastTempMinute ||
        tempEnabled != lastTempEnabled) {
      needsUpdate = true;
      lastEditingSchedule = editingSchedule;
      lastEditState = editState;
      lastTempHour = tempHour;
      lastTempMinute = tempMinute;
      lastTempEnabled = tempEnabled;
    }
  }
  
  if (currentState == MENU_FEEDING) {
    int remainingTime = relayController.getRemainingFeedTime();
    if (remainingTime != lastRemainingTime) {
      needsUpdate = true;
      lastRemainingTime = remainingTime;
    }
  }
  
  if (currentState == MENU_TIME_ADJUST) {
    if (timeEditState != lastTimeEditState ||
        tempTimeHour != lastTempTimeHour ||
        tempTimeMinute != lastTempTimeMinute ||
        tempTimeDay != lastTempTimeDay ||
        tempTimeMonth != lastTempTimeMonth ||
        tempTimeYear != lastTempTimeYear) {
      needsUpdate = true;
      lastTimeEditState = timeEditState;
      lastTempTimeHour = tempTimeHour;
      lastTempTimeMinute = tempTimeMinute;
      lastTempTimeDay = tempTimeDay;
      lastTempTimeMonth = tempTimeMonth;
      lastTempTimeYear = tempTimeYear;
    }
  }
  
  // Para el reloj, actualizar siempre
  if (currentState == MENU_CLOCK) {
    needsUpdate = true;
  }
  
  if (needsUpdate) {
    switch (currentState) {
      case MENU_CLOCK:
        lcdDisplay.showClock();
        break;
      case MENU_MAIN:
        lcdDisplay.showMainMenu(selectedOption);
        break;
      case MENU_VIEW_SCHEDULES:
        lcdDisplay.showSchedules();
        break;
      case MENU_EDIT_SCHEDULE:
        {
          int cursorPos = 0;
          switch (editState) {
            case EDIT_HOUR: cursorPos = 0; break;
            case EDIT_MINUTE: cursorPos = 1; break;
            case EDIT_ENABLED: cursorPos = 2; break;
            case EDIT_SAVE: cursorPos = 3; break;
          }
          lcdDisplay.showScheduleEditor(editingSchedule, tempHour, tempMinute, tempEnabled, cursorPos);
        }
        break;
      case MENU_FEEDING:
        lcdDisplay.showFeeding(relayController.getRemainingFeedTime());
        break;
      case MENU_STATUS:
        lcdDisplay.showStatus();
        break;
      case MENU_TIME_ADJUST:
        {
          int cursorPos = 0;
          switch (timeEditState) {
            case TIME_EDIT_HOUR: cursorPos = 0; break;
            case TIME_EDIT_MINUTE: cursorPos = 1; break;
            case TIME_EDIT_DAY: cursorPos = 2; break;
            case TIME_EDIT_MONTH: cursorPos = 3; break;
            case TIME_EDIT_YEAR: cursorPos = 4; break;
            case TIME_EDIT_SAVE: cursorPos = 5; break;
          }
          lcdDisplay.showTimeAdjust(tempTimeHour, tempTimeMinute, tempTimeDay, tempTimeMonth, tempTimeYear, cursorPos);
        }
        break;
    }
  }
}

// Verificar timeout del menú
void checkMenuTimeout() {
  if (inMenu && (millis() - lastActivity > MENU_TIMEOUT)) {
    returnToClock();
  }
}

// Actualizar tiempo de última actividad
void updateActivity() {
  lastActivity = millis();
}

// Iniciar ajuste de tiempo
void startTimeAdjust() {
  currentState = MENU_TIME_ADJUST;
  timeEditState = TIME_EDIT_HOUR;
  
  // Obtener tiempo actual del RTC
  DateTime now = rtcManager.now();
  tempTimeHour = now.hour();
  tempTimeMinute = now.minute();
  tempTimeDay = now.day();
  tempTimeMonth = now.month();
  tempTimeYear = now.year();
  
  // Forzar actualización del LCD
  updateLCD();
}

// Incrementar valor en ajuste de tiempo
void incrementTimeValue() {
  switch (timeEditState) {
    case TIME_EDIT_HOUR:
      tempTimeHour = (tempTimeHour < 23) ? tempTimeHour + 1 : 0;
      break;
    case TIME_EDIT_MINUTE:
      tempTimeMinute = (tempTimeMinute < 59) ? tempTimeMinute + 1 : 0;
      break;
    case TIME_EDIT_DAY:
      tempTimeDay = (tempTimeDay < 31) ? tempTimeDay + 1 : 1;
      break;
    case TIME_EDIT_MONTH:
      tempTimeMonth = (tempTimeMonth < 12) ? tempTimeMonth + 1 : 1;
      break;
    case TIME_EDIT_YEAR:
      tempTimeYear = (tempTimeYear < 2099) ? tempTimeYear + 1 : 2024;
      break;
  }
}

// Decrementar valor en ajuste de tiempo
void decrementTimeValue() {
  switch (timeEditState) {
    case TIME_EDIT_HOUR:
      tempTimeHour = (tempTimeHour > 0) ? tempTimeHour - 1 : 23;
      break;
    case TIME_EDIT_MINUTE:
      tempTimeMinute = (tempTimeMinute > 0) ? tempTimeMinute - 1 : 59;
      break;
    case TIME_EDIT_DAY:
      tempTimeDay = (tempTimeDay > 1) ? tempTimeDay - 1 : 31;
      break;
    case TIME_EDIT_MONTH:
      tempTimeMonth = (tempTimeMonth > 1) ? tempTimeMonth - 1 : 12;
      break;
    case TIME_EDIT_YEAR:
      tempTimeYear = (tempTimeYear > 2024) ? tempTimeYear - 1 : 2099;
      break;
  }
}

// Siguiente campo en ajuste de tiempo
void nextTimeField() {
  switch (timeEditState) {
    case TIME_EDIT_HOUR:
      timeEditState = TIME_EDIT_MINUTE;
      break;
    case TIME_EDIT_MINUTE:
      timeEditState = TIME_EDIT_DAY;
      break;
    case TIME_EDIT_DAY:
      timeEditState = TIME_EDIT_MONTH;
      break;
    case TIME_EDIT_MONTH:
      timeEditState = TIME_EDIT_YEAR;
      break;
    case TIME_EDIT_YEAR:
      timeEditState = TIME_EDIT_SAVE;
      break;
    case TIME_EDIT_SAVE:
      timeEditState = TIME_EDIT_HOUR;
      break;
  }
}

// Guardar tiempo ajustado
void saveTime() {
  // Validar fecha
  if (tempTimeDay > 31 || tempTimeDay < 1 ||
      tempTimeMonth > 12 || tempTimeMonth < 1 ||
      tempTimeYear < 2024 || tempTimeYear > 2099) {
    buttonManager.errorBeep();
    return;
  }
  
  // Ajustar RTC usando setDateTime
  rtcManager.setDateTime(tempTimeYear, tempTimeMonth, tempTimeDay, 
                        tempTimeHour, tempTimeMinute, 0);
  
  buttonManager.confirmBeep();
  currentState = MENU_MAIN;
}

// Función de debug para verificar horarios
void debugSchedules() {
  // Solo mostrar en modo debug
  if (DEBUG_MODE && SERIAL_ENABLED) {
    Serial.println("=== DEBUG HORARIOS ===");
    for (int i = 1; i <= MAX_FEED_TIMES; i++) {
      FeedTime schedule = scheduleManager.getSchedule(i);
      Serial.print("Horario ");
      Serial.print(i);
      Serial.print(": ");
      if (schedule.enabled) {
        Serial.print(schedule.hour);
        Serial.print(":");
        if (schedule.minute < 10) Serial.print("0");
        Serial.print(schedule.minute);
        Serial.println(" (HABILITADO)");
      } else {
        Serial.println("DESHABILITADO");
      }
    }
    
    DateTime now = rtcManager.now();
    Serial.print("Hora actual: ");
    Serial.print(now.hour());
    Serial.print(":");
    if (now.minute() < 10) Serial.print("0");
    Serial.print(now.minute());
    Serial.print(":");
    if (now.second() < 10) Serial.print("0");
    Serial.println(now.second());
    
    int nextSchedule = scheduleManager.getNextSchedule(rtcManager);
    Serial.print("Próximo horario: ");
    Serial.println(nextSchedule);
    Serial.println("===================");
  }
}