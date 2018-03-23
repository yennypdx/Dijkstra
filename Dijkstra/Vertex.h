#pragma once
#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <iterator>
#include <list>
#include "Edge.h"
using namespace std;

template<typename V, typename E>
class Edge;

template<typename V, typename E>
class Vertex
{
public:
	Vertex();
	Vertex(V inVertData);
	~Vertex() = default;
public:
	Vertex(const Vertex<V, E> & inCopy);
	Vertex<V, E> & operator=(const Vertex<V, E> inRhs);
public:
	void setVertData(V inData) { m_vertData = inData; }
	void setVertEdges(list<Edge<V, E>> inVertEdges) { m_vertEdges.emplace_back(inVertEdges); }
	void setProcessed(bool inProcessed) { processed = inProcessed; }
public:
	V getVertData() const { return m_vertData; }
	bool getProcessed() const { return processed; }
	list<Edge<V, E>> getVertEdges() { return m_vertEdges; }
	Edge<V, E> * getEdge(E inData, int inWeight, Vertex<V, E> * toDestination);
public:
	void deleteEdge(E inData, int inWeight, Vertex<V, E> * toDestination);
	void insertEdge(E inData, int inWeight, Vertex<V, E> * toDestination);
	bool edgeExists(E inData, int inWeight, Vertex<V, E> * toDestination);

private:
	V  m_vertData;
	bool processed;
	list<Edge<V, E>> m_vertEdges;
};

template<typename V, typename E>
inline Vertex<V, E>::Vertex()
{
	processed = false;
	m_vertEdges = list<Edge<V, E>>();
}

template<typename V, typename E>
inline Vertex<V, E>::Vertex(V inVertData)
{
	m_vertData = inVertData;
	processed = false;
	m_vertEdges = list<Edge<V, E>>();
}

template<typename V, typename E>
inline Vertex<V, E>::Vertex(const Vertex<V, E>& inCopy)
{
	m_vertData = inCopy.getVertData();
	processed = inCopy.getProcessed();

	for (list<Edge<V, E>>::const_iterator it = inCopy.m_vertEdges.begin(); it != inCopy.m_vertEdges.end(); ++it) {
		m_vertEdges.emplace_back(*it);
	}
}

template<typename V, typename E>
inline Vertex<V, E>& Vertex<V, E>::operator=(const Vertex<V, E> inRhs)
{
	if (this != &inRhs) {
		m_vertData = inRhs.m_vertData;
		processed = inRhs.getProcessed();

		for (list<Edge<V, E>>::const_iterator it = inRhs.m_vertEdges.begin(); it != inRhs.m_vertEdges.end(); ++it) {
			m_vertEdges.emplace_back(*it);
		}
	}
	return *this;
}

template<typename V, typename E>
inline void Vertex<V, E>::deleteEdge(E inData, int inWeight, Vertex<V, E> * toDestination)
{
	if (edgeExists(inData, inWeight, toDestination) == true)	{
		bool deleted = false;
		list<Edge<V, E>>::iterator it = m_vertEdges.begin();
		while (deleted == false && it != m_vertEdges.end())
		{
			Edge<V, E> * tempEdge = &(*it);
			if (toDestination->getVertData() == tempEdge->getEdgeDestination()->getVertData() && 
				inData == tempEdge->getEdgeData())	{
				m_vertEdges.erase(it);
				deleted = true;
			}
			else			{
				it++;
			}
		}
	} 
	else	{
		throw runtime_error("Exception : Edge doesn't exist.");
	}
}

template<typename V, typename E>
inline void Vertex<V, E>::insertEdge(E inData, int inWeight, Vertex<V, E> * toDestination)
{
	if (edgeExists(inData, inWeight, toDestination) == false)	{
		m_vertEdges.emplace_back(Edge<V, E>(inData, inWeight, toDestination));
	}
	else	{
		throw runtime_error("Exception : Edge already exists.");
	}
}

template<typename V, typename E>
inline bool Vertex<V, E>::edgeExists(E inData, int inWeight, Vertex<V, E> * toDestination)
{
	bool exists = false;
	Edge<V, E> * tempEdge = getEdge(inData, inWeight, toDestination);

	if (tempEdge != nullptr)
	{
		exists = true;
	}

	return exists;
}

template<typename V, typename E>
inline Edge<V, E> * Vertex<V, E>::getEdge(E inData, int inWeight, Vertex<V, E> * toDestination)
{
	Edge<V, E> *outEdge = nullptr;
	bool found = false;
	list<Edge<V, E>>::iterator it = m_vertEdges.begin();

	while (found == false && it != m_vertEdges.end()) {
		Edge<V, E> * tempEdge = &(*it);
		E tempValue = tempEdge->getEdgeData();
		if ((inData == tempValue) && toDestination->getVertData() == (tempEdge->getEdgeDestination()->getVertData())) {
			outEdge = &(*it);
			found = true;
		}
		else
		{
			it++;
		}
	}

	return outEdge;
}

#endif