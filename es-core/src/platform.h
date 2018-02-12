#pragma once
#ifndef ES_CORE_PLATFORM_H
#define ES_CORE_PLATFORM_H

#include <string>

//the Makefile defines one of these:
//#define USE_OPENGL_ES
//#define USE_OPENGL_DESKTOP

#if defined(USE_OPENGL_ES)
#include <GLES/gl.h>
#elif defined(USE_OPENGL_DESKTOP)
#include <SDL_opengl.h>
#endif

//why the hell this naming inconsistency exists is well beyond me
#if defined(WIN32)
#define sleep Sleep
#endif // WIN32

int runShutdownCommand(); // shut down the system (returns 0 if successful)
int runRestartCommand(); // restart the system (returns 0 if successful)
int runSystemCommand(const std::string& cmd_utf8); // run a utf-8 encoded in the shell (requires wstring conversion on Windows)
int quitES(const std::string& filename);
void touch(const std::string& filename);

#endif // ES_CORE_PLATFORM_H
