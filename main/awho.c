

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"


#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"


#include "netif/etharp.h"
#include "netif/etharp.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "telnet.h"

static const char *TAG = "awho";


extern void echo_application_thread(void *pvParameters);

extern void Task_lwip_init(void * pParam);

//#include "cpu.h"
// Change in lwipopts.h add 
// #define ETHARP_DEBUG                    LWIP_DBG_ON

/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t wifi_event_group;

/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
const int CONNECTED_BIT = BIT0;


static char tag[] = "telnet";

static void recvData(uint8_t *buffer, size_t size) {
	char responseMessage[100];
	ESP_LOGD(tag, "We received: %.*s", size, buffer);
	sprintf(responseMessage, "Thanks for %d bytes of data\n", size);
	telnet_esp32_sendData((uint8_t *)responseMessage, strlen(responseMessage));
}

static void telnetTask(void *data) {
	ESP_LOGD(tag, ">> telnetTask");
	telnet_esp32_listenForClients(recvData);
	ESP_LOGD(tag, "<< telnetTask");
	vTaskDelete(NULL);
}


static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        xTaskCreatePinnedToCore(&telnetTask, "telnetTask", 8048, NULL, 5, NULL, 0);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        /* This is a workaround as ESP32 WiFi libs don't currently
           auto-reassociate. */
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
}

static void initialise_wifi(void)
{
   printf("Initiated\n");


    tcpip_adapter_init();
    printf("adapter_init\n");

    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    wifi_config_t wifi_config = {
        .sta = {
	    //#include "secret.h"
            .ssid = "ssid",
            .password = "password",
        },
    };
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    printf("Connected\n");

}

int app_main(void)
{
    nvs_flash_init();
    //initialise_wifi();
    Task_lwip_init(NULL);
 
    //xTaskCreate(&echo_application_thread, "echo_thread", 2048, NULL, 12, NULL);
    // This is not needed with the real wifi task
    xTaskCreatePinnedToCore(&telnetTask, "telnetTask", 8048, NULL, 5, NULL, 0);

 /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_pad_select_gpio(GPIO_NUM_5);

    gpio_set_direction(GPIO_NUM_5, GPIO_MODE_OUTPUT);
    int level = 0;
    // Send arp request

    struct netif *netif;

    for (netif = netif_list; netif != NULL; netif = netif->next) {
        printf("%c%c%d\n",netif->name[0],netif->name[1],netif->num);
        fflush(stdout);
    }


    ip4_addr_t scanaddr;
    ip4_addr_t *cacheaddr;
    struct eth_addr *cachemac;

    netif=netif_find("en0");
    if (!netif) {
        printf("No en0");
    }

    unsigned char hostnum=1;
    char tmpBuff[20];
    while (true) {

        sprintf(tmpBuff,"192.168.1.%d",hostnum);

        IP4_ADDR(&scanaddr, 192, 168 , 1, hostnum);

        struct netif *chacheif=netif;
        for (int j=0;j<ARP_TABLE_SIZE;j++) {
                    if (1==etharp_get_entry(j, &cacheaddr, &chacheif, &cachemac))
                    {
                        printf("Found %d  %d.%d.%d.%d\n",j,IP2STR(cacheaddr));
                    }
        }

        if (hostnum==255) {
            // Clear arp cache
            printf("clean arp cache\n");
            etharp_cleanup_netif(netif); 
            vTaskDelay(800/portTICK_PERIOD_MS);
        }

        gpio_set_level(GPIO_NUM_5, level);
        if (netif)
        {
	    //printf("ARP request %s\n",tmpBuff);
            err_t ret=etharp_request(netif, &scanaddr);
            if (ret<0) {
                printf("Failed request %s\n",tmpBuff);
            }
        }

        level = !level;
        vTaskDelay(100/portTICK_PERIOD_MS);
        hostnum++;
    }

    return 0;
}

