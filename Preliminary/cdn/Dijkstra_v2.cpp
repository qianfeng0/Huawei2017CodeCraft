#include "Dijkstra_v2.h"


static std::vector<bool> sIsInS;
static std::vector< int32_t > sDistance;
static std::vector< int32_t > sPrevVertex;


void
NetLib::HeapDijkstra::BuildRount(const int32_t objVertex, const std::vector<int32_t>& preVertex, NetLib::Route& n_route)
{
	n_route.ReStart(Node(objVertex, true));

	int32_t curVertex = objVertex;
	while (preVertex[curVertex] != -1)
	{
		curVertex = preVertex[curVertex];
		n_route.Advance((Node(curVertex, false)));
	}

	//assert((*(n_route.end() - 1)).GetId() == mStartVertex);
}

bool 
NetLib::HeapDijkstra::DijkstraOnePointOfSet(const int32_t startNode, const NodeArray& targetNodes, const NetLib::Map& costMap, const NetLib::Map& remainCapMap, NetLib::Route& outRoute, const int32_t maxCost)
{
	//static std::vector<bool> sIsInS;
	//static std::vector< int32_t > sDistance;
	//static std::vector< int32_t > sPrevVertex;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//init
	outRoute.Reset();

	int32_t numOfNodes = costMap.size();
	int32_t currentVertex = -1;

	sIsInS.assign(numOfNodes, false);
	sDistance.resize(numOfNodes);
	sPrevVertex.resize(numOfNodes);
	//sDistance.assign(numOfNodes, INT32_MAX);
	//sPrevVertex.assign(numOfNodes, -1);

	for (int32_t i = 0; i < numOfNodes; i++)
	{
		if (remainCapMap[i][startNode] > 0)
			sDistance[i] = costMap[startNode][i];
		else
			sDistance[i] = INT32_MAX;

		if (sDistance[i] < INT32_MAX)
			sPrevVertex[i] = startNode;
		else
			sPrevVertex[i] = -1;
	}

	sPrevVertex[startNode] = -1;
	sIsInS[startNode] = true;
	currentVertex = startNode;

	//init
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//the start node is target

	if (targetNodes[currentVertex].IsServer())
	{
		BuildRount(currentVertex, sPrevVertex, outRoute);
		outRoute.SetCost(sDistance[currentVertex]);
		return true;
	}

	//循环从1开始，因为开始节点已经放在S中，还有numOfVertex - 1 个节点要处理
	for (int32_t i = 1; i < numOfNodes; i++)
	{
		//select min distance not in S
		int32_t nextVertex = -1;
		int32_t tempDistance = INT32_MAX;
		for (int32_t j = 0; j < numOfNodes; j++)
		{
			if (sIsInS[j] == false && sDistance[j] < tempDistance)
			{
				nextVertex = j;
				tempDistance = sDistance[j];
			}
		}

		// not found, and return
		if (nextVertex == -1 || sDistance[currentVertex] >= maxCost)
		{
			if (outRoute.GetCost() < INT32_MAX)
				return true;
			else
				return false;
		}

		//add in S
		sIsInS[nextVertex] = true;
		//the next start node
		currentVertex = nextVertex;

		//break, if loc_Route min cost and max cap
		if (sDistance[currentVertex] > outRoute.GetCost())
		{
			return true;
		}

		//is found the target node
		if (targetNodes[currentVertex].IsServer() && sDistance[currentVertex] <= outRoute.GetCost())
		{
			Route temp_Route;
			temp_Route.SetCost(sDistance[currentVertex]);
			BuildRount(currentVertex, sPrevVertex, temp_Route);
			if (temp_Route.GetCapacity(remainCapMap) > outRoute.GetCapacity(remainCapMap))
			{
				outRoute = temp_Route;
			}
		}

		//update distance
		for (int32_t j = 0; j < numOfNodes; j++)
		{
			if (sIsInS[j] == false &&
				costMap[currentVertex][j] < INT32_MAX &&
				remainCapMap[j][currentVertex] > 0)
			{
				int32_t temp = sDistance[currentVertex] + costMap[currentVertex][j];
				if (temp < sDistance[j])
				{
					sDistance[j] = temp;
					sPrevVertex[j] = currentVertex;
				}
#if (1)
				//add if current distance == record distance, select the bigger capacity
				else if (temp == sDistance[j])
				{
					int32_t temp_cap = remainCapMap[j][currentVertex];
					int32_t temp_vertex = currentVertex;
					while (sPrevVertex[temp_vertex] != -1)
					{
						if (remainCapMap[temp_vertex][sPrevVertex[temp_vertex]] < temp_cap)
						{
							temp_cap = remainCapMap[temp_vertex][sPrevVertex[temp_vertex]];
						}
						temp_vertex = sPrevVertex[temp_vertex];
					}

					int32_t cur_cap = remainCapMap[j][sPrevVertex[j]];
					temp_vertex = j;
					while (sPrevVertex[temp_vertex] != -1)
					{
						if (remainCapMap[temp_vertex][sPrevVertex[temp_vertex]] < cur_cap)
						{
							cur_cap = remainCapMap[temp_vertex][sPrevVertex[temp_vertex]];
						}
						temp_vertex = sPrevVertex[temp_vertex];
					}

					if (temp_cap > cur_cap)
					{
						sDistance[j] = temp;
						sPrevVertex[j] = currentVertex;
					}
				}
#endif
			}
		}
		// end: update distance
	}

	return false;
}

