#include "socket_itf.h"

namespace smnet{
	
	Bytes::	~Bytes(){
		if (this->_cnt == nullptr){return;}
		--(*this->_cnt);
		if ((*this->_cnt) == 0){
			delete this->_cnt;
			delete[] this->arr;
		}
	}

	Bytes& Bytes::operator= (const Bytes& bytes){
		Bytes tmp(bytes);
		this->swap(tmp);//get bytes's copy and swap and drop old data..
		return *this;
	}

	Bytes& Bytes::operator= (Bytes&& bytes){
		if (this == &bytes){return *this;}
		Bytes zero;
		this->swap(bytes);//get bytes's value.
		bytes.swap(zero);//drop this->old data; make right-value safe.
		return *this;
	}

	void Bytes::swap(Bytes& rhs){
		std::swap(rhs.arr, this->arr);
		std::swap(rhs._cnt, this->_cnt);
		std::swap(rhs._len, this->_len);
	}


}
