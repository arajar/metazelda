Super modified C++11 version of https://github.com/tcoxon/metazelda

Generate the project using prj/gen_prj.bat

Must be compiled with vs2013, not tested with older versions of visual studio.

To generate a dungeon you have to:

- Create a seed
	long seed = 1297;

- Create the dungeon constraints: max rooms / max keys / max switches
	auto constraints = std::make_shared<map::CountConstraints>(15, 4, 4);

- Create a dungeon generator with the constraints and the seed
	auto gen = std::make_shared<map::DungeonGenerator>(seed, constraints);

- Generate the dungeon!
	gen->Generate();


For testing purposes the program generates a .graphml that you can open with yED to see the actual dungeon.