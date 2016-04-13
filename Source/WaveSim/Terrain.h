#pragma once

#include "GameObject.h"

class Terrain : public GameObject
{
public:
	Terrain();
	~Terrain();

	virtual void Initialise(ID3D11Device*, HWND);
};