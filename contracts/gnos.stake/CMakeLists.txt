add_contract(gnos.stake gnos.stake ${CMAKE_CURRENT_SOURCE_DIR}/src/gnos.stake.cpp)

target_include_directories(gnos.stake
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../libs/base/include)

set_target_properties(gnos.stake
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

target_ricardian_directory(gnos.stake ${CMAKE_CURRENT_SOURCE_DIR}/ricardian)
