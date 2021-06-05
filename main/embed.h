#ifndef MAIN_EMBED_H_
#define MAIN_EMBED_H_

#define DECLARE_EMBED(name) extern const char embed_##name[] asm("_binary_"#name"_start")

DECLARE_EMBED(jquery_js);
DECLARE_EMBED(styles_css);
DECLARE_EMBED(index_html);

DECLARE_EMBED(ota_server_cert_pem);

#endif /* MAIN_EMBED_H_ */
