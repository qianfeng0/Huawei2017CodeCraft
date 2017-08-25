#include "Link.h"

NetLib::Link::Link()
:	mEdge( 0,0 ),
	mBandwidth( 0 ),
	mCost( INT32_MAX ),
	mRemainCapacity( 0 )
{

}

NetLib::Link::Link( int32_t start, int32_t end, int32_t bandwith, int32_t cost )
:	mEdge( start, end),
	mBandwidth( bandwith ),
	mCost( cost ),
	mRemainCapacity( bandwith )
{

}

NetLib::Link::Link( const Link& newObj )
:	mEdge( newObj.mEdge ),
	mBandwidth( newObj.mBandwidth ),
	mCost ( newObj.mCost ),
	mRemainCapacity( newObj.mRemainCapacity )
{

}

NetLib::Link::~Link()
{

}

NetLib::Link&
NetLib::Link::operator=(const Link& newObj)
{
	if ( this != &newObj )
	{
		mEdge = newObj.mEdge;
		mBandwidth = newObj.mBandwidth;
		mCost = newObj.mCost;
		mRemainCapacity = newObj.mRemainCapacity;
	}
	return *this;
}

void
NetLib::Link::Set( int32_t start, int32_t end, int32_t bandwith, int32_t cost, int32_t cap )
{
	mEdge.mStart = start;
	mEdge.mEnd = end;
	mBandwidth = bandwith;
	mCost = cost;
	mRemainCapacity = cap;
}

void
NetLib::Link::Set( int32_t bandwith, int32_t cost, int32_t cap )
{
	mBandwidth = bandwith;
	mCost = cost;
	mRemainCapacity = cap;
}

const NetLib::stEdge&
NetLib::Link::GetEdge() const
{
	return mEdge;
}

void
NetLib::Link::SetCost( int32_t cost )
{
	mCost = cost;
}

int32_t
NetLib::Link::GetBandwidth() const
{
	return mBandwidth;
}

int32_t
NetLib::Link::GetCost() const
{
	return mCost;
}

int32_t
NetLib::Link::GetRemainCapacity() const
{
	return mRemainCapacity;
}