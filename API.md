# API

EvLamp provides a simple API based on HTTP GET and POST requests containing JSON data.

---

## Contents

- [Get firmware information](#get-firmware-information)
- [Get current WiFi settings](#get-current-wifi-settings)
- [Set WiFi settings](#set-wifi-settings)
- [Get current LED settings](#get-current-led-settings)
- [Set LED settings](#set-led-settings)
- [Reset system settings](#reset-system-settings)
- [Reboot device](#reboot-device)
- [Get current lamp state](#get-current-lamp-state)
- [Set lamp state](#set-lamp-state)
- [Get full list of effects and its parameters](#get-full-list-of-effects-and-its-parameters)
- [Reset parameters of all effects](#reset-parameters-of-all-effects)
- [Get current effect parameters](#get-current-effect-parameters)
- [Set effect parameters](#set-effect-parameters)

---

## Get firmware information

### Request

**GET** `/api/info`

### Response

```jsonc
{
  "app_name": <String, Application name, usually "EvLamp">,
  "app_version": <String, Application version>,
  "build_date": <String, Application build date>,
  "idf_ver": <String, Version of used ESP-IDF>
}
```

**Example**

```json
{
  "app_name": "EvLamp",
  "app_version": "0.6.3",
  "build_date": "May  2 2022",
  "idf_ver": "v5.0-dev-1829-ge29e96101d-dirty"
}
```

---

## Get current WiFi settings

### Request

**GET** `/api/settings/wifi`

### Response

```jsonc
{
  "mode": <Integer, WiFi mode. 1 - Station, 2 - Access point>,
  "ip": {
    /* IP settings for both AP and station modes */
    "dhcp": <Boolean, DHCP client enable in station mode>,
    "ip": <String, IP address. In station mode only makes sense if DHCP client is disabled>,
    "netmask": <String, Network mask. In station mode only makes sense if DHCP client is disabled>,
    "gateway": <String, Gateway IP address. In station mode only makes sense if DHCP client is disabled>,
    "dns": <String, IP-address of DNS server. In station mode only makes sense if DHCP client is disabled, 
          in AP mode used as fallback DNS server'>
  },
  "ap": {
    /* Access Point settings, only makes sense in AP mode */
    "ssid": <String, SSID of Access Point>,
    "channel": <Integer, Channel number>,
    "password": <String, Access Point WPA2 password. If empty, Access point will be in open mode>,
  },
  "sta": {
    /* Station settings, only makes sense in station mode */
    "ssid": <String, Contains the SSID of the network being connected to in station mode>,
    "password": <String, WPA2 password>,
  }
}
```

**Example**

```json
{
  "mode": 1,
  "ip": {
    "dhcp": true,
    "ip": "192.168.1.1",
    "netmask": "255.255.255.0",
    "gateway": "192.168.1.1",
    "dns": "192.168.1.1"
  },
  "ap": {
    "ssid": "EvLamp",
    "channel": 6,
    "password": ""
  },
  "sta": {
    "ssid": "MyHomeWiFi",
    "password": "SuperSecret"
  }
}
```

---

## Set WiFi settings

### Request

**POST** `/api/settings/wifi`

```jsonc
{
  "mode": <Integer, WiFi mode. 1 - Station, 2 - Access point>,
  "ip": {
    /* IP settings for both AP and station modes */
    "dhcp": <Boolean, DHCP client enable in station mode>,
    "ip": <String, IP address. In station mode only makes sense if DHCP client is disabled>,
    "netmask": <String, Network mask. In station mode only makes sense if DHCP client is disabled>,
    "gateway": <String, Gateway IP address. In station mode only makes sense if DHCP client is disabled>,
    "dns": <String, IP-address of DNS server. In station mode only makes sense if DHCP client is disabled, 
          in AP mode used as fallback DNS server'>
  },
  "ap": {
    /* Access Point settings, only makes sense in AP mode */
    "ssid": <String, SSID of Access Point>,
    "channel": <Integer, Channel number>,
    "password": <String, Access Point WPA2 password. If empty, Access point will be in open mode>,
  },
  "sta": {
    /* Station settings, only makes sense in station mode */
    "ssid": <String, Contains the SSID of the network being connected to in station mode>,
    "password": <String, WPA2 password>,
  }
}
```

### Response

```jsonc
{
  "result": <Integer, Result code. 0 - no errors>,
  "name": <String, Name of the result code, "ESP_OK" for result code 0>,
  "message": <String, Error message if result code != 0>
}
```

---

## Get current LED settings

### Request

**GET** `/api/settings/leds`

### Response

```jsonc
{
  "block_width": <Integer, single block (LED matrix) width (number of columns), 8..128>,
  "block_height": <Integer, single block (LED matrix) height (number of rows), 8..128>,
  "h_blocks": <Integer, number of blocks horizontally>,
  "v_blocks": <Integer, number of blocks vertically>,
  "type": <Integer, Type of LEDs. 0 - WS2812, 1 - SK6812, 2 - APA106>,
  "current_limit": <Integer, Approximate limit of current consumed by the single block (LED matrix) in milliamps.
                   The larger this value, the higher the maximum brightness.>,
}
```

*Note: The total number of blocks (h_blocks * v_blocks) should not be more than 8*

**Example**

Single 16x16 WS2812B matrix:

```json
{
  "block_width": 16,
  "block_height": 16,
  "h_blocks": 1,
  "v_blocks": 1,
  "type": 0,
  "current_limit": 5000
}
```

---

## Set LED settings

### Request

**POST** `/api/settings/leds`

```jsonc
{
  "block_width": <Integer, single block (LED matrix) width (number of columns), 8..128>,
  "block_height": <Integer, single block (LED matrix) height (number of rows), 8..128>,
  "h_blocks": <Integer, number of blocks horizontally>,
  "v_blocks": <Integer, number of blocks vertically>,
  "type": <Integer, Type of LEDs. 0 - WS2812, 1 - SK6812, 2 - APA106>,
  "current_limit": <Integer, Approximate limit of current consumed by the single block (LED matrix) in milliamps.
                   The larger this value, the higher the maximum brightness.>,
}
```

### Response

```jsonc
{
  "result": <Integer, Result code. 0 - no errors>,
  "name": <String, Name of the result code, "ESP_OK" for result code 0>,
  "message": <String, Error message if result code != 0>
}
```

---

## Reset system settings

Resets all system settings including both WiFi and LED settings to default values.
After resetting device must be rebooted.

### Request

**GET** `/api/settings/reset`

### Response

```jsonc
{
  "result": <Integer, Result code. 0 - no errors>,
  "name": <String, Name of the result code, "ESP_OK" for result code 0>,
  "message": <String, Error message if result code != 0>
}
```

---

## Reboot device

### Request

**GET** `/api/reboot`

### Response

No response, device just reboots.

---

## Get current lamp state

### Request

**GET** `/api/lamp/state`

### Response

```jsonc
{
  "on": <Boolean, true - Lamp is on, false - Lamp is off>,
  "effect": <Integer, Current effect ID, 0-based>,
  "brightness": <Integer, LED matrix brightness, 0..255>,
  "fps": <Integer, Frames per seconds, 0..120>
}
```

---

## Set lamp state

With this function you can switch LEDs on and off, change current effect, brightness and FPS.

### Request

**POST** `/api/lamp/state`

```jsonc
{
  "on": <Boolean, true - Lamp is on, false - Lamp is off>,
  "effect": <Integer, Selected effect ID, 0-based>,
  "brightness": <Integer, LED matrix brightness, 0..255>,
  "fps": <Integer, Frames per seconds, 0..120>
}
```

Any item in request can be omitted. For example, you can switch off the lamp with this POST:
```json
{ "on": false }
```
or change effect to "Fire" (id = 1) and set brightness:
```json
{ "effect": 1, "brightness": 50 }
```

### Response

```jsonc
{
  "result": <Integer, Result code. 0 - no errors>,
  "name": <String, Name of the result code, "ESP_OK" for result code 0>,
  "message": <String, Error message if result code != 0>
}
```

---

## Get full list of effects and its parameters

### Request

**GET** `/api/effects`

### Response

```jsonc
[
  {
    "name": <String, Effect name>,
    "params": [
      {
        "name": <String, Parameter name>,
        "min": <Integer, Minimal value>,
        "max": <Integer, Maximal value>,
        "default": <Integer, Default value>,
        "value": <Integer, Current value>
      },
      ...
    ]
  },
  ...
]
```

The effect index in the response array is the effect ID.
The parameter's index in the effect's parameter list is the parameter ID.

**Example**

```json
[{
    "name": "Color lamp",
    "params": [{
        "name": "Red",
        "min": 0,
        "max": 255,
        "default": 255,
        "value": 255
      }, {
        "name": "Green",
        "min": 0,
        "max": 255,
        "default": 159,
        "value": 159
      }, {
        "name": "Blue",
        "min": 0,
        "max": 255,
        "default": 70,
        "value": 70
      }]
  },
  {
    "name": "Fire",
    "params": [{
        "name": "Palette",
        "min": 0,
        "max": 7,
        "default": 0,
        "value": 0
      }, {
        "name": "Scale",
        "min": 10,
        "max": 100,
        "default": 60,
        "value": 60
      }]
  },
  ... 
  {
    "name": "Fireflies",
    "params": [{
        "name": "Number of fireflies",
        "min": 4,
        "max": 48,
        "default": 16,
        "value": 16
      }, {
        "name": "Chaotic movements",
        "min": 0,
        "max": 1,
        "default": 1,
        "value": 1
      }, {
        "name": "Speed",
        "min": 1,
        "max": 255,
        "default": 255,
        "value": 255
      }, {
        "name": "Update time",
        "min": 1,
        "max": 10,
        "default": 3,
        "value": 3
      }]
  }]
```

---

## Reset parameters of all effects

### Request

**GET** `/api/effects/reset`

### Response

```jsonc
{
  "result": <Integer, Result code. 0 - no errors>,
  "name": <String, Name of the result code, "ESP_OK" for result code 0>,
  "message": <String, Error message if result code != 0>
}
```

---

## Get current effect parameters

### Request

**GET** `/api/lamp/effect`

### Response

```jsonc
{
  "effect": <Integer, Current effect ID, 0-based>,
  "params": [
    {
      "name": <String, Parameter name>,
      "min": <Integer, Minimal value>,
      "max": <Integer, Maximal value>,
      "default": <Integer, Default value>,
      "value": <Integer, Current value>
    },
    ...
  ]
}
```

**Example**

```json
{
  "effect": 1,
  "params": [
    {
      "name": "Palette",
      "min": 0,
      "max": 7,
      "default": 0,
      "value": 0
    },
    {
      "name": "Scale",
      "min": 10,
      "max": 100,
      "default": 60,
      "value": 60
    }
  ]
}
```

---

## Set effect parameters

### Request

**POST** `/api/lamp/effect`

```jsonc
{
  "effect": <Integer, Effect ID, 0-based>,
  "params": [
    <Integer, 0 parameter value>,
    ...
    <Integer, n parameter value>,
  ]
}
```

**Example**

Set Palette = 1 and Scale = 60 for the "Fire" effect. 

```json
{
  "effect": 1,
  "params": [ 1, 60 ]
}
```

### Response

```jsonc
{
  "result": <Integer, Result code. 0 - no errors>,
  "name": <String, Name of the result code, "ESP_OK" for result code 0>,
  "message": <String, Error message if result code != 0>
}
```
