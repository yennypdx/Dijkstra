#include "stdafx.h"
#pragma once

#ifndef DJIKSTRA_H
#define DIJKSTRA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
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
	void PopulatingDjikstraGraph(string inFile);
	ShortestPath FindShortestPath(string inOrigin, string inDestination);
public:
	uGraph<string, string> & getDijkstraGraph() { return m_Dijkstra; }

private:
	uGraph<string, string> m_Dijkstra;
};

#endif
