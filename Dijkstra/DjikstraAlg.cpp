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

ShortestPath Dijkstra::FindShortestPath(string inOrigin, string inDestination)	//checked
{
	int initNum = -1;
	int originIndex = -1;
	int destinationIndex = -1;
	int minDistanceIndex = 0;
	int minDistance = -1;
	int numOfVerts = m_Dijkstra.getVertList().size();
	
	Vertex<string, string> * vertices = new Vertex<string, string>[numOfVerts];
	string * roadtypes = new string[numOfVerts];
	double * time = new double[numOfVerts];
	
	/*---------------------Dijkstra TABLE Start--------------------*/
	int * distance = new int[numOfVerts]; 
	int * predecessor = new int[numOfVerts]; 
	for (int i = 0; i < numOfVerts; i++) { 
		distance[i] = initNum;
		predecessor[i] = initNum;
	}
	//populate vertices on the TABLE
	list<Vertex<string, string>> tempVerts = m_Dijkstra.getVertList();
	int i = 0;
	for (auto v : tempVerts) {
		vertices[i] = *&v;
		i++;
	}
	//identifying the origin & destination indexes
	for (int i = 0; i < numOfVerts; i++) {
		if (vertices[i].getVertData() == inOrigin) {
			originIndex = i;
		}
		if (vertices[i].getVertData() == inDestination) {
			destinationIndex = i;
		}
	}
	
	if (originIndex == -1 || destinationIndex == -1) {
		throw runtime_error("Bug found.");
	}
	time[originIndex] = 0;
	distance[originIndex] = 0;
	
	//DIJKSTRA algorithm start
	for (int counter = 0; counter < numOfVerts && vertices[destinationIndex].getProcessed() == false; counter++)	{
		//setting DISTANCE: minimal distance == distance at array[idx]
		for (int idx = 0; idx < numOfVerts; idx++) {
			if (vertices[idx].getProcessed() == false && distance[idx] != -1 &&
				(minDistance == -1 || distance[idx] < minDistance)) {
				minDistance = distance[idx];
				minDistanceIndex = idx;
			}
		}
		//setting PATH: copy list of edges to EdgeList
		auto edgeListOfVertex = vertices[minDistanceIndex].getVertEdges();
		for (auto edge : edgeListOfVertex) {

			//find next index (shortest) to process
			int edgeToNextVerticesIndex;
			for (int id = 0; id < numOfVerts; id++) {
				if (edge.getEdgeDestination()->getVertData() == vertices[id].getVertData()) {
					edgeToNextVerticesIndex = id;
				}
			}
			//calculating DISTANCE: total = local weight + accumulated weight along path
			int totalDistance = distance[minDistanceIndex] + edge.getEdgeWeight();
			if (totalDistance < distance[edgeToNextVerticesIndex] || distance[edgeToNextVerticesIndex] == -1) {
				//updating all arrays value
				distance[edgeToNextVerticesIndex] = totalDistance;
				predecessor[edgeToNextVerticesIndex] = minDistanceIndex;
				roadtypes[edgeToNextVerticesIndex] = edge.getEdgeData();
	
				//roadtype-based time calculation
				if (roadtypes[edgeToNextVerticesIndex] == "I-5") {
					time[edgeToNextVerticesIndex] = (time[minDistanceIndex]) + (edge.getEdgeWeight() / IFIVE);
				}
				else {
					time[edgeToNextVerticesIndex] = (time[minDistanceIndex]) + (edge.getEdgeWeight() / HWY);
				}
			}
		}
		vertices[minDistanceIndex].setProcessed(true);
		minDistance = -1;
	}

	//setting PATH ROUTE
	int predecessorValue = destinationIndex;
	stack<string> route;
	while (predecessorValue != -1) {
		if (!roadtypes[predecessorValue].empty()) {
			if (route.empty()) {
				route.push(roadtypes[predecessorValue]);
			}
			else if (roadtypes[predecessorValue] != route.top()) {
				route.push(roadtypes[predecessorValue]);
			}
		}
		predecessorValue = predecessor[predecessorValue];
	}
	
	//building route
	ShortestPath routeInformation;
	routeInformation.setDestination(route);
	routeInformation.setDistance(distance[destinationIndex]);
	routeInformation.setTime(time[destinationIndex]);
	
	return routeInformation;
}