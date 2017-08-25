#ifndef _NODE_H_
#define _NODE_H_

#include <stdint.h>

namespace NetLib
{
	/*
	namespace NodeTypeIDs
	{
		
		enum eNodeTypeID
		{
			kUnassign,
			kCommon,
			kServer,
			kConsumption,
		};
	}
	*/
	class Node
	{
	public:
		Node();

		Node( int32_t id, bool nIsServer );

		Node( const Node& newObj);

		~Node();

		Node&
		operator =(const Node& newObj);

	public:

		void Set( int32_t id, bool nIsServer );

		void SetAsServer();

		void SetAsCommon();

		int32_t GetId() const;

		bool IsServer() const;
		//NodeTypeIDs::eNodeTypeID GetType();

	private:
		int32_t mId;
		bool mIsServer;
	};
}

#endif