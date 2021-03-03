cmake_minimum_required(VERSION 2.6)

project (smncpp)

set(CXX_FLAGS
        -g
        # -DVALGRIND
        -DCHECK_PTHREAD_RETURN_VALUE
        -D_FILE_OFFSET_BITS=64
        -Wall
        -Wextra
        -Werror
        -Wconversion
        -Wno-unused-parameter
        -Wold-style-cast
        -Woverloaded-virtual
        -Wpointer-arith
        -Wshadow
        -Wwrite-strings
        -march=native
        # -MMD
        -rdynamic
        )

add_library(smncpp asio_server.cpp base_asio_socket.cpp muti-service.cpp socket_itf.cpp socket_mtd.cpp ticker.cpp)
target_link_libraries(smncpp )

install (TARGETS smncpp DESTINATION lib)
install (FILES asio_server.h assert.h base_asio_socket.h channel.h lockm.h muti-service.h socket_itf.h socket_mtd.h ticker.h DESTINATION include/smncpp)

