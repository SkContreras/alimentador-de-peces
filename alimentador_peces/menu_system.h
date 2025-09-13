/*
  menu_system.h - Sistema de navegación por menús
  
  Este módulo coordina la navegación por menús usando botones físicos
  y maneja todas las interacciones del usuario.
*/

#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "config.h"
#include "button_manager.h"
#include "display_manager.h"
#include "rtc_manager.h"
#include "schedule_manager.h"
#include "relay_controller.h"

// Estados del menú
enum MenuState {
  MENU_CLOCK,           // Pantalla principal del reloj
  MENU_MAIN,           // Menú principal
  MENU_VIEW_SCHEDULES, // Ver horarios
  MENU_EDIT_SCHEDULE,  // Editar horario específico
  MENU_FEEDING,        // Alimentando manualmente
  MENU_STATUS,         // Estado del sistema
  MENU_TIME_ADJUST     // Ajustar hora/fecha
};

// Estados del editor de horarios
enum EditState {
  EDIT_HOUR,     // Editando hora
  EDIT_MINUTE,   // Editando minuto
  EDIT_ENABLED,  // Editando estado habilitado/deshabilitado
  EDIT_SAVE      // Confirmar guardado
};

// Estados del ajuste de hora/fecha
enum TimeAdjustState {
  ADJUST_HOUR,    // Ajustando hora
  ADJUST_MINUTE,  // Ajustando minuto
  ADJUST_DAY,     // Ajustando día
  ADJUST_MONTH,   // Ajustando mes
  ADJUST_YEAR,    // Ajustando año
  ADJUST_SAVE     // Confirmar guardado
};

class MenuSystem {
private:
  MenuState currentState;
  MenuState previousState;
  int selectedOption;
  int editingSchedule;    // Horario que se está editando (1-4)
  EditState editState;
  
  // Variables de edición temporal
  int tempHour;
  int tempMinute;
  bool tempEnabled;
  
  // Variables de ajuste de hora/fecha
  TimeAdjustState timeAdjustState;
  int tempAdjustHour;
  int tempAdjustMinute;
  int tempAdjustDay;
  int tempAdjustMonth;
  int tempAdjustYear;
  
  // Control de timeout
  unsigned long lastActivity;
  bool inMenu;
  
  // Referencias a módulos
  ButtonManager* buttonManager;
  DisplayManager* displayManager;
  RTCManager* rtcManager;
  ScheduleManager* scheduleManager;
  RelayController* relayController;

public:
  // Constructor
  MenuSystem(ButtonManager* buttons, DisplayManager* display, RTCManager* rtc, 
             ScheduleManager* schedule, RelayController* relay) 
    : currentState(MENU_CLOCK), previousState(MENU_CLOCK), selectedOption(1), 
      editingSchedule(1), editState(EDIT_HOUR), tempHour(8), tempMinute(0), 
      tempEnabled(true), timeAdjustState(ADJUST_HOUR), tempAdjustHour(12), 
      tempAdjustMinute(0), tempAdjustDay(1), tempAdjustMonth(1), tempAdjustYear(2024),
      lastActivity(0), inMenu(false),
      buttonManager(buttons), displayManager(display), rtcManager(rtc), 
      scheduleManager(schedule), relayController(relay) {}

  // Inicializar sistema de menús
  void begin() {
    currentState = MENU_CLOCK;
    displayManager->setMode(DISPLAY_CLOCK);
    updateActivity();
    
    Serial.println("=== SISTEMA DE MENÚS INICIADO ===");
    Serial.println("Presiona SELECT para acceder al menú");
    Serial.println("==================================");
  }

  // Actualizar sistema de menús
  void update() {
    // Actualizar botones
    buttonManager->update();
    
    // Verificar timeout del menú
    checkMenuTimeout();
    
    // Procesar botones según el estado actual
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
    
    // Actualizar display
    displayManager->update();
  }

private:
  // Actualizar tiempo de última actividad
  void updateActivity() {
    lastActivity = millis();
  }

