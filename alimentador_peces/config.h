/*
  config.h - Configuración global del alimentador de peces
  
  Este archivo contiene todas las configuraciones del sistema:
  - Pines de conexión
  - Tiempos y delays
  - Mensajes del sistema
  - Configuraciones del LCD
*/

#ifndef CONFIG_H
#define CONFIG_H

// === CONFIGURACIÓN DE PINES ===
const int LED_PIN = 6;                // Pin del LED
const int RELAY_1_PIN = 7;              // Pin del relay
const int RELAY_2_PIN = 8;              // Pin del relay
const int RELAY_3_PIN = 9;              // Pin del relay
const int RELAY_4_PIN = 10;              // Pin del relay
const int BUZZER_PIN = 13;             // Pin del buzzer

// Pines de botones
const int BUTTON_SELECT_PIN = 2;      // Botón SELECT
const int BUTTON_UP_PIN = 3;          // Botón UP
const int BUTTON_DOWN_PIN = 4;        // Botón DOWN
const int BUTTON_CONFIRM_PIN = 5;     // Botón CONFIRM

// === CONFIGURACIÓN DEL LCD ===
const int LCD_ADDRESS = 0x27;         // Dirección I2C del LCD (cambiar si es necesario)
const int LCD_COLUMNS = 20;           // Columnas del LCD
const int LCD_ROWS = 4;               // Filas del LCD
const bool USE_LCD = true;            // Habilitar LCD

// === CONFIGURACIÓN DE TIEMPOS ===
const int FEED_DURATION = 10;          // Duración de alimentación en segundos
const unsigned long LOOP_DELAY = 100; // Delay del loop principal (ms)
const unsigned long TIME_DISPLAY_INTERVAL = 30000; // Mostrar hora cada 30s

// === CONFIGURACIÓN DE BOTONES ===
const unsigned long BUTTON_DEBOUNCE_DELAY = 50;    // Debounce de botones (ms)
const unsigned long BUTTON_LONG_PRESS_TIME = 1000; // Tiempo para pulsación larga (ms)
const unsigned long BUTTON_REPEAT_DELAY = 200;     // Delay para repetición (ms)

// === CONFIGURACIÓN DE MENÚS ===
const unsigned long MENU_TIMEOUT = 30000;          // Timeout del menú (30s)
const unsigned long DISPLAY_UPDATE_INTERVAL = 1000; // Actualización display (ms)
const unsigned long CLOCK_UPDATE_INTERVAL = 5000;   // Actualización reloj (ms)

// === HORARIOS PREDETERMINADOS ===
// Horario 1: 8:00 AM
#define DEFAULT_SCHEDULE_1_HOUR   8
#define DEFAULT_SCHEDULE_1_MINUTE 0
#define DEFAULT_SCHEDULE_1_ENABLED true

// Horario 2: 12:00 PM
#define DEFAULT_SCHEDULE_2_HOUR   12
#define DEFAULT_SCHEDULE_2_MINUTE 55
#define DEFAULT_SCHEDULE_2_ENABLED true

// Horario 3: 18:00 PM
#define DEFAULT_SCHEDULE_3_HOUR   18
#define DEFAULT_SCHEDULE_3_MINUTE 0
#define DEFAULT_SCHEDULE_3_ENABLED true

// Horario 4: 22:00 PM
#define DEFAULT_SCHEDULE_4_HOUR   22
#define DEFAULT_SCHEDULE_4_MINUTE 0
#define DEFAULT_SCHEDULE_4_ENABLED true

// === MENSAJES DEL SISTEMA ===
#define MSG_SYSTEM_START "=== ALIMENTADOR DE PECES v3.0 ==="
#define MSG_RTC_ERROR "Error: No se pudo inicializar el RTC"
#define MSG_RTC_LOST_POWER "RTC perdió la hora, configurando..."
#define MSG_FEED_START "¡Hora de alimentar! Horario #"
#define MSG_FEED_COMPLETED "Alimentación completada"
#define MSG_FEED_MANUAL "Alimentación manual iniciada"
#define MSG_FEED_STOPPED "Alimentación detenida"
#define MSG_SCHEDULE_UPDATED "Horario actualizado"
#define MSG_SCHEDULE_DISABLED "Horario deshabilitado"
#define MSG_SCHEDULE_ENABLED "Horario habilitado"
#define MSG_INVALID_TIME "Hora inválida"
#define MSG_INVALID_SCHEDULE "Horario inválido"
#define MSG_NO_SCHEDULES "No hay horarios activos"
#define MSG_BUTTON_CONTROLS "Controles: SELECT=Menu, CONFIRM=Alimentar"

