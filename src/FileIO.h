#pragma once
#include "globalVariables.h"
#include <string>
#include "ERData.h"

class FileIO
{
public:
	FileIO();
	~FileIO();

	static vector<Color4_8BIT>readPolygonColorsFile(const char* filename);
	static void writePolygonColorsFile(const char* filename, vector<Color4_8BIT>colors);

	static ERData* readERFile(const char*file);
	static bool readERFileLayout(const char*path, ERData *erdata);
	static bool writeERFile(const char*path, ERData *erdata);
	static bool writeERFile(const char*path, vector<Entity*>enlist, vector<Relationship*> relist, string comment= "");

};

