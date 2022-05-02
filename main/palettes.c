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
};
