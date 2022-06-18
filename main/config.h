#ifndef EVLAMP_CONFIG_H_
#define EVLAMP_CONFIG_H_

#include <driver/rmt.h>

////////////////////////////////////////////////////////////////////////////////
/// System

#define APP_NAME "EvLamp"

////////////////////////////////////////////////////////////////////////////////
/// Settings

#define SETTINGS_MAGIC 0xbeef0012

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
#elif CONFIG_EL_LED_TYPE_SM16703
    #define DEFAULT_LED_TYPE LED_STRIP_SM16703
#else
    #define DEFAULT_LED_TYPE LED_STRIP_WS2812
#endif

#if CONFIG_EL_MATRIX_ROTATION_90
    #define DEFAULT_SURFACE_ROTATION SURFACE_ROTATION_90
#elif CONFIG_EL_MATRIX_ROTATION_180
    #define DEFAULT_SURFACE_ROTATION SURFACE_ROTATION_180
#elif CONFIG_EL_MATRIX_ROTATION_270
    #define DEFAULT_SURFACE_ROTATION SURFACE_ROTATION_270
#else
    #define DEFAULT_SURFACE_ROTATION SURFACE_ROTATION_0
#endif

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
/// Surface

#define FPS_MAX 120
#define SURFACE_TASK_STACK_SIZE 8192
#define MAX_SURFACE_BLOCKS 8 // RMT_CHANNEL_MAX
#define MAX_BLOCK_LEDS 1024
#define MIN_BLOCK_SIZE 8
#define MAX_BLOCK_SIZE 128

#if CONFIG_EL_MATRIX_H_BLOCKS * CONFIG_EL_MATRIX_V_BLOCKS > MAX_SURFACE_BLOCKS
    #error Too much LED blocks!
#endif

////////////////////////////////////////////////////////////////////////////////
/// Webserver

#define HTTPD_STACK_SIZE 32768
#define MAX_POST_SIZE 4096

#endif /* EVLAMP_CONFIG_H_ */
