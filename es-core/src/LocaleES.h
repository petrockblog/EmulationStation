#ifndef _LOCALE_H_
#define _LOCALE_H_

#ifdef HAVE_GETTEXT
  #include <libintl.h>
  #define _(A) gettext(A)
#else
  #define _(A) A
  char* ngettext(char* msgid, char* msgid_plural, unsigned long int n);
#endif

#endif
