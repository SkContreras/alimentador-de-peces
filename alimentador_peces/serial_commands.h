/*
  serial_commands.h - Procesador de comandos seriales
  
  Este módulo maneja todos los comandos que se pueden enviar
  a través del monitor serial para controlar el sistema.
*/

#ifndef SERIAL_COMMANDS_H
#define SERIAL_COMMANDS_H

#include "config.h"
#include "rtc_manager.h"
#include "relay_controller.h"
#include "schedule_manager.h"

class SerialCommands {
private:
  RTCManager* rtcManager;
  RelayController* relayController;
  ScheduleManager* scheduleManager;

public:
  // Constructor
  SerialCommands(RTCManager* rtc, RelayController* relay, ScheduleManager* schedule) 
    : rtcManager(rtc), relayController(relay), scheduleManager(schedule) {}

  // Inicializar comandos seriales
  void begin() {
    Serial.println("Comandos seriales inicializados");
    Serial.println("Escribe 'help' para ver comandos disponibles");
  }

  // Procesar comandos seriales disponibles
  void processCommands() {
    if (!Serial.available()) return;
    
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toLowerCase(); // Convertir a minúsculas para facilitar comparación
    
    // Procesar diferentes comandos
    if (command == "help" || command == "?") {
      showHelp();
    }
    else if (command == "time") {
      rtcManager->displayCurrentTime();
    }
    else if (command == "schedules") {
      scheduleManager->displaySchedules();
    }
    else if (command == "feed") {
      if (!relayController->isFeedingActive()) {
        relayController->startFeeding(0); // Alimentación manual
      } else {
        Serial.println(MSG_ALREADY_FEEDING);
      }
    }
    else if (command == "status") {
      showSystemStatus();
    }
    else if (command == "stop") {
      relayController->emergencyStop();
    }
    else if (command == "next") {
      showNextSchedule();
    }
    else if (command.startsWith("set")) {
      processSetCommand(command);
    }
    else if (command.startsWith("enable")) {
      processEnableCommand(command);
    }
    else if (command.startsWith("disable")) {
      processDisableCommand(command);
    }
    else if (command.startsWith("test")) {
      processTestCommand(command);
    }
    else if (command != "") { // Solo mostrar error si no es comando vacío
      Serial.println(MSG_UNKNOWN_COMMAND);
    }
  }

private:
  // Mostrar ayuda con todos los comandos disponibles
  void showHelp() {
    Serial.println("\n=== Comandos Disponibles ===");
    Serial.println("help o ? - Mostrar esta ayuda");
    Serial.println("time - Mostrar hora actual");
    Serial.println("schedules - Mostrar horarios programados");
    Serial.println("status - Mostrar estado del sistema");
    Serial.println("feed - Alimentar manualmente");
    Serial.println("stop - Parada de emergencia");
    Serial.println("next - Mostrar próximo horario");
    Serial.println("");
    Serial.println("=== Configuración de Horarios ===");
    Serial.println("set X HH:MM - Configurar horario X (1-4) a HH:MM");
    Serial.println("set X off - Deshabilitar horario X");
    Serial.println("enable X - Habilitar horario X");
    Serial.println("disable X - Deshabilitar horario X");
    Serial.println("enable all - Habilitar todos los horarios");
    Serial.println("disable all - Deshabilitar todos los horarios");
    Serial.println("");
    Serial.println("=== Comandos de Prueba ===");
    Serial.println("test relay - Probar relay (3 segundos)");
    Serial.println("test led - Probar LED (parpadeo)");
    Serial.println("");
    Serial.println("Ejemplos:");
    Serial.println("  set 1 07:30  (horario 1 a las 7:30)");
    Serial.println("  disable 2    (deshabilitar horario 2)");
    Serial.println("============================\n");
  }

  // Mostrar estado completo del sistema
  void showSystemStatus() {
    Serial.println("\n=== Estado del Sistema ===");
    
    // Hora actual
    Serial.print("Hora actual: ");
    rtcManager->displayCurrentTime();
    
    // Estado del relay
    Serial.print("Relay: ");
    Serial.println(relayController->getRelayState() ? "ACTIVO" : "INACTIVO");
    
    // Estado de alimentación
    if (relayController->isFeedingActive()) {
      Serial.print("Alimentando - Tiempo restante: ");
      Serial.print(relayController->getRemainingFeedTime());
      Serial.println(" segundos");
    } else {
      Serial.println("No alimentando");
    }
    
    // Horarios habilitados
    Serial.print("Horarios habilitados: ");
    Serial.print(scheduleManager->getEnabledSchedulesCount());
    Serial.print("/");
    Serial.println(MAX_FEED_TIMES);
    
    // Próximo horario
    int nextSchedule = scheduleManager->getNextSchedule(*rtcManager);
    if (nextSchedule > 0) {
      FeedTime next = scheduleManager->getSchedule(nextSchedule);
      Serial.print("Próximo horario: #");
      Serial.print(nextSchedule);
      Serial.print(" a las ");
      printTwoDigits(next.hour);
      Serial.print(":");
      printTwoDigits(next.minute);
      Serial.println();
    } else {
      Serial.println("No hay horarios habilitados");
    }
    
    Serial.println("=========================\n");
  }

