#ifndef _DIJKSTRA_H_
#define _DIJKSTRA_H_

#include <vector>
#include "NetLibDefs.h"
#include "Map.h"
#include "Link.h"
#include "Route.h"

#if (1)
namespace NetLib
{

	class Dijkstra
	{
	public:
		Dijkstra();

		Dijkstra( const Map* costMap, const int32_t startVertex );

		Dijkstra( const Dijkstra& newObj );

		~Dijkstra();

		Dijkstra&
		operator= ( const Dijkstra& newObj );

	public:
		void Init( const Map* costMap, const int32_t startVertex );

		void GetMinCost( const int32_t termialVertex, int32_t minCost);

		void FindAllVertex();

		bool
		FindClosestServer( const Map& remainCapMap, const NodeArray& nodeArray, Route& route, const int32_t maxCost = INT32_MAX);

		bool
		FindClosestServer( const Map& remainCapMap, const NodeArray& nodeArray, const std::vector<Route> maskRoute, Route& route);
	private:


		void Reset();

		void ResetWithCap( const Map& remainCapMap );
		void BuildRount( const int32_t objVertex, Route& n_route );

		//void BuildRount( const int32_t objVertex, const Map& remainCapMap, Route& n_route );

		const Map* mCostMap;
		int32_t mStartVertex;
		int32_t mNumOfVertex;
		std::vector< int32_t > mDistance;
		std::vector< int32_t > mPrevVertex;
		std::vector< bool > mIsInS;
		int32_t mCurrentVertex;
	};

	bool DijkstraOnePointOfSet(const int32_t startNode, const std::vector< bool >& targetNodes, const NetLib::Map& costMap, NetLib::Route& outRoute);

	bool DijkstraOnePointOfSetWithCap(const int32_t startNode, const std::vector< bool >& targetNodes, const NetLib::Map& costMap, const NetLib::Map& capMap, NetLib::Route& outRoute);
	void BuildRount(int32_t objVertex, std::vector<int32_t> preVertex, NetLib::Route& n_route);
}
#endif

#if (0)
void Dijkstrafun( const NetLib::LinkMatrix& linkMap, const int32_t startVertex, std::vector<int32_t>& distance, std::vector<int32_t>& prevVertex )
{
	int32_t numOfVertex = linkMap.size();

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//init
	std::vector<bool> isIns;
	isIns.assign( numOfVertex, false );

	distance.resize( numOfVertex );
	prevVertex.resize( numOfVertex );

	for ( int32_t i = 0; i < numOfVertex; i++ )
	{
		distance[i] = linkMap[startVertex][i].GetCost();
		if ( linkMap[startVertex][i].GetCost() < INT32_MAX )
			prevVertex[i] = startVertex;
		else
			prevVertex[i] =  -1;
	}
	prevVertex[startVertex] = -1;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	isIns[startVertex] = true;

	int u = startVertex;

	//循环从1开始，因为开始节点已经放在S中，还有numOfVertex - 1 个节点要处理
	for ( int32_t i = 1; i < numOfVertex; i++ )
	{
		//选择distance最小的一个节点
		int32_t nextVertex = -1;
		int32_t tempDistance = INT32_MAX;
		for ( int32_t j = 0; j < numOfVertex; j++ )
		{
			if ( (isIns[j] == false) && (distance[j] < tempDistance) )
			{
				nextVertex = j;
				tempDistance = distance[j];
			}
		}

		//放入S集合
		isIns[nextVertex] = true;
		//下次寻找的开始节点
		u = nextVertex;

		//更新distance
		for ( int32_t j = 0; j < numOfVertex; j++ )
		{
			if ( isIns[j] == false && linkMap[u][j].GetCost() < INT32_MAX )
			{
				int32_t temp = distance[u] + linkMap[u][j].GetCost();
				if ( temp < distance[j] )
				{
					distance[j] = temp;
					prevVertex[j] = u;
				}
			}
		}
	}
}
#endif

#endif