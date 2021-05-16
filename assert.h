#ifndef ASSERT_H_XATLNPCJ
#define ASSERT_H_XATLNPCJ
#include <iostream>
#include <sstream>
#define SMSTR(str) #str 

#define __POSINFO(apd)    std::stringstream ss; \
	ss<< __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << "      "\
   	apd;	\
	throw std::runtime_error("assert error:  " + ss.str())
	
#define _assert(truth) if(!(truth)){\
	__POSINFO(<< SMSTR(truth) <<" not true." ); \
}

#define _assert2(lhs, rhs) if(lhs != rhs){ \
	__POSINFO(<< "("<<SMSTR(lhs) <<" = " << lhs << ") == (" << SMSTR(rhs) << " = " << rhs << ") not true.");\
}
#define _assert3(lhs, oper, rhs) if(!(lhs oper rhs) ){ \
	__POSINFO( << "(" << SMSTR(lhs) << " = " << lhs << ") " << SMSTR(oper) \
		<< " (" << SMSTR(rhs) << " = " << rhs << ") not true."); \
}

void print_call_stack(std::ostream& pinfo = std::cout,
		      std::ostream& perr = std::cerr);

template<class ErrorType>
inline void doThrow(const std::string& info){
	std::stringstream ss;
	print_call_stack(ss, std::cerr);
	throw ErrorType(info + ss.str());
}

#endif /* end of include guard: ASSERT_H_XATLNPCJ */
