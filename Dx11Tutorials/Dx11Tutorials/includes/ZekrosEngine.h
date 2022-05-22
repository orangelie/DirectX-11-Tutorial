/*
*
* < ZekrosEngine.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#include "Windows.h"
#include "GameTimer.h"
#include "DxUtils.h"
#include "GeometyGenerator.h"

namespace orangelie {

	namespace Engine {

		class ZekrosEngine {
		public:
			ZekrosEngine();
			ZekrosEngine(const ZekrosEngine&) = delete;
			ZekrosEngine& operator=(const ZekrosEngine&) = delete;
			virtual ~ZekrosEngine();

			void Initialize(int maxScreenWidth, int maxScreenHeight, bool isFullscreenMode);
			void Run();

			static ZekrosEngine* gZekrosEngine;
			virtual LRESULT MessageHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

		private:

		protected:
			void OnResize();
			// Virtuals
			virtual void init() = 0;
			virtual void update(float dt) = 0;
			virtual void draw(float dt) = 0;
			virtual void resize(float dt) = 0;

			// Mouse Controller
			virtual void MouseDown(WPARAM btnState, int x, int y) = 0;
			virtual void MouseUp(WPARAM btnState, int x, int y) = 0;
			virtual void MouseMove(WPARAM btnState, int x, int y) = 0;

			// Client ScreenSize
			int m_ClientWidth;
			int m_ClientHeight;

			// HWND
			HWND m_hWnd;

			// DxgiInterface
			D3D_FEATURE_LEVEL m_featureLevel;
			ComPtr<ID3D11Device> m_Device = nullptr;
			ComPtr<ID3D11DeviceContext> m_ImmediateContext = nullptr;


			// OnResize
			ComPtr<IDXGISwapChain> m_SwapChain = nullptr;
			D3D11_VIEWPORT m_Viewport = {};
			ComPtr<ID3D11RenderTargetView> m_Rtv = nullptr;
			ComPtr<ID3D11Texture2D> m_DsvBuffer = nullptr;
			ComPtr<ID3D11DepthStencilView> m_DepthStencilView = nullptr;

		private:
			std::unique_ptr<orangelie::Windows::Win32> m_Win32 = nullptr;
			orangelie::Time::GameTimer m_GameTimer;

			bool m_IsEnginePaused = false;
			bool m_IsSizeMinimized = false;
			bool m_IsSizeMaximized = false;
			bool m_IsResizing = false;

			UINT m_4xMsaaMSQuality = 0;
			
			ComPtr<IDXGIFactory> m_Factory = nullptr;
			ComPtr<IDXGIAdapter> m_Adapter = nullptr;
			ComPtr<IDXGIDevice> m_DxgiDevice = nullptr;
		};
	}
}

LRESULT _stdcall WindowProcedure(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);