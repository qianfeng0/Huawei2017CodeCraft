#ifndef _NETLIBDEFS_H_
#define _NETLIBDEFS_H_

#include <stdint.h>
#include <vector>

#define  NDEBUG
#include <assert.h>

#define ELAPSED_TIME_CHECK
//#define DEBUG_PRINTF
//#define PRINTF_LOG
#define THRESHOLD_NUMBER_OF_NODES		500
#define THRESHOLD_COST_OF_SERVER		700
namespace NetLib
{
	struct stEdge  
	{
		int32_t mStart;
		int32_t mEnd;

		stEdge()
		:	mStart(0),
			mEnd(0)
		{

		}

		stEdge( int32_t nstart, int32_t nend)
		:	mStart( nstart ),
			mEnd( nend )
		{

		}

		stEdge( const stEdge& newObj )
		:	mStart( newObj.mStart ),
			mEnd( newObj.mEnd )
		{

		}
	};

	class Link;
	typedef std::vector< std::vector< Link > >		LinkMap;

	class Flow;
	typedef std::vector< Flow >						FlowArray;

	class Node;
	typedef std::vector< Node >						NodeArray;
}


#endif // !_NETLIBDEFS_H_
