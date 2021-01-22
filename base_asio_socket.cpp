#include "base_asio_socket.h"
#include <iostream>
namespace smnet{
	size_t min(size_t a, size_t b){return a < b ? a : b;}
	const int  SMConn::read(size_t n, char * bytes) const {
		using namespace boost::asio;
		size_t acceptSize = 0;
		while(acceptSize < n) {
			auto len = this->_socket.read_some(buffer(this->_buffer, min(n-acceptSize, this->BuffSize)), this->_err);
			if (this->_err){
				return this->_err.value();
			}
			memcpy(bytes + acceptSize, this->_buffer, len);
			acceptSize += len;
		}
		return 0;
	}
	
	const int  SMConn::write(size_t n, const char *bytes) const {
		using namespace boost::asio;
		size_t sendSize = 0;
		while (sendSize < n){
			auto len = this->_socket.write_some(buffer(bytes + sendSize, min(n-sendSize, this->BuffSize)), this->_err);
			if (this->_err){
				return this->_err.value();
			}
			sendSize += len;
		}
		return 0;
	}

	const void SMConn::close() const {
		this->_socket.close();
		this->closeFlag = true;
	}


}
