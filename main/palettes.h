#ifndef EVLAMP_PALETTES_H_
#define EVLAMP_PALETTES_H_

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
    PALETTE_HEAT,
    PALETTE_OCEAN,
    PALETTE_FOREST,
    PALETTE_PARTY,

    PALETTE_MAX
};

typedef struct
{
    size_t size;
    const rgb_t *palette;
} palette_def_t;

#define C_ALICE_BLUE             { .r = 0xf0, .g = 0xf8, .b = 0xff }
#define C_AMETHYST               { .r = 0x99, .g = 0x66, .b = 0xcc }
#define C_ANTIQUE_WHITE          { .r = 0xfa, .g = 0xeb, .b = 0xd7 }
#define C_AQUA                   { .r = 0x00, .g = 0xff, .b = 0xff }
#define C_AQUAMARINE             { .r = 0x7f, .g = 0xff, .b = 0xd4 }
#define C_AZURE                  { .r = 0xf0, .g = 0xff, .b = 0xff }
#define C_BEIGE                  { .r = 0xf5, .g = 0xf5, .b = 0xdc }
#define C_BISQUE                 { .r = 0xff, .g = 0xe4, .b = 0xc4 }
#define C_BLACK                  { .r = 0x00, .g = 0x00, .b = 0x00 }
#define C_BLANCHED_ALMOND        { .r = 0xff, .g = 0xeb, .b = 0xcd }
#define C_BLUE                   { .r = 0x00, .g = 0x00, .b = 0xff }
#define C_BLUE_VIOLET            { .r = 0x8a, .g = 0x2b, .b = 0xe2 }
#define C_BROWN                  { .r = 0xa5, .g = 0x2a, .b = 0x2a }
#define C_BURLY_WOOD             { .r = 0xde, .g = 0xb8, .b = 0x87 }
#define C_CADET_BLUE             { .r = 0x5f, .g = 0x9e, .b = 0xa0 }
#define C_CHARTREUSE             { .r = 0x7f, .g = 0xff, .b = 0x00 }
#define C_CHOCOLATE              { .r = 0xd2, .g = 0x69, .b = 0x1e }
#define C_CORAL                  { .r = 0xff, .g = 0x7f, .b = 0x50 }
#define C_CORNFLOWER_BLUE        { .r = 0x64, .g = 0x95, .b = 0xed }
#define C_CORNSILK               { .r = 0xff, .g = 0xf8, .b = 0xdc }
#define C_CRIMSON                { .r = 0xdc, .g = 0x14, .b = 0x3c }
#define C_CYAN                   { .r = 0x00, .g = 0xff, .b = 0xff }
#define C_DARK_BLUE              { .r = 0x00, .g = 0x00, .b = 0x8b }
#define C_DARK_CYAN              { .r = 0x00, .g = 0x8b, .b = 0x8b }
#define C_DARK_GOLDENROD         { .r = 0xb8, .g = 0x86, .b = 0x0b }
#define C_DARK_GRAY              { .r = 0xa9, .g = 0xa9, .b = 0xa9 }
#define C_DARK_GREY              { .r = 0xa9, .g = 0xa9, .b = 0xa9 }
#define C_DARK_GREEN             { .r = 0x00, .g = 0x64, .b = 0x00 }
#define C_DARK_KHAKI             { .r = 0xbd, .g = 0xb7, .b = 0x6b }
#define C_DARK_MAGENTA           { .r = 0x8b, .g = 0x00, .b = 0x8b }
#define C_DARK_OLIVE_GREEN       { .r = 0x55, .g = 0x6b, .b = 0x2f }
#define C_DARK_ORANGE            { .r = 0xff, .g = 0x8c, .b = 0x00 }
#define C_DARK_ORCHID            { .r = 0x99, .g = 0x32, .b = 0xcc }
#define C_DARK_RED               { .r = 0x8b, .g = 0x00, .b = 0x00 }
#define C_DARK_SALMON            { .r = 0xe9, .g = 0x96, .b = 0x7a }
#define C_DARK_SEA_GREEN         { .r = 0x8f, .g = 0xbc, .b = 0x8f }
#define C_DARK_SLATE_BLUE        { .r = 0x48, .g = 0x3d, .b = 0x8b }
#define C_DARK_SLATE_GRAY        { .r = 0x2f, .g = 0x4f, .b = 0x4f }
#define C_DARK_SLATE_GREY        { .r = 0x2f, .g = 0x4f, .b = 0x4f }
#define C_DARK_TURQUOISE         { .r = 0x00, .g = 0xce, .b = 0xd1 }
#define C_DARK_VIOLET            { .r = 0x94, .g = 0x00, .b = 0xd3 }
#define C_DEEP_PINK              { .r = 0xff, .g = 0x14, .b = 0x93 }
#define C_DEEP_SKY_BLUE          { .r = 0x00, .g = 0xbf, .b = 0xff }
#define C_DIM_GRAY               { .r = 0x69, .g = 0x69, .b = 0x69 }
#define C_DIM_GREY               { .r = 0x69, .g = 0x69, .b = 0x69 }
#define C_DODGER_BLUE            { .r = 0x1e, .g = 0x90, .b = 0xff }
#define C_FIRE_BRICK             { .r = 0xb2, .g = 0x22, .b = 0x22 }
#define C_FLORAL_WHITE           { .r = 0xff, .g = 0xfa, .b = 0xf0 }
#define C_FOREST_GREEN           { .r = 0x22, .g = 0x8b, .b = 0x22 }
#define C_FUCHSIA                { .r = 0xff, .g = 0x00, .b = 0xff }
#define C_GAINSBORO              { .r = 0xdc, .g = 0xdc, .b = 0xdc }
#define C_GHOST_WHITE            { .r = 0xf8, .g = 0xf8, .b = 0xff }
#define C_GOLD                   { .r = 0xff, .g = 0xd7, .b = 0x00 }
#define C_GOLDENROD              { .r = 0xda, .g = 0xa5, .b = 0x20 }
#define C_GRAY                   { .r = 0x80, .g = 0x80, .b = 0x80 }
#define C_GREY                   { .r = 0x80, .g = 0x80, .b = 0x80 }
#define C_GREEN                  { .r = 0x00, .g = 0x80, .b = 0x00 }
#define C_GREEN_YELLOW           { .r = 0xad, .g = 0xff, .b = 0x2f }
#define C_HONEYDEW               { .r = 0xf0, .g = 0xff, .b = 0xf0 }
#define C_HOT_PINK               { .r = 0xff, .g = 0x69, .b = 0xb4 }
#define C_INDIAN_RED             { .r = 0xcd, .g = 0x5c, .b = 0x5c }
#define C_INDIGO                 { .r = 0x4b, .g = 0x00, .b = 0x82 }
#define C_IVORY                  { .r = 0xff, .g = 0xff, .b = 0xf0 }
#define C_KHAKI                  { .r = 0xf0, .g = 0xe6, .b = 0x8c }
#define C_LAVENDER               { .r = 0xe6, .g = 0xe6, .b = 0xfa }
#define C_LAVENDER_BLUSH         { .r = 0xff, .g = 0xf0, .b = 0xf5 }
#define C_LAWN_GREEN             { .r = 0x7c, .g = 0xfc, .b = 0x00 }
#define C_LEMON_CHIFFON          { .r = 0xff, .g = 0xfa, .b = 0xcd }
#define C_LIGHT_BLUE             { .r = 0xad, .g = 0xd8, .b = 0xe6 }
#define C_LIGHT_CORAL            { .r = 0xf0, .g = 0x80, .b = 0x80 }
#define C_LIGHT_CYAN             { .r = 0xe0, .g = 0xff, .b = 0xff }
#define C_LIGHT_GOLDENROD_YELLOW { .r = 0xfa, .g = 0xfa, .b = 0xd2 }
#define C_LIGHT_GREEN            { .r = 0x90, .g = 0xee, .b = 0x90 }
#define C_LIGHT_GREY             { .r = 0xd3, .g = 0xd3, .b = 0xd3 }
#define C_LIGHT_PINK             { .r = 0xff, .g = 0xb6, .b = 0xc1 }
#define C_LIGHT_SALMON           { .r = 0xff, .g = 0xa0, .b = 0x7a }
#define C_LIGHT_SEA_GREEN        { .r = 0x20, .g = 0xb2, .b = 0xaa }
#define C_LIGHT_SKY_BLUE         { .r = 0x87, .g = 0xce, .b = 0xfa }
#define C_LIGHT_SLATE_GRAY       { .r = 0x77, .g = 0x88, .b = 0x99 }
#define C_LIGHT_SLATE_GREY       { .r = 0x77, .g = 0x88, .b = 0x99 }
#define C_LIGHT_STEEL_BLUE       { .r = 0xb0, .g = 0xc4, .b = 0xde }
#define C_LIGHT_YELLOW           { .r = 0xff, .g = 0xff, .b = 0xe0 }
#define C_LIME                   { .r = 0x00, .g = 0xff, .b = 0x00 }
#define C_LIME_GREEN             { .r = 0x32, .g = 0xcd, .b = 0x32 }
#define C_LINEN                  { .r = 0xfa, .g = 0xf0, .b = 0xe6 }
#define C_MAGENTA                { .r = 0xff, .g = 0x00, .b = 0xff }
#define C_MAROON                 { .r = 0x80, .g = 0x00, .b = 0x00 }
#define C_MEDIUM_AQUAMARINE      { .r = 0x66, .g = 0xcd, .b = 0xaa }
#define C_MEDIUM_BLUE            { .r = 0x00, .g = 0x00, .b = 0xcd }
#define C_MEDIUM_ORCHID          { .r = 0xba, .g = 0x55, .b = 0xd3 }
#define C_MEDIUM_PURPLE          { .r = 0x93, .g = 0x70, .b = 0xdb }
#define C_MEDIUM_SEA_GREEN       { .r = 0x3c, .g = 0xb3, .b = 0x71 }
#define C_MEDIUM_SLATE_BLUE      { .r = 0x7b, .g = 0x68, .b = 0xee }
#define C_MEDIUM_SPRING_GREEN    { .r = 0x00, .g = 0xfa, .b = 0x9a }
#define C_MEDIUM_TURQUOISE       { .r = 0x48, .g = 0xd1, .b = 0xcc }
#define C_MEDIUM_VIOLET_RED      { .r = 0xc7, .g = 0x15, .b = 0x85 }
#define C_MIDNIGHT_BLUE          { .r = 0x19, .g = 0x19, .b = 0x70 }
#define C_MINT_CREAM             { .r = 0xf5, .g = 0xff, .b = 0xfa }
#define C_MISTY_ROSE             { .r = 0xff, .g = 0xe4, .b = 0xe1 }
#define C_MOCCASIN               { .r = 0xff, .g = 0xe4, .b = 0xb5 }
#define C_NAVAJO_WHITE           { .r = 0xff, .g = 0xde, .b = 0xad }
#define C_NAVY                   { .r = 0x00, .g = 0x00, .b = 0x80 }
#define C_OLD_LACE               { .r = 0xfd, .g = 0xf5, .b = 0xe6 }
#define C_OLIVE                  { .r = 0x80, .g = 0x80, .b = 0x00 }
#define C_OLIVE_DRAB             { .r = 0x6b, .g = 0x8e, .b = 0x23 }
#define C_ORANGE                 { .r = 0xff, .g = 0xa5, .b = 0x00 }
#define C_ORANGE_RED             { .r = 0xff, .g = 0x45, .b = 0x00 }
#define C_ORCHID                 { .r = 0xda, .g = 0x70, .b = 0xd6 }
#define C_PALE_GOLDENROD         { .r = 0xee, .g = 0xe8, .b = 0xaa }
#define C_PALE_GREEN             { .r = 0x98, .g = 0xfb, .b = 0x98 }
#define C_PALE_TURQUOISE         { .r = 0xaf, .g = 0xee, .b = 0xee }
#define C_PALE_VIOLET_RED        { .r = 0xdb, .g = 0x70, .b = 0x93 }
#define C_PAPAYA_WHIP            { .r = 0xff, .g = 0xef, .b = 0xd5 }
#define C_PEACH_PUFF             { .r = 0xff, .g = 0xda, .b = 0xb9 }
#define C_PERU                   { .r = 0xcd, .g = 0x85, .b = 0x3f }
#define C_PINK                   { .r = 0xff, .g = 0xc0, .b = 0xcb }
#define C_PLAID                  { .r = 0xcc, .g = 0x55, .b = 0x33 }
#define C_PLUM                   { .r = 0xdd, .g = 0xa0, .b = 0xdd }
#define C_POWDER_BLUE            { .r = 0xb0, .g = 0xe0, .b = 0xe6 }
#define C_PURPLE                 { .r = 0x80, .g = 0x00, .b = 0x80 }
#define C_RED                    { .r = 0xff, .g = 0x00, .b = 0x00 }
#define C_ROSY_BROWN             { .r = 0xbc, .g = 0x8f, .b = 0x8f }
#define C_ROYAL_BLUE             { .r = 0x41, .g = 0x69, .b = 0xe1 }
#define C_SADDLE_BROWN           { .r = 0x8b, .g = 0x45, .b = 0x13 }
#define C_SALMON                 { .r = 0xfa, .g = 0x80, .b = 0x72 }
#define C_SANDY_BROWN            { .r = 0xf4, .g = 0xa4, .b = 0x60 }
#define C_SEA_GREEN              { .r = 0x2e, .g = 0x8b, .b = 0x57 }
#define C_SEASHELL               { .r = 0xff, .g = 0xf5, .b = 0xee }
#define C_SIENNA                 { .r = 0xa0, .g = 0x52, .b = 0x2d }
#define C_SILVER                 { .r = 0xc0, .g = 0xc0, .b = 0xc0 }
#define C_SKY_BLUE               { .r = 0x87, .g = 0xce, .b = 0xeb }
#define C_SLATE_BLUE             { .r = 0x6a, .g = 0x5a, .b = 0xcd }
#define C_SLATE_GRAY             { .r = 0x70, .g = 0x80, .b = 0x90 }
#define C_SLATE_GREY             { .r = 0x70, .g = 0x80, .b = 0x90 }
#define C_SNOW                   { .r = 0xff, .g = 0xfa, .b = 0xfa }
#define C_SPRING_GREEN           { .r = 0x00, .g = 0xff, .b = 0x7f }
#define C_STEEL_BLUE             { .r = 0x46, .g = 0x82, .b = 0xb4 }
#define C_TAN                    { .r = 0xd2, .g = 0xb4, .b = 0x8c }
#define C_TEAL                   { .r = 0x00, .g = 0x80, .b = 0x80 }
#define C_THISTLE                { .r = 0xd8, .g = 0xbf, .b = 0xd8 }
#define C_TOMATO                 { .r = 0xff, .g = 0x63, .b = 0x47 }
#define C_TURQUOISE              { .r = 0x40, .g = 0xe0, .b = 0xd0 }
#define C_VIOLET                 { .r = 0xee, .g = 0x82, .b = 0xee }
#define C_WHEAT                  { .r = 0xf5, .g = 0xde, .b = 0xb3 }
#define C_WHITE                  { .r = 0xff, .g = 0xff, .b = 0xff }
#define C_WHITE_SMOKE            { .r = 0xf5, .g = 0xf5, .b = 0xf5 }
#define C_YELLOW                 { .r = 0xff, .g = 0xff, .b = 0x00 }
#define C_YELLOW_GREEN           { .r = 0x9a, .g = 0xcd, .b = 0x32 }
#define C_FAIRY_LIGHT            { .r = 0xff, .g = 0xe4, .b = 0x2d }
#define C_FAIRY_LIGHT_N_C_C      { .r = 0xff, .g = 0x9d, .b = 0x2a }

extern const palette_def_t palettes[PALETTE_MAX];

#endif /* EVLAMP_PALETTES_H_ */
