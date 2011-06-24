set(ATTRIBUTE_SRC Attribute.h Attribute.cpp)
set(VERTEX_SRC Vertex.h Vertex.cpp)
set(MAIN_SRC main.cpp)

set(SCORP_SOURCE ${MAIN_SRC} ${ATTRIBUTE_SRC} ${VERTEX_SRC} )

add_executable(scorp_serial ${SCORP_SOURCE})
