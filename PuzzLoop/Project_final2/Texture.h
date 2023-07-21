#pragma once

#include <iostream>
#include <string>
#include <FreeImage.h>
#include <GL/glut.h>
using namespace std;

class Texture
{
public:
	Texture();

	FIBITMAP* createBitMap(string filename);
	void generateTexture();
	void initializeTexture(string filename);

	GLuint getTextureID() const;

private:
	GLuint textureID;
	GLubyte* textureData;
	int imageWidth, imageHeight;
};

