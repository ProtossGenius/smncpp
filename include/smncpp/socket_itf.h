#ifndef SOCKET_ITF_H_BY5UU1X9
#define SOCKET_ITF_H_BY5UU1X9

#include <string>
#include <atomic>
namespace smnet{

class Bytes {
public:
	Bytes():arr(nullptr), _cnt(nullptr), _len(0){}
	Bytes(size_t len):arr(new char[len]), _cnt(new std::atomic_int(1)), _len(len){}
	Bytes(size_t len, char* arr):arr(arr), _cnt(nullptr), _len(len){}
	Bytes(const Bytes& bytes):arr(bytes.arr), _cnt(bytes._cnt), _len(bytes._len){if(nullptr != this->_cnt){	++(*this->_cnt);}	}
	Bytes(Bytes&& bytes):arr(bytes.arr), _cnt(bytes._cnt), _len(bytes._len){bytes.arr = nullptr; bytes._cnt = nullptr;}
	~Bytes();
	Bytes& operator= (const Bytes& bytes);
	Bytes& operator= (Bytes&& bytes);
	void swap(Bytes& rhs);
	size_t size(){return _len;}
	const size_t size()const {return _len;}
public:
	char* arr;
private:
	std::atomic_int* _cnt;
	size_t _len;
};


enum{ConnStatusSucc = 0};

class Conn{
public:
	virtual ~Conn() {} 
	//read @return error_code.
	virtual const int read(size_t n, char * bytes) const = 0;
	virtual const int read(Bytes data) const = 0;
	virtual const int write(size_t n, const char *bytes) const = 0;
	virtual const int write(Bytes data) const = 0;
	virtual const void close() const = 0;
	virtual const std::exception lastError()const =0;
};


}//namespace smnet


#endif /* end of include guard: SOCKET_ITF_H_BY5UU1X9 */
