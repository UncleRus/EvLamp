/*
 Author: José Bollo <jobol@nonadev.net>

 https://gitlab.com/jobol/mustach

 SPDX-License-Identifier: ISC
*/

#ifndef _mustach_wrap_h_included_
#define _mustach_wrap_h_included_

/*
 * mustach-wrap is intended to make integration of JSON
 * libraries easier by wrapping mustach extensions in a
 * single place.
 *
 * As before, using mustach and only mustach is possible
 * (by using only mustach.h) but does not implement high
 * level features coming with extensions implemented by
 * this high level wrapper.
 */
#include "mustach.h"
/*
 * Definition of the writing callbacks for mustach functions
 * producing output to callbacks.
 *
 * Two callback types are defined:
 *
 * @mustach_write_cb_t:
 *
 *    callback receiving the escaped data to be written as 3 parameters:
 *
 *    1. the 'closure', the same given to the wmustach_... function
 *    2. a pointer to a 'buffer' containing the characters to be written
 *    3. the size in bytes of the data pointed by 'buffer'
 *
 * @mustach_emit_cb_t:
 *
 *    callback receiving the data to be written and a flag indicating
 *    if escaping should be done or not as 4 parameters:
 *
 *    1. the 'closure', the same given to the emustach_... function
 *    2. a pointer to a 'buffer' containing the characters to be written
 *    3. the size in bytes of the data pointed by 'buffer'
 *    4. a boolean indicating if 'escape' should be done
 */
#ifndef _mustach_output_callbacks_defined_
#define _mustach_output_callbacks_defined_
typedef int mustach_write_cb_t(void *closure, const char *buffer, size_t size);
typedef int mustach_emit_cb_t(void *closure, const char *buffer, size_t size, int escape);
#endif

/**
 * Flags specific to mustach wrap
 */
#define Mustach_With_SingleDot    4
#define Mustach_With_Equal        8
#define Mustach_With_Compare      16
#define Mustach_With_JsonPointer  32
#define Mustach_With_ObjectIter   64
#define Mustach_With_IncPartial   128
#define Mustach_With_EscFirstCmp  256

/**
 * mustach_wrap_itf - high level wrap of mustach - interface for callbacks
 *
 * The functions sel, subsel, enter and next should return 0 or 1.
 *
 * All other functions should normally return MUSTACH_OK (zero).
 *
 * If any function returns a negative value, it means an error that
 * stop the processing and that is reported to the caller. Mustach
 * also has its own error codes. Using the macros MUSTACH_ERROR_USER
 * and MUSTACH_IS_ERROR_USER could help to avoid clashes.
 *
 * @start: If defined (can be NULL), starts the mustach processing
 *         of the closure, called at the very beginning before any
 *         mustach processing occurs.
 *
 * @stop: If defined (can be NULL), stops the mustach processing
 *        of the closure, called at the very end after all mustach
 *        processing occurered. The status returned by the processing
 *        is passed to the stop.
 *
 * @compare: If defined (can be NULL), compares the value of the
 *           currently selected item with the given value and returns
 *           a negative value if current value is lesser, a positive
 *           value if the current value is greater or zero when
 *           values are equals.
 *           If 'compare' is NULL, any comparison in mustach
 *           is going to fails.
 *
 * @sel: Selects the item of the given 'name'. If 'name' is NULL
 *       Selects the current item. Returns 1 if the selection is
 *       effective or else 0 if the selection failed.
 *
 * @subsel: Selects from the currently selected object the value of
 *          the field of given name. Returns 1 if the selection is
 *          effective or else 0 if the selection failed.
 *
 * @enter: Enters the section of 'name' if possible.
 *         Musts return 1 if entered or 0 if not entered.
 *         When 1 is returned, the function 'leave' will always be called.
 *         Conversely 'leave' is never called when enter returns 0 or
 *         a negative value.
 *         When 1 is returned, the function must activate the first
 *         item of the section.
 *
 * @next: Activates the next item of the section if it exists.
 *        Musts return 1 when the next item is activated.
 *        Musts return 0 when there is no item to activate.
 *
 * @leave: Leaves the last entered section
 *
 * @get: If defined (can be NULL), returns in 'sbuf' the value of 'name'.
 *       As an extension (see NO_ALLOW_EMPTY_TAG), the 'name' can be
 *       the empty string. In that later case an implementation can
 *       return MUSTACH_ERROR_EMPTY_TAG to refuse empty names.
 *       If 'get' is NULL and 'put' NULL the error MUSTACH_ERROR_INVALID_ITF
 *       is returned.
 *
 */
