/**
 * See Copyright Notice in picrin.h
 */

/** enable libc? */
/* #define PIC_USE_LIBC 1 */

/** enable stdio */
/* #define PIC_USE_STDIO 1 */

/** enable specific features */
/* #define PIC_USE_WRITE 1 */

/** essential external functions */
/* #define PIC_JMPBUF jmp_buf */
/* #define PIC_SETJMP(pic, buf) setjmp(buf) */
/* #define PIC_LONGJMP(pic, buf, val) longjmp((buf), (val)) */
/* #define PIC_ABORT(pic) abort() */