#pragma once

#ifndef UNDIRGRAPH_H
#define UNDIRGRAPH_H

#include <iostream>
#include <list>
#include <stack>
#include <queue>
#include <iterator>
#include <vector>
#include "Vertex.h"
#include "Edge.h"
using namespace std;

template<typename V, typename E>
class Vertex;

template<typename V, typename E>
class Edge;

template<typename V, typename E>
class uGraph
{
public:
	uGraph();
	uGraph(V inVertex);
	~uGraph();
public:
	uGraph(const uGraph<V, E> & inCopy);
	uGraph<V, E> & operator=(const uGraph<V, E> & inRhs);
public:
	void insertVertex(V inData);
	void deleteVertex(V inData);
	void insertEdge(V fromOrigin, V toDestination, E inData1, E inData2);
	void deleteEdge(V fromOrigin, V toDestination, E inData1, E inData2);
public:
	void breadthfirstSearch(V inData);
	void depthfirstSearch(V inData);
public:
	int getVertCount() { return m_vertCount; }
	list<Vertex<V, E>> getVertList() { return m_adjacent; }
	Vertex<V, E> * getVert(V inData);
	bool isEmpty();

protected:
	int m_vertCount;
	list<Vertex<V, E>> m_adjacent;
private:
	void(*m_visit) (V inData);
};

template<typename V, typename E>
inline uGraph<V, E>::uGraph()
{
	m_vertCount = 0;
	m_adjacent = list<Vertex<V, E>>();
}

template<typename V, typename E>
inline uGraph<V, E>::uGraph(V inVertex)
{
	m_adjacent = list<Vertex<V, E>>(inVertex);
}

template<typename V, typename E>
inline uGraph<V, E>::~uGraph()
{
	if (!m_adjacent.empty()) {
		m_adjacent.clear();
	}
}

template<typename V, typename E>
inline uGraph<V, E>::uGraph(const uGraph<V, E>& inCopy)
{
	if (inCopy.isEmpty() == false) {
		m_vertCount = inCopy.m_vertCount;

		list<int>::iterator it;
		for (it = m_adjacent.begin(); it != m_adjacent.end(); it++) {
			it = inCopy;
		}
	}
}

template<typename V, typename E>
inline uGraph<V, E>& uGraph<V, E>::operator=(const uGraph<V, E>& inRhs)
{
	if (this != &inRhs) {
		m_vertCount = inRhs.m_vertCount;
		m_adjacent = inRhs.m_adjacent;
	}
	return *this;
}

template<typename V, typename E>
inline void uGraph<V, E>::insertVertex(V inData)
{
	m_adjacent.emplace_back(inData);
	m_vertCount++;
}

template<typename V, typename E>
inline void uGraph<V, E>::deleteVertex(V inData)
{
	bool deleted = false;
	list<Vertex<V, E>>::iterator it = m_adjacent.begin();
	while (deleted == false && it != m_adjacent.end())
	{
		Vertex<V, E> * tempVert = &(*it);
		if (inData == tempVert->getVertData())
		{
			m_adjacent.erase(it);
			deleted = true;
		}
		else
		{
			it++;
		}
	}

	if (deleted == false)
	{
		throw Exception("Exception caught: Vertex doesn't exist.");
	}
}

template<typename V, typename E>
inline void uGraph<V, E>::insertEdge(V fromOrigin, V toDestination, E inData1, E inData2)
{
	Vertex<V, E> * vOriginPtr = getVert(fromOrigin);
	Vertex<V, E> * vDestPtr = getVert(toDestination);

	vOriginPtr->insertEdge(vDestPtr, inData1);
	vOriginPtr->insertEdge(vDestPtr, inData2);

}

template<typename V, typename E>
inline void uGraph<V, E>::deleteEdge(V fromOrigin, V toDestination, E inData1, E inData2)
{
	Vertex<V, E> * vOriginPtr = getVert(fromOrigin);
	Vertex<V, E> * vDestPtr = getVert(toDestination);

	if (vOriginPtr != nullptr && vDestPtr != nullptr)
	{
		vOriginPtr->deleteEdge(vDestPtr, inData1);
		vOriginPtr->deleteEdge(vDestPtr, inData2);
	}
	else
	{
		throw runtime_error("Exception caught: Attempting to delete edge from invalid vertex.");
	}
}

template<typename V, typename E>
inline Vertex<V, E> * uGraph<V, E>::getVert(V inData)
{
	Vertex<V, E> *outVert = nullptr;
	bool found = false;
	list<Vertex<V, E>>::iterator it = m_adjacent.begin();
	while (it != m_adjacent.end() && found == false) {
		Vertex<V, E> *tempVert = &(*it);
		V tempValue = tempVert->getVertData();
		if (inData == tempValue) {
			outVert = &(*it);
			found = true;
		}
		else
		{
			it++;
		}
	}

	return outVert;
}

template<typename V, typename E>
inline bool uGraph<V, E>::isEmpty()
{
	bool empty;
	if (m_vertCount == 0) {
		empty = true;
	}
	return empty;
}

template<typename V, typename E>
inline void uGraph<V, E>::depthfirstSearch(V inData)
{
	list<Vertex<V, E>*> queueVert;
	queueVert.emplace_back(getStartingVert());
	bool found = false;
	while (found == false && !queueVert.empty())
	{
		//Get the next Vertex
		Vertex<V, E>* tempVertPtr = queueVert.back();
		if (tempVertPtr->getVertData() == inData)
		{
			found = true;
		}
		if (tempVertPtr->getProcessed() == false)
		{
			cout << tempVertPtr->getVertData() << endl;
			tempVertPtr->setProcessed(true);
			list<Edge<V, E>> edges = tempVertPtr->getVertEdges();
			queueVert.pop_back();

			while (!edges.empty())
			{
				Edge<V, E> tempEdge = edges.front();
				queueVert.emplace_back(tempEdge.getEdgeDestination());
				edges.pop_front();
			}
		}
		else
		{
			queueVert.pop_back();
		}
	}
}

template<typename V, typename E>
inline void uGraph<V, E>::breadthfirstSearch(V inData)
{
	bool found = false;
	list<Vertex<V, E>*> queueVert;
	queueVert.emplace_back(getStartingVert());
	while (found == false && !queueVert.empty())
	{
		//Get the next Vertex
		Vertex<V, E>* tempVertPtr = queueVert.front();
		if (tempVertPtr->getVertData() == inData)
		{
			found = true;
		}
		if (tempVertPtr->getProcessed() == false)
		{
			cout << tempVertPtr->getVertData() << endl;
			tempVertPtr->setProcessed(true);
			list<Edge<V, E>> edges = tempVertPtr->getVertEdges();

			while (!edges.empty())
			{
				Edge<V, E> tempEdge = edges.front();
				queueVert.emplace_back(tempEdge.getEdgeDestination());
				edges.pop_front();
			}
		}
		else
		{
			//cout << "Already Processed" << endl;
		}
		queueVert.pop_front();
	}
}

#endif