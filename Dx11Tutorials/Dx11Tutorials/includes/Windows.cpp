/*
*
* < Windows.cpp >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "Windows.h"

namespace orangelie {

	namespace Windows {

		Win32::Win32() {

		}

		Win32::~Win32() {
		}

		void Win32::Intialize(WNDPROC wndProcedureFunc, int maxScreenWidth, int maxScreenHeight, bool isFullscreenMode) {
			m_MouduleHandle = GetModuleHandleW(0);
			m_MaxScreenWidth = maxScreenWidth;
			m_MaxScreenHeight = maxScreenHeight;
			m_IsFullscreenMode = isFullscreenMode;

			m_DesktopScreenWidth = GetSystemMetrics(SM_CXSCREEN);
			m_DesktopScreenHeight = GetSystemMetrics(SM_CYSCREEN);

			BuildWndClass(wndProcedureFunc);
			BuildDevMode();
			CreateParentProcess();

			ShowWindow(m_HwndHandle, SW_SHOW);
			SetForegroundWindow(m_HwndHandle);
			UpdateWindow(m_HwndHandle);

			ShowCursor((m_IsFullscreenMode ? FALSE : TRUE));
		}

		HWND Win32::GetHwnd() const {
			return m_HwndHandle;
		}

		// ======================================================================================================= //
		//                                                   PRIVATE                                               //
		// ======================================================================================================= //

		void Win32::BuildWndClass(WNDPROC wndProcedureFunc) {
			WNDCLASSEXW wndClass = {};

			wndClass.cbClsExtra = 0;
			wndClass.cbSize = sizeof(WNDCLASSEXW);
			wndClass.cbWndExtra = 0;
			wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			wndClass.hCursor = LoadCursor(m_MouduleHandle, IDC_ARROW);
			wndClass.hIcon = LoadIcon(m_MouduleHandle, IDI_APPLICATION);
			wndClass.hIconSm = wndClass.hIcon;
			wndClass.hInstance = m_MouduleHandle;
			wndClass.lpfnWndProc = wndProcedureFunc;
			wndClass.lpszClassName = m_ModuleClassName;
			wndClass.lpszMenuName = 0;
			wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

			RegisterClassExW(&wndClass);
		}

		void Win32::BuildDevMode() {
			DEVMODE devMode = {};

			if (m_IsFullscreenMode) {
				devMode.dmSize = sizeof(DEVMODE);
				devMode.dmPelsWidth = m_MaxScreenWidth = m_DesktopScreenWidth;
				devMode.dmPelsHeight = m_MaxScreenHeight = m_DesktopScreenHeight;
				devMode.dmBitsPerPel = 32;
				devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

				ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);

				m_WindowPositionX = m_WindowPositionY = 0;
				m_MaxScreenWidth = m_DesktopScreenWidth;
				m_MaxScreenHeight = m_DesktopScreenHeight;
			}
			else {
				m_WindowPositionX = (m_DesktopScreenWidth - m_MaxScreenWidth) / 2;
				m_WindowPositionY = (m_DesktopScreenHeight - m_MaxScreenHeight) / 2;
			}
		}

		void Win32::CreateParentProcess() {
			DWORD dwStyle_1 =
				(m_IsFullscreenMode ? WS_EX_APPWINDOW : 0);
			DWORD dwStyle_2 =
				(m_IsFullscreenMode ? WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | WS_POPUP :
				WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | WS_OVERLAPPEDWINDOW);

			m_HwndHandle = CreateWindowExW(dwStyle_1,
				m_ModuleClassName,
				m_ModuleClassName,
				dwStyle_2,
				m_WindowPositionX,
				m_WindowPositionY,
				m_MaxScreenWidth,
				m_MaxScreenHeight,
				nullptr,
				nullptr,
				m_MouduleHandle,
				nullptr);

			if (m_HwndHandle == nullptr) {
				THROW("CreateWindowExW [m_HwndHandle == nullptr]");
			}
		}
	}

}