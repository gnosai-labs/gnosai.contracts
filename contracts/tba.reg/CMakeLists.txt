add_contract(tba.reg tba.reg ${CMAKE_CURRENT_SOURCE_DIR}/src/tba.reg.cpp)

target_include_directories(tba.reg
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../libs/base/include)

set_target_properties(tba.reg
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

target_ricardian_directory(tba.reg ${CMAKE_CURRENT_SOURCE_DIR}/ricardian)
