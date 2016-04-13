//Code originating from Rastertek has been utilised and modified for rendering within this class

#include "Mesh.h"

Mesh::Mesh()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_texture = 0;
	m_isDynamicVertexBuffer = false;
	m_model = 0;
}

Mesh::Mesh(const Mesh& other)
{
}

Mesh::~Mesh()
{
}

void Mesh::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFilename, char* textureFilename)
{
	//Load model
	if (modelFilename != NULL)
	{
		LoadModel(modelFilename);
	}

	//Initialise buffers
	InitialiseBuffers(device);

	//Load texture
	LoadTexture(device, deviceContext, textureFilename);
}

void Mesh::SetVertexBufferDynamic()
{
	m_isDynamicVertexBuffer = true;
}


void Mesh::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
	ReleaseModel();
	return;
}

void Mesh::ShutdownBuffers()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
	return;
}

void Mesh::Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix)
{	

	RenderBuffers(deviceContext);
	return;
}

int Mesh::GetIndexCount()
{
	return m_indexCount;
}

void Mesh::InitialiseBuffers(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result = 0;


	//Setup and Create both buffers

	//Setup the vertex buffer description
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = m_vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Now create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		Console::Log("ERROR: Failed to create vertex buffer.");
	}

	//Setup the index buffer description
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//Give the subresource structure a pointer to the index data
	indexData.pSysMem = m_indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create the index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		Console::Log("ERROR: Failed to create vertex buffer.");
	}

	//Release array
	delete[] m_indices;
	m_indices = 0;
}

void Mesh::LoadModel(char* filename)
{
	ifstream fin;
	char input;

	//Open the model file
	string name = filename;
	string path = "../WaveSim/Data/Models/" + name;
	fin.open(path);

	if (fin.fail())
	{
		Console::Log("Could not open model file");
	}

	//Read up to the value of vertex count
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	//Read in the vertex count
	fin >> m_vertexCount;

	m_indexCount = m_vertexCount;

	//Create the model instance
	m_model = new Model[m_vertexCount];

	//Read up to the beginning of the data
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	//Read in the vertex data
	for (int i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}


	//Create the vertex and index arrays
	m_vertices = new Vertex[m_vertexCount];
	m_indices = new unsigned long[m_indexCount];

	//Close the file
	fin.close();

	//Load the vertices and indices with the model data
	for (int i = 0; i < m_vertexCount; i++)
	{
		m_vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		m_vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		m_vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		m_indices[i] = i;
	}

}


void Mesh::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride, offset;

	//Set stride and offset
	stride = sizeof(Vertex);
	offset = 0;


	//Dynamic vertex buffer
	if (m_isDynamicVertexBuffer) {
		D3D11_MAPPED_SUBRESOURCE resource;
		deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, &m_vertices[0], sizeof(Vertex)* m_vertexCount);
		deviceContext->Unmap(m_vertexBuffer, 0);
	}
	
	//Set the vertex and index buffer to active so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set the type of primitive to be rendered
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return;
}

Mesh::Vertex* Mesh::GetVertices()
{
	return m_vertices;
}

int Mesh::GetVertexCount()
{
	return m_vertexCount;
}

ID3D11ShaderResourceView* Mesh::GetTexture()
{
	return m_texture->GetTexture();
}

void Mesh::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	m_texture = new Texture;
	m_texture->Initialise(device, deviceContext, filename);
	return;
}

void Mesh::ReleaseTexture()
{
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = 0;
	}
	return;
}

void Mesh::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}
	return;
}