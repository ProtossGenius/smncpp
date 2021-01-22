#ifndef MUTI_SERVICE_H_KVQZ5HCL
#define MUTI_SERVICE_H_KVQZ5HCL

#include "asio_server.h"
#include "channel.h"
#include <string>
#include <boost/asio.hpp>
#include <map>

namespace smnet{

	class Bytes;
	//Service means it always read a [size-array].
	class Service: public Session{
		public:
		Service(boost::asio::ip::tcp::socket sock): Session(std::move(sock)){}
		protected:
			virtual void _run(const Bytes& buff) = 0;
		private:
			void run() override;
		private:
			int32_t readLen;
			char *pReadLen;
	};

	class MutiService: public Service{

		private:
			void _run(const Bytes& buff) override;
		private:
			channel<std::string> _sonsMsg; //accept msg from it's sons.
	};
}

#endif /* end of include guard: MUTI_SERVICE_H_KVQZ5HCL */
