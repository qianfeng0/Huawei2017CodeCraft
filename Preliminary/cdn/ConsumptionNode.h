#ifndef _CONSUMPTIONNODE_H_
#define _CONSUMPTIONNODE_H_

#include <stdint.h>

#include "NetLibDefs.h"
#include "Flow.h"
#include "Dijkstra.h"
namespace NetLib
{
	class ConsumptionNode
	{
	public:

		ConsumptionNode();

		ConsumptionNode( int32_t id, int32_t connetctId, int32_t flow );
		
		ConsumptionNode( const ConsumptionNode& newObj );

		~ConsumptionNode();

		ConsumptionNode&
		operator =( const ConsumptionNode& newObj );

	public:

		void Init( int32_t id, int32_t connetctId, int32_t flow, const Map* costMap );

		int32_t GetId();

		int32_t GetConnectNodeId();

		int32_t GetFlowRequirement();

		const std::vector< Flow >&
		GetFlowArray() const;

		int32_t GetUnassignFlow() const;
		int32_t GetCurrentCost() const;

		void AddFlow( const Flow& newFlow);

		void DelteFlow(const Flow newFlow);

		void ClearFlows();

	private:

		

		int32_t mId;
		int32_t mConnectNodeId;
		int32_t mFlowRequirement;

		FlowArray mFlowArray;
	};
}

#endif