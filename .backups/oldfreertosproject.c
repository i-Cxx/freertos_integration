#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/uart.h"

#include "FreeRTOS.h"
#include "task.h"

// SPI Defines
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

// I2C Defines
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

// UART Defines
#define UART_ID uart1
#define BAUD_RATE 115200
#define UART_TX_PIN 4
#define UART_RX_PIN 5

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void) pcTaskName;
    (void) xTask;
    // Hier kannst du Fehlerbehandlung machen, z.B. Endlosschleife oder Debug-Ausgabe
    while(1) {}
}

void vApplicationMallocFailedHook(void)
{
    // Wird aufgerufen, wenn pvPortMalloc fehlschlägt
    while(1) {}
}

void main_task(void *pvParameters) {
    stdio_init_all();

    // SPI init
    spi_init(SPI_PORT, 1000*1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);

    // I2C init
    i2c_init(I2C_PORT, 400*1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Timer example
    add_alarm_in_ms(2000, NULL, NULL, false);

    // UART init
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_puts(UART_ID, "Hello, UART from FreeRTOS Task!\n");

    while (true) {
        printf("Hello, world from FreeRTOS!\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main() {
    xTaskCreate(main_task, "MainTask", 1024, NULL, 1, NULL);
    vTaskStartScheduler();

    while (true) {
        // Should never reach here
    }
}
