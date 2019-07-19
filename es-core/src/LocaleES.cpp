#include "LocaleES.h"

#ifndef HAVE_GETTEXT
char* ngettext(char* msgid, char* msgid_plural, unsigned long int n) {
  if(n != 1) {
    return msgid_plural;
  }
  return msgid;
}
#endif
