add_contract(gnos.vault gnos.vault ${CMAKE_CURRENT_SOURCE_DIR}/src/gnos.vault.cpp)

target_include_directories(gnos.vault
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(gnos.vault
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ricardian/gnos.vault.contracts.md.in ${CMAKE_CURRENT_BINARY_DIR}/ricardian/gnos.vault.contracts.md @ONLY )

target_compile_options( gnos.vault PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR}/ricardian -R${CMAKE_CURRENT_BINARY_DIR}/ricardian )
