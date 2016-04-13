#pragma once

#include "Mesh.h"

class PlaneMesh : public Mesh
{

public:
	PlaneMesh();
	~PlaneMesh();

	virtual void LoadModel(int,int,float);
		
private:
	int m_width;
	int m_height;
};