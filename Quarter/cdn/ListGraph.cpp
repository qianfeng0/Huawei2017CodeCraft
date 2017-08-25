#include "ListGraph.h"

NetLib::ListGraph::ListGraph()
{

}

NetLib::ListGraph::ListGraph(int32_t numOfVertex)
{
	Init(numOfVertex);
}

NetLib::ListGraph::~ListGraph()
{

}

void
NetLib::ListGraph::Init(int32_t numOfVertex)
{
	this->clear();
	this->resize(numOfVertex);
	for (int32_t i = 0; i < numOfVertex; i++)
	{
		this->at(i).reserve(20);
		//this->at(i).push_back(stVertex(i, 0, INT32_MAX));
	}
}

void
NetLib::ListGraph::AddEdge(int32_t start, int32_t end, int32_t cost, int32_t cap)
{
	this->at(start).push_back(stVertex(end, cost, cap));
}