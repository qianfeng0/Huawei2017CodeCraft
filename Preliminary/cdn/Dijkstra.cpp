#include "Dijkstra.h"
//#include <assert.h>

NetLib::Dijkstra::Dijkstra()
:	mCostMap(nullptr),
	mStartVertex(-1),
	mNumOfVertex(0),
	mDistance(),
	mPrevVertex(),
	mIsInS(),
	mCurrentVertex(-1)
{

}

NetLib::Dijkstra::Dijkstra( const Map* costMap, const int32_t startVertex )
{
	Init( costMap, startVertex );
}

NetLib::Dijkstra::Dijkstra( const Dijkstra& newObj )
:	mCostMap(newObj.mCostMap),
	mStartVertex( newObj.mStartVertex ),
	mNumOfVertex( newObj.mNumOfVertex ),
	mDistance( newObj.mDistance),
	mPrevVertex( newObj.mPrevVertex ),
	mIsInS( newObj.mIsInS ),
	mCurrentVertex( newObj.mCurrentVertex)
{

}
NetLib::Dijkstra::~Dijkstra()
{

}

NetLib::Dijkstra&
NetLib::Dijkstra::operator= ( const Dijkstra& newObj )
{
	if ( this != &newObj )
	{
		mCostMap = (newObj.mCostMap);
		mStartVertex = ( newObj.mStartVertex );
		mNumOfVertex = ( newObj.mNumOfVertex );
		mDistance = ( newObj.mDistance);
		mPrevVertex = ( newObj.mPrevVertex );
		mIsInS = ( newObj.mIsInS );
		mCurrentVertex = ( newObj.mCurrentVertex);
	}
	return *this;
}

void
NetLib::Dijkstra::Init( const Map* costMap, const int32_t startVertex )
{
	mNumOfVertex = costMap->size();

	mCostMap = costMap;
	mStartVertex = startVertex;

	mIsInS.assign( mNumOfVertex, false );

	mDistance.resize( costMap->size() );
	mPrevVertex.resize( costMap->size() );

	for ( int32_t i = 0; i < mNumOfVertex; i++ )
	{
		mDistance[i] = (*costMap)[startVertex][i];
		if ( (*costMap)[startVertex][i] < INT32_MAX )
			mPrevVertex[i] = startVertex;
		else
			mPrevVertex[i] =  -1;
	}
	mPrevVertex[startVertex] = -1;

	mIsInS[startVertex] = true;

	mCurrentVertex = startVertex;
}

void
NetLib::Dijkstra::Reset()
{
	mIsInS.assign( mNumOfVertex, false );

	for ( int32_t i = 0; i < mNumOfVertex; i++ )
	{
		mDistance[i] = (*mCostMap)[mStartVertex][i];
		if ( (*mCostMap)[mStartVertex][i] < INT32_MAX )
			mPrevVertex[i] = mStartVertex;
		else
			mPrevVertex[i] =  -1;
	}
	mPrevVertex[mStartVertex] = -1;

	mIsInS[mStartVertex] = true;

	mCurrentVertex = mStartVertex;
}

void
NetLib::Dijkstra::ResetWithCap( const Map& remainCapMap )
{
	mIsInS.assign( mNumOfVertex, false );

	for ( int32_t i = 0; i < mNumOfVertex; i++ )
	{
		if ( remainCapMap[i][mStartVertex] > 0 )
			mDistance[i] = (*mCostMap)[mStartVertex][i];
		else
			mDistance[i] = INT32_MAX;

		//if ( (*mCostMap)[mStartVertex][i] < INT32_MAX )  //??bug?
		if (mDistance[i] < INT32_MAX)
			mPrevVertex[i] = mStartVertex;
		else
			mPrevVertex[i] =  -1;
	}
	mPrevVertex[mStartVertex] = -1;

	mIsInS[mStartVertex] = true;

	mCurrentVertex = mStartVertex;
}

void
NetLib::Dijkstra::FindAllVertex()
{
	//循环从1开始，因为开始节点已经放在S中，还有numOfVertex - 1 个节点要处理
	for ( int32_t i = 1; i < mNumOfVertex; i++ )
	{
		//选择distance最小的一个节点
		int32_t nextVertex = -1;
		int32_t tempDistance = INT32_MAX;
		for ( int32_t j = 0; j < mNumOfVertex; j++ )
		{
			if ( (mIsInS[j] == false) && (mDistance[j] < tempDistance) )
			{
				nextVertex = j;
				tempDistance = mDistance[j];
			}
		}

		//放入S集合
		mIsInS[nextVertex] = true;
		//下次寻找的开始节点
		mCurrentVertex = nextVertex;

		//更新distance
		for ( int32_t j = 0; j < mNumOfVertex; j++ )
		{
			if ( mIsInS[j] == false && (*mCostMap)[mCurrentVertex][j] < INT32_MAX )
			{
				int32_t temp = mDistance[mCurrentVertex] + (*mCostMap)[mCurrentVertex][j];
				if ( temp < mDistance[j] )
				{
					mDistance[j] = temp;
					mPrevVertex[j] = mCurrentVertex;
				}
			}
		}
	}
}

