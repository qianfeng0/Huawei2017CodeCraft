#ifndef _ROUTE_H_
#define _ROUTE_H_

#include <vector>
#include <string>
#include "NetLibDefs.h"
//#include "Node.h"
#include "Map.h"

namespace NetLib
{
	class Route : public std::vector< int32_t >
	{
	public:
		Route();

		Route( const int32_t startNode );

		Route( const Route& newObj );

		~Route();

		Route&
		operator =( const Route& newObj );

	public:

		void
		Reset();

		void
		ReStart ( const int32_t startNode );

		void
		Advance ( const int32_t startNode);

		void
		rewind ( );

		std::string 
		ToStr() const;

	public:

		bool
		operator== ( const Route& newObj ) const;

		int32_t GetCost() const;

		//int32_t GetCapacity() const;

		int32_t GetCapacity( const Map& n_capMap ) const;

		bool IsCoincide ( const Route& n_route ) const;
		/*
		void
		Set( int32_t n_cost, int32_t n_cap );
		*/
		void
		SetCost( int32_t n_cost );

		//void
		//SetCapacity( int32_t n_cap );

	private:
		int32_t mCost;
		//int32_t mCapacity;
	};
}
#endif