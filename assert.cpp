#include "assert.h"

#include <execinfo.h>

void print_call_stack(std::ostream& pinfo, std::ostream& perr) {
    enum { deep = 100 };
    int nptrs;
    void* buffer[100];
    char** stackList = nullptr;
    nptrs = backtrace(buffer, deep);
    pinfo << "backtrace() returns " << nptrs << " address" << std::endl;
    stackList = backtrace_symbols(buffer, nptrs);

    if (nullptr == stackList) {
		perr << "backtrace_symbols returns nullptr" << std::endl;
		return;
    }

    for (int i = 0; i < nptrs; ++i) {
	pinfo << "\t stack<" << i << ">:  " << stackList[i] << std::endl;
    }

    free(stackList);
}
