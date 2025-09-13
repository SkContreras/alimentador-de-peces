/*
  button_manager.h - Gestor de botones con anti-rebote y funciones avanzadas
  
  Este módulo maneja todos los botones del sistema incluyendo:
  - Anti-rebote (debounce)
  - Detección de pulsación larga
  - Repetición automática
  - Estados de botones
*/

#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include "config.h"

// Estados de los botones
enum ButtonState {
  BUTTON_RELEASED,
  BUTTON_PRESSED,
  BUTTON_LONG_PRESS,
  BUTTON_REPEAT
};

// Estructura para cada botón
struct Button {
  int pin;
  ButtonState state;
  ButtonState lastState;
  unsigned long lastPressTime;
  unsigned long lastReleaseTime;
  unsigned long lastRepeatTime;
  bool isPressed;
  bool wasPressed;
  bool longPressDetected;
  bool repeatActive;
};

class ButtonManager {
private:
  Button buttons[4];  // SELECT, UP, DOWN, CONFIRM
  
public:
  // Constructor
  ButtonManager() {
    // Inicializar botones
    buttons[0] = {BUTTON_SELECT_PIN, BUTTON_RELEASED, BUTTON_RELEASED, 0, 0, 0, false, false, false, false};
    buttons[1] = {BUTTON_UP_PIN, BUTTON_RELEASED, BUTTON_RELEASED, 0, 0, 0, false, false, false, false};
    buttons[2] = {BUTTON_DOWN_PIN, BUTTON_RELEASED, BUTTON_RELEASED, 0, 0, 0, false, false, false, false};
    buttons[3] = {BUTTON_CONFIRM_PIN, BUTTON_RELEASED, BUTTON_RELEASED, 0, 0, 0, false, false, false, false};
  }

  // Inicializar botones
  void begin() {
    pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP);
    pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
    pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
    pinMode(BUTTON_CONFIRM_PIN, INPUT_PULLUP);
    
    // Inicializar buzzer si está configurado
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
    
    // Sistema de botones inicializado
  }

  // Actualizar estado de todos los botones
  void update() {
    for (int i = 0; i < 4; i++) {
      updateButton(buttons[i]);
    }
  }

  // Verificar si un botón fue presionado (una sola vez)
  bool wasPressed(int buttonIndex) {
    if (buttonIndex < 0 || buttonIndex > 3) return false;
    
    if (buttons[buttonIndex].wasPressed) {
      buttons[buttonIndex].wasPressed = false;
      return true;
    }
    return false;
  }

  // Verificar si un botón está siendo presionado
  bool isPressed(int buttonIndex) {
    if (buttonIndex < 0 || buttonIndex > 3) return false;
    return buttons[buttonIndex].isPressed;
  }

  // Verificar si un botón tiene pulsación larga
  bool isLongPressed(int buttonIndex) {
    if (buttonIndex < 0 || buttonIndex > 3) return false;
    
    if (buttons[buttonIndex].longPressDetected) {
      buttons[buttonIndex].longPressDetected = false;
      return true;
    }
    return false;
  }

  // Verificar si un botón está en modo repetición
  bool isRepeating(int buttonIndex) {
    if (buttonIndex < 0 || buttonIndex > 3) return false;
    
    if (buttons[buttonIndex].repeatActive && 
        (millis() - buttons[buttonIndex].lastRepeatTime > BUTTON_REPEAT_DELAY)) {
      buttons[buttonIndex].lastRepeatTime = millis();
      return true;
    }
    return false;
  }

  // Funciones de conveniencia para botones específicos
  bool selectPressed() { return wasPressed(0); }
  bool upPressed() { return wasPressed(1); }
  bool downPressed() { return wasPressed(2); }
  bool confirmPressed() { return wasPressed(3); }

  bool selectLongPressed() { return isLongPressed(0); }
  bool upLongPressed() { return isLongPressed(1); }
  bool downLongPressed() { return isLongPressed(2); }
  bool confirmLongPressed() { return isLongPressed(3); }

  bool upRepeating() { return isRepeating(1); }
  bool downRepeating() { return isRepeating(2); }

  // Verificar si algún botón fue presionado
  bool anyButtonPressed() {
    return wasPressed(0) || wasPressed(1) || wasPressed(2) || wasPressed(3);
  }

  // Reproducir sonido de retroalimentación
  void beep(int duration = 100) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Reproducir sonido de confirmación
  void confirmBeep() {
    beep(50);
    delay(50);
    beep(50);
  }

  // Reproducir sonido de error
  void errorBeep() {
    beep(200);
    delay(100);
    beep(200);
  }

  // Obtener estado de un botón como texto (para debug)
  String getButtonStateText(int buttonIndex) {
    if (buttonIndex < 0 || buttonIndex > 3) return "INVALID";
    
    switch (buttons[buttonIndex].state) {
      case BUTTON_RELEASED: return "RELEASED";
      case BUTTON_PRESSED: return "PRESSED";
      case BUTTON_LONG_PRESS: return "LONG_PRESS";
      case BUTTON_REPEAT: return "REPEAT";
      default: return "UNKNOWN";
    }
  }

  // Mostrar estado de todos los botones (para debug) - ELIMINADO PARA AHORRAR MEMORIA

private:
  // Actualizar estado de un botón individual
  void updateButton(Button& button) {
    bool currentReading = digitalRead(button.pin) == LOW; // LOW = presionado (pull-up)
    unsigned long currentTime = millis();
    
    // Guardar estado anterior
    button.lastState = button.state;
    
    // Anti-rebote
    if (currentReading != button.isPressed) {
      if (currentTime - button.lastReleaseTime > BUTTON_DEBOUNCE_DELAY) {
        button.isPressed = currentReading;
        
        if (button.isPressed) {
          // Botón fue presionado
          button.lastPressTime = currentTime;
          button.state = BUTTON_PRESSED;
          button.wasPressed = true;
          button.longPressDetected = false;
          button.repeatActive = false;
        } else {
          // Botón fue liberado
          button.lastReleaseTime = currentTime;
          button.state = BUTTON_RELEASED;
          button.longPressDetected = false;
          button.repeatActive = false;
        }
      }
    }
    
    // Detectar pulsación larga
    if (button.isPressed && !button.longPressDetected && 
        (currentTime - button.lastPressTime > BUTTON_LONG_PRESS_TIME)) {
      button.state = BUTTON_LONG_PRESS;
      button.longPressDetected = true;
      button.repeatActive = true;
      button.lastRepeatTime = currentTime;
    }
    
    // Activar repetición automática
    if (button.longPressDetected && button.isPressed) {
      button.state = BUTTON_REPEAT;
      button.repeatActive = true;
    }
  }
};

// Índices de botones para facilitar el uso
#define BTN_SELECT 0
#define BTN_UP     1
#define BTN_DOWN   2
#define BTN_CONFIRM 3

#endif // BUTTON_MANAGER_H
