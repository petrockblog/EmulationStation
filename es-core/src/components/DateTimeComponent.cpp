#include "components/DateTimeComponent.h"

#include "utils/StringUtil.h"
#include "Log.h"
#include "Renderer.h"
#include "Log.h"
#include "Locale.h"
#include "Settings.h"
#include <boost/date_time.hpp>

DateTimeComponent::DateTimeComponent(Window* window) : TextComponent(window), mDisplayRelative(false)
{
	setFormat("");
}

DateTimeComponent::DateTimeComponent(Window* window, const std::string& text, const std::shared_ptr<Font>& font, unsigned int color, Alignment align,
	Vector3f pos, Vector2f size, unsigned int bgcolor) : TextComponent(window, text, font, color, align, pos, size, bgcolor), mDisplayRelative(false)
{
	setFormat("");
}

void DateTimeComponent::setValue(const std::string& val)
{
	mTime = val;
	onTextChanged();
}

std::string DateTimeComponent::getValue() const
{
	return mTime;
}

void DateTimeComponent::setFormat(const std::string& format)
{
	if (format.empty())
	{
		// FIXME i18n only handling dd/mm/yyyy and mm/dd/yyyy
		// There has to be a better way to find the date order...
		std::stringstream tmp;
		tmp << boost::locale::as::date << 1542844800; // 2018/11/22
		if (tmp.str()[0] == '1')
			mFormat = "%m/%d/%Y";
		else
			mFormat = "%d/%m/%Y";
	}
	else
		mFormat = format;
	onTextChanged();
}

void DateTimeComponent::setDisplayRelative(bool displayRelative)
{
	mDisplayRelative = displayRelative;
	onTextChanged();
}

void DateTimeComponent::onTextChanged()
{
	mText = getDisplayString();

	TextComponent::onTextChanged();
}

std::string DateTimeComponent::getDisplayString() const
{
	if (mDisplayRelative) {
		//relative time
		if(mTime.getTime() == 0)
			return _("never");

		Utils::Time::DateTime now(Utils::Time::now());
		Utils::Time::Duration dur(now.getTime() - mTime.getTime());

		char buf[64];
		std::stringstream formatted;
		if(dur.getDays() > 0)
			formatted << boost::locale::format(ngettext("{1} day ago", "{1} days ago", dur.getDays())) % dur.getDays();
		else if(dur.getHours() > 0)
			formatted << boost::locale::format(ngettext("{1} hour ago", "{1} hours ago", dur.getHours())) % dur.getHours();
		else if(dur.getMinutes() > 0)
			formatted << boost::locale::format(ngettext("{1} minute ago", "{1} minutes ago", dur.getMinutes())) % dur.getMinutes();
		else
			formatted << boost::locale::format(ngettext("{1} second ago", "{1} seconds ago", dur.getSeconds())) % dur.getSeconds();

		return std::string(buf);
	}

	if(mTime.getTime() == 0)
		return _("unknown");

	return Utils::Time::timeToString(mTime.getTime(), mFormat);
}

void DateTimeComponent::render(const Transform4x4f& parentTrans)
{
	TextComponent::render(parentTrans);
}


void DateTimeComponent::applyTheme(const std::shared_ptr<ThemeData>& theme, const std::string& view, const std::string& element, unsigned int properties)
{
	GuiComponent::applyTheme(theme, view, element, properties);

	using namespace ThemeFlags;

	const ThemeData::ThemeElement* elem = theme->getElement(view, element, "datetime");
	if(!elem)
		return;

	if(elem->has("displayRelative"))
		setDisplayRelative(elem->get<bool>("displayRelative"));

	if(elem->has("format"))
		setFormat(elem->get<std::string>("format"));

	if (properties & COLOR && elem->has("color"))
		setColor(elem->get<unsigned int>("color"));

	setRenderBackground(false);
	if (properties & COLOR && elem->has("backgroundColor")) {
		setBackgroundColor(elem->get<unsigned int>("backgroundColor"));
		setRenderBackground(true);
	}

	if(properties & ALIGNMENT && elem->has("alignment"))
	{
		std::string str = elem->get<std::string>("alignment");
		if(str == "left")
			setHorizontalAlignment(ALIGN_LEFT);
		else if(str == "center")
			setHorizontalAlignment(ALIGN_CENTER);
		else if(str == "right")
			setHorizontalAlignment(ALIGN_RIGHT);
		else
		LOG(LogError) << "Unknown text alignment string: " << str;
	}

	if(properties & FORCE_UPPERCASE && elem->has("forceUppercase"))
		setUppercase(elem->get<bool>("forceUppercase"));

	if(properties & LINE_SPACING && elem->has("lineSpacing"))
		setLineSpacing(elem->get<float>("lineSpacing"));

	setFont(Font::getFromTheme(elem, properties, mFont));
}
