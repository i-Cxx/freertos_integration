// webserver.c

#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/watchdog.h"
#include "hardware/structs/watchdog.h"

#include "tusb_lwip_glue.h"
#include "lwip/apps/httpd.h"

#include "FreeRTOS.h"
#include "task.h"

#define LED_PIN 25

// CGI Handler
static const char *cgi_toggle_led(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    gpio_put(LED_PIN, !gpio_get(LED_PIN));
    return "/index.html";
}

static const char *cgi_reset_usb_boot(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    reset_usb_boot(0, 0);
    return "/index.html";
}

static const tCGI cgi_handlers[] = {
    {"/toggle_led", cgi_toggle_led},
    {"/reset_usb_boot", cgi_reset_usb_boot}
};

static void webserver_task(void *pvParameters)
{
    (void) pvParameters;

    // Hardware-LED konfigurieren
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Netzwerk Stack initialisieren
    init_lwip();
    wait_for_netif_is_up();
    dhcpd_init();

    // HTTP Server initialisieren
    httpd_init();
    http_set_cgi_handlers(cgi_handlers, LWIP_ARRAYSIZE(cgi_handlers));

    while (1)
    {
        tud_task();       // TinyUSB Device stack Task
        service_traffic(); // LWIP HTTP Traffic Service
        vTaskDelay(pdMS_TO_TICKS(10)); // kurze Pause, damit andere Tasks CPU bekommen
    }
}

// Extern in header:
void start_webserver_task(void);
