#ifndef MAIN_CONFIG_H_
#define MAIN_CONFIG_H_

////////////////////////////////////////////////////////////////////////////////
/// System

#define APP_NAME "EvLamp"

////////////////////////////////////////////////////////////////////////////////
/// Settings

#define SETTINGS_MAGIC 0xbeef0003 // TODO project version to magic

#ifdef CONFIG_EL_WIFI_MODE_AP
    #define DEFAULT_WIFI_MODE WIFI_MODE_AP
#else
    #define DEFAULT_WIFI_MODE WIFI_MODE_STA
#endif

#if CONFIG_EL_WIFI_DHCP
    #define DEFAULT_WIFI_DHCP true
#else
    #define DEFAULT_WIFI_DHCP false
#endif

#define DEFAULT_WIFI_AP_MAXCONN 5

#if CONFIG_EL_LED_TYPE_APA106
    #define DEFAULT_LED_TYPE LED_STRIP_APA106
#elif CONFIG_EL_LED_TYPE_SK6812
    #define DEFAULT_LED_TYPE LED_STRIP_SK6812
#else
    #define DEFAULT_LED_TYPE LED_STRIP_WS2812
#endif

#define MIN_LED_CURRENT_LIMIT 100

////////////////////////////////////////////////////////////////////////////////
/// Bus

#define BUS_QUEUE_LEN 5
#define BUS_TIMEOUT_MS 500
#define BUS_EVENT_DATA_SIZE 64

////////////////////////////////////////////////////////////////////////////////
/// Main task

#define MAIN_TASK_STACK_SIZE 8192
#define MAIN_TASK_PRIORITY 5

////////////////////////////////////////////////////////////////////////////////
/// Input task

#define INPUT_TASK_STACK_SIZE 2048
#define INPUT_TASK_PRIORITY 5

////////////////////////////////////////////////////////////////////////////////
/// Surface

#define RMT_CHANNEL RMT_CHANNEL_0
#define SINGLE_LED_CURRENT_MA 60.0f
#define FPS_MAX 120

////////////////////////////////////////////////////////////////////////////////
/// Webserver

#define HTTPD_STACK_SIZE 32768
#define MAX_POST_SIZE 4096

#endif /* MAIN_CONFIG_H_ */
