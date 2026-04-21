add_contract(gnos gnos ${CMAKE_CURRENT_SOURCE_DIR}/src/gnos.cpp)

target_include_directories(gnos
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(gnos
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

target_ricardian_directory(gnos ${CMAKE_CURRENT_SOURCE_DIR}/ricardian)
