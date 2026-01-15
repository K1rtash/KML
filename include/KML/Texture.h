#ifndef KML_TEXTURE_H
#define KML_TEXTURE_H

#include <string>

bool load(const char* file);
unsigned int getID(std::string name);
void bind(unsigned int id);

#endif