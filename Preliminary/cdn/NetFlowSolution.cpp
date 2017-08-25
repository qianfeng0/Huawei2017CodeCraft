#include "NetFlowSolution.h"

#include "CommonUtils.h"
#include <stdio.h>
#include "ElapsedTime.h"
#include "Dijkstra_v2.h"

NetLib::NetFlowSolution::NetFlowSolution(void)
:	mNumOfNodes(0),
	mNumOfLinks(0),
	mNumOfConsumNode(0),
	mServerCost(0),
	mCost(INT32_MAX),
	//mFirstIteComplete(false),
	mMinCost(INT32_MAX),
	mListGraph()
{
	
}

NetLib::NetFlowSolution::NetFlowSolution( const int32_t numOfNode )
:	mNumOfNodes(numOfNode),
	mNumOfLinks(0),
	mNumOfConsumNode(0),
	mServerCost(0),
	mCost(INT32_MAX),
	//mFirstIteComplete(false),
	mMinCost(INT32_MAX),
	mListGraph(numOfNode)
{
	//MatAlloc( mNumOfNodes );
}

NetLib::NetFlowSolution::~NetFlowSolution()
{

}

/*
void
NetLib::NetMap::AddLink(const Link& newLink )
{
	assert( (newLink.GetEdge().mStart < mNumOfNodes) && (newLink.GetEdge().mEnd < mNumOfNodes));

	mLinkMap[newLink.GetEdge().mStart][newLink.GetEdge().mEnd] = newLink;
}
*/
void
NetLib::NetFlowSolution::AddServer( int32_t new_serverId )
{
	//Node tempNode( new_serverId, false);

	//mServerNodeArray.push_back( tempNode );
	//mAllNodeArray[tempNode.GetId()] = tempNode;
	mAllNodeArray[new_serverId].SetAsServer();
}
/*
const NetLib::Link&
NetLib::NetMap::GetLink( int32_t start, int32_t end ) const
{
	return mLinkMap[start][end];
}

const NetLib::Link&
NetLib::NetMap::GetLink( const stEdge& nEdge ) const
{
	return mLinkMap[nEdge.mStart][nEdge.mEnd];
}
*/
const NetLib::LinkMap&
NetLib::NetFlowSolution::GetLinkMap() const
{
	return mLinkMap;
}

const NetLib::Map&
NetLib::NetFlowSolution::GetCostMap() const
{
	return mCostMap;
}

const NetLib::Map&
NetLib::NetFlowSolution::GetRemainCapacityMap() const
{
	return mRemainCapacityMap;
}

int32_t
NetLib::NetFlowSolution::GetNumOfServer(NodeArray& n_servers) const {
	int32_t numOfserver = 0;
	for (int32_t i = n_servers.size() - 1; i >= 0; i--)
	{
		if (n_servers[i].IsServer())
			numOfserver++;
	}
	return numOfserver;
}

int32_t
NetLib::NetFlowSolution::GetNumOfServer(std::vector<bool>& n_servers) const {
	int32_t numOfserver = 0;
	for (int32_t i = n_servers.size() - 1; i >= 0; i--) {
		if (n_servers[i])
			numOfserver++;
	}
	return numOfserver;
}

const NetLib::NodeArray&
NetLib::NetFlowSolution::GetNodeArray() const
{
	return mAllNodeArray;
}

int32_t
NetLib::NetFlowSolution::GetCost() const
{
	//return CalculateSumCost(mConsumptionNodeArray, mCostMap);
	return CalculateSumCost(mMinCostConsumers, mCostMap);
}
void
NetLib::NetFlowSolution::LoadNet( char* topo[], int line_num )
{
	std::vector< int32_t > numbers;
	numbers.reserve(4);

	int32_t currentLineIndex = 0;

	//summary
	CommonUtils::LoadNumberInLine( topo[currentLineIndex], numbers);
	mNumOfNodes = numbers[0];
	mNumOfLinks = numbers[1];
	mNumOfConsumNode = numbers[2];
	currentLineIndex += 2;

	//server cost
	CommonUtils::LoadNumberInLine( topo[currentLineIndex], numbers);
	mServerCost = numbers[0];
	currentLineIndex += 2;

	//init nodes
	InitAllNode( mNumOfNodes );

	InitLinkMap( mNumOfNodes );
	mListGraph.Init(mNumOfNodes);
	//Link newLink;
	for ( int32_t i = 0; i < mNumOfLinks; i++)
	{
		CommonUtils::LoadNumberInLine( topo[currentLineIndex], numbers);
		mLinkMap[numbers[0]][numbers[1]].Set( numbers[0], numbers[1], numbers[2], numbers[3], numbers[2] );

		//newLink.Set( numbers[0], numbers[1], numbers[2], numbers[3], numbers[2] );
		//mLinks[newLink.mStart][newLink.mEnd] = newLink;
		//AddLink(newLink);
		mLinkMap[numbers[1]][numbers[0]].Set( numbers[1], numbers[0], numbers[2], numbers[3], numbers[2] );
		//newLink.Set( numbers[1], numbers[0], numbers[2], numbers[3], numbers[2] );
		//mLinks[newLink.mStart][newLink.mEnd] = newLink;
		//AddLink(newLink);
		currentLineIndex++;

		mListGraph.AddEdge(numbers[0], numbers[1], numbers[3], numbers[2]);
		mListGraph.AddEdge(numbers[1], numbers[0], numbers[3], numbers[2]);
	}

	for ( int32_t i = mLinkMap.size() - 1; i >= 0; i-- )
	{
		mLinkMap[i][i].Set( INT32_MAX, 0, INT32_MAX );
	}

	currentLineIndex++;

	//init cost map
	InitCostMap( mNumOfNodes, mLinkMap );

	CreatRemainCapacityMap( mNumOfNodes, mLinkMap, mRemainCapacityMap );

#if (0)
	//get consumption node
	ConsumptionNode newConsumptionNode;
	mConsumptionNodeArray.reserve( mNumOfConsumNode );
	for ( int32_t i = 0; i < mNumOfConsumNode; i++ )
	{
		CommonUtils::LoadNumberInLine( topo[currentLineIndex], numbers);
		newConsumptionNode.Init( numbers[0], numbers[1], numbers[2], &mCostMap );
		mConsumptionNodeArray.push_back( newConsumptionNode );
		currentLineIndex++;
	}
#endif
	
	mConsumptionNodeArray.resize( mNumOfConsumNode );
	
	for ( int32_t i = 0; i < mNumOfConsumNode; i++ )
	{
		CommonUtils::LoadNumberInLine( topo[currentLineIndex], numbers);
		currentLineIndex++;
		mConsumptionNodeArray[i].Init( numbers[0], numbers[1], numbers[2], &mCostMap );
	}
	
}

void
NetLib::NetFlowSolution::InitAllNode( int32_t numOfNode )
{
	mAllNodeArray.resize(mNumOfNodes);
	for ( int32_t i = numOfNode - 1; i >= 0; i-- )
	{
		mAllNodeArray[i].Set( i, false );
	}
}

void
NetLib::NetFlowSolution::InitCostMap( int32_t numOfNode, const LinkMap& linkMap )
{
	mCostMap.ReDim(numOfNode);

	for ( int32_t i = 0; i < numOfNode; i++ )
	{
		for ( int32_t j = 0; j < numOfNode; j++ )
		{
			mCostMap[i][j] = linkMap[i][j].GetCost();
		}
	}
}

void
NetLib::NetFlowSolution::CreatRemainCapacityMap( int32_t numOfNode, const LinkMap& linkMap, Map& out_capMap ) const
{
	out_capMap.ReDim( numOfNode );

	for ( int32_t i = 0; i < numOfNode; i++ )
	{
		for ( int32_t j = 0; j < numOfNode; j++ )
		{
			out_capMap[i][j] = linkMap[i][j].GetRemainCapacity();
		}
	}
}

