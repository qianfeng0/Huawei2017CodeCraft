#ifndef _NODE_H_
#define _NODE_H_

#include <stdint.h>
#include <vector>
//#include "NetLibDefs.h"

namespace NetLib
{
	struct stServerLevel {
		int outputFlow;
		int cost;

		stServerLevel() : outputFlow(0), cost(0) {};
		stServerLevel(int flow, int c) : outputFlow(flow), cost(c) {};
	};

	class Node
	{
	public:
		Node();

		Node( int32_t id, bool nIsServer, stServerLevel& nlevel);

		Node( const Node& newObj);

		~Node();

		Node&
		operator =(const Node& newObj);

	public:

		void Set( int32_t id, bool nIsServer, const stServerLevel& nlevel );

		void SetAsServer(const stServerLevel& nlevel);

		void SetAsCommon();

		int32_t GetId() const;

		bool IsServer() const;

		int32_t GetResidueFlow() const;

		const stServerLevel& GetServerLevel() const;
		//NodeTypeIDs::eNodeTypeID GetType();
		void UseFlow(int32_t flow);
		void FreeFlow(int32_t flow);
		void ResetFlow();
		int BestLevel(const std::vector<stServerLevel>& levelArray) const;
	private:
		int32_t mId;
		stServerLevel mlevel;
		int32_t mResidueFlow;
		bool mIsServer;

	};
}

#endif