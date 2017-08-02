#pragma once

#include <string>
#include <Eigen/Dense>

unsigned int getColorTween(unsigned int color, unsigned int endColor, int frame, int endFrame, bool reverse = false);

unsigned char getRed(unsigned int color);
unsigned char getGreen(unsigned int color);
unsigned char getBlue(unsigned int color);
unsigned char getAlpha(unsigned int color);

unsigned int addAlpha(unsigned int color, unsigned char alpha = 0xFF);

unsigned int createColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
int getColorDifference(unsigned char color, unsigned char compare);