// === MENSAJES DE COMANDOS SERIALES ===
#define MSG_COMMAND_HELP "Comandos disponibles:"
#define MSG_COMMAND_TIME "time - Ver hora actual"
#define MSG_COMMAND_SCHEDULES "schedules - Ver horarios"
#define MSG_COMMAND_FEED "feed - Alimentar manualmente"
#define MSG_COMMAND_STOP "stop - Parar alimentación"
#define MSG_COMMAND_NEXT "next - Ver próximo horario"
#define MSG_COMMAND_SET "set X HH:MM - Configurar horario X"
#define MSG_COMMAND_SET_OFF "set X off - Deshabilitar horario X"
#define MSG_COMMAND_ENABLE "enable X - Habilitar horario X"
#define MSG_COMMAND_DISABLE "disable X - Deshabilitar horario X"
#define MSG_COMMAND_ENABLE_ALL "enable all - Habilitar todos"
#define MSG_COMMAND_DISABLE_ALL "disable all - Deshabilitar todos"
#define MSG_COMMAND_TEST_RELAY "test relay - Probar relay"
#define MSG_COMMAND_TEST_LED "test led - Probar LED"
#define MSG_COMMAND_HELP_CMD "help - Mostrar ayuda"

// === MENSAJES DE ERROR ===
#define MSG_ERROR_INVALID_COMMAND "Comando inválido"
#define MSG_ERROR_INVALID_SCHEDULE_NUM "Número de horario inválido"
#define MSG_ERROR_INVALID_TIME_FORMAT "Formato de hora inválido"
#define MSG_ERROR_SCHEDULE_NOT_FOUND "Horario no encontrado"
#define MSG_ERROR_ALREADY_FEEDING "Ya se está alimentando"
#define MSG_ERROR_NOT_FEEDING "No hay alimentación activa"

// === CONFIGURACIÓN DE DEBUG ===
const bool DEBUG_MODE = true;         // Habilitar mensajes de debug
const bool SERIAL_ENABLED = true;     // Habilitar comunicación serial

// === CONFIGURACIÓN DE MEMORIA EEPROM ===
const int EEPROM_SCHEDULE_START = 0;  // Dirección inicial para horarios
const int EEPROM_SCHEDULE_SIZE = 3;   // Tamaño de cada horario (hora, minuto, enabled)

// === CONFIGURACIÓN DE ALIMENTACIÓN ===
const int MIN_FEED_DURATION = 1;      // Duración mínima (segundos)
const int MAX_FEED_DURATION = 30;     // Duración máxima (segundos)
const int EMERGENCY_STOP_DURATION = 1; // Duración para parada de emergencia

// === CONFIGURACIÓN DE VALIDACIÓN ===
const int MIN_HOUR = 0;               // Hora mínima
const int MAX_HOUR = 23;              // Hora máxima
const int MIN_MINUTE = 0;             // Minuto mínimo
const int MAX_MINUTE = 59;            // Minuto máximo
const int MIN_YEAR = 2000;            // Año mínimo
const int MAX_YEAR = 2099;            // Año máximo

// === CONFIGURACIÓN DE BEEPERS ===
const int BEEP_SHORT = 100;           // Duración beep corto (ms)
const int BEEP_LONG = 300;            // Duración beep largo (ms)
const int BEEP_ERROR = 500;           // Duración beep de error (ms)
const int BEEP_FREQUENCY = 1000;      // Frecuencia del beep (Hz)

// === CONFIGURACIÓN DE COMUNICACIÓN SERIAL ===
const int SERIAL_BAUD_RATE = 9600;    // Velocidad del puerto serie

// === ESTRUCTURAS DE DATOS ===
// Estructura para horarios de alimentación
struct FeedTime {
  int hour;      // Hora (0-23)
  int minute;    // Minuto (0-59)
  bool enabled;  // Habilitado/deshabilitado
};

// === CONSTANTES DE HORARIOS ===
const int MAX_FEED_TIMES = 4;         // Número máximo de horarios

// === MENSAJES FALTANTES ===
#define MSG_ALREADY_FEEDING "Ya se está alimentando"
#define MSG_FEED_ACTIVATED "Alimentación activada - Horario #"
#define MSG_UNKNOWN_COMMAND "Comando desconocido"
#define MSG_SYSTEM_END "=== SISTEMA FINALIZADO ==="

#endif // CONFIG_H