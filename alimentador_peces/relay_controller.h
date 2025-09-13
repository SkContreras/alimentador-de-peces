/*
  relay_controller.h - Controlador del relay y LED indicador
  
  Este módulo maneja el control del relay que activa el alimentador
  y el LED indicador de estado.
*/

#ifndef RELAY_CONTROLLER_H
#define RELAY_CONTROLLER_H

#include "config.h"

class RelayController {
private:
  unsigned long feedStartTime;
  bool isFeeding;

public:
  // Constructor
  RelayController() : feedStartTime(0), isFeeding(false) {}

  // Inicializar los pines del relay y LED
  void begin() {
    pinMode(RELAY_1_PIN, OUTPUT);
    pinMode(RELAY_2_PIN, OUTPUT);
    pinMode(RELAY_3_PIN, OUTPUT);
    pinMode(RELAY_4_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    
    // Asegurar que el relay esté apagado al inicio
    digitalWrite(RELAY_1_PIN, LOW);
    digitalWrite(RELAY_2_PIN, LOW);
    digitalWrite(RELAY_3_PIN, LOW);
    digitalWrite(RELAY_4_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    
    // Verificar que los pines se configuraron correctamente
    delay(10);
    if (digitalRead(RELAY_1_PIN) != LOW || digitalRead(LED_PIN) != LOW) {
      // Error en configuración de pines
      isFeeding = false;
    }
  }

  // Iniciar proceso de alimentación
  void startFeeding(int scheduleNumber = 0) {
    if (isFeeding) {
      return;
    }
    
    // Activar relay y LED
    digitalWrite(RELAY_1_PIN, HIGH);
    digitalWrite(RELAY_2_PIN, HIGH);
    digitalWrite(RELAY_3_PIN, HIGH);
    digitalWrite(RELAY_4_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    
    // Registrar tiempo de inicio
    feedStartTime = millis();
    isFeeding = true;
  }

  // Controlar la duración de la alimentación
  void update() {
    if (isFeeding) {
      unsigned long currentTime = millis();
      unsigned long elapsed = currentTime - feedStartTime;
      
      // Protección contra overflow de millis()
      if (elapsed >= FEED_DURATION * 1000) {
        stopFeeding();
      }
      
      // Protección contra alimentación muy larga (máximo 30 segundos)
      if (elapsed > 30000) {
        emergencyStop();
      }
    }
  }

  // Detener proceso de alimentación
  void stopFeeding() {
    if (!isFeeding) return;
    
    // Desactivar relay y LED
    digitalWrite(RELAY_1_PIN, LOW);
    digitalWrite(RELAY_2_PIN, LOW);
    digitalWrite(RELAY_3_PIN, LOW);
    digitalWrite(RELAY_4_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    
    isFeeding = false;
  }

  // Verificar si está alimentando actualmente
  bool isFeedingActive() {
    return isFeeding;
  }

  // Obtener tiempo restante de alimentación en segundos
  int getRemainingFeedTime() {
    if (!isFeeding) return 0;
    
    unsigned long currentTime = millis();
    unsigned long elapsed = (currentTime - feedStartTime) / 1000;
    int remaining = FEED_DURATION - elapsed;
    return (remaining > 0) ? remaining : 0;
  }

  // Activar/desactivar relay manualmente (para pruebas)
  void setRelayState(bool state) {
    digitalWrite(RELAY_1_PIN, state ? HIGH : LOW);
    digitalWrite(RELAY_2_PIN, state ? HIGH : LOW);
    digitalWrite(RELAY_3_PIN, state ? HIGH : LOW);
    digitalWrite(RELAY_4_PIN, state ? HIGH : LOW);
  }

  // Activar/desactivar LED manualmente (para pruebas)
  void setLedState(bool state) {
    digitalWrite(LED_PIN, state ? HIGH : LOW);
  }

  // Parpadear LED (para indicaciones especiales)
  void blinkLed(int times = 3, int delayMs = 200) {
    bool originalState = digitalRead(LED_PIN);
    
    for (int i = 0; i < times; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(delayMs);
      digitalWrite(LED_PIN, LOW);
      delay(delayMs);
    }
    
    // Restaurar estado original
    digitalWrite(LED_PIN, originalState);
  }

  // Obtener estado actual del relay
  bool getRelayState() {
    return digitalRead(RELAY_1_PIN) == HIGH;
  }

  // Obtener estado actual del LED
  bool getLedState() {
    return digitalRead(LED_PIN) == HIGH;
  }

  // Forzar parada de emergencia
  void emergencyStop() {
    digitalWrite(RELAY_1_PIN, LOW);
    digitalWrite(RELAY_2_PIN, LOW);
    digitalWrite(RELAY_3_PIN, LOW);
    digitalWrite(RELAY_4_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    isFeeding = false;
  }
};

#endif // RELAY_CONTROLLER_H