  // Verificar timeout del menú
  void checkMenuTimeout() {
    if (inMenu && (millis() - lastActivity > MENU_TIMEOUT)) {
      returnToClock();
    }
  }

  // Volver al reloj principal
  void returnToClock() {
    currentState = MENU_CLOCK;
    displayManager->setMode(DISPLAY_CLOCK);
    inMenu = false;
    buttonManager->beep();
    Serial.println("\n⏰ Volviendo al reloj principal...\n");
  }

  // Entrar al menú principal
  void enterMainMenu() {
    currentState = MENU_MAIN;
    displayManager->setMode(DISPLAY_MENU);
    selectedOption = 1;
    inMenu = true;
    updateActivity();
    buttonManager->confirmBeep();
    displayManager->showMainMenu();
  }

  // Manejar modo reloj
  void handleClockMode() {
    if (buttonManager->selectPressed()) {
      enterMainMenu();
    }
    
    if (buttonManager->confirmPressed()) {
      // Alimentar manualmente desde el reloj
      if (!relayController->isFeedingActive()) {
        relayController->startFeeding(0);
        buttonManager->confirmBeep();
        displayManager->showMessage("Alimentando manualmente");
      }
    }
  }

  // Manejar menú principal
  void handleMainMenu() {
    updateActivity();
    
    if (buttonManager->upPressed()) {
      selectedOption = (selectedOption > 1) ? selectedOption - 1 : 9;
      buttonManager->beep();
      showMainMenuOption();
    }
    
    if (buttonManager->downPressed()) {
      selectedOption = (selectedOption < 9) ? selectedOption + 1 : 1;
      buttonManager->beep();
      showMainMenuOption();
    }
    
    if (buttonManager->confirmPressed()) {
      executeMainMenuOption();
    }
    
    if (buttonManager->selectLongPressed()) {
      returnToClock();
    }
  }

  // Mostrar opción seleccionada del menú principal
  void showMainMenuOption() {
    // Mostrar en LCD
    displayManager->showMainMenuLCD(selectedOption);
    
    // También en Serial para compatibilidad
    String options[] = {"", "Ver Horarios", "Editar Horario 1", "Editar Horario 2", 
                       "Editar Horario 3", "Editar Horario 4", "Alimentar Ahora", 
                       "Ver Estado", "Ajustar Hora", "Salir"};
    
    Serial.print("► ");
    Serial.println(options[selectedOption]);
  }

