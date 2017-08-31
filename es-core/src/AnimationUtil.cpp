#include "AnimationUtil.h"
#include "resources/ResourceManager.h"
#include "platform.h"
#include "Log.h"

unsigned int getColorTween(unsigned int currentColor, unsigned int endColor, int frame, int endFrame, bool reverse)
{
	// Reversed frame
	if (reverse) {
		frame = endFrame - frame;
		if (frame < 0) frame = 0;
	}

	// Get bytes of source and targer colors
	unsigned char r = getRed(currentColor);
	unsigned char g = getGreen(currentColor);
	unsigned char b = getBlue(currentColor);
	unsigned char a = getAlpha(currentColor);
	unsigned char er = getRed(endColor);
	unsigned char eg = getGreen(endColor);
	unsigned char eb = getBlue(endColor);
	unsigned char ea = getAlpha(endColor);

	// get Color differences and divide to get amount per frame
	int tar = (int)(getColorDifference(er, r) / endFrame);
	int tag = (int)(getColorDifference(eg, g) / endFrame);
	int tab = (int)(getColorDifference(eb, b) / endFrame);
	int tae = (int)(getColorDifference(ea, a) / endFrame);

	// Get per frame iteration
	r = (unsigned char)(r + (tar * frame));
	g = (unsigned char)(g + (tag * frame));
	b = (unsigned char)(b + (tab * frame));
	a = (unsigned char)(a + (tae * frame));

	// shift back to color format and return (return target if at final frame)
	if (frame == endFrame) return endColor;
	return createColor(r, g, b, a);
}

unsigned char getRed(unsigned int color) {
	unsigned char red;
	red = color >> 24;
	return red;
}

unsigned char getGreen(unsigned int color) {
	unsigned char green;
	green = color >> 16;
	return green;
}

unsigned char getBlue(unsigned int color) {
	unsigned char blue;
	blue = color >> 8;
	return blue;
}

unsigned char getAlpha(unsigned int color) {
	unsigned char alpha;
	alpha = color >> 0;
	return alpha;
}

unsigned int addAlpha(unsigned int color, unsigned char alpha) {
	//  Shift all bytes down
	color << 8;

	color = getRed(color) << 24 | getGreen(color) << 16 | getBlue(color) << 8 | alpha;
	return color;
}

unsigned int createColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	unsigned int c;
	c = r << 24 | g << 16 | b << 8 | a;
	return c;
}

int getColorDifference(unsigned char color, unsigned char compare) {
	int diff = (color - compare);
	//if (diff < 0) diff *= -1;
	return diff;
}