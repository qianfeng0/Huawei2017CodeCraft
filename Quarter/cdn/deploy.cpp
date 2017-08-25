#include "deploy.h"
#include <stdio.h>

#include <string>
#include "Link.h"
#include "NetFlowSolution_Quarter.h"
#include "CommonUtils.h"
//#include "Dijkstra.h"
#include "ElapsedTime.h"

//你要完成的功能总入口
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
	timeStart();

	NetLib::NetFlowSolution netSoltion;
	netSoltion.LoadNetQuarter(topo, line_num );

#if (0)
	std::vector <int32_t> servers;
	//case5
	servers = { 0,3,22 };
	//case0
	//servers = { 7, 13, 15, 22, 37, 38, 43 };
	//case1
	//servers = { 6,7,13,17,35,41,48 };
	//case2
	//servers = {12,18,23,29,31,38,48};
	//case3
	//servers = { 10,22,26,29,35 };
	//case4
	//servers = { 12,15,20,22,26,37,48 };

	//step2
	//0-case0
	//servers = { 149,36,22,135,147,25,54,69,11,85,72,20,48,128,126,30,37,159,123,131,49,81,61,97,137,114,17,89,103,4,60,46,139,56,83,59,155,106,108,66 };
	//servers = { 4,11,17,20,22,25,30,36,37,46,49,54,56,57,60,61,66,69,72,81,83,85,89,97,103,106,108,114,123,126,128,131,135,137,139,147,155,159 };
	//0-case1
	//servers = { 17,62,25,4,135,122,153,119,139,57,8,63,147,34,109,29,42,126,76,118,9,70,120,44,90,15,154,35,149,142,67,28,103,80,130,146,85,10,75,102 };
	//servers = { 4,8,9,10,15,17,25,28,29,34,35,42,44,57,62,63,67,70,75,76,80,85,90,102,103,109,118,119,120,122,126,130,135,139,141,142,146,147,149,153,154 };
	//0-case2
	//servers = { 6,8,12,15,21,38,40,51,54,59,63,64,66,71,74,79,80,84,98,101,102,105,110,114,115,119,120,125,126,136,140,141,144,148,149,154,155,159 };
	if (netSoltion.CalculateFlowsByConfirmServer(servers))
	{
		printf("have solution \n");
	}
	else
	{
		printf("error solution\n");
	}
#else
	if (!netSoltion.Process())
	{
		printf("solution error!!\n");
		return;
	}

#endif

	std::string str = netSoltion.stringFlow();

	// 需要输出的内容
	//char * topo_file = (char *)"17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";
	const char * topo_file = str.c_str();

	//printf("%s\n", topo_file);
	printf("final cost : %d\n", netSoltion.GetCost());
	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(topo_file, filename);

}
