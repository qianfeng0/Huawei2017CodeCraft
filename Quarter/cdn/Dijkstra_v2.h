#ifndef _DIJLKSTRA_V2_H_
#define _DIJLKSTRA_V2_H_
#include <stdint.h>
#include <vector>

#include "NetLibDefs.h"
#include "Map.h"
#include "Route.h"
#include "ListGraph.h"
#include "Node.h"

namespace NetLib
{
	namespace HeapDijkstra 
	{
		//bool DijkstraOnePointOfSet(const int32_t startNode, const NodeArray& targetNodes, const NetLib::Map& costMap, const NetLib::Map& remainCapMap, NetLib::Route& outRoute, const int32_t maxCost = INT32_MAX);
		bool DijkstraOnePointOfSet(const int32_t startNode, const NodeArray& targetNodes, const NetLib::ListGraph& listGraph, const NetLib::Map& remainCapMap, NetLib::Route& outRoute, const int32_t maxCost = INT32_MAX);

		bool DijkstraOnePointOfSet(const int32_t startNode, const std::vector< bool >& targetNodes, const NetLib::Map& costMap, NetLib::Route& outRoute);

		void BuildRount(const int32_t objVertex, const std::vector<int32_t>& preVertex, NetLib::Route& n_route);

		//std::vector<bool> sIsInS;
		//std::vector< int32_t > sDistance;
		//std::vector< int32_t > sPrevVertex;
	}
}


#endif // _DIJLKSTRA_V2_H_
