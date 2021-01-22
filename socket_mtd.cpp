#include "socket_mtd.h"
#include "socket_itf.h"
namespace smnet{

bool isBigEndian(void){
	typedef union {
	    int i;
	    char c;
	}my_union;
    my_union u;
	u.c = 0;
    u.i = 1;
    return (u.i != u.c);
}

//is Little Edian;
bool IS_BIG_EDIAN(){
   static bool ibe	= isBigEndian();
   return ibe;
}

void netEdianChange(char *pLen, size_t len){
	if(IS_BIG_EDIAN()){return;}
	for (int i = 0, j = len-1; i < j; ++i, --j){
		std::swap(pLen[i], pLen[j]);
	}
}

template<typename Struct>
int readStruct(std::shared_ptr<Conn> c, Struct& obj){
	char	*pLen = static_cast<char*>((void*)&obj);
	int result = c->read(sizeof(Struct), pLen);
	netEdianChange(pLen, sizeof(Struct));
	return result;
}

template<typename Struct>
int writeStruct(std::shared_ptr<Conn> c, Struct len){
	char *pLen = static_cast<char*>((void*)&len);
	netEdianChange(pLen, sizeof(Struct));
	int result = c->write(sizeof(Struct), pLen);
	return result;
}

int writeLenBytes(std::shared_ptr<Conn> c, int32_t len, const char *bytes){
	int ret = writeStruct(c, static_cast<int32_t>(len));

	if (ret != ConnStatusSucc){
		return ret;
	}

	return c->write(len, bytes);
}


int readLenBytes(std::shared_ptr<Conn> c, Bytes& bytes){
	int32_t len;
	int ret = readStruct(c, len);
	if(ret != ConnStatusSucc){return ret;}
	bytes = Bytes(len);
	return c->read(len, bytes.arr);
}

int writeString(std::shared_ptr<Conn> c, std::string& str){
	auto len = str.size();
	return writeLenBytes(c, len, str.c_str());
}

int writeString(std::shared_ptr<Conn> c, std::string&& str){
	auto len = str.size();
	return writeLenBytes(c, len, str.c_str());
}

int readString(std::shared_ptr<Conn> c, std::string& str){
	int32_t len;
	int ret = readStruct(c, len);
	if (ret != ConnStatusSucc){
		return ret;
	}

	Bytes bytes(len+1);
	ret = c->read(len, bytes.arr);
	if (ret != ConnStatusSucc){
		return ret;
	}

	bytes.arr[len] = 0;
	str = bytes.arr;
	return ConnStatusSucc;
}

template<class Pb>
int readPb(std::shared_ptr<Conn> c, Pb& pb){
	int32_t len;
	int ret = readStruct(c, len);
	if (ret != ConnStatusSucc){
		return ret;
	}

	Bytes buff(len);
	ret = c->read(len, buff.arr);

	if (ret != ConnStatusSucc){
		return ret;
	}

	pb.ParseFromArray(buff.arr, len);
	return ConnStatusSucc;	
}

} // namespace smnet
