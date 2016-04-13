//Code originating from Rastertek has been utilised and modified within this class

#include "Camera.h"


Camera::Camera()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_moveSpeed = 10.0f;
	m_scrollZoomSpeed = 2.0f;

	SetRotation(0, 0, 0);
}


Camera::Camera(const Camera& other)
{
}


Camera::~Camera()
{
}


void Camera::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void Camera::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	float yaw, pitch, roll;
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	m_rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	return;
}


XMFLOAT3 Camera::GetPosition()
{
	return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}


XMFLOAT3 Camera::GetRotation()
{
	return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}


void Camera::Update(float dt, Input* input)
{
	//Keyboard camera movement
	if (input->IsKeyDown(DIK_W))
	{
		UpdatePosition(0, 0, m_moveSpeed*dt);
	}

	if (input->IsKeyDown(DIK_S))
	{
		UpdatePosition(0, 0, -m_moveSpeed*dt);
	}

	if (input->IsKeyDown(DIK_A))
	{
		UpdatePosition(-m_moveSpeed*dt, 0, 0);
	}

	if (input->IsKeyDown(DIK_D))
	{
		UpdatePosition(m_moveSpeed*dt, 0, 0);
	}


	if (input->IsKeyDown(DIK_SPACE))
	{
		UpdatePosition(0, m_moveSpeed*dt, 0);
	}

	//Faster move speed with shift
	if (input->IsKeyDown(DIK_LSHIFT))
	{
		m_moveSpeed = 30;
	}
	else
	{
		m_moveSpeed = 10;
	}

	//Right mouse button
	if (input->IsMouseDown(1))
	{
		//Update the rotation based on mouse
		XMFLOAT2 mouse = input->GetMouseMovement();

		UpdateRotation(mouse.y*dt, mouse.x*dt, 0);
	}


	//Mouse Scroll
	float scroll = input->GetMouseScroll();
	if (scroll != 0)
	{
		UpdatePosition(0, 0, scroll*m_scrollZoomSpeed*dt);
	}

}

void Camera::UpdatePosition(float xMove, float yMove, float zMove)
{
	XMFLOAT3 move = XMFLOAT3(xMove, yMove, zMove);
	XMVECTOR moveVector;
	// Load it into a XMVECTOR structure.
	moveVector = XMLoadFloat3(&move);
	moveVector = XMVector3TransformCoord(moveVector, m_rotationMatrix);
	XMStoreFloat3(&move, moveVector);
	SetPosition(m_positionX + move.x, m_positionY + move.y, m_positionZ + move.z);
}

void Camera::UpdateRotation(float xTurn, float yTurn, float zTurn)
{
	SetRotation(m_rotationX + xTurn, m_rotationY + yTurn, m_rotationZ + zTurn);
}

void Camera::Render()
{
	XMFLOAT3 position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;

	upVector = XMLoadFloat3(&m_UP);

	//Setup camera position
	position = XMFLOAT3(m_positionX, m_positionY, m_positionZ);
	positionVector = XMLoadFloat3(&position);

	//Setup where the camera is looking
	lookAt = XMFLOAT3(0, 0, 1);
	lookAtVector = XMLoadFloat3(&lookAt);

	

	//Transform lookat and up by rotation matrix
	lookAtVector = XMVector3TransformCoord(lookAtVector, m_rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, m_rotationMatrix);

	//Translate to position
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	//Create view matrix
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}

void Camera::RenderReflection(float height)
{
	XMFLOAT3 up, position, lookAt;
	float yaw, pitch, roll;
	

	//Up vector
	up = XMFLOAT3(0, 1, 0);

	//World position of reflection camera
	position = XMFLOAT3(m_positionX, -m_positionY + (height*2.0f), m_positionZ);

	lookAt = XMFLOAT3(0, 0, 1);

	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	//Transform lookat and up by rotation matrix
	
	XMVECTOR lookAtVec = XMVector3TransformCoord(XMLoadFloat3(&lookAt), rotationMatrix);
	XMVECTOR upVec = XMVector3TransformCoord(XMLoadFloat3(&up), rotationMatrix);
	XMVECTOR positionVec = XMLoadFloat3(&position);

	lookAtVec = XMVectorAdd(positionVec, lookAtVec);

	m_reflectionViewMatrix = XMMatrixLookAtLH(positionVec, lookAtVec, upVec);

	return;
}


void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}


void Camera::GetReflectionViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_reflectionViewMatrix;
	return;
}