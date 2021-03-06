
//#include <assert.h>
#include "Route.h"


NetLib::Route::Route()
:	std::vector< Node >(),
	mCost(INT32_MAX)
	//mCapacity(INT32_MAX)
{
	this->reserve(5);
}

NetLib::Route::Route( const Node startNode )
:	std::vector< Node >(),
	mCost(0)
	//mCapacity(INT32_MAX)
{
	this->push_back( startNode );
}

NetLib::Route::Route( const Route& newObj )
:	std::vector< Node >( newObj ),
	mCost(newObj.mCost)
	//mCapacity(newObj.mCapacity)
{

}

NetLib::Route::~Route()
{

}

NetLib::Route&
NetLib::Route::operator=( const Route& newObj )
{
	if ( this != &newObj )
	{
		std::vector< Node >::operator=( newObj );
		mCost = newObj.mCost;
		//mCapacity = newObj.mCapacity;
	}
	return *this;
}


bool
NetLib::Route::operator== ( const Route& newObj ) const
{
	if ( this == &newObj )
		return true;

	if ( std::vector< Node >::size() != newObj.size() )
		return false;

	for ( int32_t i = std::vector< Node >::size() - 1; i >= 0; i--)
	{
		if ( std::vector< Node >::at(i).GetId() != newObj.at(i).GetId() )
		{
			return false;
		}
	}
	return true;
}

void
NetLib::Route::Reset()
{
	std::vector< Node >::clear();
	mCost = INT32_MAX;
}
void
NetLib::Route::ReStart( const Node startNode )
{
	this->clear();
	this->push_back( startNode );
}

void
NetLib::Route::Advance( const Node advanceNode )
{
	this->push_back( advanceNode );
}

void
NetLib::Route::rewind( )
{
	this->pop_back();
}

std::string
NetLib::Route::ToStr() const
{
	std::string str;
	int32_t routeLenght = this->size();
	for ( int32_t i = 0; i < routeLenght; i++ )
	{
		str += std::to_string( this->at(i).GetId() ) + " ";
	}
	return str;
}

int32_t
NetLib::Route::GetCost() const
{
	return mCost;
}

#if(0)
int32_t
NetLib::Route::GetCapacity() const
{
	return mCapacity;
}
#endif

int32_t
NetLib::Route::GetCapacity( const Map& n_capMap ) const
{
	if ( this->size() == 0 )
	{
		return 0;
	}
	int32_t cap = INT32_MAX;
	int32_t size = this->size();
	int32_t edge_start = 0;
	int32_t edge_end = 0;
	for ( int32_t i = 0; i < size - 1; i++ )
	{
		edge_start = this->at(i).GetId();
		edge_end = this->at(i + 1).GetId();

		if ( n_capMap[edge_start][edge_end] < cap )
		{
			cap = n_capMap[edge_start][edge_end];
		}
	}
	return cap;
}
/*
void
NetLib::Route::Set( int32_t n_cost, int32_t n_cap )
{
	mCost = n_cost;
	mCapacity =n_cap;
}
*/
void
NetLib::Route::SetCost( int32_t n_cost )
{
	mCost = n_cost;
}
/*
void
NetLib::Route::SetCapacity( int32_t n_cap )
{
	mCapacity = n_cap;
}
*/

bool
NetLib::Route::IsCoincide ( const Route& n_route ) const
{
	if ( this->size() < 2 )
	{
		return false;
	}
	assert( n_route.size() >= 2 );
	//assert( this->size() >= 2);
	int32_t n_route_size = n_route.size();
	int32_t this_route_size = this->size();

	for ( int32_t i = 0; i < n_route_size - 1; i++ )
	{
		int32_t start = n_route[i].GetId();
		int32_t end = n_route[i+1].GetId();
		for ( int32_t j = 0; j < this_route_size - 1; j++ )
		{
			if ( this->at(j).GetId() == start )
			{
				if ( this->at(j+1).GetId() == end )
				{
					return true;
				}
			}
		}
	}
	return false;
}