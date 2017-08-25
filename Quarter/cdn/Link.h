#ifndef _LINK_H
#define _LINK_H

#include <stdint.h>
#include "NetLibDefs.h"

namespace NetLib
{
	/*
	namespace LinkTypeIDs
	{
		enum eLinkTypeID
		{
			kUnassign,
			kReal,
			kVirtual
		};
	}
	*/
	class Link
	{
	public:
		Link();

		Link( int32_t start, int32_t end, int32_t bandwith, int32_t cost );

		Link( const Link& newObj );

		~Link();

		Link&
		operator =( const Link& newObj );

	public:
		void Set( int32_t start, int32_t end, int32_t bandwith, int32_t cost, int32_t cap );
		
		void Set( int32_t bandwith, int32_t cost, int32_t cap );

		void SetCost( int32_t cost );

		const stEdge& 
		GetEdge() const;
		
		int32_t GetBandwidth() const;

		int32_t GetCost() const;

		int32_t GetRemainCapacity() const;

		//int32_t GetType() const;

	public:
		stEdge mEdge;
		int32_t mBandwidth;
		int32_t mCost;
		int32_t mRemainCapacity;

		//LinkTypeIDs::eLinkTypeID mType;
	};

}
#endif