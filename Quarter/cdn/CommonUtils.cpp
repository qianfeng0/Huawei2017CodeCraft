#include "CommonUtils.h"
//#include <assert.h>

int32_t
CommonUtils::Str2Num( char* strStart, int32_t length )
{
	int32_t result = 0;

	for ( int32_t i = 0; i < length; i++ )
	{
		if ( *(strStart + i) < '0' || *(strStart + i) > '9')
		{
			continue;
		}
		result *= 10;
		result += *(strStart + i) - '0';
	}
	return result;
}

int32_t
CommonUtils::Str2Num( const char* strStart, const char* strEnd )
{
	int32_t result = 0;
	while ( strStart < strEnd )
	{
		
		if ( *(strStart) < '0' || *(strStart) > '9')
		{
			continue;
		}
		
		result *= 10;
		result += *(strStart) - '0';
		strStart++;
	}
	return result;
}

int32_t
CommonUtils::LoadNumberInLine( char* strline, std::vector< int32_t >& numbers)
{
	numbers.clear();

	char* current_ptr = nullptr;
	char* numStart_ptr = nullptr;
	current_ptr = strline;

	bool foundNumber = false;
	while ( *current_ptr != '\0' )
	{
		if ( !foundNumber )
		{
			if ( *current_ptr >= '0' && *current_ptr <= '9' )
			{
				numStart_ptr = current_ptr;
				foundNumber = true;
			}
		}
		else
		{
			if ( *current_ptr < '0' || *current_ptr > '9' )
			{
				numbers.push_back( CommonUtils::Str2Num(numStart_ptr, current_ptr) );
				foundNumber = false;
			}
		}
		current_ptr++;
	}

	if (foundNumber)
	{
		numbers.push_back( CommonUtils::Str2Num(numStart_ptr, current_ptr) );
		foundNumber = false;
	}
	return numbers.size();
}

void
CommonUtils::DeleteRepeatEle(std::vector<int32_t>& eles)
{
	int32_t length = eles.size();
	int32_t diffEleLength = 0;
	int32_t i, j;
	for (i = 0; i < length; i++)
	{
		//check repeat
		for (j = 0; j < diffEleLength; j++){
			if (eles[i] == eles[j])
				break;
		}
		if (j == diffEleLength){
			eles[diffEleLength++] = eles[i];
		}
		
	}
	eles.resize(diffEleLength);
}