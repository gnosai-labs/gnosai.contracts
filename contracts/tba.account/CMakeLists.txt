add_contract(tba.account tba.account ${CMAKE_CURRENT_SOURCE_DIR}/src/tba.account.cpp)

target_include_directories(tba.account
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../libs/base/include)

set_target_properties(tba.account
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

target_ricardian_directory(tba.account ${CMAKE_CURRENT_SOURCE_DIR}/ricardian)
