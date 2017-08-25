#include "Flow.h"

NetLib::Flow::Flow()
:	Route(),
	mFlowValue(0),
	mConsumptionID(-1)
{

}

NetLib::Flow::Flow( const Route& new_route, int32_t new_flowValue, int32_t new_consumptionID )
:	Route( new_route ),
	mFlowValue( new_flowValue ),
	mConsumptionID( new_consumptionID )
{

}

NetLib::Flow::Flow( const Flow& newObj )
:	Route( newObj ),
	mFlowValue( newObj.mFlowValue ),
	mConsumptionID( newObj.mConsumptionID )
{

}

NetLib::Flow::~Flow()
{

}

NetLib::Flow&
NetLib::Flow::operator =( const Flow& newObj )
{
	if ( this != &newObj )
	{
		Route::operator=( newObj );
		mFlowValue = newObj.mFlowValue;
		mConsumptionID = newObj.mConsumptionID;
	}
	return *this;
}

void
NetLib::Flow::SetFlowValue(int32_t new_flow )
{
	mFlowValue = new_flow;
}

void
NetLib::Flow::SetConsumptionNodeID( const int32_t id )
{
	mConsumptionID = id;
}

int32_t
NetLib::Flow::GetFlowValue() const
{
	return mFlowValue;
}

int32_t
NetLib::Flow::GetConsumptionNodeID() const
{
	return mConsumptionID;
}