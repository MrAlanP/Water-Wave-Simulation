//Code originating from Rastertek has been utilised and modified within this class
#include "TextureShader.h"

TextureShader::TextureShader()
{
	m_sampleState = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_sampleState = 0;
	m_matrixBuffer = 0;
	m_lightBuffer = 0;
	m_cameraBuffer = 0;
}

TextureShader::~TextureShader()
{

}

void TextureShader::Initialise(ID3D11Device* device, HWND hwnd)
{
	InitialiseShader(device, hwnd, L"../WaveSim/light.vs", L"../WaveSim/light.ps");

	return;
}

void TextureShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projMatrix, 
	ID3D11ShaderResourceView* texture, Light* light, XMFLOAT3 cameraPosition, float alpha)
{
	SetShaderParams(deviceContext, worldMatrix, viewMatrix, projMatrix, texture, light, cameraPosition, alpha);

	RenderShader(deviceContext, indexCount);
	return;
}

void TextureShader::InitialiseShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC alphaBufferDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	HRESULT result = D3DCompileFromFile(vsFilename, NULL, NULL, "VertexLightShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return;
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile(psFilename, NULL, NULL, "PixelLightShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return;
	}

	//Create vertex and pixel shaders
	device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);


	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_layout);

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	device->CreateSamplerState(&samplerDesc, &m_sampleState);

	//Create buffers
	CreateBuffer(device, &m_lightBuffer, sizeof(LightBuffer));
	CreateBuffer(device, &m_matrixBuffer, sizeof(MatrixBuffer));
	CreateBuffer(device, &m_cameraBuffer, sizeof(CameraBuffer));
	CreateBuffer(device, &m_alphaBuffer, sizeof(AlphaBuffer));

	return;
}

void TextureShader::CreateBuffer(ID3D11Device* device,ID3D11Buffer** buffer, int byteWidth)
{
	//Buffer desc
	D3D11_BUFFER_DESC bufferDesc;

	//Setup the buffer description
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	//Create the buffer with the bufferdesc
	device->CreateBuffer(&bufferDesc, NULL, buffer);
}


void TextureShader::Shutdown()
{
	ShutdownShader();
	return;
}

void TextureShader::ShutdownShader()
{
	// Release the light constant buffer.
	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}

	// Release the camera constant buffer.
	if (m_cameraBuffer)
	{
		m_cameraBuffer->Release();
		m_cameraBuffer = 0;
	}

	//Release the alpha buffer
	if (m_alphaBuffer)
	{
		m_alphaBuffer->Release();
		m_alphaBuffer = 0;
	}

	// Release the matrix constant buffer.
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the sampler state.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	return;
}


void TextureShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

void TextureShader::SetShaderParams(ID3D11DeviceContext* deviceContext, XMMATRIX* worldMatrix, XMMATRIX* viewMatrix,
	XMMATRIX* projectionMatrix, ID3D11ShaderResourceView* texture, Light* light, XMFLOAT3 cameraPosition, float alpha)
{

	SetVertexShaderParams(deviceContext, worldMatrix, viewMatrix, projectionMatrix, cameraPosition);
	SetPixelShaderParams(deviceContext, texture, light, alpha);
	return;
}

void TextureShader::SetVertexShaderParams(ID3D11DeviceContext* deviceContext, XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projectionMatrix, XMFLOAT3 cameraPosition)
{
	SetShaderMatrixBuffer(deviceContext, worldMatrix, viewMatrix, projectionMatrix);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	CameraBuffer* cameraPtr;
	//Lock camera buffer so it can be written to
	deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	cameraPtr = (CameraBuffer*)mappedResource.pData;
	cameraPtr->cameraPosition = cameraPosition;
	cameraPtr->padding = 0;

	//Unlock camera buffer
	deviceContext->Unmap(m_cameraBuffer, 0);

	int bufferNumber = 1;
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);
}

void TextureShader::SetPixelShaderParams(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, Light* light, float alpha)
{
	LightBuffer* lightPtr;
	AlphaBuffer* alphaPtr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

	// Lock the light constant buffer so it can be written to.
	deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	lightPtr = (LightBuffer*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	lightPtr->ambientColour = light->GetAmbientColour();
	lightPtr->diffuseColour = light->GetDiffuseColour();
	lightPtr->lightDirection = light->GetDirection();
	lightPtr->specularColour = light->GetSpecularColour();
	lightPtr->specularPower = light->GetSpecularPower();

	// Unlock the constant buffer.
	deviceContext->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	int bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	//Lock the alpha buffer
	deviceContext->Map(m_alphaBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	//Set buffer to constant buffer
	alphaPtr = (AlphaBuffer*)mappedResource.pData;
	alphaPtr->alphaAmount = alpha;

	//Unlock alpha buffer
	deviceContext->Unmap(m_alphaBuffer, 0);

	//Buffer position in pixel shader
	bufferNumber = 1;

	//Set constant buffer to alphabuffer
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_alphaBuffer);
}

void TextureShader::SetShaderMatrixBuffer(ID3D11DeviceContext* context, XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projectionMatrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBuffer* dataPtr;
	// Transpose the matrices to prepare them for the shader.
	*worldMatrix = XMMatrixTranspose(*worldMatrix);
	*viewMatrix = XMMatrixTranspose(*viewMatrix);
	*projectionMatrix = XMMatrixTranspose(*projectionMatrix);

	// Lock the constant buffer so it can be written to.
	context->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBuffer*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = *worldMatrix;
	dataPtr->view = *viewMatrix;
	dataPtr->projection = *projectionMatrix;

	// Unlock the constant buffer.
	context->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	int bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	context->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
}



void TextureShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}