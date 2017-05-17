#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include "stdafx.h"

using namespace std;



class TextureLoader
{
public:
	TextureLoader(void);

	int LoadBMP(string location, GLuint &texture);
};

#endif