  // Mostrar próximo horario de alimentación
  void showNextSchedule() {
    int nextSchedule = scheduleManager->getNextSchedule(*rtcManager);
    if (nextSchedule > 0) {
      FeedTime next = scheduleManager->getSchedule(nextSchedule);
      Serial.print("Próximo horario: #");
      Serial.print(nextSchedule);
      Serial.print(" a las ");
      printTwoDigits(next.hour);
      Serial.print(":");
      printTwoDigits(next.minute);
      Serial.println();
    } else {
      Serial.println("No hay horarios habilitados");
    }
  }

  // Procesar comando "set"
  void processSetCommand(String command) {
    // Formato: "set 1 08:30" o "set 1 off"
    int firstSpace = command.indexOf(' ', 4);
    if (firstSpace == -1) {
      Serial.println("Formato incorrecto. Uso: set X HH:MM o set X off");
      return;
    }
    
    int scheduleNum = command.substring(4, firstSpace).toInt();
    String timeStr = command.substring(firstSpace + 1);
    
    if (scheduleNum < 1 || scheduleNum > MAX_FEED_TIMES) {
      Serial.print("Número de horario debe ser 1-");
      Serial.println(MAX_FEED_TIMES);
      return;
    }
    
    if (timeStr == "off") {
      if (scheduleManager->enableSchedule(scheduleNum, false)) {
        Serial.print("Horario ");
        Serial.print(scheduleNum);
        Serial.println(" deshabilitado");
      }
    }
    else if (timeStr.indexOf(':') == 2 && timeStr.length() == 5) {
      int hour = timeStr.substring(0, 2).toInt();
      int minute = timeStr.substring(3, 5).toInt();
      
      if (scheduleManager->setSchedule(scheduleNum, hour, minute)) {
        Serial.print("Horario ");
        Serial.print(scheduleNum);
        Serial.print(" configurado a ");
        Serial.println(timeStr);
      } else {
        Serial.println("Hora inválida. Formato: HH:MM (00:00 - 23:59)");
      }
    }
    else {
      Serial.println("Formato incorrecto. Uso: set X HH:MM o set X off");
    }
  }

  // Procesar comando "enable"
  void processEnableCommand(String command) {
    String parameter = command.substring(7); // Después de "enable "
    
    if (parameter == "all") {
      scheduleManager->enableAllSchedules();
      Serial.println("Todos los horarios habilitados");
    } else {
      int scheduleNum = parameter.toInt();
      if (scheduleNum >= 1 && scheduleNum <= MAX_FEED_TIMES) {
        if (scheduleManager->enableSchedule(scheduleNum, true)) {
          Serial.print("Horario ");
          Serial.print(scheduleNum);
          Serial.println(" habilitado");
        }
      } else {
        Serial.println("Número de horario inválido (1-4) o usa 'enable all'");
      }
    }
  }

  // Procesar comando "disable"
  void processDisableCommand(String command) {
    String parameter = command.substring(8); // Después de "disable "
    
    if (parameter == "all") {
      scheduleManager->disableAllSchedules();
      Serial.println("Todos los horarios deshabilitados");
    } else {
      int scheduleNum = parameter.toInt();
      if (scheduleNum >= 1 && scheduleNum <= MAX_FEED_TIMES) {
        if (scheduleManager->enableSchedule(scheduleNum, false)) {
          Serial.print("Horario ");
          Serial.print(scheduleNum);
          Serial.println(" deshabilitado");
        }
      } else {
        Serial.println("Número de horario inválido (1-4) o usa 'disable all'");
      }
    }
  }

  // Procesar comandos de prueba
  void processTestCommand(String command) {
    String parameter = command.substring(5); // Después de "test "
    
    if (parameter == "relay") {
      Serial.println("Probando relay por 3 segundos...");
      relayController->setRelayState(true);
      delay(3000);
      relayController->setRelayState(false);
      Serial.println("Prueba de relay completada");
    }
    else if (parameter == "led") {
      Serial.println("Probando LED...");
      relayController->blinkLed(5, 300);
      Serial.println("Prueba de LED completada");
    }
    else {
      Serial.println("Pruebas disponibles: 'test relay' o 'test led'");
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

#endif // SERIAL_COMMANDS_H
