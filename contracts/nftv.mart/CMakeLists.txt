add_contract(nftv.mart nftv.mart ${CMAKE_CURRENT_SOURCE_DIR}/src/nftv.mart.cpp)

target_include_directories(nftv.mart
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../libs/base/include)

set_target_properties(nftv.mart
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

target_ricardian_directory(nftv.mart ${CMAKE_CURRENT_SOURCE_DIR}/ricardian)