bool
NetLib::Dijkstra::FindClosestServer( const Map& remainCapMap, const NodeArray& nodeArray, Route& route , const int32_t maxCost)
{
	route.Reset();

	ResetWithCap(remainCapMap);

	//the closest server was found
	for ( int32_t i = 0; i < mNumOfVertex; i++ )
	{
		if ( (mIsInS[i] == true) && nodeArray[i].IsServer() )
		{
			int32_t cost = mDistance[i];
			route.SetCost(cost);
			BuildRount( i, route );
			return true;
		}
	}

	//Route loc_Route;

	//循环从1开始，因为开始节点已经放在S中，还有numOfVertex - 1 个节点要处理
	for ( int32_t i = 1; i < mNumOfVertex; i++ )
	{
		//选择distance最小的一个节点
		int32_t nextVertex = -1;
		int32_t tempDistance = INT32_MAX;
		for ( int32_t j = 0; j < mNumOfVertex; j++ )
		{
			if ( mIsInS[j] == false && 
				mDistance[j] < tempDistance )//&& 
				//remainCapMap[j][mPrevVertex[j]] > 0 )
			{
				nextVertex = j;
				tempDistance = mDistance[j];
			}
		}

		// not found
		if ( nextVertex == -1 || mDistance[mCurrentVertex] >= maxCost)
		{
			if ( route.GetCost() < INT32_MAX )
				return true;
			else
				return false;
		}
		//放入S集合
		mIsInS[nextVertex] = true;
		//下次寻找的开始节点
		mCurrentVertex = nextVertex;
		
		//break, if loc_Route min cost and max cap
		if (mDistance[mCurrentVertex] > route.GetCost())
		{
			return true;
		}

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//find the server node
		if (nodeArray[mCurrentVertex].IsServer() && mDistance[mCurrentVertex] <= route.GetCost())
		{
			Route temp_Route;
			temp_Route.SetCost(mDistance[mCurrentVertex]);
			//route.SetCost(cost);
			BuildRount(mCurrentVertex, temp_Route);
			if (temp_Route.GetCapacity(remainCapMap) > route.GetCapacity(remainCapMap))
			{
				route = temp_Route;
			}
			//return true;
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		//更新distance
		for ( int32_t j = 0; j < mNumOfVertex; j++ )
		{
			if ( mIsInS[j] == false && 
				(*mCostMap)[mCurrentVertex][j] < INT32_MAX &&  
				remainCapMap[j][mCurrentVertex] > 0 )
			{
				int32_t temp = mDistance[mCurrentVertex] + (*mCostMap)[mCurrentVertex][j];
				if ( temp < mDistance[j] )
				{
					mDistance[j] = temp;
					mPrevVertex[j] = mCurrentVertex;
				}
#if (0)
				//add
				else if ( temp == mDistance[j])
				{
					int32_t temp_cap = remainCapMap[j][mCurrentVertex];
					int32_t temp_vertex = mCurrentVertex;
					while ( mPrevVertex[temp_vertex] != -1 )
					{
						if ( remainCapMap[temp_vertex][mPrevVertex[temp_vertex]] < temp_cap )
						{
							temp_cap = remainCapMap[temp_vertex][mPrevVertex[temp_vertex]];
						}
						temp_vertex = mPrevVertex[temp_vertex];
					}

					int32_t cur_cap = remainCapMap[j][mPrevVertex[j]];
					temp_vertex = j;
					while ( mPrevVertex[temp_vertex] != -1 )
					{
						if ( remainCapMap[temp_vertex][mPrevVertex[temp_vertex]] < cur_cap )
						{
							cur_cap = remainCapMap[temp_vertex][mPrevVertex[temp_vertex]];
						}
						temp_vertex = mPrevVertex[temp_vertex];
					}

					if ( temp_cap > cur_cap )
					{
						mDistance[j] = temp;
						mPrevVertex[j] = mCurrentVertex;
					}
				}
#endif
			}
		}
	}
	return false;
}

void
NetLib::Dijkstra::BuildRount( int32_t objVertex, Route& n_route )
{
	n_route.ReStart( Node( objVertex, true ) );

	int32_t curVertex = objVertex;
	while (mPrevVertex[curVertex] != -1)
	{
		curVertex = mPrevVertex[curVertex];
		n_route.Advance( ( Node( curVertex, false ) ) );
	}

	assert( (*(n_route.end() - 1)).GetId() == mStartVertex );
}

bool NetLib::DijkstraOnePointOfSet(const int32_t startNode, const std::vector< bool >& targetNodes, const NetLib::Map& costMap, NetLib::Route& outRoute)
{
	static std::vector<bool> sIsInS;
	static std::vector< int32_t > sDistance;
	static std::vector< int32_t > sPrevVertex;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//start: init
	outRoute.Reset();

	int32_t numOfNodes = costMap.size();
	int32_t currentVertex = -1;

	sIsInS.assign(numOfNodes, false);
	sDistance.assign(numOfNodes, INT32_MAX);
	sPrevVertex.assign(numOfNodes, -1);

	for (int32_t i = 0; i < numOfNodes; i++)
	{
		sDistance[i] = costMap[startNode][i];
		if (costMap[startNode][i] < INT32_MAX)
			sPrevVertex[i] = startNode;
		else
			sPrevVertex[i] = -1;
	}

	sPrevVertex[startNode] = -1;
	sIsInS[startNode] = true;
	currentVertex = startNode;
	//end: init
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//start node is target
	if (targetNodes[currentVertex])
	{
		NetLib::Route temp_Route;
		temp_Route.SetCost(sDistance[currentVertex]);
		BuildRount(currentVertex, sPrevVertex, temp_Route);
		outRoute = temp_Route;
		return true;
	}

	//循环从1开始，因为开始节点已经放在S中，还有numOfVertex - 1 个节点要处理
	for (int32_t i = 1; i < numOfNodes; i++)
	{
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//select min distance node
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

		//放入S集合
		sIsInS[nextVertex] = true;
		//下次寻找的开始节点
		currentVertex = nextVertex;

		//break, if loc_Route min cost and max cap
		if (sDistance[currentVertex] > outRoute.GetCost())
		{
			return true;
		}

		//find the server node
		if (targetNodes[currentVertex] && sDistance[currentVertex] < outRoute.GetCost())
		{
			NetLib::Route temp_Route;
			temp_Route.SetCost(sDistance[currentVertex]);
			BuildRount(currentVertex, sPrevVertex, temp_Route);
			outRoute = temp_Route;
		}

		//update distance
		for (int32_t j = 0; j < numOfNodes; j++)
		{
			if (sIsInS[j] == false &&
				costMap[currentVertex][j] < INT32_MAX )
			{
				int32_t temp = sDistance[currentVertex] + costMap[currentVertex][j];
				if (temp < sDistance[j])
				{
					sDistance[j] = temp;
					sPrevVertex[j] = currentVertex;
				}
			}
		}
		//end update distance
	}
	return false;
}

#if (1)
bool NetLib::DijkstraOnePointOfSetWithCap(const int32_t startNode, const std::vector< bool >& targetNodes, const NetLib::Map& costMap, const NetLib::Map& remainCapMap, NetLib::Route& outRoute)
{
	static std::vector<bool> sIsInS;
	static std::vector< int32_t > sDistance;
	static std::vector< int32_t > sPrevVertex;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//init
	outRoute.Reset();

	int32_t numOfNodes = costMap.size();
	int32_t currentVertex = -1;

	sIsInS.assign(numOfNodes, false);
	sDistance.assign(numOfNodes, INT32_MAX);
	sPrevVertex.assign(numOfNodes, -1);

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
	if (targetNodes[currentVertex])
	{
		Route temp_Route;
		temp_Route.SetCost(sDistance[currentVertex]);
		BuildRount(currentVertex, sPrevVertex, temp_Route);
		if (temp_Route.GetCapacity(remainCapMap) > outRoute.GetCapacity(remainCapMap))
		{
			outRoute = temp_Route;
		}
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
		if (nextVertex == -1)
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
		if (targetNodes[currentVertex] && sDistance[currentVertex] <= outRoute.GetCost())
		{
			Route temp_Route;
			temp_Route.SetCost(sDistance[currentVertex]);
			BuildRount(currentVertex, sPrevVertex, temp_Route);
			if (temp_Route.GetCapacity(remainCapMap) > outRoute.GetCapacity(remainCapMap))
			{
				outRoute = temp_Route;
			}
		}

#if (1)
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
#endif
void
NetLib::BuildRount(int32_t objVertex, std::vector<int32_t> preVertex, NetLib::Route& n_route)
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