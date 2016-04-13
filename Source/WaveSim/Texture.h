//Code originating from Rastertek has been utilised and modified within this class

#pragma once

#include <d3d11.h>
#include <stdio.h>
#include <fstream>
using namespace std;

class Texture
{

private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	Texture();
	~Texture();

	void Initialise(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
	

private:
	void LoadTarga(char*, int&, int&);

	unsigned char				*m_targaData;
	ID3D11Texture2D				*m_texture;
	ID3D11ShaderResourceView	*m_textureView;
};