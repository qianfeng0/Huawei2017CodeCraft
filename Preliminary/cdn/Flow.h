#ifndef _FLOW_H_
#define _FLOW_H_

#include <stdint.h>
#include <string>

#include "Route.h"

namespace NetLib
{
	class Flow : public Route
	{
	public:

		Flow();

		Flow( const Route& new_route, int32_t new_flowValue, int32_t new_consumptionID );

		Flow( const Flow& newObj );

		~Flow();

		Flow&
		operator =( const Flow& newObj );

	public:
		
		void 
		SetFlowValue( int32_t new_flow);

		void
		SetConsumptionNodeID( const int32_t id );

		int32_t
		GetFlowValue() const;

		int32_t
		GetConsumptionNodeID() const;


	private:
		int32_t mFlowValue;
		int32_t mConsumptionID;
	};
}

#endif