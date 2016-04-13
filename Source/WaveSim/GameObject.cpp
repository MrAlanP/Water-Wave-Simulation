#include "GameObject.h"

GameObject::GameObject()
{
	m_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_rotation = XMFLOAT3(0, 0, 0);
	//Default mesh alpha to 1.0f
	m_alpha = 1.0f;
}

GameObject::GameObject(const GameObject& other)
{
}

GameObject::~GameObject()
{
}

void GameObject::Initialise(ID3D11Device *device, HWND hwnd)
{
	CreateShader(device, hwnd);
	
}

void GameObject::CreateShader(ID3D11Device *device, HWND hwnd)
{
	m_textureShader = new TextureShader;
	// Initialise the shader
	m_textureShader->Initialise(device, hwnd);
}

void GameObject::Render(ID3D11DeviceContext *context, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, Light *light, XMFLOAT3 cameraPos)
{
	//Render the mesh instance
	RenderMesh(context);
	//Render shaders
	RenderShader(context, viewMatrix, projectionMatrix, light, cameraPos);
}

void GameObject::RenderMesh(ID3D11DeviceContext *context)
{
	if (m_mesh != nullptr)
	{
		m_mesh->Render(context, m_modelMatrix);
	}
}

void GameObject::RenderShader(ID3D11DeviceContext *context, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, Light *light, XMFLOAT3 cameraPos)
{
	m_textureShader->Render(context, m_mesh->GetIndexCount(),
		&GetModelMatrix(), &viewMatrix, &projectionMatrix,
		m_mesh->GetTexture(), light, cameraPos, m_alpha);
}

void GameObject::SetPosition(XMFLOAT3 newPosition)
{
	m_position = newPosition;
	m_rotation = XMFLOAT3(0, 0, 0);

	//Update model matrix
	XMStoreFloat4x4(&m_modelMatrix,
					XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) *
					XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z) *
					XMMatrixTranslation(m_position.x, m_position.y, m_position.z));
}

void GameObject::SetMesh(Mesh* mesh)
{
	m_mesh = mesh;
}

Mesh* GameObject::GetMesh()
{
	return m_mesh;
}

XMMATRIX GameObject::GetModelMatrix()
{
	return XMLoadFloat4x4(&m_modelMatrix);
}

//Release mesh and shader
void GameObject::Shutdown()
{
	if (m_mesh)
	{
		m_mesh->Shutdown();
		delete m_mesh;
		m_mesh = 0;
	}

	if (m_textureShader)
	{
		m_textureShader->Shutdown();
		delete m_textureShader;
		m_textureShader = 0;
	}
}
