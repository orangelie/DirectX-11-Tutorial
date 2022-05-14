#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#include "Dx11Init.h"

int __stdcall WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{

#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try {
		std::unique_ptr<Dx11Init> _main_Ptr(new Dx11Init);
		_main_Ptr->Initialize(960, 860, false);
		_main_Ptr->Run();

	}
	catch (const std::exception& e) {
		MessageBoxA(0, e.what(), "# Exception Error #", MB_OK);
		return -1;
	}

	return 0;
}