bool
NetLib::HeapDijkstra::DijkstraOnePointOfSet(const int32_t startNode, const NodeArray& targetNodes, const NetLib::ListGraph& listGraph, const NetLib::Map& remainCapMap, NetLib::Route& outRoute, const int32_t maxCost)
{
	//static std::vector<bool> sIsInS;
	//static std::vector< int32_t > sDistance;
	//static std::vector< int32_t > sPrevVertex;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//init
	outRoute.Reset();

	int32_t numOfNodes = listGraph.size();
	int32_t currentVertex = -1;

	sIsInS.assign(numOfNodes, false);

#if (0)
	sDistance.resize(numOfNodes);
	sPrevVertex.resize(numOfNodes);
	for (int32_t i = 0; i < numOfNodes; i++)
	{
		if (remainCapMap[i][startNode] > 0)
			sDistance[i] = costMap[startNode][i];
		else
			sDistance[i] = INT32_MAX;

		if (sDistance[i] < INT32_MAX)
			sPrevVertex[i] = startNode;
		else
			sPrevVertex[i] = -1;
	}
#endif
	sDistance.assign(numOfNodes, INT32_MAX);
	sPrevVertex.assign(numOfNodes, -1);

	for (int32_t i = listGraph[startNode].size() - 1; i >= 0; i--)
	{
		if (remainCapMap[listGraph[startNode][i].GetId()][startNode] > 0)
		{
			sDistance[listGraph[startNode][i].GetId()] = listGraph[startNode][i].GetCost();
			sPrevVertex[listGraph[startNode][i].GetId()] = startNode;
		}
	}
	sDistance[startNode] = 0;
	sPrevVertex[startNode] = -1;
	sIsInS[startNode] = true;
	currentVertex = startNode;

	//init
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//the start node is target

	if (targetNodes[currentVertex].IsServer())
	{
		BuildRount(currentVertex, sPrevVertex, outRoute);
		outRoute.SetCost(sDistance[currentVertex]);
		return true;
	}

	//循环从1开始，因为开始节点已经放在S中，还有numOfVertex - 1 个节点要处理
	for (int32_t i = 1; i < numOfNodes; i++)
	{
		//select min distance not in S
		int32_t nextVertex = -1;
		int32_t tempDistance = INT32_MAX;
		for (int32_t j = 0; j < numOfNodes; j++)
		{
			if (sIsInS[j] == false && sDistance[j] < tempDistance)
			{
				nextVertex = j;
				tempDistance = sDistance[j];
			}
		}

		// not found, and return
		if (nextVertex == -1 || sDistance[currentVertex] >= maxCost)
		{
			if (outRoute.GetCost() < INT32_MAX)
				return true;
			else
				return false;
		}

		//add in S
		sIsInS[nextVertex] = true;
		//the next start node
		currentVertex = nextVertex;

		//break, if loc_Route min cost and max cap
		if (sDistance[currentVertex] > outRoute.GetCost())
		{
			return true;
		}

		//is found the target node
		if (targetNodes[currentVertex].IsServer() && sDistance[currentVertex] <= outRoute.GetCost())
		{
			Route temp_Route;
			temp_Route.SetCost(sDistance[currentVertex]);
			BuildRount(currentVertex, sPrevVertex, temp_Route);
			if (temp_Route.GetCapacity(remainCapMap) > outRoute.GetCapacity(remainCapMap))
			{
				outRoute = temp_Route;
			}
		}

		for (int32_t j = listGraph[currentVertex].size() - 1; j >= 0; j--)
		{
			int updateId = listGraph[currentVertex][j].GetId();
			if (sIsInS[updateId] == false && remainCapMap[updateId][currentVertex] > 0)
			{
				int32_t temp = sDistance[currentVertex] + listGraph[currentVertex][j].GetCost();
				if (temp < sDistance[updateId])
				{
					sDistance[updateId] = temp;
					sPrevVertex[updateId] = currentVertex;
				}
#if (1)
				//add if current distance == record distance, select the bigger capacity
				else if (temp == sDistance[updateId])
				{
					int32_t temp_cap = remainCapMap[updateId][currentVertex];
					int32_t temp_vertex = currentVertex;
					while (sPrevVertex[temp_vertex] != -1)
					{
						if (remainCapMap[temp_vertex][sPrevVertex[temp_vertex]] < temp_cap)
						{
							temp_cap = remainCapMap[temp_vertex][sPrevVertex[temp_vertex]];
						}
						temp_vertex = sPrevVertex[temp_vertex];
					}

					int32_t cur_cap = remainCapMap[updateId][sPrevVertex[updateId]];
					temp_vertex = updateId;
					while (sPrevVertex[temp_vertex] != -1)
					{
						if (remainCapMap[temp_vertex][sPrevVertex[temp_vertex]] < cur_cap)
						{
							cur_cap = remainCapMap[temp_vertex][sPrevVertex[temp_vertex]];
						}
						temp_vertex = sPrevVertex[temp_vertex];
					}

					if (temp_cap > cur_cap)
					{
						sDistance[updateId] = temp;
						sPrevVertex[updateId] = currentVertex;
					}
				}
#endif
			}
		}
#if (0)
		//update distance
		for (int32_t j = 0; j < numOfNodes; j++)
		{
			if (sIsInS[j] == false &&
				costMap[currentVertex][j] < INT32_MAX &&
				remainCapMap[j][currentVertex] > 0)
			{
				int32_t temp = sDistance[currentVertex] + costMap[currentVertex][j];
				if (temp < sDistance[j])
				{
					sDistance[j] = temp;
					sPrevVertex[j] = currentVertex;
				}
#if (1)
				//add if current distance == record distance, select the bigger capacity
				else if (temp == sDistance[j])
				{
					int32_t temp_cap = remainCapMap[j][currentVertex];
					int32_t temp_vertex = currentVertex;
					while (sPrevVertex[temp_vertex] != -1)
					{
						if (remainCapMap[temp_vertex][sPrevVertex[temp_vertex]] < temp_cap)
						{
							temp_cap = remainCapMap[temp_vertex][sPrevVertex[temp_vertex]];
						}
						temp_vertex = sPrevVertex[temp_vertex];
					}

					int32_t cur_cap = remainCapMap[j][sPrevVertex[j]];
					temp_vertex = j;
					while (sPrevVertex[temp_vertex] != -1)
					{
						if (remainCapMap[temp_vertex][sPrevVertex[temp_vertex]] < cur_cap)
						{
							cur_cap = remainCapMap[temp_vertex][sPrevVertex[temp_vertex]];
						}
						temp_vertex = sPrevVertex[temp_vertex];
					}

					if (temp_cap > cur_cap)
					{
						sDistance[j] = temp;
						sPrevVertex[j] = currentVertex;
					}
				}
#endif
			}
		}
		// end: update distance
#endif
	}

	return false;
}