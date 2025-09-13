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
  int relayPins[4];
  int activeRelays;

  // Método privado para controlar todos los relays
  void setAllRelays(bool state) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(relayPins[i], state ? HIGH : LOW);
    }
  }

public:
  // Constructor
  RelayController() : feedStartTime(0), isFeeding(false), activeRelays(0) {
    relayPins[0] = RELAY_1_PIN;
    relayPins[1] = RELAY_2_PIN;
    relayPins[2] = RELAY_3_PIN;
    relayPins[3] = RELAY_4_PIN;
  }

  // Inicializar los pines del relay y LED
  void begin() {
    // Configurar pines de relays
    for (int i = 0; i < 4; i++) {
      pinMode(relayPins[i], OUTPUT);
    }
    pinMode(LED_PIN, OUTPUT);
    
    // Asegurar que todos los relays estén apagados al inicio
    setAllRelays(false);
    digitalWrite(LED_PIN, LOW);
    
    // Verificar que los pines se configuraron correctamente
    delay(10);
    if (digitalRead(relayPins[0]) != LOW || digitalRead(LED_PIN) != LOW) {
      isFeeding = false;
    }
  }

  // Iniciar proceso de alimentación
  void startFeeding(int scheduleNumber = 0) {
    if (isFeeding) {
      return;
    }
    
    // Activar todos los relays y LED
    setAllRelays(true);
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
    
    // Desactivar todos los relays y LED
    setAllRelays(false);
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

  // Activar/desactivar todos los relays manualmente (para pruebas)
  void setRelayState(bool state) {
    setAllRelays(state);
  }

  // Activar/desactivar un relay específico (para pruebas individuales)
  void setRelayState(int relayNumber, bool state) {
    if (relayNumber >= 1 && relayNumber <= 4) {
      digitalWrite(relayPins[relayNumber - 1], state ? HIGH : LOW);
    }
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

  // Obtener estado actual de todos los relays (true si al menos uno está activo)
  bool getRelayState() {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(relayPins[i]) == HIGH) {
        return true;
      }
    }
    return false;
  }

  // Obtener estado de un relay específico
  bool getRelayState(int relayNumber) {
    if (relayNumber >= 1 && relayNumber <= 4) {
      return digitalRead(relayPins[relayNumber - 1]) == HIGH;
    }
    return false;
  }

  // Obtener estado actual del LED
  bool getLedState() {
    return digitalRead(LED_PIN) == HIGH;
  }

  // Forzar parada de emergencia
  void emergencyStop() {
    setAllRelays(false);
    digitalWrite(LED_PIN, LOW);
    isFeeding = false;
  }

  // Iniciar alimentación con relays específicos
  void startFeedingWithRelays(int relayMask) {
    if (isFeeding) {
      return;
    }
    
    // Activar solo los relays especificados en el mask
    for (int i = 0; i < 4; i++) {
      if (relayMask & (1 << i)) {
        digitalWrite(relayPins[i], HIGH);
      }
    }
    digitalWrite(LED_PIN, HIGH);
    
    feedStartTime = millis();
    isFeeding = true;
    activeRelays = relayMask;
  }

  // Obtener número de relays activos
  int getActiveRelayCount() {
    int count = 0;
    for (int i = 0; i < 4; i++) {
      if (digitalRead(relayPins[i]) == HIGH) {
        count++;
      }
    }
    return count;
  }

  // Probar todos los relays secuencialmente
  void testAllRelays(int delayMs = 500) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(relayPins[i], HIGH);
      delay(delayMs);
      digitalWrite(relayPins[i], LOW);
    }
  }
};

#endif // RELAY_CONTROLLER_H
