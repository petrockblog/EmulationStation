#pragma once

#include <boost/locale.hpp>

#define _(STRING) boost::locale::gettext(STRING)