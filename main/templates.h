#ifndef MAIN_TEMPLATES_H_
#define MAIN_TEMPLATES_H_

#define DECLARE_TPL(name) extern const char template_##name[] asm("_binary_"#name"_tpl_start")

DECLARE_TPL(header);
DECLARE_TPL(footer);
DECLARE_TPL(index);
DECLARE_TPL(wifi_form);
DECLARE_TPL(wifi_done);
DECLARE_TPL(leds_form);
DECLARE_TPL(leds_done);

#endif /* MAIN_TEMPLATES_H_ */
