cmake_minimum_required (VERSION 3.8)

add_executable (Dx11Init  "Dx11Init.h" "main.cpp" ${Dx11Init_Includes})
target_link_libraries(Dx11Init ${CMAKE_SOURCE_DIR}/Lib/x64/d3dx11.lib)
target_link_libraries(Dx11Init ${CMAKE_SOURCE_DIR}/Lib/x64/d3d11.lib)
target_link_libraries(Dx11Init ${CMAKE_SOURCE_DIR}/Lib/x64/d3d10.lib)
target_link_libraries(Dx11Init ${CMAKE_SOURCE_DIR}/Lib/x64/d3dx10.lib)
target_link_libraries(Dx11Init ${CMAKE_SOURCE_DIR}/Lib/x64/dxgi.lib)
target_link_libraries(Dx11Init ${CMAKE_SOURCE_DIR}/Lib/x64/dxguid.lib)