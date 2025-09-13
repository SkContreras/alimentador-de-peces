/*
  rtc_manager.h - Gestor del reloj de tiempo real DS3231
  
  Este módulo maneja todas las operaciones relacionadas con el RTC DS3231,
  incluyendo inicialización, lectura de tiempo y configuración.
*/

#ifndef RTC_MANAGER_H
#define RTC_MANAGER_H

#include <Wire.h>
#include <RTClib.h>
#include "config.h"

class RTCManager {
private:
  RTC_DS3231 rtc;
  unsigned long lastTimeDisplay;

public:
  // Constructor
  RTCManager() : lastTimeDisplay(0) {}

  // Inicializar el RTC
  bool begin() {
    if (!rtc.begin()) {
      return false;
    }
    
    // Si el RTC perdió la hora, configurar con la hora de compilación
    if (rtc.lostPower()) {
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    
    return true;
  }

  // Obtener la fecha y hora actual
  DateTime now() {
    return rtc.now();
  }

  // Mostrar la hora actual en formato legible
  void displayCurrentTime() {
    DateTime currentTime = rtc.now();
    displayTime(currentTime);
  }

  // Mostrar una hora específica en formato legible
  void displayTime(DateTime time) {
    Serial.print("Hora actual: ");
    printTwoDigits(time.hour());
    Serial.print(":");
    printTwoDigits(time.minute());
    Serial.print(":");
    printTwoDigits(time.second());
    Serial.print(" - ");
    Serial.print(time.day());
    Serial.print("/");
    Serial.print(time.month());
    Serial.print("/");
    Serial.println(time.year());
  }

  // Mostrar hora periódicamente (cada TIME_DISPLAY_INTERVAL)
  void displayTimeIfNeeded() {
    if (millis() - lastTimeDisplay > TIME_DISPLAY_INTERVAL) {
      displayCurrentTime();
      lastTimeDisplay = millis();
    }
  }

  // Verificar si es una hora y minuto específicos (segundo = 0)
  bool isExactTime(int hour, int minute) {
    DateTime currentTime = rtc.now();
    return (currentTime.hour() == hour && 
            currentTime.minute() == minute && 
            currentTime.second() == 0);
  }

  // Verificar si estamos en un minuto específico
  bool isInMinute(int hour, int minute) {
    DateTime currentTime = rtc.now();
    return (currentTime.hour() == hour && currentTime.minute() == minute);
  }

  // Configurar fecha y hora manualmente
  void setDateTime(int year, int month, int day, int hour, int minute, int second) {
    rtc.adjust(DateTime(year, month, day, hour, minute, second));
  }

  // Obtener componentes individuales del tiempo
  int getCurrentHour() {
    return rtc.now().hour();
  }

  int getCurrentMinute() {
    return rtc.now().minute();
  }

  int getCurrentSecond() {
    return rtc.now().second();
  }

  int getCurrentDay() {
    return rtc.now().day();
  }

  int getCurrentMonth() {
    return rtc.now().month();
  }

  int getCurrentYear() {
    return rtc.now().year();
  }

  // === FUNCIONES PARA AJUSTE CON BOTONES ===
  
  // Configurar solo la hora (mantiene fecha actual)
  void setTime(int hour, int minute, int second = 0) {
    DateTime currentTime = rtc.now();
    rtc.adjust(DateTime(currentTime.year(), currentTime.month(), currentTime.day(), 
                       hour, minute, second));
    
    Serial.print("Hora ajustada a: ");
    printTwoDigits(hour);
    Serial.print(":");
    printTwoDigits(minute);
    Serial.print(":");
    printTwoDigits(second);
    Serial.println();
  }

  // Configurar solo la fecha (mantiene hora actual)
  void setDate(int year, int month, int day) {
    DateTime currentTime = rtc.now();
    rtc.adjust(DateTime(year, month, day, 
                       currentTime.hour(), currentTime.minute(), currentTime.second()));
    
    Serial.print("Fecha ajustada a: ");
    Serial.print(day);
    Serial.print("/");
    Serial.print(month);
    Serial.print("/");
    Serial.println(year);
  }

  // Incrementar hora (con rollover)
  void incrementHour() {
    DateTime currentTime = rtc.now();
    int newHour = (currentTime.hour() + 1) % 24;
    setTime(newHour, currentTime.minute(), currentTime.second());
  }

  // Decrementar hora (con rollover)
  void decrementHour() {
    DateTime currentTime = rtc.now();
    int newHour = (currentTime.hour() - 1 + 24) % 24;
    setTime(newHour, currentTime.minute(), currentTime.second());
  }

  // Incrementar minuto (con rollover)
  void incrementMinute() {
    DateTime currentTime = rtc.now();
    int newMinute = (currentTime.minute() + 1) % 60;
    int newHour = currentTime.hour();
    
    // Si los minutos pasan de 59 a 0, incrementar hora
    if (currentTime.minute() == 59) {
      newHour = (newHour + 1) % 24;
    }
    
    setTime(newHour, newMinute, 0); // Resetear segundos al ajustar
  }

  // Decrementar minuto (con rollover)
  void decrementMinute() {
    DateTime currentTime = rtc.now();
    int newMinute = (currentTime.minute() - 1 + 60) % 60;
    int newHour = currentTime.hour();
    
    // Si los minutos pasan de 0 a 59, decrementar hora
    if (currentTime.minute() == 0) {
      newHour = (newHour - 1 + 24) % 24;
    }
    
    setTime(newHour, newMinute, 0); // Resetear segundos al ajustar
  }

  // Incrementar día (con validación de mes/año)
  void incrementDay() {
    DateTime currentTime = rtc.now();
    DateTime newTime = DateTime(currentTime.year(), currentTime.month(), currentTime.day() + 1,
                               currentTime.hour(), currentTime.minute(), currentTime.second());
    rtc.adjust(newTime);
    Serial.println("Día incrementado");
  }

  // Decrementar día (con validación de mes/año)
  void decrementDay() {
    DateTime currentTime = rtc.now();
    DateTime newTime = DateTime(currentTime.year(), currentTime.month(), currentTime.day() - 1,
                               currentTime.hour(), currentTime.minute(), currentTime.second());
    rtc.adjust(newTime);
    Serial.println("Día decrementado");
  }

  // Incrementar mes
  void incrementMonth() {
    DateTime currentTime = rtc.now();
    int newMonth = currentTime.month() + 1;
    int newYear = currentTime.year();
    
    if (newMonth > 12) {
      newMonth = 1;
      newYear++;
    }
    
    setDate(newYear, newMonth, currentTime.day());
  }

  // Decrementar mes
  void decrementMonth() {
    DateTime currentTime = rtc.now();
    int newMonth = currentTime.month() - 1;
    int newYear = currentTime.year();
    
    if (newMonth < 1) {
      newMonth = 12;
      newYear--;
    }
    
    setDate(newYear, newMonth, currentTime.day());
  }

  // Incrementar año
  void incrementYear() {
    DateTime currentTime = rtc.now();
    setDate(currentTime.year() + 1, currentTime.month(), currentTime.day());
  }

  // Decrementar año
  void decrementYear() {
    DateTime currentTime = rtc.now();
    setDate(currentTime.year() - 1, currentTime.month(), currentTime.day());
  }

  // Validar si una fecha es válida
  bool isValidDate(int year, int month, int day) {
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    if (year < 2000 || year > 2099) return false;
    
    // Validación básica de días por mes
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Año bisiesto
    if (month == 2 && isLeapYear(year)) {
      return day <= 29;
    }
    
    return day <= daysInMonth[month - 1];
  }

  // Verificar si es año bisiesto
  bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
  }

private:
  // Función auxiliar para imprimir números con dos dígitos
  void printTwoDigits(int number) {
    if (number < 10) {
      Serial.print("0");
    }
    Serial.print(number);
  }
};

#endif // RTC_MANAGER_H
