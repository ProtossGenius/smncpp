#include "asio_server.h"

namespace smnet{

	void Server::start(){
				this->_acceptor.async_accept(this->_socket, [this](boost::system::error_code ec){
						if (!ec){
							auto newSession = _sessionMaker(std::move(this->_socket));
							newSession->start();
						}
						start();
					});
			}
}
