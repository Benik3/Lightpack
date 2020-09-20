#ifndef DXGIFRAMEGRABBER_HPP
#define DXGIFRAMEGRABBER_HPP

#include "GAPIProxyFrameGrabber.hpp"
#include "LoggableTrait.hpp"

class IPCContext;
class ProxyFuncVFTable;
class Logger;
struct ID3D10Texture2D;
struct ID3D11Texture2D1;
struct IDXGISwapChain4;
struct ID3D10Device;
struct ID3D11Device5;
struct ID3D11DeviceContext4;
enum DXGI_FORMAT;

class DxgiFrameGrabber : public GAPIProxyFrameGrabber, LoggableTrait
{
public:
	static DxgiFrameGrabber *m_this;
	static DxgiFrameGrabber *getInstance(HANDLE syncRunMutex) {
		if (!m_this)
			m_this = new DxgiFrameGrabber(syncRunMutex, Logger::getInstance());
		return m_this;
	}
	static DxgiFrameGrabber *getInstance() {
		return m_this;
	}
	static bool hasInstance() {
		return m_this != NULL;
	}
	~DxgiFrameGrabber() {
		if (this->isHooksInstalled())
			this->removeHooks();
		this->free();
		m_this = NULL;
	}

	virtual bool init();
	virtual bool isGAPILoaded();
	virtual bool isHooksInstalled();
	virtual bool installHooks();
	virtual bool removeHooks();
	virtual void free();
	void freeDeviceData10();
	void freeDeviceData11();

	friend HRESULT WINAPI DXGIPresent(IDXGISwapChain4 * sc, UINT b, UINT c);

protected:
	DxgiFrameGrabber(HANDLE syncRunMutex, Logger *logger);
	void D3D10Grab(ID3D10Texture2D* pBackBuffer);
	bool D3D10Map();
	void D3D11Grab(ID3D11Texture2D1 *pBackBuffer);
	bool D3D11Map();

	ProxyFuncVFTable *m_dxgiPresentProxyFuncVFTable;
	ID3D10Texture2D *m_mapTexture10;
	ID3D10Device *m_mapDevice10;
	ID3D11Texture2D1 *m_mapTexture11;
	ID3D11Device5 *m_mapDevice11;
	ID3D11DeviceContext4 *m_mapDeviceContext11;
	UINT m_mapWidth;
	UINT m_mapHeight;
	DXGI_FORMAT m_mapFormat;
	UINT m_frameCount;
	UINT m_lastGrab;
	bool m_mapPending;
private:
	void ** calcDxgiPresentPointer();
};

#endif // DXGIFRAMEGRABBER_HPP
