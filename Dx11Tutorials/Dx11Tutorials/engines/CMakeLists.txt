cmake_minimum_required (VERSION 3.8) # not mutable

add_executable (Dx11Init  "Dx11Init.h" "main_Dx11Init.cpp" ${Dx11Init_Includes})
target_link_libraries(Dx11Init ${CMAKE_SOURCE_DIR}/Lib/x64/d3dx11.lib)
target_link_libraries(Dx11Init ${CMAKE_SOURCE_DIR}/Lib/x64/d3d11.lib)
target_link_libraries(Dx11Init ${CMAKE_SOURCE_DIR}/Lib/x64/dxgi.lib)



add_executable (Dx11Draw  "Dx11Draw.h" "main_Dx11Draw.cpp" ${Dx11Draw_Includes})
target_link_libraries(Dx11Draw ${CMAKE_SOURCE_DIR}/Lib/x64/d3d11.lib)
target_link_libraries(Dx11Draw ${CMAKE_SOURCE_DIR}/Lib/x64/d3dx11.lib)
target_link_libraries(Dx11Draw ${CMAKE_SOURCE_DIR}/Lib/x64/Effects11d.lib)
target_link_libraries(Dx11Draw ${CMAKE_SOURCE_DIR}/Lib/x64/dxgi.lib)