  // Ejecutar opción del menú principal
  void executeMainMenuOption() {
    buttonManager->confirmBeep();
    
    switch (selectedOption) {
      case 1: // Ver Horarios
        currentState = MENU_VIEW_SCHEDULES;
        displayManager->setMode(DISPLAY_SCHEDULE_VIEW);
        displayManager->showSchedules();
        displayManager->showSchedulesLCD();
        break;
        
      case 2: // Editar Horario 1
      case 3: // Editar Horario 2
      case 4: // Editar Horario 3
      case 5: // Editar Horario 4
        editingSchedule = selectedOption - 1;
        startScheduleEdit();
        break;
        
      case 6: // Alimentar Ahora
        if (!relayController->isFeedingActive()) {
          relayController->startFeeding(0);
          displayManager->showMessage("Alimentando manualmente");
          displayManager->showConfirmation("Alimentando");
          currentState = MENU_FEEDING;
        } else {
          displayManager->showError("Ya se está alimentando");
          displayManager->showErrorLCD("Ya alimentando");
        }
        break;
        
      case 7: // Ver Estado
        currentState = MENU_STATUS;
        displayManager->setMode(DISPLAY_STATUS);
        displayManager->showStatus();
        displayManager->showStatusLCD();
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
    
    if (buttonManager->selectPressed() || buttonManager->confirmPressed()) {
      currentState = MENU_MAIN;
      displayManager->setMode(DISPLAY_MENU);
      displayManager->showMainMenu();
      buttonManager->beep();
    }
  }

  // Iniciar edición de horario
  void startScheduleEdit() {
    currentState = MENU_EDIT_SCHEDULE;
    displayManager->setMode(DISPLAY_SCHEDULE_EDIT);
    editState = EDIT_HOUR;
    
    // Cargar valores actuales
    FeedTime current = scheduleManager->getSchedule(editingSchedule);
    tempHour = current.hour;
    tempMinute = current.minute;
    tempEnabled = current.enabled;
    
    updateScheduleEditor();
  }

  // Manejar edición de horario
  void handleEditSchedule() {
    updateActivity();
    
    if (buttonManager->upPressed() || buttonManager->upRepeating()) {
      incrementEditValue();
      buttonManager->beep();
      updateScheduleEditor();
    }
    
    if (buttonManager->downPressed() || buttonManager->downRepeating()) {
      decrementEditValue();
      buttonManager->beep();
      updateScheduleEditor();
    }
    
    if (buttonManager->selectPressed()) {
      nextEditField();
      buttonManager->beep();
      updateScheduleEditor();
    }
    
    if (buttonManager->confirmPressed()) {
      if (editState == EDIT_SAVE) {
        saveSchedule();
      } else {
        nextEditField();
        buttonManager->beep();
        updateScheduleEditor();
      }
    }
    
    if (buttonManager->selectLongPressed()) {
      // Cancelar edición
      currentState = MENU_MAIN;
      displayManager->setMode(DISPLAY_MENU);
      displayManager->showMessage("Edición cancelada");
      buttonManager->errorBeep();
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

  // Actualizar editor de horario
  void updateScheduleEditor() {
    int cursorPos = 0;
    switch (editState) {
      case EDIT_HOUR: cursorPos = 0; break;
      case EDIT_MINUTE: cursorPos = 1; break;
      case EDIT_ENABLED: cursorPos = 2; break;
      case EDIT_SAVE: cursorPos = 3; break;
    }
    
    // Mostrar en LCD
    displayManager->showScheduleEditorLCD(editingSchedule, tempHour, tempMinute, 
                                        tempEnabled, cursorPos);
    
    // También en Serial
    displayManager->showScheduleEditor(editingSchedule, tempHour, tempMinute, 
                                     tempEnabled, cursorPos);
  }

  // Guardar horario editado
  void saveSchedule() {
    if (scheduleManager->setSchedule(editingSchedule, tempHour, tempMinute)) {
      scheduleManager->enableSchedule(editingSchedule, tempEnabled);
      
      buttonManager->confirmBeep();
      displayManager->showConfirmation("Horario guardado correctamente");
      
      // Volver al menú principal
      currentState = MENU_MAIN;
      displayManager->setMode(DISPLAY_MENU);
      
      Serial.print("✅ Horario ");
      Serial.print(editingSchedule);
      Serial.print(" configurado: ");
      if (tempHour < 10) Serial.print("0");
      Serial.print(tempHour);
      Serial.print(":");
      if (tempMinute < 10) Serial.print("0");
      Serial.print(tempMinute);
      Serial.print(" - ");
      Serial.println(tempEnabled ? "HABILITADO" : "DESHABILITADO");
      
    } else {
      buttonManager->errorBeep();
      displayManager->showError("Error al guardar horario");
    }
  }

  // Manejar alimentación manual
  void handleFeeding() {
    updateActivity();
    
    // Mostrar estado de alimentación en LCD
    if (relayController->isFeedingActive()) {
      displayManager->showFeedingLCD(relayController->getRemainingFeedTime());
    }
    
    if (!relayController->isFeedingActive()) {
      // Alimentación terminada, volver al menú
      currentState = MENU_MAIN;
      displayManager->setMode(DISPLAY_MENU);
      displayManager->showMessage("Alimentación completada");
      displayManager->showConfirmation("Completado");
    }
    
    if (buttonManager->selectPressed()) {
      // Parar alimentación
      relayController->emergencyStop();
      currentState = MENU_MAIN;
      displayManager->setMode(DISPLAY_MENU);
      displayManager->showMessage("Alimentación detenida");
      displayManager->showConfirmation("Detenido");
      buttonManager->beep();
    }
  }

  // Manejar estado del sistema
  void handleStatus() {
    updateActivity();
    
    if (buttonManager->selectPressed() || buttonManager->confirmPressed()) {
      currentState = MENU_MAIN;
      displayManager->setMode(DISPLAY_MENU);
      displayManager->showMainMenu();
      buttonManager->beep();
    }
  }

  // === FUNCIONES DE AJUSTE DE HORA ===

  // Iniciar ajuste de hora
  void startTimeAdjust() {
    currentState = MENU_TIME_ADJUST;
    displayManager->setMode(DISPLAY_TIME_ADJUST);
    timeAdjustState = ADJUST_HOUR;
    
    // Cargar valores actuales del RTC
    tempAdjustHour = rtcManager->getCurrentHour();
    tempAdjustMinute = rtcManager->getCurrentMinute();
    tempAdjustDay = rtcManager->getCurrentDay();
    tempAdjustMonth = rtcManager->getCurrentMonth();
    tempAdjustYear = rtcManager->getCurrentYear();
    
    updateTimeAdjustDisplay();
    Serial.println("Iniciando ajuste de hora/fecha");
  }

  // Manejar ajuste de hora/fecha
  void handleTimeAdjust() {
    updateActivity();
    
    if (buttonManager->upPressed() || buttonManager->upRepeating()) {
      incrementTimeAdjustValue();
      buttonManager->beep();
      updateTimeAdjustDisplay();
    }
    
    if (buttonManager->downPressed() || buttonManager->downRepeating()) {
      decrementTimeAdjustValue();
      buttonManager->beep();
      updateTimeAdjustDisplay();
    }
    
    if (buttonManager->selectPressed()) {
      nextTimeAdjustField();
      buttonManager->beep();
      updateTimeAdjustDisplay();
    }
    
    if (buttonManager->confirmPressed()) {
      if (timeAdjustState == ADJUST_SAVE) {
        saveTimeAdjust();
      } else {
        nextTimeAdjustField();
        buttonManager->beep();
        updateTimeAdjustDisplay();
      }
    }
    
    if (buttonManager->selectLongPressed()) {
      // Cancelar ajuste
      currentState = MENU_MAIN;
      displayManager->setMode(DISPLAY_MENU);
      displayManager->showMessage("Ajuste cancelado");
      buttonManager->errorBeep();
    }
  }

  // Incrementar valor en ajuste de hora
  void incrementTimeAdjustValue() {
    switch (timeAdjustState) {
      case ADJUST_HOUR:
        tempAdjustHour = (tempAdjustHour < 23) ? tempAdjustHour + 1 : 0;
        break;
      case ADJUST_MINUTE:
        tempAdjustMinute = (tempAdjustMinute < 59) ? tempAdjustMinute + 1 : 0;
        break;
      case ADJUST_DAY:
        tempAdjustDay = (tempAdjustDay < 31) ? tempAdjustDay + 1 : 1;
        break;
      case ADJUST_MONTH:
        tempAdjustMonth = (tempAdjustMonth < 12) ? tempAdjustMonth + 1 : 1;
        break;
      case ADJUST_YEAR:
        tempAdjustYear = (tempAdjustYear < 2099) ? tempAdjustYear + 1 : 2000;
        break;
    }
  }

  // Decrementar valor en ajuste de hora
  void decrementTimeAdjustValue() {
    switch (timeAdjustState) {
      case ADJUST_HOUR:
        tempAdjustHour = (tempAdjustHour > 0) ? tempAdjustHour - 1 : 23;
        break;
      case ADJUST_MINUTE:
        tempAdjustMinute = (tempAdjustMinute > 0) ? tempAdjustMinute - 1 : 59;
        break;
      case ADJUST_DAY:
        tempAdjustDay = (tempAdjustDay > 1) ? tempAdjustDay - 1 : 31;
        break;
      case ADJUST_MONTH:
        tempAdjustMonth = (tempAdjustMonth > 1) ? tempAdjustMonth - 1 : 12;
        break;
      case ADJUST_YEAR:
        tempAdjustYear = (tempAdjustYear > 2000) ? tempAdjustYear - 1 : 2099;
        break;
    }
  }

  // Siguiente campo de ajuste
  void nextTimeAdjustField() {
    switch (timeAdjustState) {
      case ADJUST_HOUR:
        timeAdjustState = ADJUST_MINUTE;
        break;
      case ADJUST_MINUTE:
        timeAdjustState = ADJUST_DAY;
        break;
      case ADJUST_DAY:
        timeAdjustState = ADJUST_MONTH;
        break;
      case ADJUST_MONTH:
        timeAdjustState = ADJUST_YEAR;
        break;
      case ADJUST_YEAR:
        timeAdjustState = ADJUST_SAVE;
        break;
      case ADJUST_SAVE:
        timeAdjustState = ADJUST_HOUR;
        break;
    }
  }

  // Actualizar display de ajuste de hora
  void updateTimeAdjustDisplay() {
    int cursorPos = 0;
    switch (timeAdjustState) {
      case ADJUST_HOUR: cursorPos = 0; break;
      case ADJUST_MINUTE: cursorPos = 1; break;
      case ADJUST_DAY: cursorPos = 2; break;
      case ADJUST_MONTH: cursorPos = 3; break;
      case ADJUST_YEAR: cursorPos = 4; break;
      case ADJUST_SAVE: cursorPos = 5; break;
    }
    
    // Mostrar en LCD
    displayManager->showTimeAdjustLCD(tempAdjustHour, tempAdjustMinute, 
                                    tempAdjustDay, tempAdjustMonth, 
                                    tempAdjustYear, cursorPos);
    
    // También en Serial para debug
    Serial.print("Ajustando: ");
    Serial.print(tempAdjustHour);
    Serial.print(":");
    Serial.print(tempAdjustMinute);
    Serial.print(" ");
    Serial.print(tempAdjustDay);
    Serial.print("/");
    Serial.print(tempAdjustMonth);
    Serial.print("/");
    Serial.println(tempAdjustYear);
  }

  // Guardar ajuste de hora
  void saveTimeAdjust() {
    // Validar fecha antes de guardar
    if (rtcManager->isValidDate(tempAdjustYear, tempAdjustMonth, tempAdjustDay)) {
      rtcManager->setDateTime(tempAdjustYear, tempAdjustMonth, tempAdjustDay,
                             tempAdjustHour, tempAdjustMinute, 0);
      
      buttonManager->confirmBeep();
      displayManager->showConfirmation("Hora ajustada");
      
      // Volver al menú principal
      currentState = MENU_MAIN;
      displayManager->setMode(DISPLAY_MENU);
      
      Serial.print("✅ Hora ajustada: ");
      Serial.print(tempAdjustHour);
      Serial.print(":");
      Serial.print(tempAdjustMinute);
      Serial.print(" ");
      Serial.print(tempAdjustDay);
      Serial.print("/");
      Serial.print(tempAdjustMonth);
      Serial.print("/");
      Serial.println(tempAdjustYear);
      
    } else {
      buttonManager->errorBeep();
      displayManager->showError("Fecha inválida");
      Serial.println("❌ Error: Fecha inválida");
    }
  }
};

#endif // MENU_SYSTEM_H
