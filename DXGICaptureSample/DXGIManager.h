#pragma once

#include <windows.h>
#include <atlbase.h>
#include <dxgitype.h>
#include <dxgi1_2.h>
#include <d3d11.h>
#include <wincodec.h>
#include <vector>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "gdiplus.lib")

using namespace std;

class DXGIPointerInfo;

class DXGIPointerInfo
{
public:
	DXGIPointerInfo(BYTE* pPointerShape, UINT uiPointerShapeBufSize, DXGI_OUTDUPL_FRAME_INFO fi, DXGI_OUTDUPL_POINTER_SHAPE_INFO psi);
	~DXGIPointerInfo();
	BYTE* GetBuffer();
	UINT GetBufferSize();
	DXGI_OUTDUPL_FRAME_INFO& GetFrameInfo();
	DXGI_OUTDUPL_POINTER_SHAPE_INFO& GetShapeInfo();

private:
	BYTE* m_pPointerShape;
	UINT m_uiPointerShapeBufSize;
	DXGI_OUTDUPL_POINTER_SHAPE_INFO m_PSI;
	DXGI_OUTDUPL_FRAME_INFO m_FI;
};

class DXGIOutputDuplication
{
public:
	DXGIOutputDuplication(IDXGIAdapter1* pAdapter,
		ID3D11Device* pD3DDevice,
		ID3D11DeviceContext* pD3DDeviceContext,
		IDXGIOutput1* pDXGIOutput1,
		IDXGIOutputDuplication* pDXGIOutputDuplication);
	
	HRESULT GetDesc(DXGI_OUTPUT_DESC& desc);
	HRESULT AcquireNextFrame(IDXGISurface1** pD3D11Texture2D, DXGIPointerInfo*& pDXGIPointer);
	HRESULT ReleaseFrame();
	
	bool IsPrimary();

private:
	CComPtr<IDXGIAdapter1> m_Adapter;
	CComPtr<ID3D11Device> m_D3DDevice;
	CComPtr<ID3D11DeviceContext> m_D3DDeviceContext;
	CComPtr<IDXGIOutput1> m_DXGIOutput1;
	CComPtr<IDXGIOutputDuplication> m_DXGIOutputDuplication;
};

class DXGIManager
{
public:
	DXGIManager();
	~DXGIManager();
	HRESULT Init();

	HRESULT SetCaptureSource(INT index = 0);
	vector<DXGI_OUTPUT_DESC> GetAllOutputs();

	HRESULT GetOutputRect(RECT& rc);
	HRESULT GetOutputBits(BYTE* pBits, RECT& rcDest);
private:
	int GetMonitorCount();
	vector<DXGIOutputDuplication> GetOutputDuplication();
	void DrawMousePointer(BYTE* pDesktopBits, RECT rcDesktop, RECT rcDest);
private:
	CComPtr<IDXGIFactory1> m_spDXGIFactory1;
	vector<DXGIOutputDuplication> m_vOutputs;
	bool m_bInitialized;
	UINT m_uiCurrentOutput1;
	RECT m_rcCurrentOutput;
	BYTE* m_pBuf;

	CComPtr<IWICImagingFactory> m_spWICFactory;
	ULONG_PTR m_gdiplusToken;
	DXGIPointerInfo* m_pDXGIPointer;
};