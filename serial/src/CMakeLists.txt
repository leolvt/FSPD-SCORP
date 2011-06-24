set(UTIL_SRC Util.h Util.cpp)
set(GRAPH_SRC Graph.h Graph.cpp)
set(EDGE_SRC Edge.h Edge.cpp)
set(VERTEX_SRC Vertex.h Vertex.cpp)
set(ATTRIBUTE_SRC Attribute.h Attribute.cpp)
set(MAIN_SRC main.cpp)

set(SCORP_SOURCE 
    ${MAIN_SRC}
    ${UTIL_SRC} 
    ${ATTRIBUTE_SRC} 
    ${EDGE_SRC}
    ${VERTEX_SRC} 
    ${GRAPH_SRC}
)

add_executable(scorp_serial ${SCORP_SOURCE})