void
NetLib::NetFlowSolution::ResetConsumptionNodeArray(std::vector< ConsumptionNode >& n_ConsumptionNodeArray) const
{
	for ( int32_t i = n_ConsumptionNodeArray.size() - 1; i >= 0; i-- )
	{
		n_ConsumptionNodeArray[i].ClearFlows();
	}
}

void 
NetLib::NetFlowSolution::ResetServerArray(NodeArray& n_serverArray)
{
	for (int32_t i = n_serverArray.size() - 1; i >= 0; i --)
	{
		n_serverArray[i].SetAsCommon();
	}
}

void 
NetLib::NetFlowSolution::CopyInternalToLocal( std::vector< ConsumptionNode >& n_ConsumptionNodeArray, Map& n_RemainCapacityMap, int32_t& n_cost ) const
{
	n_ConsumptionNodeArray = mConsumptionNodeArray;
	n_RemainCapacityMap = mRemainCapacityMap;
	n_cost = mCost;
}

void 
NetLib::NetFlowSolution::CopyLocalToInternal( const std::vector< ConsumptionNode >& n_ConsumptionNodeArray, const Map& n_RemainCapacityMap, const int32_t& n_cost)
{
	mConsumptionNodeArray = n_ConsumptionNodeArray;
	mRemainCapacityMap = n_RemainCapacityMap;
	mCost = n_cost;
}

void
NetLib::NetFlowSolution::InitLinkMap( int32_t numOfNode )
{
	mLinkMap.resize( mNumOfNodes );
	for ( int32_t i = mLinkMap.size() - 1; i >= 0; i-- )
	{
		mLinkMap[i].resize(mNumOfNodes);
	}
}
#if (0)
void 
NetLib::NetFlowSolution::CalculateStatistics()
{
	float density_consumptionNode = mNumOfConsumNode / mNumOfNodes;
	float avr_consumptionValue = 0;
	float avr_linkCap = 0;
	float avr_linkCost = 0;

	for (int32_t i = mConsumptionNodeArray.size() - 1; i >= 0; i--)
	{
		avr_consumptionValue += mConsumptionNodeArray[i].GetFlowRequirement();
	}
	avr_consumptionValue /= mConsumptionNodeArray.size();

	for ( int32_t i = 0; i < mNumOfNodes; i++ )
	{
		for ( int32_t j = 0; j < mNumOfNodes; j++ )
		{

		}
	}
}
#endif
void
NetLib::NetFlowSolution::CalculateWorstResult()
{
	Flow tempFlow;
	for ( int32_t i = mConsumptionNodeArray.size() - 1; i >= 0; i-- )
	{
		AddServer( mConsumptionNodeArray[i].GetConnectNodeId() );

		tempFlow.ReStart( mAllNodeArray[ mConsumptionNodeArray[i].GetConnectNodeId() ] );
		tempFlow.SetFlowValue( mConsumptionNodeArray[i].GetFlowRequirement() );
		mConsumptionNodeArray[i].AddFlow(tempFlow);
	}
}

std::string
NetLib::NetFlowSolution::stringFlow( )
{
	std::string str = "";

	int32_t numOfFlow = 0;
	for (int32_t i = mMinCostConsumers.size() - 1; i >= 0; i--)
	{
		numOfFlow += mMinCostConsumers[i].GetFlowArray().size();
	}
	str += std::to_string(numOfFlow) + "\n";


	for (int32_t i = mMinCostConsumers.size() - 1; i >= 0; i--)
	{
		const FlowArray& rFlowArray = mMinCostConsumers[i].GetFlowArray();
		for (int32_t j = rFlowArray.size() - 1; j >= 0; j--)
		{
			str += "\n";
			str += rFlowArray[j].ToStr();
			str += std::to_string(mMinCostConsumers[i].GetId()) + " ";
			str += std::to_string(rFlowArray[j].GetFlowValue());
		}
	}
#if (0)
	std::string str = "";

	int32_t numOfFlow = 0;
	for ( int32_t i = mConsumptionNodeArray.size() - 1; i >= 0; i-- )
	{
		numOfFlow += mConsumptionNodeArray[i].GetFlowArray().size();
	}
	str += std::to_string( numOfFlow ) + "\n";

	
	for ( int32_t i = mConsumptionNodeArray.size() - 1; i >= 0; i-- )
	{
		const FlowArray& rFlowArray = mConsumptionNodeArray[i].GetFlowArray();
		for ( int32_t j = rFlowArray.size() - 1; j >= 0; j-- )
		{
			str += "\n";
			str += rFlowArray[j].ToStr();
			str += std::to_string( mConsumptionNodeArray[i].GetId() ) + " ";
			str += std::to_string( rFlowArray[j].GetFlowValue() );
		}
	}
#endif
	return str;
}

bool
NetLib::NetFlowSolution::CalculateFlowsByConfirmServer(std::vector< int32_t > serverIDArray)
{
	for ( int32_t i = serverIDArray.size() - 1; i >= 0; i-- )
	{
		AddServer(serverIDArray[i]);
	}

	
	//if (CalculateFlows(mAllNodeArray, mConsumptionNodeArray, mRemainCapacityMap, mMinCost) == false)
	if (CalculateFlows(mAllNodeArray, mConsumptionNodeArray, mRemainCapacityMap, mCost) == false)
	{
		return false;
	}
	return true;
}

