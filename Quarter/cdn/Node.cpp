#include "Node.h"
//#include <assert.h>
#include "NetLibDefs.h"

NetLib::Node::Node()
:	mId( -1 ),
	mIsServer( false ),
	mlevel(),
	mResidueFlow(0)
{

}

NetLib::Node::Node( int32_t id, bool nIsServer, stServerLevel& nlevel)
:	mId( id ),
	mIsServer( nIsServer ),
	mlevel(nlevel),
	mResidueFlow(nlevel.outputFlow)
{

}

NetLib::Node::Node( const Node& newObj )
:	mId( newObj.mId ),
	mIsServer( newObj.mIsServer ),
	mlevel(newObj.mlevel),
	mResidueFlow(newObj.mResidueFlow)
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
		mlevel = newObj.mlevel;
		mResidueFlow = newObj.mResidueFlow;
	}

	return *this;
}

void
NetLib::Node::Set( int32_t id, bool nIsServer, const stServerLevel& nlevel)
{
	mId = id;
	mIsServer = nIsServer;
	mlevel = nlevel;
	mResidueFlow = nlevel.outputFlow;
}

void
NetLib::Node::SetAsServer(const stServerLevel& nlevel)
{
	mIsServer = true;
	mlevel = nlevel;
	mResidueFlow = nlevel.outputFlow;
}

void
NetLib::Node::SetAsCommon()
{
	mIsServer = false;
	mlevel = stServerLevel();
	mResidueFlow = 0;
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

int32_t 
NetLib::Node::GetResidueFlow() const
{
	return mResidueFlow;
}

void
NetLib::Node::UseFlow(int32_t flow)
{
	mResidueFlow -= flow;
}

void 
NetLib::Node::FreeFlow(int32_t flow)
{
	mResidueFlow += flow;
	assert(mResidueFlow <= mlevel.outputFlow);
}

void 
NetLib::Node::ResetFlow()
{
	mResidueFlow = mlevel.outputFlow;
}

const NetLib::stServerLevel&
NetLib::Node::GetServerLevel() const
{
	return mlevel;
}

int 
NetLib::Node::BestLevel(const std::vector<stServerLevel>& levelArray) const
{
	//int serverCost = mlevel.cost;
	int outFlow = mlevel.outputFlow - mResidueFlow;
	int level = levelArray.size() - 1;
	for (int j = 0; j < levelArray.size(); j++) {
		if (levelArray[j].outputFlow >= outFlow)
		{
			//serverCost = levelArray[j].cost;
			level = j;
			break;
		}
	}
	return level;
}