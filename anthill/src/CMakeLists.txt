set(ANTHILL_INC_PATH /opt/libanthill/include CACHE PATH "Anthill include path")
set(ANTHILL_INC ${ANTHILL_INC} ${ANTHILL_INC_PATH})
set(ANTHILL_INC ${ANTHILL_INC} ${ANTHILL_INC_PATH}/FilterDev)
set(ANTHILL_INC ${ANTHILL_INC} ${ANTHILL_INC_PATH}/eventAPI)

set(ANTHILL_LIB_PATH /opt/libanthill/lib CACHE PATH "Anthill library path")
set(ANTHILL_LIB ${ANTHILL_LIB} /opt/libanthill/lib)

set(LIBS ${LIBS} ds expat ahevent)

include_directories(${ANTHILL_INC})
link_directories(${ANTHILL_LIB})

set(COMMON_SRC Util.cpp QuasiClique.cpp)
set(MANAGER_SRC ${COMMON_SRC} ManagerFilter.cpp)
set(PROCESS_SRC ${COMMON_SRC} ProcessFilter.cpp)
set(PRINTER_SRC ${COMMON_SRC} PrinterFilter.cpp)
set(MAIN_SRC main.cpp)

add_library(printer SHARED ${PRINTER_SRC})
target_link_libraries(printer ${LIBS})

add_library(manager SHARED ${MANAGER_SRC})
target_link_libraries(manager ${LIBS})

add_library(process SHARED ${PROCESS_SRC})
target_link_libraries(process ${LIBS})

add_executable(main ${MAIN_SRC})
target_link_libraries(main ${LIBS})

install(TARGETS printer manager process main DESTINATION bin)
