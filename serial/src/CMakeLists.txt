set(UTIL_SRC Util.h Util.cpp)
set(QUASI_CLIQUE_SRC QuasiClique.h QuasiClique.cpp)
set(GRAPH_SRC Graph.h Graph.cpp)
set(MAIN_SRC main.cpp)

set(SCORP_SOURCE 
    ${MAIN_SRC}
    ${UTIL_SRC} 
    ${QUASI_CLIQUE_SRC}
    ${GRAPH_SRC}
)

set(CMAKE_CXX_FLAGS_DEBUG "-pg")

add_executable(scorp_serial ${SCORP_SOURCE})
