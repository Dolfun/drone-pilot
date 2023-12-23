if(NOT CONFIG_LWIP_IPV6 AND NOT CMAKE_BUILD_EARLY_EXPANSION)
  message(STATUS "IPV6 support is disabled so the asio component will not be built")
  idf_component_register()
  return()
endif()

set(asio_sources "asio.cc")
set(asio_requires lwip)

idf_component_register(
  SRCS ${asio_sources}
  INCLUDE_DIRS
    "${COMPONENT_DIR}/asio/asio/include"
    "${COMPONENT_DIR}"
  PRIV_INCLUDE_DIRS ${asio_priv_includes}
  PRIV_REQUIRES ${asio_requires}
)

target_compile_definitions(
  ${COMPONENT_LIB} 
    PUBLIC SA_RESTART=0x01
    SA_NOCLDSTOP=0x2
    SA_NOCLDWAIT=0x4
    ASIO_DISABLE_SERIAL_PORT
    ASIO_SEPARATE_COMPILATION
    ASIO_STANDALONE
    ASIO_HAS_PTHREADS
    OPENSSL_NO_ENGINE
)

if(NOT CONFIG_COMPILER_CXX_EXCEPTIONS)
    target_compile_definitions(${COMPONENT_LIB} PUBLIC ASIO_NO_EXCEPTIONS)
endif()

if(NOT CONFIG_COMPILER_RTTI)
    target_compile_definitions(${COMPONENT_LIB} PUBLIC ASIO_NO_TYPEID)
endif() 