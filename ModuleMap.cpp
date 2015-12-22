#include "ModuleMap.h"
#include <vector>
#include <sstream>

using namespace tinyxml2;
using namespace std;

ModuleMap::ModuleMap(){

	//Loads the level
	XMLDocument doc;
	doc.LoadFile("../Ogmo/Level_1.oel");

	//Reads width and height
	int w = stoi(doc.FirstChildElement("level")->Attribute("width")) / 16;
	int h = stoi(doc.FirstChildElement("level")->Attribute("height")) / 16;

	//Loads the World values and separates them by the commas
	vector<int> worldValues;
	stringstream ss(doc.FirstChildElement("level")->FirstChildElement("World")->GetText());

	int i;
	while (ss >> i)
	{
		worldValues.push_back(i);

		if (ss.peek() == ',')
			ss.ignore();
	}
}

