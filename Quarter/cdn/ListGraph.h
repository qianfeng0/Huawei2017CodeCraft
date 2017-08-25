#ifndef _LISTGRAPH_H_
#define _LISTGRAPH_H_

#include "NetLibDefs.h"

namespace NetLib 
{
	struct stVertex
	{
		int32_t id;
		int32_t cost;
		int32_t cap;

		stVertex() 
		: id(-1), cost(INT32_MAX), cap(0){

		}

		stVertex(int32_t nid, int32_t ncost, int32_t ncap)
		: id(nid), cost(ncost), cap(ncap){

		}

		int32_t GetId() const {
			return id;
		}
		int32_t GetCost() const {
			return cost;
		}
		int32_t GetCap() const{
			return cap;
		}

	};

	class ListGraph : public std::vector<std::vector<stVertex>>
	{
	public:
		ListGraph ();
		ListGraph (int32_t numOfVertex);
		~ListGraph ();

	private:
		ListGraph (const ListGraph& newObj);
		ListGraph& operator= (const ListGraph& newObj);
	
	public:
		void Init (int32_t numOfVertex);

		void AddEdge (int32_t start, int32_t end, int32_t cost, int32_t cap);
	};
}

#endif