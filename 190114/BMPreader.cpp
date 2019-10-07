// #include <stdio.h>
#include <iostream>
using namespace std;
#include <GL/glut.h>

#define _CRT_SECURE_NO_WARNINGS


unsigned int LoadTextureBMPWH(const char  *filename, int width, int height)
{
	GLuint texture;
	unsigned char * data, *stored;
	FILE  *file;
	int a = fopen_s(&file, filename, "rb");

	if (file == NULL) {
		cout << filename << " cant found." << endl;
		return 0;
	}
	cout << filename << " success." << endl;
	data = (unsigned char*)malloc(width  *height * 3);
	stored = (unsigned char*)malloc(width * height * 4);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	for (int i = 0; i < width *height; ++i)
	{
		int index = i * 3;
		int indexS = i * 4;
		unsigned char B, R, G;
		B = data[index];
		G = data[index + 1];
		R = data[index + 2];

		stored[indexS] = R;
		stored[indexS + 1] = G;
		stored[indexS + 2] = B;
		if (R == (unsigned int)(0) && G == (unsigned int)(0) && B == (unsigned int)(255))
		{
			stored[indexS + 3] = (unsigned int)(0);
		}
		else
		{
			stored[indexS + 3] = (unsigned int)(255);
		}
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, stored);
	free(data);
	free(stored);
	return texture;
}