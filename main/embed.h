#ifndef EVLAMP_EMBED_H_
#define EVLAMP_EMBED_H_

#define DECLARE_EMBED(name) extern const char embed_##name[] asm("_binary_"#name"_start")

DECLARE_EMBED(jquery_js);
DECLARE_EMBED(styles_css);
DECLARE_EMBED(index_html);

#endif /* EVLAMP_EMBED_H_ */
