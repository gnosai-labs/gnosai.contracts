add_contract(mv.reg mv.reg ${CMAKE_CURRENT_SOURCE_DIR}/src/mv.reg.cpp)

target_include_directories(mv.reg
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../libs/base/include)

set_target_properties(mv.reg
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

target_ricardian_directory(mv.reg ${CMAKE_CURRENT_SOURCE_DIR}/ricardian)
