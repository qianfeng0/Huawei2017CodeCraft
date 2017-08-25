#ifndef _MAP_H_
#define _MAP_H_

#include <stdint.h>
#include <vector>

namespace NetLib
{
	class Map : public std::vector< std::vector< int32_t > >
	{
	public:
		Map();

		Map( int32_t dim );

		Map( const Map& newObj );

		~Map();
		
		Map&
		operator =( const Map& newObj );
		
		void
		ReDim( int32_t dim );
	};
}

#endif