bool 
NetLib::NetFlowSolution::Process()
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//start: init worst cost

	SelectServer(mAllNodeArray);

	int32_t numOfserver = 0;
	numOfserver = GetNumOfServer(mAllNodeArray);
	printf("initial number of server: %d \n", numOfserver);
	if (!CalculateFlows(mAllNodeArray, mConsumptionNodeArray, mRemainCapacityMap, mCost))
	{
		assert(false);
		printf("initial server error \n");
		return false;
	}

	//end: init worst cost
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	std::vector<int32_t> delArrayID;
	bool isRepeatCal = true;
	bool isFirstValid = false;

	while (1)
	{
#ifdef ELAPSED_TIME_CHECK
		if (elapsedTime() > 87)
		{
			if (mMinCost > CalculateSumCost(mConsumptionNodeArray, mCostMap))
			{
				mMinCostConsumers = mConsumptionNodeArray;
			}
			break;
		}
#endif
		
#ifdef DEBUG_PRINTF
		static int32_t cnt = 0;
		cnt++;
		printf("times: %d\n", cnt);
#endif


#if (0)
		usedServer.assign(mNumOfNodes, false);
		int32_t serverID = 0;
		for (int32_t i = mConsumptionNodeArray.size() - 1; i >= 0; i--)
		{
			for (int32_t j = mConsumptionNodeArray[i].GetFlowArray().size() - 1; j >= 0; j--)
			{
				serverID = mConsumptionNodeArray[i].GetFlowArray()[j][0].GetId();
				usedServer[serverID] = true;
			}
		}
#endif
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//start: delete one server to min cost
		std::vector< ConsumptionNode > loc_minCost_ConsumptionNodeArray;
		Map loc_minCost_RemainCapacityMap;
		int32_t loc_minCost = INT32_MAX;
		CopyInternalToLocal(loc_minCost_ConsumptionNodeArray, loc_minCost_RemainCapacityMap, loc_minCost);
		
		std::vector< ConsumptionNode > loc_ConsumptionNodeArray;
		Map loc_RemainCapacityMap;
		int32_t loc_Cost = INT32_MAX;

		int32_t loc_deleteServerID = -1;

		//find the used server node, try to be deleted
		std::vector<bool> usedServer;
		GetUsedServer(loc_minCost_ConsumptionNodeArray, usedServer);

		//try to delete used server node
		if (!isFirstValid)
		{
			for (int32_t i = usedServer.size() - 1; i >= 0; i--)
			{
				if (usedServer[i] == true)
				{
					CopyInternalToLocal(loc_ConsumptionNodeArray, loc_RemainCapacityMap, loc_Cost);
					if (mNumOfNodes > THRESHOLD_NUMBER_OF_NODES) {
						DeleteServerAndFlows(i, mAllNodeArray, loc_ConsumptionNodeArray, loc_RemainCapacityMap);
					}
					else {
						CreatRemainCapacityMap(mNumOfNodes, mLinkMap, loc_RemainCapacityMap);
						ResetConsumptionNodeArray(loc_ConsumptionNodeArray);
						mAllNodeArray[i].SetAsCommon();
					}

					if (CalculateFlows(mAllNodeArray, loc_ConsumptionNodeArray, loc_RemainCapacityMap, loc_Cost))
					{
						if (loc_Cost < loc_minCost)
						{
							loc_minCost = loc_Cost;
							loc_minCost_ConsumptionNodeArray = loc_ConsumptionNodeArray;
							loc_minCost_RemainCapacityMap = loc_RemainCapacityMap;
							loc_deleteServerID = i;
						}
					}
					mAllNodeArray[i].SetAsServer();
				}
			}
		}
		else
		{
			std::vector<int32_t> deltaCost;
			deltaCost.assign(mNumOfNodes, 0);
			for (int32_t i = usedServer.size() - 1; i >= 0; i--)
			{
				if (usedServer[i] == true)
				{
					CopyInternalToLocal(loc_ConsumptionNodeArray, loc_RemainCapacityMap, loc_Cost);
					if (mNumOfNodes > THRESHOLD_NUMBER_OF_NODES) {
						DeleteServerAndFlows(i, mAllNodeArray, loc_ConsumptionNodeArray, loc_RemainCapacityMap);
					}
					else {
						CreatRemainCapacityMap(mNumOfNodes, mLinkMap, loc_RemainCapacityMap);
						ResetConsumptionNodeArray(loc_ConsumptionNodeArray);
						mAllNodeArray[i].SetAsCommon();
					}

					if (CalculateFlows(mAllNodeArray, loc_ConsumptionNodeArray, loc_RemainCapacityMap, loc_Cost))
					{
						deltaCost[i] = mCost - loc_Cost;
					}
					mAllNodeArray[i].SetAsServer();
				}
			}
			float maxValue = 0.0f;
			for (int32_t i = deltaCost.size() - 1; i >= 0; i--)
			{
				if (deltaCost[i] > 0)
				{
					//float temp = (float)deltaCost[i] / ((float)mSumInfo[i]+1);
					float temp = (float)deltaCost[i] / ((float)mSumInfo[i] + 5);
					if (temp > maxValue)
					{
						maxValue = temp;
						loc_deleteServerID = i;
					}
				}
			}
			if (loc_deleteServerID != -1)
			{
				CopyInternalToLocal(loc_ConsumptionNodeArray, loc_RemainCapacityMap, loc_Cost);
				if (mNumOfNodes > THRESHOLD_NUMBER_OF_NODES) {
					DeleteServerAndFlows(loc_deleteServerID, mAllNodeArray, loc_ConsumptionNodeArray, loc_RemainCapacityMap);
				}
				else {
					CreatRemainCapacityMap(mNumOfNodes, mLinkMap, loc_RemainCapacityMap);
					ResetConsumptionNodeArray(loc_ConsumptionNodeArray);
					mAllNodeArray[loc_deleteServerID].SetAsCommon();
				}
				if (!CalculateFlows(mAllNodeArray, loc_ConsumptionNodeArray, loc_RemainCapacityMap, loc_Cost))
					assert(false);
				loc_minCost = loc_Cost;
				loc_minCost_ConsumptionNodeArray = loc_ConsumptionNodeArray;
				loc_minCost_RemainCapacityMap = loc_RemainCapacityMap;
				mAllNodeArray[loc_deleteServerID].SetAsServer();
			}
		}

		if (loc_deleteServerID != -1)
		{
			mAllNodeArray[loc_deleteServerID].SetAsCommon();
			CopyLocalToInternal(loc_minCost_ConsumptionNodeArray, loc_minCost_RemainCapacityMap, loc_minCost);

			delArrayID.push_back(loc_deleteServerID);
		}
		else
		{
			if ( isRepeatCal )
			{
				int32_t currentCost = CalculateSumCost(mConsumptionNodeArray, mCostMap);
#ifdef PRINTF_LOG
				printf("~~current server= %d, min cost= %d\n", GetNumOfServer(mAllNodeArray), currentCost);
#endif
				if (currentCost < mMinCost)
				{
					mMinCostConsumers = mConsumptionNodeArray;
					mMinCostServers = mAllNodeArray;
					mMinCost = currentCost;
#ifdef PRINTF_LOG
					printf("delete success!, current server= %d, min cost= %d\n", GetNumOfServer(mAllNodeArray), mMinCost);
#endif
				}
				else
				{
					mConsumptionNodeArray = mMinCostConsumers;
					mAllNodeArray = mMinCostServers;
#ifdef PRINTF_LOG
					printf("delete failed!, current server= %d, min cost= %d\n", GetNumOfServer(mAllNodeArray), mMinCost);
#endif
				}

				static int32_t offset = 0;

				std::vector<bool> additionOfServers;
				if (mNumOfNodes > 200)
					GetServerFromInfo(mC2CInfo, 6 + offset, additionOfServers);
				else
					GetServerFromInfo(mSumInfo, 6 + offset, additionOfServers);

				offset++;

				int32_t numOfAddS = 0;
				for (int32_t i = mAllNodeArray.size() - 1; i >= 0; i--) {
					if (additionOfServers[i]) {
						if (!mAllNodeArray[i].IsServer()) {
							numOfAddS++;
						}
						mAllNodeArray[i].SetAsServer();
					}
				}
				if (numOfAddS == 0 && isFirstValid == true) {
					break;
				}
				else if (numOfAddS == 0)
				{
					isFirstValid = true;
					offset = 0;
					if (mNumOfNodes > 200)
						GetServerFromInfo(mC2CInfo, 6 + offset, additionOfServers);
					else
						GetServerFromInfo(mSumInfo, 6 + offset, additionOfServers);
					offset++;

					int32_t numOfAddS = 0;
					for (int32_t i = mAllNodeArray.size() - 1; i >= 0; i--) {
						if (additionOfServers[i]) {
							if (!mAllNodeArray[i].IsServer()) {
								numOfAddS++;
							}
							mAllNodeArray[i].SetAsServer();
						}
					}
				}
#ifdef PRINTF_LOG
				printf("add server: %d\n", numOfAddS);
#endif
				//init flows
				InitFlowsAndCapMapByServers(mAllNodeArray, mConsumptionNodeArray, mRemainCapacityMap, mCost);
#if (0)
				static int32_t offset = -1;
				if (offset > 5)
					break;
				std::vector<bool> additionOfServers;
				offset++;
				if (currentCost < mMinCost)
				{
					mMinCostConsumers = mConsumptionNodeArray;
					mMinCost = currentCost;
					printf("current server= %d, min cost= %d\n", GetNumOfServer(mAllNodeArray), mMinCost);

					//add servers
					GetServerFromInfo(mC2CInfo, 6 + offset, additionOfServers);
					offset++;
					int32_t numOfAddS = 0;
					for (int32_t i = mAllNodeArray.size() - 1; i >= 0; i--) {
						if (additionOfServers[i]) {
							if (!mAllNodeArray[i].IsServer()) {
								numOfAddS++;
							}
							mAllNodeArray[i].SetAsServer();
						}
					}
#if (0)
					for (int32_t i = mAllNodeArray.size() - 1; i >= 0; i--) {
						if (mServerC2C[i].IsServer()) {
							if (!mAllNodeArray[i].IsServer()) {
								numOfAddS++;
							}
							mAllNodeArray[i].SetAsServer();
						}
					}
#endif
					printf("add server: %d\n", numOfAddS);

					//init flows
					InitFlowsAndCapMapByServers(mAllNodeArray, mConsumptionNodeArray, mRemainCapacityMap, mCost);
				}
				else
				{
					mConsumptionNodeArray = mMinCostConsumers;

					//break;
				}
#endif
#if (0)
				isRepeatCal = false;

				//save result
				mMinCostConsumers = mConsumptionNodeArray;
				mFirstIteComplete = true;
				mMinCost = CalculateSumCost(mMinCostConsumers, mCostMap);
				printf("1st ite: current server= %d, min cost= %d\n", GetNumOfServer(mAllNodeArray), mMinCost);


				//add servers
				int32_t numOfAddS = 0;
				for (int32_t i = mAllNodeArray.size() - 1; i >= 0; i--){
					if (mServerC2C[i].IsServer()){
						if (!mAllNodeArray[i].IsServer()) {
							numOfAddS++;
						}
						mAllNodeArray[i].SetAsServer();
					}
				}
				printf("add server: %d\n", numOfAddS);
				//init flows
				InitFlowsAndCapMapByServers(mAllNodeArray, mConsumptionNodeArray, mRemainCapacityMap, mCost);
#endif
			}
			else {
				break;
			}
		}
	}

	printf("result : current server= %d, min cost= %d\n", GetNumOfServer(mAllNodeArray), CalculateSumCost(mMinCostConsumers, mCostMap));

