#pragma once

#include <iostream>
#include <GL/gl.h>

std::string loadTextFile(std::string name);
void gl_print_str(std::string name, GLenum _enum);
void gl_info();
void gl_print_extentions();
void gl_check_error();
const char* gl_get_error_string(GLenum err);
