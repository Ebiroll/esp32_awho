#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include <string.h>
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netif.h"
#include "netif/etharp.h"


//#include "cpu.h"
// Change in lwipopts.h add 
// #define ETHARP_DEBUG                    LWIP_DBG_ON

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

int app_main(void)
{
    // To ensure proper MMU setup, not needed
    //cpu_configure_region_protection();
    nvs_flash_init();
    //system_init();
    printf("Initiated\n");

    tcpip_adapter_init();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    // For qemu debugging
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    wifi_config_t sta_config = {
        .sta = {
	   //#include "secret.h"
	   .ssid = "ssid",
	   .password = "password",
	   .bssid_set = false
        }
    };
    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() );
    printf("Connected\n");

    //gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
    int level = 0;
    // Send arp request

    struct netif *netif;

    for (netif = netif_list; netif != NULL; netif = netif->next) {
        printf("%c%c%d\n",netif->name[0],netif->name[1],netif->num);
        fflush(stdout);
    }


    ip4_addr_t scanaddr;

    netif=netif_find("wl0");
    if (!netif) {
        printf("No wl0");
        netif=netif_find("en0");
        if (!netif) {
            printf("No en0");
        }
    }

    unsigned char hostnum=1;
    char tmpBuff[20];
    while (true) {

        sprintf(tmpBuff,"192.168.1.%d",hostnum);

        IP4_ADDR(&scanaddr, 192, 168 , 1, hostnum);

        if (hostnum%10==0) 
        {
            printf("Netlist\n");
            for (netif = netif_list; netif != NULL; netif = netif->next) {
                printf("%c%c%d\n",netif->name[0],netif->name[1],netif->num);
                fflush(stdout);
            }
            netif=netif_find("en0");
        }



        //gpio_set_level(GPIO_NUM_4, level);
        if (netif)
        {
            printf("ARP request %s\n",tmpBuff);
            err_t ret=etharp_request(netif, &scanaddr);
            if (ret<0) {
                printf("Failed request %s\n",tmpBuff);
            }
        }

        level = !level;
	    printf(".");
        //vTaskDelay(300 / portTICK_PERIOD_MS);
        hostnum++;
    }

    return 0;
}