#if (0)
	printf("current ite: current server= %d, min cost= %d\n", GetNumOfServer(mAllNodeArray), CalculateSumCost(mConsumptionNodeArray, mCostMap));
	if (mFirstIteComplete)
	{
		if (CalculateSumCost(mMinCostConsumers, mCostMap) <= CalculateSumCost(mConsumptionNodeArray, mCostMap))
		{
			printf("select 1st result\n");
			mConsumptionNodeArray = mMinCostConsumers;
		}
	}
#endif
	return true;
}

bool
NetLib::NetFlowSolution::CalculateFlows( const NodeArray& re_NodeArray, std::vector< ConsumptionNode >& re_ConsumptionNodeArray, Map& re_RemainCapacityMap, int32_t& out_cost ) const
{
#if (0)
	std::vector< bool > serverNodes;
	serverNodes.assign(mNumOfNodes, false);
	for (int32_t i = 0; i < mNumOfNodes; i++) {
		serverNodes[i] = re_NodeArray[i].IsServer();
	}
#endif
	out_cost = INT32_MAX;

	bool loc_solutionVaild = false;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//START: Calculate flows
	while (!loc_solutionVaild)
	{
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//START: find min cost route of consumption
		loc_solutionVaild = true;

		std::vector< FlowArray > newConsumptionFlowArray;
		newConsumptionFlowArray.resize(re_ConsumptionNodeArray.size() );
		Flow temp_flow;
		for ( int32_t i = re_ConsumptionNodeArray.size() - 1; i >= 0; i-- )
		{
			while (re_ConsumptionNodeArray[i].GetUnassignFlow() != 0 )
			{
				assert(re_ConsumptionNodeArray[i].GetUnassignFlow() > 0);

				//solution invalid
				loc_solutionVaild = false;

				//use Dijkstra find the min cost of flow
				int32_t maxRouteCost = (mServerCost - re_ConsumptionNodeArray[i].GetCurrentCost()) / re_ConsumptionNodeArray[i].GetUnassignFlow() + 0;// *3 / 2;
#if (1)
				if (mNumOfNodes > THRESHOLD_NUMBER_OF_NODES && mServerCost > 700)
				{
					maxRouteCost = (700 - re_ConsumptionNodeArray[i].GetCurrentCost()) / re_ConsumptionNodeArray[i].GetUnassignFlow() + 0;
				}
#endif
				//if ( !re_ConsumptionNodeArray[i].GetDijkstra().FindClosestServer(re_RemainCapacityMap, re_NodeArray, temp_flow, maxRouteCost) )
				//if (!re_ConsumptionNodeArray[i].GetDijkstra().FindClosestServer(re_RemainCapacityMap, re_NodeArray, temp_flow))
				//if (!DijkstraOnePointOfSetWithCap(re_ConsumptionNodeArray[i].GetConnectNodeId(), serverNodes, mCostMap, re_RemainCapacityMap, temp_flow))
				//if (!HeapDijkstra::DijkstraOnePointOfSet(re_ConsumptionNodeArray[i].GetConnectNodeId(), re_NodeArray, mCostMap, re_RemainCapacityMap, temp_flow, maxRouteCost))
				if (!HeapDijkstra::DijkstraOnePointOfSet(re_ConsumptionNodeArray[i].GetConnectNodeId(), re_NodeArray, mListGraph, re_RemainCapacityMap, temp_flow, maxRouteCost))
				{
					//can not find route to server
					return false;
				}
				//assign consumption id and flow value
				int32_t flowValue = re_ConsumptionNodeArray[i].GetFlowRequirement();
				int32_t cap = temp_flow.GetCapacity( re_RemainCapacityMap );
				if ( cap < flowValue )
				{
					flowValue = cap;
				}
				temp_flow.SetConsumptionNodeID(i);
				assert(flowValue > 0);
				temp_flow.SetFlowValue(flowValue);

				//add in new flow array and 
				newConsumptionFlowArray[i].push_back( temp_flow );
				AddFlowAndUpdataCap( temp_flow, re_ConsumptionNodeArray, re_RemainCapacityMap );
			}

			//restore remain capacity map
			for ( int32_t j = newConsumptionFlowArray[i].size() - 1; j >= 0; j-- )
			{
				DeleteFlowInCapacityMap(newConsumptionFlowArray[i][j], re_RemainCapacityMap);
			}
		}
		// END: find min cost route of consumption
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		// now capacity map: (last iter fix flow)

		//update capacity map (add new flow in capacity map)
		for (int32_t i = newConsumptionFlowArray.size() - 1; i >= 0; i--)
		{
			for (int32_t j = newConsumptionFlowArray[i].size() - 1; j >= 0; j--)
			{
				AddFlowInCapacityMap(newConsumptionFlowArray[i][j], re_RemainCapacityMap);
			}
		}

		// now capacity map: (last iter fix flow) + (new flow)

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//START: deal over width

		Route overBandwidthRoute;
		while ( !CheckOneOverBandwidth(re_RemainCapacityMap, overBandwidthRoute ) )
		{
			assert(overBandwidthRoute.size() == 2);
			
			int32_t overBandwidth_startID = overBandwidthRoute[0].GetId();
			int32_t overBandwidth_endID = overBandwidthRoute[1].GetId();
			assert(mCostMap[overBandwidth_startID][overBandwidth_endID] < INT32_MAX);
			FlowArray conflictFlowArray;

			//find the conflict flow and delete it in the ConsumptionNodeArray and capacity map
			for (int32_t i = newConsumptionFlowArray.size() - 1; i >= 0; i-- )
			{
				for (int32_t j = newConsumptionFlowArray[i].size() - 1; j >= 0; j--)
				{
					if (newConsumptionFlowArray[i][j].IsCoincide(overBandwidthRoute))
					{
						//add in conflict array
						conflictFlowArray.push_back( newConsumptionFlowArray[i][j] );

						//delete in ConsumptionNodeArray,and update capacity map
						DeleteFlowAndUpdataCap(newConsumptionFlowArray[i][j], re_ConsumptionNodeArray, re_RemainCapacityMap);

						//delete in new consumption flow array
						newConsumptionFlowArray[i].erase(newConsumptionFlowArray[i].begin() + j);
					}
				}
			}

			assert(conflictFlowArray.size() >= 2);
			// now capacity map: (last iter fix flow) + (new flow) - (the conflict flow)

			//mask over bandwidth route
			re_RemainCapacityMap[overBandwidth_startID][overBandwidth_endID] -= 10000;

			// now capacity map: (last iter fix flow) + (new flow) - (the conflict flow) - (over bandwidth route)

			//find second min cost of conflict flows
			std::vector<Route> conflictFlowArray_SecMinCost;
			conflictFlowArray_SecMinCost.resize(conflictFlowArray.size());

			for ( int32_t i = conflictFlowArray.size() - 1; i >= 0; i-- )
			{
				const Flow &reFlow = conflictFlowArray[i];
				int32_t consumptionID = reFlow.GetConsumptionNodeID();

				int32_t maxRouteCost = (mServerCost - re_ConsumptionNodeArray[consumptionID].GetCurrentCost()) / re_ConsumptionNodeArray[consumptionID].GetUnassignFlow() + 0;// *3 / 2;
#if (1)
				if (mNumOfNodes > THRESHOLD_NUMBER_OF_NODES && mServerCost > 700)
				{
					maxRouteCost = (700 - re_ConsumptionNodeArray[consumptionID].GetCurrentCost()) / re_ConsumptionNodeArray[consumptionID].GetUnassignFlow() + 0;// *3 / 2;
				}
#endif																																							  //if (re_ConsumptionNodeArray[consumptionID].GetDijkstra().FindClosestServer(re_RemainCapacityMap, re_NodeArray, conflictFlowArray_SecMinCost[i], maxRouteCost))
				//if (re_ConsumptionNodeArray[consumptionID].GetDijkstra().FindClosestServer(re_RemainCapacityMap, re_NodeArray, conflictFlowArray_SecMinCost[i]))
				//if (!HeapDijkstra::DijkstraOnePointOfSet(re_ConsumptionNodeArray[consumptionID].GetConnectNodeId(), re_NodeArray, mCostMap, re_RemainCapacityMap, conflictFlowArray_SecMinCost[i], maxRouteCost))
				if (!HeapDijkstra::DijkstraOnePointOfSet(re_ConsumptionNodeArray[consumptionID].GetConnectNodeId(), re_NodeArray, mListGraph, re_RemainCapacityMap, conflictFlowArray_SecMinCost[i], maxRouteCost))
				{
					//assert(reFlow.GetCost() <= conflictFlowArray_SecMinCost[i].GetCost());
				}
			}

			re_RemainCapacityMap[overBandwidth_startID][overBandwidth_endID] += 10000;
			// now capacity map: (last iter fix flow) + (new flow) - (the conflict flow)

			while ( re_RemainCapacityMap[overBandwidth_startID][overBandwidth_endID] != 0 )
			{
				assert(re_RemainCapacityMap[overBandwidth_startID][overBandwidth_endID] > 0);
				int32_t select_flowIndex = -1;
				//int32_t maxDeltaCost = -1;
				int32_t maxDeltaCost = INT32_MIN;
				int32_t minCap = INT32_MAX;
				for ( int32_t i = conflictFlowArray.size() - 1; i >= 0; i-- )
				{
					if ( conflictFlowArray_SecMinCost[i].GetCost() - conflictFlowArray[i].GetCost() > maxDeltaCost )
					{
						select_flowIndex = i;
						maxDeltaCost = conflictFlowArray_SecMinCost[i].GetCost() - conflictFlowArray[i].GetCost();
						minCap = conflictFlowArray_SecMinCost[i].GetCapacity(re_RemainCapacityMap);
					}
					else if ( conflictFlowArray_SecMinCost[i].GetCost() - conflictFlowArray[i].GetCost() == maxDeltaCost )
					{
						if (conflictFlowArray_SecMinCost[i].GetCapacity(re_RemainCapacityMap) < minCap)
						{
							select_flowIndex = i;
							maxDeltaCost = conflictFlowArray_SecMinCost[i].GetCost() - conflictFlowArray[i].GetCost();
							minCap = conflictFlowArray_SecMinCost[i].GetCapacity(re_RemainCapacityMap);
						}
					}
				}

				if (select_flowIndex == -1)
				{
					break;
				}

				int32_t routeCap = 0;
				routeCap = conflictFlowArray[select_flowIndex].GetCapacity(re_RemainCapacityMap);
				
				if (routeCap <= 0)
				{
					break;
				}
				if (conflictFlowArray[select_flowIndex].GetFlowValue() > routeCap)
				{
					//assert(routeCap > 0);
					conflictFlowArray[select_flowIndex].SetFlowValue(routeCap);
				}
				
				AddFlowAndUpdataCap( conflictFlowArray[select_flowIndex], re_ConsumptionNodeArray, re_RemainCapacityMap );
				newConsumptionFlowArray[conflictFlowArray[select_flowIndex].GetConsumptionNodeID()].push_back(conflictFlowArray[select_flowIndex]);
				
				conflictFlowArray.erase(conflictFlowArray.begin() + select_flowIndex);
				conflictFlowArray_SecMinCost.erase(conflictFlowArray_SecMinCost.begin() + select_flowIndex);
			}
		}
		//End: deal over width
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	}

	//END: Calculate flows
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//check flows and bandwidth

	bool checkConsumptionFlow = CheckConsumptionFlow(re_ConsumptionNodeArray);
	assert(checkConsumptionFlow == true);

	if (!checkConsumptionFlow) {
		printf("calculate flows error!!\n");
		return false;
	}

	//calculate cost
	//int32_t sumCost = CalculateSumCost(re_ConsumptionNodeArray, mCostMap);
	int32_t sumCost2 = CalculateSumCost(re_ConsumptionNodeArray, mCostMap, re_NodeArray);
	out_cost = sumCost2;
	return true;

}

