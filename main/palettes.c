#include "palettes.h"

#define DECL_PALETTE(pal) { .size = sizeof(pal) / sizeof(rgb_t), .palette = pal }

static const rgb_t p_fire[]           = { C_BLACK, C_RED, C_YELLOW, C_WHITE };
static const rgb_t p_wood_fire[]      = { C_BLACK, C_ORANGE_RED, C_ORANGE, C_GOLD };
static const rgb_t p_sodium_fire[]    = { C_BLACK, C_ORANGE, C_GOLD, C_GOLDENROD };
static const rgb_t p_copper_fire[]    = { C_BLACK, C_GREEN, C_GREEN_YELLOW, C_LIME_GREEN };
static const rgb_t p_alcohol_fire[]   = { C_BLACK, C_BLUE, C_DEEP_SKY_BLUE, C_LIGHT_SKY_BLUE };
static const rgb_t p_rubidium_fire[]  = { C_BLACK, C_INDIGO, C_INDIGO, C_DARK_BLUE };
static const rgb_t p_potassium_fire[] = { C_BLACK, C_INDIGO, C_MEDIUM_PURPLE, C_DEEP_PINK };
static const rgb_t p_lithium_fire[]   = { C_BLACK, C_FIRE_BRICK, C_PINK, C_DEEP_PINK };

static const rgb_t p_cloud[] = {
    C_BLUE, C_DARK_BLUE, C_DARK_BLUE, C_DARK_BLUE,
    C_DARK_BLUE, C_DARK_BLUE, C_DARK_BLUE, C_DARK_BLUE,
    C_BLUE, C_DARK_BLUE, C_SKY_BLUE, C_SKY_BLUE,
    C_LIGHT_BLUE, C_WHITE, C_LIGHT_BLUE, C_SKY_BLUE
};
static const rgb_t p_lava[] = {
    C_BLACK, C_MAROON, C_BLACK, C_MAROON,
    C_DARK_RED, C_MAROON, C_DARK_RED, C_MAROON,
    C_DARK_RED, C_DARK_RED, C_RED, C_ORANGE,
    C_WHITE, C_ORANGE, C_RED, C_DARK_RED
};
static const rgb_t p_heat[] = {
    C_BLACK,
    { .r = 0x33, .g = 0, .b = 0 }, { .r = 0x66, .g = 0, .b = 0 }, { .r = 0x99, .g = 0, .b = 0 }, C_RED,
    { .r = 0xff, .g = 0x33, .b = 0 }, { .r = 0xff, .g = 0x66, .b = 0 }, { .r = 0xff, .g = 0x99, .b = 0 }, C_YELLOW,
    { .r = 0xff, .g = 0xff, .b = 0x33 }, { .r = 0xff, .g = 0xff, .b = 0x66 }, { .r = 0xff, .g = 0xff, .b = 0x99 }, C_WHITE
};
static const rgb_t p_ocean[] = {
    C_MIDNIGHT_BLUE, C_DARK_BLUE, C_MIDNIGHT_BLUE, C_NAVY,
    C_DARK_BLUE, C_MEDIUM_BLUE, C_SEA_GREEN, C_TEAL,
    C_CADET_BLUE, C_BLUE, C_DARK_CYAN, C_CORNFLOWER_BLUE,
    C_AQUAMARINE, C_SEA_GREEN, C_AQUA, C_LIGHT_SKY_BLUE
};
static const rgb_t p_forest[] = {
    C_DARK_GREEN, C_DARK_GREEN, C_DARK_OLIVE_GREEN, C_DARK_GREEN,
    C_GREEN, C_FOREST_GREEN, C_OLIVE_DRAB, C_GREEN,
    C_SEA_GREEN, C_MEDIUM_AQUAMARINE, C_LIME_GREEN, C_YELLOW_GREEN,
    C_LIGHT_GREEN, C_LAWN_GREEN, C_MEDIUM_AQUAMARINE, C_FOREST_GREEN
};
static const rgb_t p_party[] = {
    C_RED, C_BLACK, { .r = 0xab, .g = 0x55, .b = 0 }, C_BLACK,
    { .r = 0xab, .g = 0xab, .b = 0 }, C_BLACK, C_GREEN, C_BLACK,
    { .r = 0, .g = 0xab, .b = 0x55 }, C_BLACK, C_BLUE, C_BLACK,
    { .r = 0x55, .g = 0, .b = 0xab }, C_BLACK, { .r = 0xab, .g = 0, .b = 0x55 }, C_BLACK
};

const palette_def_t palettes[PALETTE_MAX] = {
    [PALETTE_FIRE]           = DECL_PALETTE(p_fire),
    [PALETTE_WOOD_FIRE]      = DECL_PALETTE(p_wood_fire),
    [PALETTE_SODIUM_FIRE]    = DECL_PALETTE(p_sodium_fire),
    [PALETTE_COPPER_FIRE]    = DECL_PALETTE(p_copper_fire),
    [PALETTE_ALCOHOL_FIRE]   = DECL_PALETTE(p_alcohol_fire),
    [PALETTE_RUBIDIUM_FIRE]  = DECL_PALETTE(p_rubidium_fire),
    [PALETTE_POTASSIUM_FIRE] = DECL_PALETTE(p_potassium_fire),
    [PALETTE_LITHIUM_FIRE]   = DECL_PALETTE(p_lithium_fire),
    [PALETTE_CLOUD]          = DECL_PALETTE(p_cloud),
    [PALETTE_LAVA]           = DECL_PALETTE(p_lava),
    [PALETTE_HEAT]           = DECL_PALETTE(p_heat),
    [PALETTE_OCEAN]          = DECL_PALETTE(p_ocean),
    [PALETTE_FOREST]         = DECL_PALETTE(p_forest),
    [PALETTE_PARTY]          = DECL_PALETTE(p_party),
};
