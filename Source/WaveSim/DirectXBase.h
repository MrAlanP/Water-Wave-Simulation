//Code originating from Rastertek has been utilised and modified within this class

#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;


class DirectXBase
{

private:
	enum RasterState
	{
		Solid,
		Wireframe
	};
public:
	DirectXBase();
	DirectXBase(const DirectXBase&);
	~DirectXBase();

	void* operator new(size_t);
	void operator delete(void*);

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();
	
	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*);

	void ToggleWireframe();
	void ToggleCulling(bool);
	void ToggleZBuffer(bool);

	ID3D11DepthStencilView* GetDepthStencilView();
	void SetBackBufferRenderTarget();

private:
	bool m_vsync_enabled;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilState* m_depthDisabledStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	ID3D11RasterizerState* m_wireframeRasterState;
	ID3D11RasterizerState* m_rasterStateNoCulling;
	ID3D11BlendState* m_blendState;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;

	RasterState m_currentRasterState;
};
