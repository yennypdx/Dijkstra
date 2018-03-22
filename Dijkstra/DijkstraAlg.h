#include "stdafx.h"
#pragma once

#ifndef DJIKSTRA_H
#define DIJKSTRA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <iterator>
#include <list>
#include "ShortestPath.h"
#include "uGraph.h"
using namespace std;
const double IFIVE = 65.0;
const double HWY = 55.0;

class Dijkstra
{
public:
	vector<string> DataParsing(string inLine, const string delim);
	void OverloadingDjikstraGraph(string inFile);
public:
	ShortestPath FindShortestPath(string inOrigin, string inDestination);
	void ProcessTables(int * distance, int * predecessor, Vertex<string, string> * vertices, int edgeToNextVerticesIndex);
	ShortestPath GettingRoute(int * distance, int * predecessor, Vertex<string, string> * vertices, int destinationIndex);
	double GetTotalTime(int * distance, int * predecessor, Vertex<string, string> * vertices, stack<int> * RoutingIndexStack);
public:
	uGraph<string, string> & getDijkstraGraph() { return m_Dijkstra; }

private:
	uGraph<string, string> m_Dijkstra;
};

#endif
