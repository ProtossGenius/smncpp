cmake_minimum_required(VERSION 2.6)

project (smncpp)

add_library(smncpp asio_server.cpp base_asio_socket.cpp muti-service.cpp socket_itf.cpp socket_mtd.cpp ticker.cpp)
target_link_libraries(smncpp )

install (TARGETS smncpp DESTINATION bin/smncpp)
install (FILES asio_server.h assert.h base_asio_socket.h channel.h lockm.h muti-service.h socket_itf.h socket_mtd.h ticker.h DESTINATION include/smncpp)