void
NetLib::NetFlowSolution::AddFlowAndUpdataCap( const Flow& n_flow, std::vector< ConsumptionNode >& n_ConsumptionNodeArray, Map& n_remainCapacityMap ) const
{
	n_ConsumptionNodeArray[n_flow.GetConsumptionNodeID()].AddFlow(n_flow);
	AddFlowInCapacityMap(n_flow, n_remainCapacityMap);
}

void
NetLib::NetFlowSolution::AddFlowInCapacityMap( const Flow& n_flow, Map& n_remainCapacityMap ) const
{
	int32_t flow_value = n_flow.GetFlowValue();
	int32_t size = n_flow.size();
	int32_t edge_start = 0;
	int32_t edge_end = 0;
	for ( int32_t i = 0; i < size - 1; i++ )
	{
		edge_start = n_flow[i].GetId();
		edge_end = n_flow[i + 1].GetId();
		n_remainCapacityMap[edge_start][edge_end] -= flow_value;
	}
}

void
NetLib::NetFlowSolution::DeleteFlowAndUpdataCap(const Flow n_flow, std::vector< ConsumptionNode >& n_ConsumptionNodeArray, Map& n_remainCapacityMap) const
{
	DeleteFlowInCapacityMap(n_flow, n_remainCapacityMap);
	n_ConsumptionNodeArray[n_flow.GetConsumptionNodeID()].DelteFlow(n_flow);
}

