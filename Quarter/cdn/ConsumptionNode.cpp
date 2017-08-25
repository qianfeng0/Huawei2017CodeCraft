
//#include <assert.h>

#include "ConsumptionNode.h"

NetLib::ConsumptionNode::ConsumptionNode()
:	mId( -1 ),
	mConnectNodeId( -1 ),
	mFlowRequirement( 0 ),
	mFlowArray()
{

}

NetLib::ConsumptionNode::ConsumptionNode( int32_t id, int32_t connetctId, int32_t flow )
:	mId ( id ),
	mConnectNodeId( connetctId ),
	mFlowRequirement( flow ),
	mFlowArray()
{

}

NetLib::ConsumptionNode::ConsumptionNode( const ConsumptionNode& newObj )
:	mId( newObj.mId ),
	mConnectNodeId ( newObj.mConnectNodeId ),
	mFlowRequirement( newObj.mFlowRequirement ),
	mFlowArray( newObj.mFlowArray )
{

}

NetLib::ConsumptionNode::~ConsumptionNode()
{

}


NetLib::ConsumptionNode&
NetLib::ConsumptionNode::operator=( const ConsumptionNode& newObj )
{
	if ( this != &newObj )
	{
		mId = newObj.mId;
		mConnectNodeId = newObj.mConnectNodeId;
		mFlowRequirement = newObj.mFlowRequirement;
		mFlowArray = newObj.mFlowArray;
	}
	return *this;
}

void
NetLib::ConsumptionNode::Init( int32_t id, int32_t connetctId, int32_t flow, const Map* costMap )
{
	mId = id;
	mConnectNodeId = connetctId;
	mFlowRequirement = flow;
}

int32_t
NetLib::ConsumptionNode::GetId()
{
	return mId;
}

int32_t
NetLib::ConsumptionNode::GetConnectNodeId()
{
	return mConnectNodeId;
}

int32_t
NetLib::ConsumptionNode::GetFlowRequirement()
{
	int32_t require = mFlowRequirement;
	for ( int32_t i = mFlowArray.size() - 1; i >= 0; i-- )
	{
		require -= mFlowArray[i].GetFlowValue();
	}
	assert(require >= 0);
	return require;
}


int32_t
NetLib::ConsumptionNode::GetUnassignFlow() const
{
	int32_t unassign_flow = mFlowRequirement;
	for ( int32_t i = mFlowArray.size() - 1; i >= 0; i-- )
	{
		unassign_flow -= mFlowArray[i].GetFlowValue();
	}
	assert( unassign_flow >= 0 );
	return unassign_flow;
}

int32_t
NetLib::ConsumptionNode::GetCurrentCost() const
{
	int32_t cost = 0;
	for (int32_t i = mFlowArray.size() - 1; i >= 0; i--)
	{
		cost += mFlowArray[i].GetCost() * mFlowArray[i].GetFlowValue();
	}
	return cost;
}

const std::vector< NetLib::Flow >&
NetLib::ConsumptionNode::GetFlowArray() const
{
	return mFlowArray;
}

void
NetLib::ConsumptionNode::AddFlow( const Flow& newFlow)
{
	for ( int32_t i = mFlowArray.size() - 1; i >= 0; i-- )
	{
		if ( mFlowArray[i] == newFlow )
		{
			int32_t flowValue = mFlowArray[i].GetFlowValue();
			flowValue += newFlow.GetFlowValue();
			assert(flowValue <= mFlowRequirement);
			mFlowArray[i].SetFlowValue(flowValue);

			return;
		}
	}
	mFlowArray.push_back( newFlow );
}

void
NetLib::ConsumptionNode::DelteFlow( const Flow newFlow )
{
	for (int32_t i = mFlowArray.size() - 1; i >= 0; i--)
	{
		if (mFlowArray[i] == newFlow)
		{
			int32_t flowValue = mFlowArray[i].GetFlowValue();
			flowValue -= newFlow.GetFlowValue();

			assert(flowValue >= 0);

			if (flowValue > 0) {
				mFlowArray[i].SetFlowValue(flowValue);
				return;
			}
			else if(flowValue == 0){
				mFlowArray.erase(mFlowArray.begin() + i);
				return;
			}
		}
	}
	assert(false);
}

void
NetLib::ConsumptionNode::ClearFlows()
{
	mFlowArray.clear();
}