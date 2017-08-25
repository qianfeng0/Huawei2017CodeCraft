#include "Node.h"
//#include <assert.h>

NetLib::Node::Node()
:	mId( -1 ),
	mIsServer( false )
{

}

NetLib::Node::Node( int32_t id, bool nIsServer )
:	mId( id ),
	mIsServer( nIsServer )
{

}

NetLib::Node::Node( const Node& newObj )
:	mId( newObj.mId ),
	mIsServer( newObj.mIsServer )
{

}

NetLib::Node::~Node()
{

}

NetLib::Node&
NetLib::Node::operator=( const Node& newObj )
{
	if ( this != &newObj )
	{
		mId = newObj.mId;
		mIsServer = newObj.mIsServer;
	}

	return *this;
}

void
NetLib::Node::Set( int32_t id, bool nIsServer )
{
	mId = id;
	mIsServer = nIsServer;
}

void
NetLib::Node::SetAsServer()
{
	mIsServer = true;
}

void
NetLib::Node::SetAsCommon()
{
	mIsServer = false;
}
int32_t 
NetLib::Node::GetId() const
{
	return mId;
}

bool
NetLib::Node::IsServer() const
{
	return mIsServer;
}