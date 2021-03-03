#include "assert.h"

#include <execinfo.h>
#include <iostream>
#include <ostream>

void print_trace(std::ostream& pinfo = std::cout, std::ostream& perr = std::cerr) {
    enum { deep = 100 };
    int nptrs;
    void* buffer[100];
    char** stackList = nullptr;
    nptrs = backtrace(buffer, deep);
    pinfo << "backtrace() returns " << nptrs << "address" << std::endl;
    stackList = (char**)backtrace_symbols(buffer, nptrs);

    if (nullptr == stackList) {
	perr << "backtrace_symbols returns nullptr" << std::endl;
	return;
    }
	for(int i = 0; i < nptrs; ++i){
		pinfo << stackList[nptrs] << std::endl;
	}
}
