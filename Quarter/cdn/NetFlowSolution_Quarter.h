#ifndef _NETFLOWSOLUTION_H_
#define _NETFLOWSOLUTION_H_

#include <stdint.h>
#include <vector>
#include <string>

#include "NetLibDefs.h"
#include "Link.h"
#include "ConsumptionNode.h"
#include "Node.h"
#include "Map.h"
#include "ListGraph.h"

namespace NetLib
{
	/*
	struct StStatistics
	{
		float density_consumptionNode;
		float average_consumptionRequireFlow;
		float average_linkCost;
	};
	*/
	/*
	struct stServerLevel {
		int outputFlow;
		int cost;

		stServerLevel(int flow, int c) : outputFlow(flow), cost(c) {};
	};
	*/

	class NetFlowSolution
	{
	public:

		NetFlowSolution( );

		NetFlowSolution( const int32_t numOfNode );

		~NetFlowSolution();

	public:
		void
		LoadNetQuarter(char* topo[], int line_num);

		void
		AddServer( int32_t new_serverId );

		void 
		CalculateWorstResult();

		std::string
		stringFlow();

		bool 
		Process();

		bool
		CalculateFlows( NodeArray& re_NodeArray,
						std::vector< ConsumptionNode >& re_ConsumptionNodeArray, 
						Map& re_RemainCapacityMap, 
						int32_t& out_cost,
						std::vector<bool>& serverRord) const;

		bool
		CalculateFlowsByConfirmServer( std::vector< int32_t > serverIDArray );

		void
		DeleteServerAndFlows( int32_t deleteServerID, NodeArray& re_NodeArray, std::vector< ConsumptionNode >& re_ConsumptionArray, Map& re_CapacityMap ) const;
	
		int32_t GetCost() const;
	public:
		/*
		const Link&
		GetLink( int32_t start, int32_t end ) const;

		const Link&
		GetLink( const stEdge& nEdge) const;
		*/
		const LinkMap&
		GetLinkMap() const;

		const Map&
		GetCostMap() const;

		const Map&
		GetRemainCapacityMap() const;

		const NodeArray&
		GetNodeArray() const;

	private:
		//~~~~~~~~~~~~~~~~~~~~~~
		//structor
		NetFlowSolution(const NetFlowSolution& newObj);

		NetFlowSolution&
		operator =(const NetFlowSolution& newObj);

	private:

		void InitAllNode( int32_t numOfNode );

		void InitLinkMap( int32_t numOfNode );

		void InitCostMap( int32_t numOfNode, const LinkMap& linkMap );

		void CreatRemainCapacityMap ( int32_t numOfNode, const LinkMap& linkMap, Map& out_capMap) const;

		void ResetConsumptionNodeArray(std::vector< ConsumptionNode >& n_ConsumptionNodeArray) const;

		void ResetServers(NodeArray& n_servers) const;

		void ResetServerArray(NodeArray& n_serverArray);

		void CopyInternalToLocal( std::vector< ConsumptionNode >& n_ConsumptionNodeArray, Map& n_RemainCapacityMap, NodeArray& n_nodeArray,int32_t& n_cost ) const;

		void CopyLocalToInternal( const std::vector< ConsumptionNode >& n_ConsumptionNodeArray, const Map& n_RemainCapacityMap, const NodeArray& n_nodeArray, const int32_t& n_cost);

	private:
		void AddFlowAndUpdataCap( const Flow& n_flow, std::vector< ConsumptionNode >& n_ConsumptionNodeArray, Map& n_remainCapacityMap, NodeArray& n_nodeArray ) const;
	
		void AddFlowInCapacityMap( const Flow& n_flow, Map& n_remainCapacityMap, NodeArray& n_nodeArray ) const;

		void DeleteFlowAndUpdataCap(const Flow n_flow, std::vector< ConsumptionNode >& n_ConsumptionNodeArray, Map& n_remainCapacityMap, NodeArray& n_nodeArray) const;

		void DeleteFlowInCapacityMap( const Flow& n_flow, Map& n_remainCapacityMap, NodeArray& n_nodeArray) const;

		bool CheckAllOverBandwidth( const Map& n_remainCapacityMap, std::vector< Route >& overRouteArray );

		bool CheckOneOverBandwidth( const Map& n_remainCapacityMap, Route& overRoute ) const;
		bool CheckOneOverServerFlow(const NodeArray& n_nodeArray, int32_t& serverID) const;

		bool CheckConsumptionFlow( const std::vector< ConsumptionNode >& n_consumptionArray, const Map& n_capMap, const NodeArray& n_nodeArray ) const;

		//int32_t CalculateSumCost( const std::vector< ConsumptionNode >& n_consumptionArray, const Map& n_costMap ) const;

		int32_t CalculateSumCost( const std::vector< ConsumptionNode >& n_consumptionArray, const Map& n_costMap, const NodeArray& n_nodeArray, bool onlyConsumersFlow) const;

		void SetAllNodeAsServer( NodeArray& re_allNodeArray );

		void SelectServer( NodeArray& nodeArray );
		void SelectServer(std::vector<int32_t>& serverArray);

		void RouteCostTobeginAndEnd(const std::vector<int32_t> n_cost, const int32_t Index, int32_t& costToBegin, int32_t& costToEnd) const;
#if (0)
		bool DelOneServerToMinCost(std::vector<ConsumptionNode>& n_consumerNodes, 
								   NodeArray& n_servers,
								   Map& n_remainCapMap,
								   int32_t& n_cost,
								   int32_t& n_deleteID);
#endif
		int32_t GetNumOfServer( NodeArray& n_servers) const;

		int32_t GetNumOfServer(std::vector<bool>& n_servers) const;

		void InitFlowsAndCapMapByServers(NodeArray& n_servers, std::vector<ConsumptionNode>& n_consumers, Map& n_capMap, int32_t& n_cost);

		void GetUsedServer(const std::vector<ConsumptionNode>& consumerArray,std::vector<bool>& usedServer) const;
		
		void GetServerFromInfo(const std::vector<int32_t>& C2CInfo, const int32_t th, std::vector<bool>& servers);

		void tryDelServer(const std::vector<ConsumptionNode>& n_consumers, const Map& n_remainCap, const NodeArray& n_servers, const int32_t n_cost, const std::vector<bool>& tryDelServer, std::vector<int32_t>& deltaCost);
	//~~~~~~~~~~~~~~~~~~~~
	//numbers
	private:

		int32_t mNumOfNodes;
		int32_t mNumOfLinks;
		int32_t mNumOfConsumNode;
		int32_t mServerCost;
		
		std::vector<stServerLevel>  mServerLevelArray;
		std::vector<int32_t> mDeployServerCostArray;

		//graph
		LinkMap mLinkMap;
		Map mCostMap;
		ListGraph mListGraph;

		std::vector< ConsumptionNode > mConsumptionNodeArray;
		Map mRemainCapacityMap;
		std::vector< int32_t > mServerArray;
		NodeArray mAllNodeArray;
		int32_t mCost;

		std::vector< ConsumptionNode > mMinCostConsumers;
		NodeArray mMinCostServers;
		Map mMinRemainCap;
		int32_t mMinCost;

		//bool mFirstIteComplete;
		NodeArray mServerC2C;

		std::vector<int32_t> mC2CInfo;
		std::vector<int32_t> mCInfo;
		std::vector<int32_t> mSumInfo;

		std::vector<bool> mServerRecord;
	};
}
#endif // !_NETMAP_H_
