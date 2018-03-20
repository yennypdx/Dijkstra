#include "stdafx.h"
#pragma once
#include "DijkstraAlg.h"
#include "ShortestPath.h"

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
	return list;
	//caller: vector<string> data = DataParsing(buffer, ",");
}

void Dijkstra::PopulatingDjikstraGraph(string inFile)
{
	ifstream dataFile;
	dataFile.open(inFile);
	string singleLine;

	if (dataFile.is_open()) {
		getline(dataFile, singleLine);

		do {
			vector<string> data = DataParsing(singleLine, ",");
			string origin = data[0];
			string destination = data[1];
			string path = data[2];
			int mileage = stoi(data[3]);

			//setting point to point vertexes
			auto startingPoint = m_Dijkstra.getVert(origin);
			auto endingPoint = m_Dijkstra.getVert(destination);

			//setting the origin vertex
			if (!startingPoint) {
				m_Dijkstra.insertVertex(origin);
				startingPoint = m_Dijkstra.getVert(origin);
			}

			//setting the destination vertex
			if (!endingPoint) {
				m_Dijkstra.insertVertex(destination);
				endingPoint = m_Dijkstra.getVert(destination);
			}

			//populate the edges
			m_Dijkstra.insertEdge(origin, destination, path, to_string(mileage));
			getline(dataFile, singleLine);
		} while (!dataFile.eof());
	}
	else {
		throw runtime_error("Cannot open file.");
	}
}

ShortestPath Dijkstra::FindShortestPath(string inOrigin, string inDestination)
{
	int startingPoint = -1;
	int endingPoint = -1;
	int minVal = 0;
	int minDistance = -1;
	int numOfVerts = m_Dijkstra.getVertCount();
	vector<Vertex<string, string>> vertices(numOfVerts);
	vector<double> time(numOfVerts);
	vector<string> roadtypes(numOfVerts);

	//create the distance and predecessor arrays
	vector<int> distance(numOfVerts);
	vector<int> predecessor(numOfVerts);
	//initialize the two dijkstra sets value
	for (int i = 0; i < numOfVerts; i++) {
		distance[i] = -1;
		predecessor[i] = -1;
	}

	//populate vertices on the graph
	list<Vertex<string, string>>::iterator it;
	for (it = m_Dijkstra.getVertList().begin(); it != m_Dijkstra.getVertList().end(); it++) {
		vertices.emplace_back(*it);
	}

	//identifying the point to point locations
	for (int i = 0; i < numOfVerts; i++) {
		if (vertices[i].getVertData() == inOrigin) {
			startingPoint = i;
		}
		if (vertices[i].getVertData() == inDestination) {
			endingPoint = i;
		}
	}
	time[startingPoint] = 0;
	distance[startingPoint] = 0;

	//starting djikstra algorithm search
	for (int i = 0; i < numOfVerts && !vertices[endingPoint].getProcessed(); i++) {

		//distance algorithm
		for (int j = 0; j < numOfVerts; j++) {
			if (!vertices[j].getProcessed() && distance[j] != -1 &&
				(minDistance == -1 || distance[j] < minDistance)) {
				minDistance = distance[j];
				minVal = j;
			}
		}
		//updating edge values
		auto edgeList = vertices[minVal].getVertEdges();
		for (auto edge : edgeList) {
			int edgeVal = 0;
			for (int a = 0; a < numOfVerts; a++) {
				if (edge.getEdgeDestination()->getVertData() == vertices[a].getVertData()) {
					edgeVal = a;
				}
			}
			//calculate the total distance
			int totalDistance = distance[minVal] + edge.getEdgeWeight();
			if (totalDistance < distance[edgeVal] || distance[edgeVal] == -1) {
				distance[edgeVal] = totalDistance;
				predecessor[edgeVal] = minVal;
				roadtypes[edgeVal] = edge.getEdgeData();

				//calculate the timing based on the road types
				if (roadtypes[edgeVal] == "I-5") {
					time[edgeVal] = time[minVal] + (edge.getEdgeWeight() / IFIVE);
				}
				else {
					time[edgeVal] = time[minVal] + (edge.getEdgeWeight() / HWY);
				}
			}
		}
		vertices[minVal].setProcessed(true);
		minDistance = -1;
	}
	//updating predecessor values
	int predecessorVal = endingPoint;
	stack<string> route;
	while (predecessorVal != -1) {
		if (!roadtypes[predecessorVal].empty()) {
			if (route.empty()) {
				route.push(roadtypes[predecessorVal]);
			}
			else if (roadtypes[predecessorVal] != route.top()) {
				route.push(roadtypes[predecessorVal]);
			}
			predecessorVal = predecessor[predecessorVal];
		}
	}

	//building route
	ShortestPath routeInformation;
	routeInformation.setDestination(route);
	routeInformation.setDistance(distance[endingPoint]);
	routeInformation.setTime(time[endingPoint]);

	return routeInformation;
}
