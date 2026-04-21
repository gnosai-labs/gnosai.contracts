add_contract(gnos.dao gnos.dao ${CMAKE_CURRENT_SOURCE_DIR}/src/gnos.dao.cpp)

target_include_directories(gnos.dao
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../libs/base/include)

set_target_properties(gnos.dao
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

target_ricardian_directory(gnos.dao ${CMAKE_CURRENT_SOURCE_DIR}/ricardian)
