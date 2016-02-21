/**
 * See Copyright Notice in picrin.h
 */

#include "picrin.h"
#include "picrin/extra.h"
#include "picrin/private/object.h"
#include "picrin/private/state.h"

pic_value
pic_get_backtrace(pic_state *pic)
{
  size_t ai = pic_enter(pic);
  struct callinfo *ci;
  pic_value trace;

  trace = pic_lit_value(pic, "");

  for (ci = pic->ci; ci != pic->cibase; --ci) {
    pic_value proc = ci->fp[0];

    trace = pic_str_cat(pic, trace, pic_lit_value(pic, "  at "));
    trace = pic_str_cat(pic, trace, pic_lit_value(pic, "(anonymous lambda)"));

    if (pic_proc_func_p(pic_proc_ptr(pic, proc))) {
      trace = pic_str_cat(pic, trace, pic_lit_value(pic, " (native function)\n"));
    } else {
      trace = pic_str_cat(pic, trace, pic_lit_value(pic, " (unknown location)\n")); /* TODO */
    }
  }

  pic_leave(pic, ai);
  pic_protect(pic, trace);

  return trace;
}

void
pic_print_backtrace(pic_state *pic, xFILE *file)
{
  pic_value err = pic_err(pic);

  assert(! pic_invalid_p(pic, err));

  if (! pic_error_p(pic, err)) {
    xfprintf(pic, file, "raise: ");
    pic_fwrite(pic, err, file);
  } else {
    struct error *e;
    pic_value elem, it;

    e = pic_error_ptr(pic, err);
    if (! pic_eq_p(pic, pic_obj_value(e->type), pic_intern_lit(pic, ""))) {
      pic_fwrite(pic, pic_obj_value(e->type), file);
      xfprintf(pic, file, " ");
    }
    xfprintf(pic, file, "error: ");
    pic_fwrite(pic, pic_obj_value(e->msg), file);

    pic_for_each (elem, e->irrs, it) { /* print error irritants */
      xfprintf(pic, file, " ");
      pic_fwrite(pic, elem, file);
    }
    xfprintf(pic, file, "\n");

    xfputs(pic, pic_str(pic, pic_obj_value(e->stack)), file);
  }
}
