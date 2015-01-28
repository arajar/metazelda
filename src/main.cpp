
#include "dungeongenerator.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

void main()
{
	long seed = 1297;
	auto constraints = std::make_shared<map::CountConstraints>(15, 4, 4);
	auto gen = std::make_shared<map::DungeonGenerator>(seed, constraints);
	gen->Generate();

	auto dun = gen->GetDungeon();

	std::string name = "map_";
	name += std::to_string(seed);
	name += ".graphml";
	std::ofstream fs(name);
	if (fs)
	{
		fs << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		fs << "<graphml xmlns = \"http://graphml.graphdrawing.org/xmlns\" xmlns:y=\"http://www.yworks.com/xml/graphml\">\n";
		fs << "<key for =\"node\" id =\"d1\" yfiles.type =\"nodegraphics\"/>\n";
		fs << "\t<graph id = \"G\" edgedefault = \"undirected\">\n";
		//snprintf(hexcol, sizeof hexcol, "%02x%02x%02x", red, green, blue);
		for (const auto& room : dun->GetRooms())
		{
			fs << "\t\t<node id=\"n";
			fs << room->coords_.coords_.x;
			fs << room->coords_.coords_.y;
			fs << "\">\n";

			fs << "\t\t\t<data key=\"d1\">\n";
			fs << "\t\t\t\t<y:ShapeNode>\n";
			fs << "\t\t\t\t\t<y:NodeLabel>\n";
			if (room->IsBoss())
				fs << "boss\n";
			else if (room->IsGoal())
				fs << "exit\n";
			else if (room->IsStart())
				fs << "start\n";
			else if (room->IsSwitch())
				fs << "switch\n";
			else if (room->item_ == map::Symbol::Switch_On)
				fs << "ON\n";
			else if (room->item_ == map::Symbol::Switch_Off)
				fs << "OFF\n";
			else
				fs << (int)(room->intensity_ * 100);
			fs << "\t\t\t\t\t</y:NodeLabel>\n";

			fs << "\t\t\t\t</y:ShapeNode>\n";
			fs << "\t\t\t</data>\n";

			fs << "\t\t</node>\n";
		}

		int edge = 0;
		for (const auto& room : dun->GetRooms())
		{
			if (!room->children_.empty())
			{
				for (const auto& child : room->children_)
				{
					fs << "\t\t<edge id=\"n";
					fs << edge++;
					fs << "\" source=\"n";
					fs << room->coords_.coords_.x;
					fs << room->coords_.coords_.y;
					fs << "\" target=\"n";
					fs << child->coords_.coords_.x;
					fs << child->coords_.coords_.y;
					fs << "\"/>\n";
				}
			}
		}

		fs << "\t</graph>\n";
		fs << "</graphml>\n";
		fs.close();
	}


	int a = 3;
}