/*
*
* < Windows.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include <d3dcommon.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <DirectXColors.h>

#include <d3dx11effect.h>
#include <D3DX11.h>

#if not defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <windowsx.h>
#include <wrl.h>

using namespace Microsoft::WRL;
using namespace DirectX;


#define THROW(e) { \
	std::string __err_message = std::string(__FILE__) + ": " + e; \
	throw std::runtime_error(__err_message); \
} \

#define HR(e) { \
	if(FAILED(e)) { \
		std::string __err_message = std::string(__FILE__) + std::to_string(__LINE__) + std::string(__FUNCTION__); \
		throw std::runtime_error(__err_message); \
	} \
} \

#define NULLPTR_CHECK(e) { \
	if(e == nullptr) MessageBoxA(0, std::to_string(__LINE__).c_str(), "Checked !", MB_OK); \
} \

namespace orangelie {
	namespace CppStdUtil {
		template <class _Tp>
		_Tp& unmove(_Tp&& ____TpTp__) {
			return ____TpTp__;
		}
	}
}

namespace orangelie {
	namespace Windows {
		class Win32 {
		public:
			Win32();
			Win32(const Win32&) = delete;
			Win32& operator=(const Win32&) = delete;
			~Win32();

			void Intialize(WNDPROC wndProcedureFunc, int maxScreenWidth, int maxScreenHeight, bool isFullscreenMode);

			HWND GetHwnd() const;

		private:
			void BuildWndClass(WNDPROC wndProcedureFunc);
			void BuildDevMode();
			void CreateParentProcess();

		private:
			const wchar_t*	m_ModuleClassName = L"ZekrosEngine@orangelie";

			HWND			m_HwndHandle;
			HINSTANCE		m_MouduleHandle;

			int				m_MaxScreenWidth;
			int				m_MaxScreenHeight;
			int				m_IsFullscreenMode;

			int				m_DesktopScreenWidth;
			int				m_DesktopScreenHeight;
			int				m_WindowPositionX;
			int				m_WindowPositionY;
		};
	}
}