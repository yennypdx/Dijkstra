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
	const V& getVertData() { return m_vertData; }
	const bool& getProcessed() { return processed; }
	list<Edge<V, E>> getVertEdges() { return m_vertEdges; }
	Edge<V, E> * getEdge(Vertex<V, E> * toDestination, E inData);
public:
	void deleteEdge(Vertex<V, E> * toDestination, E inData);
	void insertEdge(Vertex<V, E> * toDestination, E inData);
	bool edgeExists(Vertex<V, E> * toDestination, E inData);

private:
	V  m_vertData;
	bool processed;
	list<Edge<V, E>> m_vertEdges;
};

template<typename V, typename E>
inline Vertex<V, E>::Vertex()
{
	m_inDegree = 0;
	m_outDegree = 0;
	m_vertData = 0;
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

	for (list<Edge<V, E>>::iterator it = m_vertEdges.begin(); it != m_vertEdges.end(); ++it) {
		m_vertEdges.emplace_back(*it);
	}
}

template<typename V, typename E>
inline Vertex<V, E>& Vertex<V, E>::operator=(const Vertex<V, E> inRhs)
{
	if (this != &inRhs) {
		m_vertData = inRhs.m_vertData;
	}
	return *this;
}

template<typename V, typename E>
inline void Vertex<V, E>::deleteEdge(Vertex<V, E> * toDestination, E inData)
{
	if (edgeExists(toDestination, inData) == true)	{
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
inline void Vertex<V, E>::insertEdge(Vertex<V, E> * toDestination, E inData)
{
	if (edgeExists(toDestination, inData) == false)	{
		m_vertEdges.emplace_back(toDestination, inData);
	}
	else	{
		throw runtime_error("Exception : Edge already exists.");
	}
}

template<typename V, typename E>
inline bool Vertex<V, E>::edgeExists(Vertex<V, E> * toDestination, E inData)
{
	bool exists = false;
	Edge<V, E> * tempEdge = getEdge(toDestination, inData);

	if (tempEdge != nullptr)
	{
		exists = true;
	}

	return exists;
}

template<typename V, typename E>
inline Edge<V, E> * Vertex<V, E>::getEdge(Vertex<V, E> * toDestination, E inData)
{
	Edge<V, E> *outEdge = nullptr;
	bool found = false;
	list<Edge<V, E>>::iterator it = m_vertEdges.begin();

	while (found == false & it != m_vertEdges.end()) {
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