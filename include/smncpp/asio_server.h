#ifndef ASIO_SERVER_H_N3AYWMGX
#define ASIO_SERVER_H_N3AYWMGX

#include "base_asio_socket.h"
#include <memory>
#include <functional>
#include <boost/asio.hpp>

namespace smnet{

	class Session:public std::enable_shared_from_this<Session>{
		typedef  boost::asio::ip::tcp tcp;
		public:
			Session(tcp::socket sock): _conn(new SMConn(std::move(sock))){}
			virtual ~Session(){_conn->close();}

		public:
			void start() {
				run();
			}
		protected:
			virtual void run() = 0;
			std::shared_ptr<SMConn> _conn;
	};


	class Server{
		typedef  boost::asio::ip::tcp tcp;
		public:
			Server(boost::asio::io_service& ioc, short port, std::function<std::shared_ptr<Session>(tcp::socket sock)> sessionMaker):_acceptor(ioc, tcp::endpoint(tcp::v4(), port)),
	   						_socket(ioc), _sessionMaker(sessionMaker)	{}
			~Server(){_acceptor.close();}

			void start();
		private:
			tcp::acceptor _acceptor;
			tcp::socket _socket;
			std::function<std::shared_ptr<Session>(tcp::socket sock)> _sessionMaker;
	};

}


#endif /* end of include guard: ASIO_SERVER_H_N3AYWMGX */
