#include "stdafx.h"
#pragma once
#include "DijkstraAlg.h"

 vector<string> Dijkstra::DataParsing(string inLine, const string delim)
{
	vector<string> list;
	string str = inLine;
	size_t pos = 0;
	string token;

	while ((pos = str.find(delim)) != string::npos) {
		token = str.substr(0, pos);
		list.push_back(token);
		str.erase(0, pos + delim.length());
	}
	list.push_back(str);
	return list;
	//caller: vector<string> data = DataParsing(buffer, ",");
}

void Dijkstra::OverloadingDjikstraGraph(string inFile)
{
	ifstream dataFile;
	dataFile.open(inFile);
	string singleLine;

	if (dataFile.is_open()) {
		getline(dataFile, singleLine);

		while (dataFile)
		{
			vector<string> data = DataParsing(singleLine, ",");
			string origin = data[0];
			string destination = data[1];
			string path = data[2];
			int mileage = stoi(data[3]);

			//setting the origin and destination vertices
			m_Dijkstra.insertVertex(origin);
			m_Dijkstra.insertVertex(destination);

			//creating the edges
			m_Dijkstra.insertEdgeGraph(origin, destination, path, mileage);
			getline(dataFile, singleLine);
		} 
	}
	else {
		throw runtime_error("Cannot open file.");
	}
}

ShortestPath Dijkstra::FindShortestPath(string inOrigin, string inDestination)
{
	int numOfVerts = m_Dijkstra.getVertList().size();
	Vertex<string, string> * vertices = new Vertex<string, string>[numOfVerts];
	//initializers
	int initNum = -1;
	int originIndex = -1;
	int destinationIndex = -1;

	//arrays for dijkstra table
	int * distance = new int[numOfVerts];
	int * predecessor = new int[numOfVerts]; 
	
	//assigning index to vertices
	int i = 0;
	list<Vertex<string, string>> tempVerts = m_Dijkstra.getVertList();
	for (auto v : tempVerts) {
		vertices[i] = *&v;
		i++;
	}

	//initialize the two table with -1
	for (int i = 0; i < numOfVerts; i++) {
		distance[i] = initNum;
		predecessor[i] = initNum;
	}

	//find origin and destination
	for (int i = 0; i < numOfVerts; i++) {
		if (vertices[i].getVertData() == inOrigin) {
			originIndex = i;
		}
		if (vertices[i].getVertData() == inDestination) {
			destinationIndex = i;
		}
	}
	
	//zero the distance of the origin
	distance[originIndex] = 0;
	
	//get the edges of origin
	auto edgesOfOrigin = vertices[originIndex].getVertEdges();
	int edgeToNextVerticesIndex = 0;

	//update distance array for each edges
	for (auto edge : edgesOfOrigin) {
		for (int val = 0; val < edgesOfOrigin.size(); val++) {
			if (edge.getEdgeDestination()->getVertData() == vertices[val].getVertData()) {
				distance[val] = edge.getEdgeWeight();
				predecessor[val] = originIndex;
			}
		}
	}
	//update process status of origin
	vertices[originIndex].setProcessed(true);

	ProcessTables(distance, predecessor, vertices, edgeToNextVerticesIndex);
	GettingRoute(distance, predecessor, vertices, destinationIndex);

	//return ShortestPath
	ShortestPath routeInformation;
	routeInformation.setDistance(distance[destinationIndex]);

	return routeInformation;
}

void Dijkstra::ProcessTables(int * distance, int * predecessor, Vertex<string, string> * vertices, int edgeToNextVerticesIndex)
{
	bool vertProcess;
	//do process table 
	do {
		vertProcess = false;
		int idx = 0;
		int shortestIdx = -1;
		//find shortest distance of (unprocessed vertices and not -1)
		for (idx = 0; idx < m_Dijkstra.getVertList().size(); idx++) {
			if (vertices[idx].getProcessed() == false && distance[idx] != -1) {
				if (shortestIdx == -1 || distance[idx] < shortestIdx) {
					vertProcess = true;
					shortestIdx = idx;
				}
			}
		}

		if (vertProcess == true) {
			//get the edges of the "shortestVertex"
			auto shortestVertexEdges = vertices[shortestIdx].getVertEdges();

			//update distance array for each edges
			for (auto edge : shortestVertexEdges) {
				for (int val = 0; val < shortestVertexEdges.size(); val++) {
					if (edge.getEdgeDestination()->getVertData() == vertices[val].getVertData()) {
						//ONLY update if new value is shorter
						if (edge.getEdgeWeight() < distance[val]) {
							distance[val] = edge.getEdgeWeight();
							predecessor[val] = shortestIdx;
						}
					}
				}
			}
			//update process status of the "shortestVertex"
			vertices[shortestIdx].setProcessed(true);
		}
	} while (vertProcess == true);
}

ShortestPath Dijkstra::GettingRoute(int * distance, int * predecessor, Vertex<string, string> * vertices, int destinationIndex)
{
	stack<int> * RoutingIndexStack = new stack<int>();
	stack<string>stringRIS;
	int CurrentPredecessorValue = predecessor[destinationIndex];
	string vertexDataForStack = vertices[destinationIndex].getVertData();

	//push the index of the destination onto the "RoutingIndexStack" (bottom of stack)
	RoutingIndexStack->emplace(CurrentPredecessorValue);
	stringRIS.push(vertexDataForStack);

	while (CurrentPredecessorValue != -1) {
		for (int pts = 0; pts < RoutingIndexStack->size(); pts++) {
			//get the "PredecessorValue" of the top of "RIS"
			CurrentPredecessorValue = predecessor[CurrentPredecessorValue];

			//push the "PredecessorValue" onto the "RIS"
			RoutingIndexStack->push(CurrentPredecessorValue);

			//get the string version of the "CurrentPredecessorValue"
			vertexDataForStack = vertices[CurrentPredecessorValue].getVertData();
			stringRIS.push(vertexDataForStack);
		}
	}
	GetTotalTime(distance, predecessor, vertices, RoutingIndexStack);

	ShortestPath routeInformation;
	routeInformation.setDestination(stringRIS);
	return routeInformation;
}

double Dijkstra::GetTotalTime(int * distance, int * predecessor, Vertex<string, string> * vertices, stack<int> * RoutingIndexStack)
{
	int topOfStackValue;
	int numOfVerts = m_Dijkstra.getVertList().size();
	string * roadtypes = new string[numOfVerts];
	double totalTime = 0.0;

	//while "RIS" is not empty
	while (!RoutingIndexStack->empty()) {
		//pop the top of the stack (TOS) == Origin
		topOfStackValue = RoutingIndexStack->top();
		RoutingIndexStack->pop();
		//Look at current TOS for edge data if equal
		list<Edge<string,string>> tosEdgeList = vertices[topOfStackValue].getVertEdges();
		//Compare Origin's Edge Data with CTOS to get the right ROADTYPE
		auto edgeDestinationToSearch = RoutingIndexStack->top(); // Destination
		for (auto edge : tosEdgeList)	{
			if (edge.getEdgeDestination()->getVertData() == vertices[edgeDestinationToSearch].getVertData()) {
				//roadtype-based time calculation
				if (edge.getEdgeData() == "I-5") {
					double time = edge.getEdgeWeight() / IFIVE;
					totalTime += time;
				}
				else {
					double time = edge.getEdgeWeight() / HWY;
					totalTime += time;
				}
			}
		}
	}

	return totalTime;
}