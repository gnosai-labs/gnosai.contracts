add_contract(song.reg song.reg ${CMAKE_CURRENT_SOURCE_DIR}/src/song.reg.cpp)

target_include_directories(song.reg
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../gnos.ntoken/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../libs/base/include)

set_target_properties(song.reg
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

target_ricardian_directory(song.reg ${CMAKE_CURRENT_SOURCE_DIR}/ricardian)
