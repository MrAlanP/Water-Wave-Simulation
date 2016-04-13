//Code originating from Rastertek has been utilised and modified within this class

#pragma once

#include <directxmath.h>
#include "Input.h"
#include <d3d11.h>
using namespace DirectX;

__declspec(align(16)) class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Update(float dt, Input*);
	void UpdatePosition(float,float,float);
	void UpdateRotation(float, float, float);
	void Render();
	void GetViewMatrix(XMMATRIX&);

	void RenderReflection(float);
	void GetReflectionViewMatrix(XMMATRIX&);
	

private:
	float		m_positionX, m_positionY, m_positionZ;
	float		m_rotationX, m_rotationY, m_rotationZ;
	XMMATRIX	m_viewMatrix;
	XMMATRIX	m_rotationMatrix;
	XMMATRIX	m_reflectionViewMatrix;
	float		m_moveSpeed;
	float		m_scrollZoomSpeed;

	const XMFLOAT3 m_UP = XMFLOAT3(0, 1, 0);


public:
	//Aligns the data on the heap is aligned with 16byte alignment
	void* operator new(size_t i)
	{
		return _mm_malloc(i,16);
	}

		void operator delete(void* p)
	{
		_mm_free(p);
	}
};

