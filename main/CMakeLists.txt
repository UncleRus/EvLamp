idf_component_register(
    INCLUDE_DIRS .
    SRCS
        main.c
        settings.c
        wifi.c
        bus.c
        effect.c
        surface.c
        input.c
        
        effects/crazybees.c
        effects/dna.c
        effects/fire.c
        effects/lamp.c
        effects/lava.c
        effects/matrix.c
        effects/plasma_waves.c
        effects/rain.c
        effects/rainbow.c
        effects/rays.c
        effects/sparkles.c
        effects/waterfall.c
        effects/metaballs.c
        effects/water.c
        effects/holiday.c
        effects/hoop.c
)

#spiffs_create_partition_image(storage ../spiffs_image FLASH_IN_PROJECT)
