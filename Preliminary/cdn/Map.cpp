#include "Map.h"

NetLib::Map::Map()
:	std::vector< std::vector< int32_t > >()
{

}

NetLib::Map::Map( int32_t dim )
:	std::vector< std::vector< int32_t > >()
{
	this->resize(dim);
	for ( int32_t i = 0; i < dim; i++ )
	{
		this->at(i).assign( dim, 0 );
	}
}

NetLib::Map::Map( const Map& newObj )
:	std::vector< std::vector< int32_t > >( newObj )
{

}

NetLib::Map::~Map()
{

}

NetLib::Map&
NetLib::Map::operator=( const Map& newObj )
{
	if ( this != &newObj )
	{
		std::vector< std::vector< int32_t > >::operator=(newObj);
	}
	return *this;
}

void
NetLib::Map::ReDim( int32_t dim )
{
	this->resize(dim);
	for ( int32_t i = 0; i < dim; i++ )
	{
		this->at(i).assign( dim, 0 );
	}
}