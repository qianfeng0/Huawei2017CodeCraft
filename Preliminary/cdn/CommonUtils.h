#ifndef _COMMONUTILS_H_
#define _COMMONUTILS_H_

#include <stdint.h>
#include <vector>

namespace CommonUtils
{
	int32_t Str2Num( char* strStart, int32_t length );

	int32_t Str2Num( const char* strStart, const char* strEnd );

	void
	LoadNumberInLine( char* strline, std::vector< int32_t >& numbers );

	void
	DeleteRepeatEle(std::vector<int32_t>& eles);
}

#endif // !_COMMONUTILS_H_
