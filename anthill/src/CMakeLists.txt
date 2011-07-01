set(ANTHILL_INC_PATH /opt/libanthill/include CACHE PATH "Anthill include path")
set(ANTHILL_INC ${ANTHILL_INC} ${ANTHILL_INC_PATH})
set(ANTHILL_INC ${ANTHILL_INC} ${ANTHILL_INC_PATH}/FilterDev)
set(ANTHILL_INC ${ANTHILL_INC} ${ANTHILL_INC_PATH}/eventAPI)

set(ANTHILL_LIB_PATH /opt/libanthill/lib CACHE PATH "Anthill library path")
set(ANTHILL_LIB ${ANTHILL_LIB} /opt/libanthill/lib)

set(LIBS ${LIBS} ds expat ahevent)

include_directories(${ANTHILL_INC})
link_directories(${ANTHILL_LIB})

add_library(manager SHARED ManagerFilter.cpp)
target_link_libraries(manager ${LIBS})

add_library(process SHARED ProcessFilter.cpp)
target_link_libraries(process ${LIBS})

add_executable(main main.cpp)
target_link_libraries(main ${LIBS})

install(TARGETS manager process main DESTINATION bin)
