cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)
project(AntHello)

add_subdirectory(src)

configure_file(bin/initScript.in bin/initScript @ONLY)

set(FILE_INSTALL ${FILE_INSTALL} bin/conf.xml)
set(PROG_INSTALL ${PROG_INSTALL} ${CMAKE_BINARY_DIR}/bin/initScript
    ${CMAKE_SOURCE_DIR}/bin/generate.pl)

install(FILES ${FILE_INSTALL} DESTINATION bin)
install(PROGRAMS ${PROG_INSTALL} DESTINATION bin)
