add_contract(gnos.ntoken gnos.ntoken ${CMAKE_CURRENT_SOURCE_DIR}/src/gnos.ntoken.cpp)

target_include_directories(gnos.ntoken
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../libs/base/include)

set_target_properties(gnos.ntoken
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

target_ricardian_directory(gnos.ntoken ${CMAKE_CURRENT_SOURCE_DIR}/ricardian)
