/*
  schedule_manager.h - Gestor de horarios de alimentación
  
  Este módulo maneja todos los horarios programables de alimentación,
  incluyendo configuración, verificación y persistencia.
*/

#ifndef SCHEDULE_MANAGER_H
#define SCHEDULE_MANAGER_H

#include "config.h"
#include "rtc_manager.h"

class ScheduleManager {
private:
  FeedTime feedTimes[MAX_FEED_TIMES];
  bool lastFeedStatus[MAX_FEED_TIMES];

public:
  // Constructor
  ScheduleManager() {
    // Inicializar horarios predeterminados
    feedTimes[0] = {DEFAULT_SCHEDULE_1_HOUR, DEFAULT_SCHEDULE_1_MINUTE, DEFAULT_SCHEDULE_1_ENABLED};
    feedTimes[1] = {DEFAULT_SCHEDULE_2_HOUR, DEFAULT_SCHEDULE_2_MINUTE, DEFAULT_SCHEDULE_2_ENABLED};
    feedTimes[2] = {DEFAULT_SCHEDULE_3_HOUR, DEFAULT_SCHEDULE_3_MINUTE, DEFAULT_SCHEDULE_3_ENABLED};
    feedTimes[3] = {DEFAULT_SCHEDULE_4_HOUR, DEFAULT_SCHEDULE_4_MINUTE, DEFAULT_SCHEDULE_4_ENABLED};
    
    // Inicializar estados
    for (int i = 0; i < MAX_FEED_TIMES; i++) {
      lastFeedStatus[i] = false;
    }
  }

  // Inicializar el gestor de horarios
  void begin() {
    // Cargar horarios desde EEPROM si están disponibles
    // Por ahora usar los valores predeterminados
  }

  // Verificar si es momento de alimentar y retornar el número de horario (1-4) o 0 si no
  int checkFeedTime(RTCManager& rtcManager) {
    for (int i = 0; i < MAX_FEED_TIMES; i++) {
      if (!feedTimes[i].enabled) continue;
      
      // Verificar si estamos en el minuto correcto (más flexible que segundo exacto)
      bool shouldFeed = rtcManager.isInMinute(feedTimes[i].hour, feedTimes[i].minute);
      
      // Activar alimentación solo una vez por minuto
      if (shouldFeed && !lastFeedStatus[i]) {
        updateLastFeedStatus(rtcManager);
        return i + 1; // Retornar número de horario (1-4)
      }
    }
    
    // Actualizar estados sin activar alimentación
    updateLastFeedStatus(rtcManager);
    return 0; // No hay horario que activar
  }

  // Configurar un horario específico
  bool setSchedule(int scheduleNumber, int hour, int minute) {
    if (scheduleNumber < 1 || scheduleNumber > MAX_FEED_TIMES) {
      return false;
    }
    
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
      return false;
    }
    
    int index = scheduleNumber - 1;
    feedTimes[index].hour = hour;
    feedTimes[index].minute = minute;
    feedTimes[index].enabled = true;
    
    return true;
  }

  // Habilitar o deshabilitar un horario
  bool enableSchedule(int scheduleNumber, bool enabled) {
    if (scheduleNumber < 1 || scheduleNumber > MAX_FEED_TIMES) {
      return false;
    }
    
    feedTimes[scheduleNumber - 1].enabled = enabled;
    return true;
  }

  // Obtener información de un horario específico
  FeedTime getSchedule(int scheduleNumber) {
    if (scheduleNumber < 1 || scheduleNumber > MAX_FEED_TIMES) {
      return {-1, -1, false}; // Horario inválido
    }
    
    return feedTimes[scheduleNumber - 1];
  }

  // Mostrar todos los horarios programados
  void displaySchedules() {
    Serial.println("\n=== Horarios Programados ===");
    for (int i = 0; i < MAX_FEED_TIMES; i++) {
      Serial.print("Horario ");
      Serial.print(i + 1);
      Serial.print(": ");
      
      if (feedTimes[i].enabled) {
        printTwoDigits(feedTimes[i].hour);
        Serial.print(":");
        printTwoDigits(feedTimes[i].minute);
        Serial.println(" (Habilitado)");
      } else {
        Serial.println("Deshabilitado");
      }
    }
    Serial.print("Duración: ");
    Serial.print(FEED_DURATION);
    Serial.println(" segundos");
    Serial.println("============================\n");
  }

  // Mostrar horarios de forma compacta (para inicio del sistema)
  void displaySchedulesCompact() {
    Serial.println("Horarios programados:");
    for (int i = 0; i < MAX_FEED_TIMES; i++) {
      if (feedTimes[i].enabled) {
        Serial.print("Horario ");
        Serial.print(i + 1);
        Serial.print(": ");
        printTwoDigits(feedTimes[i].hour);
        Serial.print(":");
        printTwoDigits(feedTimes[i].minute);
        Serial.println();
      }
    }
  }

  // Verificar si algún horario está habilitado
  bool hasEnabledSchedules() {
    for (int i = 0; i < MAX_FEED_TIMES; i++) {
      if (feedTimes[i].enabled) {
        return true;
      }
    }
    return false;
  }

  // Contar horarios habilitados
  int getEnabledSchedulesCount() {
    int count = 0;
    for (int i = 0; i < MAX_FEED_TIMES; i++) {
      if (feedTimes[i].enabled) {
        count++;
      }
    }
    return count;
  }

  // Obtener el próximo horario de alimentación
  int getNextSchedule(RTCManager& rtcManager) {
    DateTime now = rtcManager.now();
    int currentMinutes = now.hour() * 60 + now.minute();
    int nextSchedule = -1;
    int minDifference = 24 * 60; // Un día completo en minutos
    
    for (int i = 0; i < MAX_FEED_TIMES; i++) {
      if (!feedTimes[i].enabled) continue;
      
      int scheduleMinutes = feedTimes[i].hour * 60 + feedTimes[i].minute;
      int difference;
      
      if (scheduleMinutes > currentMinutes) {
        difference = scheduleMinutes - currentMinutes;
      } else {
        difference = (24 * 60) - currentMinutes + scheduleMinutes; // Próximo día
      }
      
      if (difference < minDifference) {
        minDifference = difference;
        nextSchedule = i + 1;
      }
    }
    
    return nextSchedule;
  }

  // Deshabilitar todos los horarios
  void disableAllSchedules() {
    for (int i = 0; i < MAX_FEED_TIMES; i++) {
      feedTimes[i].enabled = false;
    }
  }

  // Habilitar todos los horarios
  void enableAllSchedules() {
    for (int i = 0; i < MAX_FEED_TIMES; i++) {
      feedTimes[i].enabled = true;
    }
  }

private:
  // Actualizar estado del último minuto para todos los horarios
  void updateLastFeedStatus(RTCManager& rtcManager) {
    for (int i = 0; i < MAX_FEED_TIMES; i++) {
      lastFeedStatus[i] = rtcManager.isInMinute(feedTimes[i].hour, feedTimes[i].minute);
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

#endif // SCHEDULE_MANAGER_H
