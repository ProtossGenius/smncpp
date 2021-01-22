#ifndef SOCKET_MTD_H_J7GWOTFX
#define SOCKET_MTD_H_J7GWOTFX
#include <memory>
#include <string>
#include "socket_itf.h"

namespace smnet{


bool IS_BIG_EDIAN();

void netEdianChange(char *pLen, size_t len);

template<typename Struct>
int readStruct(std::shared_ptr<Conn> c, Struct& obj);


template<typename Struct>
int writeStruct(std::shared_ptr<Conn> c, Struct len);


//writeLenBytes  will send byte length before send bytes.
int writeLenBytes(const Conn & c, int32_t len, const char *bytes);

int readLenBytes(std::shared_ptr<Conn> c, Bytes& bytes);

int writeString(std::shared_ptr<Conn> c, std::string& str);

int writeString(std::shared_ptr<Conn> c, std::string&& str);

int readString(std::shared_ptr<Conn> c, std::string& str);

template<class Pb>
int readPb(std::shared_ptr<Conn> c, Pb& pb);

}
#endif /* end of include guard: SOCKET_MTD_H_J7GWOTFX */
