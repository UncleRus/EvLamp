#ifndef EVLAMP_PALETTES_H
#define EVLAMP_PALETTES_H

#include <color.h>

enum {
    PALETTE_FIRE = 0,
    PALETTE_WOOD_FIRE,
    PALETTE_SODIUM_FIRE,
    PALETTE_COPPER_FIRE,
    PALETTE_ALCOHOL_FIRE,
    PALETTE_RUBIDIUM_FIRE,
    PALETTE_POTASSIUM_FIRE,
    PALETTE_LITHIUM_FIRE,

    PALETTE_CLOUD,
    PALETTE_LAVA,
//    PALETTE_FOREST,
//    PALETTE_RAINBOW,
//    PALETTE_HEAT,

    PALETTE_MAX
};

typedef struct
{
    size_t size;
    const rgb_t *palette;
} palette_def_t;

#define C_BLACK          { .r = 0,    .g = 0,    .b = 0 }
#define C_WHITE          { .r = 0xff, .g = 0xff, .b = 0xff }
#define C_GREEN          { .r = 0,    .g = 0xff, .b = 0 }
#define C_BLUE           { .r = 0,    .g = 0,    .b = 0xff }
#define C_DARK_BLUE      { .r = 0,    .g = 0,    .b = 0x8b }
#define C_RED            { .r = 0xff, .g = 0,    .b = 0 }
#define C_YELLOW         { .r = 0xff, .g = 0xff, .b = 0 }
#define C_ORANGE_RED     { .r = 0xff, .g = 0x45, .b = 0 }
#define C_ORANGE         { .r = 0xff, .g = 0x80, .b = 0 }
#define C_GOLD           { .r = 0xff, .g = 0xdf, .b = 0 }
#define C_GOLDENROD      { .r = 0xda, .g = 0xa5, .b = 0x20 }
#define C_GREEN_YELLOW   { .r = 0xad, .g = 0xff, .b = 0x2f }
#define C_LIME_GREEN     { .r = 0x32, .g = 0xcd, .b = 0x32 }
#define C_DEEP_SKY_BLUE  { .r = 0,    .g = 0xbf, .b = 0xff }
#define C_LIGHT_SKY_BLUE { .r = 0x87, .g = 0xce, .b = 0xfa }
#define C_INDIGO         { .r = 0x4b, .g = 0,    .b = 0x82 }
#define C_MEDIUM_PURPLE  { .r = 0x93, .g = 0x70, .b = 0xdb }
#define C_DEEP_PINK      { .r = 0xff, .g = 0x14, .b = 0x93 }
#define C_PINK           { .r = 0xff, .g = 0xc0, .b = 0xcb }
#define C_FIRE_BRICK     { .r = 0xb2, .g = 0x22, .b = 0x22 }
#define C_NAVY           { .r = 0,    .g = 0,    .b = 0x80 }
#define C_SKY_BLUE       { .r = 0x87, .g = 0xce, .b = 0xeb }
#define C_LIGHT_BLUE     { .r = 0xad, .g = 0xd8, .b = 0xe6 }
#define C_MAROON         { .r = 0x80, .g = 0,    .b = 0    }
#define C_DARK_RED       { .r = 0x8b, .g = 0,    .b = 0    }

extern const palette_def_t palettes[PALETTE_MAX];

#endif //EVLAMP_PALETTES_H
