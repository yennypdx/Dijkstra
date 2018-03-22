#include "stdafx.h"
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
	uGraph(V inLocation);
	~uGraph();
public:
	uGraph(const uGraph<V, E> & inCopy);
	uGraph<V, E> & operator=(const uGraph<V, E> & inRhs);
public:
	void insertVertex(V inLocation);
	void deleteVertex(V inLocation);
	void insertEdgeGraph(V fromOrigin, V toDestination, E path, int miles);
	void deleteEdgeGraph(V fromOrigin, V toDestination, E path, int miles);
public:
	void breadthfirstSearch(V inLocation);
	void depthfirstSearch(V inLocation);
public:
	int getVertCount() { return m_vertCount; }
	list<Vertex<V, E>> getVertList() { return m_adjacent; }
	Vertex<V, E> * getVert(V inLocation);
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
inline uGraph<V, E>::uGraph(V inLocation)
{
	m_adjacent = list<Vertex<V, E>>(inLocation);
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
inline void uGraph<V, E>::insertVertex(V inLocation)
{
	bool exist = false;
	list<Vertex<V, E>>::iterator it = m_adjacent.begin();
	while (exist == false && it != m_adjacent.end()) {
		Vertex<V, E> * tempVert = &(*it);
		if (inLocation == tempVert->getVertData()) {
			exist = true;
		}
		else {
			it++;
		}
	}

	if (exist == false)	{
		m_adjacent.emplace_back(inLocation);
		m_vertCount++;
	}
}

template<typename V, typename E>
inline void uGraph<V, E>::deleteVertex(V inLocation)
{
	bool deleted = false;
	list<Vertex<V, E>>::iterator it = m_adjacent.begin();
	while (deleted == false && it != m_adjacent.end())
	{
		Vertex<V, E> * tempVert = &(*it);
		if (inLocation == tempVert->getVertData())
		{
			m_adjacent.erase(it);
			deleted = true;
		}
		else
		{
			it++;
		}
	}

	if (deleted == false)	{
		throw runtime_error("Exception caught: Vertex doesn't exist.");
	}
}

template<typename V, typename E>
inline void uGraph<V, E>::insertEdgeGraph(V fromOrigin, V toDestination, E path, int miles)
{
	Vertex<V, E> *vOrigin = getVert(fromOrigin);
	Vertex<V, E> *vDestination = getVert(toDestination);
	vOrigin->insertEdge(path, miles, vDestination);
}

template<typename V, typename E>
inline void uGraph<V, E>::deleteEdgeGraph(V fromOrigin, V toDestination, E path, int miles)
{
	Vertex<V, E> *vOrigin = getVert(fromOrigin);
	Vertex<V, E> *vDestination = getVert(toDestination);

	if (vOrigin != nullptr && vDestination != nullptr)	{
		vOrigin->deleteEdge(path, miles, vDestination);
	}
	else	{
		throw runtime_error("Exception caught: Attempting to delete edge from invalid vertex.");
	}
}

template<typename V, typename E>
inline Vertex<V, E> * uGraph<V, E>::getVert(V inData)
{
	Vertex<V, E> *outVert = nullptr;
	bool found = false;
	list<Vertex<V, E>>::iterator it = m_adjacent.begin();

	while (it != m_adjacent.end() && found != true) {
		Vertex<V, E> *tempVert = &(*it);
		V tempValue = tempVert->getVertData();
		if (inData == tempValue) {
			//outVert = &(*it);
			outVert = tempVert;
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