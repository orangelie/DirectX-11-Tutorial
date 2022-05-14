cmake_minimum_required (VERSION 3.8) # not mutable

set(ZekrosEngine_Includes "../includes/ZekrosEngine.cpp" "../includes/ZekrosEngine.h") # not mutable
set(Windows_Includes "../includes/Windows.cpp" "../includes/Windows.h") # not mutable
set(GameTimer_Includes "../includes/GameTimer.cpp" "../includes/GameTimer.h") # not mutable
set(Utils_Includes "../includes/DxUtils.h") # not mutable

set(Dx11Init_Includes ${ZekrosEngine_Includes} ${Windows_Includes} ${GameTimer_Includes} ${Utils_Includes})
set(Dx11Draw_Includes ${ZekrosEngine_Includes} ${Windows_Includes} ${GameTimer_Includes} ${Utils_Includes})

add_subdirectory ("engines") # not mutable


project("Dx11Init")
target_include_directories(Dx11Init PUBLIC "${CMAKE_SOURCE_DIR}/DxInclude")

project("Dx11Draw")
target_include_directories(Dx11Draw PUBLIC "${CMAKE_SOURCE_DIR}/DxInclude")