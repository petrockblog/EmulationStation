#pragma once

#include <boost/locale.hpp>

#define _(STRING) boost::locale::gettext(STRING)
#define __(SINGULAR, PLURAL, COUNT) boost::locale::format(boost::locale::ngettext(SINGULAR, PLURAL, COUNT)) % COUNT