cmake_minimum_required (VERSION 3.8)

set(ZekrosEngine_Includes "../includes/ZekrosEngine.cpp" "../includes/ZekrosEngine.h")
set(Windows_Includes "../includes/Windows.cpp" "../includes/Windows.h")
set(GameTimer_Includes "../includes/GameTimer.cpp" "../includes/GameTimer.h")
set(Dx11Init_Includes ${ZekrosEngine_Includes} ${Windows_Includes} ${GameTimer_Includes})

project("Dx11Init")
add_subdirectory ("engines")

target_include_directories(Dx11Init PUBLIC "${CMAKE_SOURCE_DIR}/DxInclude")