struct mustach_wrap_itf {
	int (*start)(void *closure);
	void (*stop)(void *closure, int status);
	int (*compare)(void *closure, const char *value);
	int (*sel)(void *closure, const char *name);
	int (*subsel)(void *closure, const char *name);
	int (*enter)(void *closure, int objiter);
	int (*next)(void *closure);
	int (*leave)(void *closure);
	int (*get)(void *closure, struct mustach_sbuf *sbuf, int key);
};

/**
 * Mustach interface used internally by mustach wrapper functions.
 * Can be used for overriding behaviour.
 */
extern const struct mustach_itf mustach_wrap_itf;

/**
 * mustach_wrap_file - Renders the mustache 'template' in 'file' for an abstract
 * wrapper of interface 'itf' and 'closure'.
 *
 * @template: the template string to instanciate
 * @length:   length of the template or zero if unknown and template null terminated
 * @itf:      the interface of the abstract wrapper
 * @closure:  the closure of the abstract wrapper
 * @file:     the file where to write the result
 *
 * Returns 0 in case of success, -1 with errno set in case of system error
 * a other negative value in case of error.
 */
extern int mustach_wrap_file(const char *template, size_t length, const struct mustach_wrap_itf *itf, void *closure, int flags, FILE *file);

/**
 * mustach_wrap_fd - Renders the mustache 'template' in 'fd' for an abstract
 * wrapper of interface 'itf' and 'closure'.
 *
 * @template: the template string to instanciate
 * @length:   length of the template or zero if unknown and template null terminated
 * @itf:      the interface of the abstract wrapper
 * @closure:  the closure of the abstract wrapper
 * @fd:       the file descriptor number where to write the result
 *
 * Returns 0 in case of success, -1 with errno set in case of system error
 * a other negative value in case of error.
 */
extern int mustach_wrap_fd(const char *template, size_t length, const struct mustach_wrap_itf *itf, void *closure, int flags, int fd);

/**
 * mustach_wrap_mem - Renders the mustache 'template' in 'result' for an abstract
 * wrapper of interface 'itf' and 'closure'.
 *
 * @template: the template string to instanciate
 * @length:   length of the template or zero if unknown and template null terminated
 * @itf:      the interface of the abstract wrapper
 * @closure:  the closure of the abstract wrapper
 * @result:   the pointer receiving the result when 0 is returned
 * @size:     the size of the returned result
 *
 * Returns 0 in case of success, -1 with errno set in case of system error
 * a other negative value in case of error.
 */
extern int mustach_wrap_mem(const char *template, size_t length, const struct mustach_wrap_itf *itf, void *closure, int flags, char **result, size_t *size);

/**
 * mustach_wrap_write - Renders the mustache 'template' for an abstract
 * wrapper of interface 'itf' and 'closure' to custom writer
 * 'writecb' with 'writeclosure'.
 *
 * @template: the template string to instanciate
 * @length:   length of the template or zero if unknown and template null terminated
 * @itf:      the interface of the abstract wrapper
 * @closure:  the closure of the abstract wrapper
 * @writecb:  the function that write values
 * @closure:  the closure for the write function
 *
 * Returns 0 in case of success, -1 with errno set in case of system error
 * a other negative value in case of error.
 */
extern int mustach_wrap_write(const char *template, size_t length, const struct mustach_wrap_itf *itf, void *closure, int flags, mustach_write_cb_t *writecb, void *writeclosure);

/**
 * mustach_wrap_emit - Renders the mustache 'template' for an abstract
 * wrapper of interface 'itf' and 'closure' to custom emiter 'emitcb'
 * with 'emitclosure'.
 *
 * @template: the template string to instanciate
 * @length:   length of the template or zero if unknown and template null terminated
 * @itf:      the interface of the abstract wrapper
 * @closure:  the closure of the abstract wrapper
 * @emitcb:   the function that emit values
 * @closure:  the closure for the write function
 *
 * Returns 0 in case of success, -1 with errno set in case of system error
 * a other negative value in case of error.
 */
extern int mustach_wrap_emit(const char *template, size_t length, const struct mustach_wrap_itf *itf, void *closure, int flags, mustach_emit_cb_t *emitcb, void *emitclosure);

#endif