void
NetLib::NetFlowSolution::DeleteFlowInCapacityMap( const Flow& n_flow, Map& n_remainCapacityMap ) const
{
	int32_t flow_value = n_flow.GetFlowValue();
	int32_t size = n_flow.size();
	int32_t edge_start = 0;
	int32_t edge_end = 0;
	for ( int32_t i = 0; i < size - 1; i++ )
	{
		edge_start = n_flow[i].GetId();
		edge_end = n_flow[i + 1].GetId();
		n_remainCapacityMap[edge_start][edge_end] += flow_value;
		assert( n_remainCapacityMap[edge_start][edge_end] <= mLinkMap[edge_start][edge_end].GetBandwidth() );
	}
}

bool
NetLib::NetFlowSolution::CheckAllOverBandwidth( const Map& n_remainCapacityMap, std::vector< Route >& overRouteArray )
{
	int32_t dim = n_remainCapacityMap.size();
	Route temp_route;
	for ( int32_t i = 0; i < dim; i++ )
	{
		for ( int32_t j = 0; j < dim; j++ )
		{
			if ( n_remainCapacityMap[i][j] < 0 )
			{
				temp_route.ReStart( Node( i, false ) );
				temp_route.Advance( Node( j, false ) );
				overRouteArray.push_back( temp_route );
			}
		}
	}

	if ( overRouteArray.size() == 0 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool
NetLib::NetFlowSolution::CheckOneOverBandwidth( const Map& n_remainCapacityMap, Route& overRoute ) const
{
	int32_t dim = n_remainCapacityMap.size();
	for ( int32_t i = 0; i < dim; i++ )
	{
		for ( int32_t j = 0; j < dim; j++ )
		{
			if ( n_remainCapacityMap[i][j] < 0 )
			{
				overRoute.ReStart( Node( i, false ) );
				overRoute.Advance( Node( j, false ) );
				return false;
			}
		}
	}
	return true;
}

bool
NetLib::NetFlowSolution::CheckConsumptionFlow( const std::vector< ConsumptionNode >& n_consumptionArray ) const
{
	for ( int32_t i = n_consumptionArray.size() - 1; i >= 0; i-- )
	{
		for ( int32_t j = n_consumptionArray[i].GetFlowArray().size() - 1; j >= 0; j-- )
		{
			if ( n_consumptionArray[i].GetFlowArray()[j].GetFlowValue() <= 0 )
			{
				return false;
			}
		}
	}


	for ( int32_t i = n_consumptionArray.size() - 1; i >= 0; i-- )
	{
		if ( n_consumptionArray[i].GetUnassignFlow() != 0 )
		{
			return false;
		}
	}

	Map useCap;
	CreatRemainCapacityMap(mNumOfNodes, mLinkMap, useCap);
	for ( int32_t i = n_consumptionArray.size() - 1; i >= 0; i-- )
	{
		for ( int32_t j = n_consumptionArray[i].GetFlowArray().size() - 1; j >= 0; j-- )
		{
			AddFlowInCapacityMap(n_consumptionArray[i].GetFlowArray()[j], useCap);
		}
	}

	for ( int32_t i = 0; i < mNumOfNodes; i++ )
	{
		for ( int32_t j = 0; j < mNumOfNodes; j++ )
		{
			if ( useCap[i][j] < 0 )
			{
				return false;
			}
		}
	}
	return true;
}

int32_t
NetLib::NetFlowSolution::CalculateSumCost( const std::vector< ConsumptionNode >& n_consumptionArray, const Map& n_costMap ) const
{
	int32_t sumCost = 0;

	std::vector< bool > serverArray;
	serverArray.assign(mNumOfNodes, false);

	int32_t flowValue = 0;
	int32_t cost = 0;
	for ( int32_t i = n_consumptionArray.size() - 1; i >= 0; i-- )
	{
		const FlowArray& re_flowArray = n_consumptionArray[i].GetFlowArray();
		for ( int32_t j = re_flowArray.size() - 1; j >= 0; j-- )
		{
			flowValue = re_flowArray[j].GetFlowValue();
			serverArray[re_flowArray[j][0].GetId()] = true;
			
			const Flow& re_flow = re_flowArray[j];
			for ( int32_t k = 0; k < re_flow.size() - 1; k++ )
			{
				cost = n_costMap[re_flow[k].GetId()][re_flow[k + 1].GetId()];
				sumCost += cost * flowValue;
			}
		}
	}

	for ( int32_t i = serverArray.size() - 1; i >= 0; i-- )
	{
		if ( serverArray[i] )
		{
			sumCost += mServerCost;
		}
	}

	return sumCost;
}

int32_t
NetLib::NetFlowSolution::CalculateSumCost( const std::vector< ConsumptionNode >& n_consumptionArray, const Map& n_costMap, const NodeArray& n_nodeArray ) const
{
	int32_t sumCost = 0;

	//std::vector< bool > serverArray;
	//serverArray.assign(mNumOfNodes, false);

	int32_t flowValue = 0;
	int32_t cost = 0;
	for (int32_t i = n_consumptionArray.size() - 1; i >= 0; i--)
	{
		const FlowArray& re_flowArray = n_consumptionArray[i].GetFlowArray();
		for (int32_t j = re_flowArray.size() - 1; j >= 0; j--)
		{
			flowValue = re_flowArray[j].GetFlowValue();
			//serverArray[re_flowArray[j][0].GetId()] = true;

			const Flow& re_flow = re_flowArray[j];
			for ( int32_t k = 0; k < re_flow.size() - 1; k++ )
			{
				cost = n_costMap[re_flow[k].GetId()][re_flow[k + 1].GetId()];
				sumCost += cost * flowValue;
			}
		}
	}
	
	for (int32_t i = n_nodeArray.size() - 1; i >= 0; i--)
	{
		if (n_nodeArray[i].IsServer())
		{
			sumCost += mServerCost;
		}
	}

	return sumCost;
}

void
NetLib::NetFlowSolution::SetAllNodeAsServer(NodeArray& re_allNodeArray)
{
	for ( int32_t i = re_allNodeArray.size() - 1; i >= 0; i-- )
	{
		re_allNodeArray[i].SetAsServer();
	}
}



void
NetLib::NetFlowSolution::DeleteServerAndFlows( int32_t deleteServerID, NodeArray& re_NodeArray, std::vector< ConsumptionNode >& re_ConsumptionArray, Map& re_CapacityMap ) const
{

	re_NodeArray[deleteServerID].SetAsCommon();

	for ( int32_t i = re_ConsumptionArray.size() - 1; i >= 0; i--)
	{
		ConsumptionNode& re_ConsumptionNode = re_ConsumptionArray[i];
		for ( int32_t j = (re_ConsumptionNode.GetFlowArray().size() - 1); j >= 0; j-- )
		{
			
			int32_t size;
			if (re_ConsumptionNode.GetFlowArray().size() == 0)
			{
				size = re_ConsumptionNode.GetFlowArray().size() - 1;
			}
			
			if ( re_ConsumptionNode.GetFlowArray()[j][0].GetId() == deleteServerID )
			{
				DeleteFlowAndUpdataCap(re_ConsumptionNode.GetFlowArray()[j], re_ConsumptionArray, re_CapacityMap);
			}
		}
	}
}

#define USE_MODE			0

void
NetLib::NetFlowSolution::SelectServer( NodeArray& nodeArray )
{
	//mark the node connect to the consumption node
	std::vector< bool > consumptionNodes;
	consumptionNodes.assign(mNumOfNodes, false);
	for ( int32_t i = mConsumptionNodeArray.size() - 1; i >= 0; i-- )
	{
		consumptionNodes[mConsumptionNodeArray[i].GetConnectNodeId()] = true;
	}
#if (USE_MODE == 0)
	const int32_t numofClosest = 2;
#else
	const int32_t numofClosest = 1;
#endif
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//start: find the route ,closest consumption node to consumption node
	std::vector< Route > routeArray;
	routeArray.reserve(mNumOfConsumNode);
	Route tempRoute;
	//int32_t closestConsumerID;
	//int32_t closestConsumerID2;
	std::vector<int32_t> closestID;
	for ( int32_t i = 0; i < (int32_t)consumptionNodes.size(); i++ )
	{
		if ( consumptionNodes[i] )
		{
			closestID.clear();
			consumptionNodes[i] = false;
			for (int32_t j = 0; j < numofClosest; j++)
			{
				NetLib::DijkstraOnePointOfSet(i, consumptionNodes, mCostMap, tempRoute);
				routeArray.push_back(tempRoute);
				closestID.push_back(tempRoute.begin()->GetId());
				consumptionNodes[tempRoute.begin()->GetId()] = false;
			}

			for (int32_t j = 0; j < numofClosest; j++)
			{
				consumptionNodes[closestID[j]] = true;
			}
			consumptionNodes[i] = true;
		}
	}
	//end: find the route ,closest consumption node to consumption node
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// count nodes in consumer-consumer 
	std::vector<int32_t> count_routeInC2C;
	count_routeInC2C.assign(mNumOfNodes, 0);
	//set the node to server in all route
	for ( int32_t i = routeArray.size() - 1; i >= 0; i-- )
	{
		for ( int32_t j = routeArray[i].size() -1; j >= 0; j-- )
		{
			count_routeInC2C[routeArray[i][j].GetId()]++;
		}
	}


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//start: count min cost server node in c-c route

	//init require flows array; init node connect to consumer node
	std::vector< int32_t > requireFlows;
	std::vector< int32_t > nodeConnectToConsumerNode;
	requireFlows.assign(mNumOfConsumNode, 0);
	nodeConnectToConsumerNode.assign(mNumOfNodes, -1);
	for ( int32_t i = requireFlows.size() - 1; i >= 0; i-- )
	{
		requireFlows[i] = mConsumptionNodeArray[i].GetFlowRequirement();
		nodeConnectToConsumerNode[mConsumptionNodeArray[i].GetConnectNodeId()] = mConsumptionNodeArray[i].GetId();
	}
#if (USE_MODE == 0)
	//find server node
	std::vector<int32_t> count_minCostServerInC2C;
	count_minCostServerInC2C.assign(mNumOfNodes, 0);

	//start/end id in route
	int32_t startID, endID;

	//start/end consumer node requirement flow
	int32_t startRequireFlow, endRequireFlow;

	//route cost (node-node)
	std::vector<int32_t> routeCost;
	routeCost.reserve(10);

	for ( int32_t i = routeArray.size() - 1; i >= 0; i-- )
	{
		routeCost.clear();
		const Route& re_route = routeArray[i];

		startID = re_route.begin()->GetId();
		endID = (re_route.end() - 1)->GetId();

		startRequireFlow = requireFlows[nodeConnectToConsumerNode[startID]];
		endRequireFlow = requireFlows[nodeConnectToConsumerNode[endID]];

		//get the node-node cost
		for ( int32_t j = 0; j < (int32_t)re_route.size() - 1; j++ )
		{
			routeCost.push_back(mCostMap[re_route[j].GetId()][re_route[j + 1].GetId()]);
		}

		//find one server position when min cost
		int32_t minCost = INT32_MAX;
		int32_t minIndex = -1;
		int32_t costToBegin, costToEnd;
		int32_t tempCost;
		for (int32_t j = 0; j < (int32_t)re_route.size(); j++)
		{
			RouteCostTobeginAndEnd(routeCost, j, costToBegin, costToEnd);
			tempCost = startRequireFlow * costToBegin + endRequireFlow * costToEnd + mServerCost;
			if (tempCost < minCost)
			{
				minCost = tempCost;
				minIndex = j;
			}
		}

		if (minCost >= mServerCost * 2)
		{
			count_minCostServerInC2C[startID] += 1;
			count_minCostServerInC2C[endID] += 1;
		}
		else
		{
			count_minCostServerInC2C[re_route[minIndex].GetId()] += 1;
		}
	}

	//end: count min cost server node in c-c route
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#else

	std::vector<int32_t> count_minCostServerIn3C;
	count_minCostServerIn3C.assign(mNumOfNodes, 0);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::vector<ConsumptionNode> loc_Consumers;
	Map loc_remainCap;
	NodeArray loc_servers = mAllNodeArray;
	int32_t loc_Cost;

	int32_t times = 0;
	for ( int32_t m = 0; m < routeArray.size()/ numofClosest; m++ )
	{
		times++;
#ifdef DEBUG_PRINTF
		printf("select time:%d\n", times);
#endif
		CreatRemainCapacityMap(mNumOfNodes, mLinkMap, loc_remainCap);
		ResetServerArray(loc_servers);
		loc_Consumers.clear();

		tempRoute = routeArray[numofClosest * m + 0];
		loc_Consumers.push_back(mConsumptionNodeArray[nodeConnectToConsumerNode[(tempRoute.end() - 1)->GetId()]]);
		for (int32_t j = 0; j < numofClosest; j++)
		{
			tempRoute = routeArray[numofClosest * m + j];
			loc_Consumers.push_back(mConsumptionNodeArray[nodeConnectToConsumerNode[tempRoute[0].GetId()]]);
			for (int32_t k = tempRoute.size() - 1; k >= 0; k--)
			{
				loc_servers[tempRoute[k].GetId()].SetAsServer();
			}
		}
		if (!CalculateFlows(loc_servers, loc_Consumers, loc_remainCap, loc_Cost))
			assert(false);
		int32_t delID;
		while (DelOneServerToMinCost(loc_Consumers, loc_servers, loc_remainCap, loc_Cost, delID));

		std::vector<int32_t> selectServersID;
		selectServersID.reserve(10);
		for (int32_t i = loc_Consumers.size() - 1; i >= 0; i--)
		{
			const std::vector<Flow>& reFlows = loc_Consumers[i].GetFlowArray();
			for (int32_t j = reFlows.size() - 1; j >= 0; j--)
			{
				selectServersID.push_back(reFlows[j][0].GetId());
			}
		}
		CommonUtils::DeleteRepeatEle(selectServersID);

		for (int32_t i = selectServersID.size() - 1; i >= 0; i--)
		{
			count_minCostServerIn3C[selectServersID[i]]++;
		}
	}
#endif
	
	//set server
#if (USE_MODE == 0)
#if (0)
	std::vector<int32_t> count;
	count.assign(mNumOfNodes,0);
	for (int32_t i = count.size() - 1; i >= 0; i--){
		count[i] = count_routeInC2C[i] + count_minCostServerInC2C[i];
	}
	for (int32_t i = count.size() - 1; i >= 0; i--) {
		//if (count[i] >= numofClosest * 2) {
		if (count[i] >= 3) {
			nodeArray[i].SetAsServer();
		}
	}
#else
	//set server
	//if number of nodes > 500, set server by count_routeInC2C and count_minCostServerInC2C
	//if number of nodes <= 500, set all consumer nodes to server, after complete first ite, add server by mServerC2C
	mServerC2C = nodeArray;
	mC2CInfo = count_routeInC2C;
	mCInfo = count_minCostServerInC2C;
	mSumInfo.assign(mNumOfNodes, 0);
	for (int32_t i = 0; i < mNumOfNodes; i++)
	{
		mSumInfo[i] = mC2CInfo[i] + mCInfo[i];
	}

	for (int32_t i = count_routeInC2C.size() - 1; i >= 0; i--){
#if (0)
		if (count_routeInC2C[i] >= numofClosest * 2){
			if (mNumOfNodes > THRESHOLD_NUMBER_OF_NODES) {
				nodeArray[i].SetAsServer();
			}
		}
#endif
		if (count_routeInC2C[i] >= numofClosest * 3) {
			mServerC2C[i].SetAsServer();
		}
	}
	printf("number of server C2C: %d \n", GetNumOfServer(nodeArray));
	printf("number of server C2C addition: %d \n", GetNumOfServer(mServerC2C));

#if (1)
	if (mNumOfNodes > THRESHOLD_NUMBER_OF_NODES) {
		int th_minCostServerInC2C = 1;
		if (mServerCost > THRESHOLD_COST_OF_SERVER) {
			th_minCostServerInC2C = 2;
		}
		for (int32_t i = count_minCostServerInC2C.size() - 1; i >= 0; i--) {
			if (count_minCostServerInC2C[i] >= th_minCostServerInC2C) {
				nodeArray[i].SetAsServer();
			}
		}
	}
	else {
		// set all consumer nodes to server, if number of node <= 500
		for (int32_t i = mConsumptionNodeArray.size() - 1; i >= 0; i--) {
			nodeArray[mConsumptionNodeArray[i].GetConnectNodeId()].SetAsServer();
		}
	}
#endif
#endif
#else
	//set server
	for (int32_t i = count_routeInC2C.size() - 1; i >= 0; i--)
	{
		if (count_routeInC2C[i] >= numofClosest * 2)
		{
			nodeArray[i].SetAsServer();
		}
	}

	for (int32_t i = count_minCostServerIn3C.size() - 1; i >= 0; i--){
		if (count_minCostServerIn3C[i] >= 1){
			nodeArray[i].SetAsServer();
		}
	}
#endif

#if (0)
	while (1)
	{
		ResetConsumptionNodeArray(mConsumptionNodeArray);
		CreatRemainCapacityMap(mNumOfNodes, mLinkMap, mRemainCapacityMap);

		if (!CalculateFlows(nodeArray, mConsumptionNodeArray, mRemainCapacityMap, mMinCost)) {
			for (int32_t i = mConsumptionNodeArray.size() - 1; i >= 0; i--) {
				if (mConsumptionNodeArray[i].GetUnassignFlow() != 0) {
					nodeArray[mConsumptionNodeArray[i].GetConnectNodeId()].SetAsServer();
					printf("init server has not solution, add server\n");
					break;
				}
			}
		}
		else {
			break;
		}
	}
#endif
	InitFlowsAndCapMapByServers(nodeArray, mConsumptionNodeArray, mRemainCapacityMap, mCost);
}

void 
NetLib::NetFlowSolution::InitFlowsAndCapMapByServers(NodeArray& n_servers, std::vector<ConsumptionNode>& n_consumers, Map& n_capMap, int32_t& n_cost) const
{
	while (1)
	{
		ResetConsumptionNodeArray(n_consumers);
		CreatRemainCapacityMap(mNumOfNodes, mLinkMap, n_capMap);

		if (!CalculateFlows(n_servers, n_consumers, n_capMap, n_cost)) {
			for (int32_t i = mConsumptionNodeArray.size() - 1; i >= 0; i--) {
				if (mConsumptionNodeArray[i].GetUnassignFlow() != 0) {
					n_servers[n_consumers[i].GetConnectNodeId()].SetAsServer();
#ifdef PRINTF_LOG
					printf("init server has not solution, add server\n");
#endif
					break;
				}
			}
		}
		else {
			break;
		}
	}
}

void
NetLib::NetFlowSolution::RouteCostTobeginAndEnd(const std::vector<int32_t> n_cost, const int32_t Index, int32_t& costToBegin, int32_t& costToEnd) const
{
	costToBegin = 0;
	costToEnd = 0;
	for ( int32_t i = 0; i < (int32_t)n_cost.size(); i++ )
	{
		costToEnd += n_cost[i];
		if ( i < Index )
		{
			costToBegin = costToEnd;
		}
	}
	costToEnd -= costToBegin;
}

bool 
NetLib::NetFlowSolution::DelOneServerToMinCost(std::vector<ConsumptionNode>& n_consumerNodes, NodeArray& n_servers, NetLib::Map& n_remainCapMap, int32_t& n_cost, int32_t& n_deleteID) const
{
	n_deleteID = -1;

	std::vector<ConsumptionNode> loc_minCost_Consumers;
	Map loc_minCost_remainCap;
	int32_t loc_minCost = n_cost;
	int32_t loc_deleteServerID = -1;

	std::vector<ConsumptionNode> loc_Consumers;
	Map loc_remainCap;
	int32_t loc_Cost;

	//find the used server node, try to be deleted
	std::vector<bool> usedServer;
	usedServer.assign(mNumOfNodes, false);
	int32_t serverID = 0;
	for (int32_t i = n_consumerNodes.size() - 1; i >= 0; i--)
	{
		for (int32_t j = n_consumerNodes[i].GetFlowArray().size() - 1; j >= 0; j--)
		{
			serverID = n_consumerNodes[i].GetFlowArray()[j][0].GetId();
			usedServer[serverID] = true;
		}
	}

	for (int32_t i = usedServer.size() - 1; i >= 0; i--)
	{
		if (usedServer[i] == true)
		{
			loc_Consumers = n_consumerNodes;
			loc_remainCap = n_remainCapMap;
#if (0)
			CreatRemainCapacityMap(mNumOfNodes, mLinkMap, loc_RemainCapacityMap);
			ResetConsumptionNodeArray(loc_ConsumptionNodeArray);
			mAllNodeArray[i].SetAsCommon();
#else
			DeleteServerAndFlows(i, n_servers, loc_Consumers, loc_remainCap);
#endif
			if (CalculateFlows(n_servers, loc_Consumers, loc_remainCap, loc_Cost))
			{
				if (loc_Cost < loc_minCost)
				{
					loc_minCost = loc_Cost;
					loc_minCost_Consumers = loc_Consumers;
					loc_minCost_remainCap = loc_remainCap;
					loc_deleteServerID = i;
				}
			}
			n_servers[i].SetAsServer();
		}
	}

	if (loc_deleteServerID == -1)
	{
		n_deleteID = -1;
		return false;
	}

	n_consumerNodes = loc_minCost_Consumers;
	n_remainCapMap = loc_minCost_remainCap;
	n_servers[loc_deleteServerID].SetAsCommon();
	n_cost = loc_minCost;
	n_deleteID = loc_deleteServerID;
	return true;
}

void 
NetLib::NetFlowSolution::GetUsedServer(const std::vector<ConsumptionNode>& consumerArray, std::vector<bool>& usedServer) const
{
	usedServer.assign(mNumOfNodes, false);
	int32_t serverID = 0;
	for (int32_t i = mConsumptionNodeArray.size() - 1; i >= 0; i--)
	{
		for (int32_t j = mConsumptionNodeArray[i].GetFlowArray().size() - 1; j >= 0; j--)
		{
			serverID = mConsumptionNodeArray[i].GetFlowArray()[j][0].GetId();
			usedServer[serverID] = true;
		}
	}
}

void 
NetLib::NetFlowSolution::GetServerFromInfo(const std::vector<int32_t>& C2CInfo, const int32_t th, std::vector<bool>& servers)
{
	servers.assign(C2CInfo.size(), false);
	for (int32_t i = C2CInfo.size() - 1; i >= 0; i--) {
		if (C2CInfo[i] >= th) {
			servers[i] = true;
		}
	}
}