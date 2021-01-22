#include "muti-service.h"
#include "socket_itf.h"
#include "socket_mtd.h"
namespace smnet{

void Service::run() {
	auto self = shared_from_this();
	auto& sock = this->_conn->getSocket();
	sock.async_read_some(boost::asio::buffer(pReadLen, 4), [this, self](boost::system::error_code ec, 
			std::size_t lLen){
		if (ec){return;}
		if (lLen < 4){return;}
		netEdianChange(this->pReadLen, 4);
		Bytes buff(this->readLen);
		this->_conn->read(this->readLen, buff.arr);
		this->_run(std::move(buff));
		run();
	});

}

}
