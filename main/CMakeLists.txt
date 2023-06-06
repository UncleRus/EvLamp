set(EMBED ../files)

idf_component_register(
	INCLUDE_DIRS .
	SRCS
		common.c
		main.c
		settings.c
		wifi.c
		bus.c
		effect.c
		surface.c
		input.c
		webserver.c
		api.c
		palettes.c
		
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
		effects/twister.c
		effects/flower.c
		effects/polar_lights.c
		effects/gif.c
		effects/relax.c
		effects/wheel.c
		effects/peacock.c
		effects/colors.c
		effects/diagonals.c
		effects/whirls.c
		effects/sun.c
		effects/spaceships.c
		effects/fireflies.c
		effects/tunnel.c
		effects/dunes.c
		effects/octopus.c

	EMBED_TXTFILES
		${EMBED}/web/jquery.js
		${EMBED}/web/styles.css
		${EMBED}/web/index.html
		
	EMBED_FILES
		${EMBED}/gifs/heart.gif
		${EMBED}/gifs/heart2.gif
		${EMBED}/gifs/boldman.gif
		${EMBED}/gifs/coin.gif
		${EMBED}/gifs/disk.gif
		${EMBED}/gifs/girl.gif
		${EMBED}/gifs/hairyman.gif
		${EMBED}/gifs/hero.gif
		${EMBED}/gifs/disk.gif
		${EMBED}/gifs/torch.gif
		${EMBED}/gifs/zombie.gif
		${EMBED}/gifs/cutebear.gif
)

#spiffs_create_partition_image(storage ../spiffs_image FLASH_IN_PROJECT)
