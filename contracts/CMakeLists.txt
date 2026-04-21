cmake_minimum_required(VERSION 3.5)

project(contracts)

option(SYSTEM_CONFIGURABLE_WASM_LIMITS
       "Enables use of the host functions activated by the CONFIGURABLE_WASM_LIMITS protocol feature" ON)

option(SYSTEM_BLOCKCHAIN_PARAMETERS
       "Enables use of the host functions activated by the BLOCKCHAIN_PARAMETERS protocol feature" ON)

find_package(flon.cdt)

set(CDT_VERSION_MIN "0.3")
set(CDT_VERSION_SOFT_MAX "5.1")

if(SYSTEM_ENABLE_CDT_VERSION_CHECK)
  set(VERSION_MATCH_ERROR_MSG "")
  CDT_CHECK_VERSION(VERSION_OUTPUT "${CDT_VERSION}" "${CDT_VERSION_MIN}" "${CDT_VERSION_SOFT_MAX}"
                      "${CDT_VERSION_HARD_MAX}" VERSION_MATCH_ERROR_MSG)
  if(VERSION_OUTPUT STREQUAL "MATCH")
    message(STATUS "Using CDT version ${CDT_VERSION}")
  elseif(VERSION_OUTPUT STREQUAL "WARN")
    message(
      WARNING
        "Using CDT version ${CDT_VERSION} even though it exceeds the maximum supported version of ${CDT_VERSION_SOFT_MAX}; continuing with configuration, however build may fail.\nIt is recommended to use CDT version ${CDT_VERSION_SOFT_MAX}.x"
    )
  else()
    message(
      FATAL_ERROR
        "Found CDT version ${CDT_VERSION} but it does not satisfy version requirements: ${VERSION_MATCH_ERROR_MSG}\nPlease use CDT version ${CDT_VERSION_SOFT_MAX}.x"
    )
  endif()
endif()

add_subdirectory(song.reg)
add_subdirectory(mv.reg)
add_subdirectory(gnos.ntoken)
add_subdirectory(tba.reg)
add_subdirectory(tba.account)
add_subdirectory(nftv.mart)
add_subdirectory(gnos.stake)
