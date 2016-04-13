//Code originating from Rastertek has been utilised and modified for rendering within this class

#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include "Console.h"
#include "Texture.h"
using namespace DirectX;

#include <fstream>
using namespace std;


class Mesh
{
public:
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

private:
	struct Model
	{
		float x, y, z; //Vertex Position
		float tu, tv; //UVs
		float nx, ny, nz; //Normals
	};


public:
	Mesh();
	Mesh(const Mesh&);
	~Mesh();

	void Initialise(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*, XMFLOAT4X4);
	void SetVertexBufferDynamic();

	int GetIndexCount();
	Vertex* GetVertices();
	int GetVertexCount();

	ID3D11ShaderResourceView* GetTexture();

protected:
	Vertex			*m_vertices;
	unsigned long	*m_indices;
	int				m_vertexCount, m_indexCount;
	bool			m_isDynamicVertexBuffer;
	Texture			*m_texture;

private:
	void InitialiseBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	virtual void LoadModel() {};
	void LoadModel(char*);
	void ReleaseModel();
	void LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	
	ID3D11Buffer	*m_vertexBuffer, *m_indexBuffer;
	Model			*m_model;
	
};