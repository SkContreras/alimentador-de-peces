/*
  diagnostico_lcd.ino - Diagnóstico simple del LCD
  Este sketch verifica si el LCD funciona correctamente
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Configuración del LCD
const int LCD_ADDRESS = 0x27;  // Cambiar si es necesario
const int LCD_COLUMNS = 20;
const int LCD_ROWS = 4;

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void setup() {
  // Inicializar I2C
  Wire.begin();
  delay(100);
  
  // Inicializar LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Mostrar mensaje de prueba
  lcd.setCursor(0, 0);
  lcd.print("LCD FUNCIONANDO");
  lcd.setCursor(0, 1);
  lcd.print("Direccion: 0x");
  lcd.print(LCD_ADDRESS, HEX);
  lcd.setCursor(0, 2);
  lcd.print("20x4 I2C");
  lcd.setCursor(0, 3);
  lcd.print("Test exitoso");
  
  delay(3000);
  
  // Mostrar contador
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Contador:");
  
  for (int i = 0; i < 10; i++) {
    lcd.setCursor(0, 1);
    lcd.print("Segundo: ");
    lcd.print(i);
    lcd.print("   ");
    delay(1000);
  }
  
  // Mostrar mensaje final
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("LCD OK - Usar en");
  lcd.setCursor(0, 2);
  lcd.print("alimentador_peces.ino");
}

void loop() {
  // Nada que hacer aquí
  delay(1000);
}
