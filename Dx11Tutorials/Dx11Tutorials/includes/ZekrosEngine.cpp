/*
*
* < ZekrosEngine.cpp >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "ZekrosEngine.h"

namespace orangelie {

	namespace Engine {

		ZekrosEngine::ZekrosEngine() {
			gZekrosEngine = this;
		}

		ZekrosEngine::~ZekrosEngine() {
		}

		void ZekrosEngine::Initialize(int maxScreenWidth, int maxScreenHeight, bool isFullscreenMode) {
			UINT createDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

			// Win32
			m_Win32 = std::make_unique<orangelie::Windows::Win32>();
			m_Win32->Intialize(WindowProcedure, maxScreenWidth, maxScreenHeight, isFullscreenMode);
			m_hWnd = m_Win32->GetHwnd();
			
			HR(D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				createDeviceFlags,
				nullptr,
				0,
				D3D11_SDK_VERSION,
				m_Device.GetAddressOf(),
				&m_featureLevel,
				m_ImmediateContext.GetAddressOf()));

			if (m_featureLevel != D3D_FEATURE_LEVEL_11_0) {
				HR(-1);
			}

			HR(m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaMSQuality));
			assert(m_4xMsaaMSQuality > 0);

			DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
			swapChainDesc.BufferCount = 1;
			swapChainDesc.BufferDesc.Width = maxScreenWidth;
			swapChainDesc.BufferDesc.Height = maxScreenHeight;
			swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
			swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.Flags = 0;
			swapChainDesc.OutputWindow = m_hWnd;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Windowed = !isFullscreenMode;

			HR(m_Device->QueryInterface(IID_PPV_ARGS(m_DxgiDevice.GetAddressOf())));
			HR(m_DxgiDevice->GetParent(IID_PPV_ARGS(m_Adapter.GetAddressOf())));
			HR(m_Adapter->GetParent(IID_PPV_ARGS(m_Factory.GetAddressOf())));

			HR(m_Factory->CreateSwapChain(m_Device.Get(), &swapChainDesc, m_SwapChain.GetAddressOf()));

			OnResize();

			init();
		}

		void ZekrosEngine::Run() {
			MSG msg = {};

			m_GameTimer.Reset();

			while(msg.message != WM_QUIT) {
				if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessageW(&msg);
				}
				else {
					m_GameTimer.Tick();

					if (m_IsEnginePaused == false) {
						update(m_GameTimer.DeltaTime());
						draw(m_GameTimer.DeltaTime());
					}
					else {
						Sleep(100);
					}
				}
			}
		}
		
		LRESULT ZekrosEngine::MessageHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
			switch (uMessage) {
			case WM_ACTIVATE:
				if (LOWORD(wParam) == WA_INACTIVE) {
					m_GameTimer.Stop();
					m_IsEnginePaused = true;
				}
				else {
					m_GameTimer.Start();
					m_IsEnginePaused = false;
				}
				return 0;

			case WM_SIZE:
				m_ClientWidth = LOWORD(lParam);
				m_ClientHeight = HIWORD(lParam);

				if (wParam == SIZE_MINIMIZED) {
					m_IsSizeMinimized = true;
					m_IsSizeMaximized = false;
					m_IsEnginePaused = true;
				}

				else if (wParam == SIZE_MAXIMIZED) {
					m_IsSizeMinimized = false;
					m_IsSizeMaximized = true;
					m_IsEnginePaused = false;
					OnResize();
				}

				else if (wParam == SIZE_RESTORED) {
					if (m_IsSizeMinimized) {
						m_IsSizeMinimized = false;
						m_IsEnginePaused = false;
						OnResize();
					}
					else if (m_IsSizeMaximized) {
						m_IsSizeMaximized = false;
						m_IsEnginePaused = false;
						OnResize();
					}
					else if (m_IsResizing) {

					}
				}
				
				return 0;

			case WM_ENTERSIZEMOVE:
				m_IsEnginePaused = true;
				m_IsResizing = true;
				m_GameTimer.Stop();
				return 0;
			case WM_EXITSIZEMOVE:
				m_IsEnginePaused = false;
				m_IsResizing = false;
				m_GameTimer.Start();
				OnResize();
				return 0;

			case WM_MOUSEMOVE:
				MouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				return 0;

			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_LBUTTONDOWN:
				MouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				return 0;

			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			case WM_LBUTTONUP:
				MouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				return 0;

			case WM_GETMINMAXINFO:
				((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
				((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
				return 0;

			case WM_MENUCHAR:
				return MAKELRESULT(0, MNC_CLOSE);

			case WM_KEYDOWN:
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
					PostQuitMessage(0);
				}
				return 0;

			case WM_DESTROY:
			case WM_CLOSE:
				PostQuitMessage(0);
				return 0;
			}

			return DefWindowProcW(hWnd, uMessage, wParam, lParam);
		}

		void ZekrosEngine::OnResize() {
			assert(m_SwapChain);

			if(m_Rtv != nullptr) m_Rtv->Release();
			if (m_DsvBuffer != nullptr) m_DsvBuffer->Release();
			if (m_DepthStencilView != nullptr) m_DepthStencilView->Release();

			HR(m_SwapChain->ResizeBuffers(1, m_ClientWidth, m_ClientHeight,
				DXGI_FORMAT_R8G8B8A8_UNORM, 0));

			ComPtr<ID3D11Texture2D> renderTargetPtr = nullptr;
			HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(renderTargetPtr.GetAddressOf())));
			HR(m_Device->CreateRenderTargetView(renderTargetPtr.Get(), nullptr, m_Rtv.GetAddressOf()));

			D3D11_TEXTURE2D_DESC depthStencilDesc = {};
			depthStencilDesc.ArraySize = 1;
			depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthStencilDesc.CPUAccessFlags = 0;
			depthStencilDesc.Height = m_ClientHeight;
			depthStencilDesc.Width = m_ClientWidth;
			depthStencilDesc.MipLevels = 1;
			depthStencilDesc.MiscFlags = 0;
			depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
			depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

			HR(m_Device->CreateTexture2D(&depthStencilDesc, nullptr, m_DsvBuffer.GetAddressOf()));
			HR(m_Device->CreateDepthStencilView(m_DsvBuffer.Get(), nullptr, m_DepthStencilView.GetAddressOf()));

			m_ImmediateContext->OMSetRenderTargets(1, m_Rtv.GetAddressOf(), m_DepthStencilView.Get());

			m_Viewport.Height = (float)m_ClientHeight;
			m_Viewport.Width = (float)m_ClientWidth;
			m_Viewport.MinDepth = 0.0f;
			m_Viewport.MaxDepth = 1.0f;
			m_Viewport.TopLeftX = 0.0f;
			m_Viewport.TopLeftY = 0.0f;

			m_ImmediateContext->RSSetViewports(1, &m_Viewport);
		}
	}
}

orangelie::Engine::ZekrosEngine* orangelie::Engine::ZekrosEngine::gZekrosEngine = nullptr;
LRESULT _stdcall WindowProcedure(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	return orangelie::Engine::ZekrosEngine::gZekrosEngine->MessageHandler(hWnd, uMessage, wParam, lParam);
}