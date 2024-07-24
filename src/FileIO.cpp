#include "FileIO.h"
#include "LogConsole.h"
#include <map>
#include <qfile.h>
#include <qtextstream.h>
#include <regex>
#include <set>
#include <tuple>
#include <utility>
#include "ElementUtils.h"
#include <iostream>
#include <string>
#include <fstream> 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <QDateTime>

FileIO::FileIO()
{
}

FileIO::~FileIO()
{
}

vector<Color4_8BIT> FileIO::readPolygonColorsFile(const char * path)
{
	vector<Vec4> colors;

	QFile myfile(path);

	char chararray[100];
	int primitiveNums[4];

	if (!myfile.open(QIODevice::ReadOnly))
	{
		LogConsole::showError("Color File does not exist.");
		return colors;
	}

	QTextStream in(&myfile);

	int lineNum = 0;
	while (!in.atEnd())
	{
		QString line = in.readLine();

		stringstream ss(line.toStdString());
		string temp;
		for (int i = 0; i < 4; i++) {
			getline(ss, temp, ' ');
			primitiveNums[i] = stof(temp);
		}

		Vec4 color(primitiveNums[0], primitiveNums[1], primitiveNums[2], primitiveNums[3]);
		colors.push_back(color);

		lineNum++;

		if (lineNum > 11)
			break;
	}

	myfile.close();

	return colors;
}

void FileIO::writePolygonColorsFile(const char * filename, vector<Color4_8BIT> colors)
{
	/*write to file*/
	FILE *this_file = fopen(filename, "w");

	if (this_file)
	{
		for (int i = 0; i < 11; i++) {

			int R = colors[i].R;
			int G = colors[i].G;
			int B = colors[i].B;
			int A = colors[i].Alpha;

			fprintf(this_file, "%d %d %d %d\n", R, G, B, A);
		}
		fclose(this_file);
	}

	LogConsole::writeConsole("Energies are saved");
}

ERData * FileIO::readERFile(const char * path)
{
	ERData *newData = new ERData();

	QFile myfile(path);

	if (!myfile.open(QIODevice::ReadOnly))
	{
		LogConsole::showError("Color File does not exist.");
		return NULL;
	}

	QTextStream in(&myfile);

	int lineNum = 0;
	int en_start_lineNum = 0;
	int primitiveNums[2];
	bool sizeRead = false;
	bool hasZValue = true;
	bool hasREColors = false;
	char Delimiter = ' ';
	auto is_number = [](const std::string& s)
	{
		std::string::const_iterator it = s.begin();
		while (it != s.end() && isdigit(*it)) ++it;
		return !s.empty() && it == s.end();
	};

	auto isFloat = [](string myString) {
		std::istringstream iss(myString);
		float f;
		iss >> noskipws >> f; // noskipws considers leading whitespace invalid
		
        // Check the entire string was consumed and if either failbit or badbit is set
		return iss.eof() && !iss.fail();
	};

	while (!in.atEnd())
	{
		lineNum++;
		QString line = in.readLine();
		string linestr = line.toStdString();

		if (linestr == "Nary Relationship Data") {
			continue;
		}

		if (linestr == "Nary Relationship Data 2D") {
			hasZValue = false;
			continue;
		}

		if (linestr.find("Comma Delimiter") != string::npos)
		{
			Delimiter = ',';
			continue;
		}

		if (linestr.find("Relationship Colors Saved") != string::npos)
		{
			hasREColors = true;
			continue;
		}

		if (linestr.empty()) {
			continue;
		}

		if (linestr[0] == '#') {
			newData->appendFileComment(linestr);
			continue;
		}

		if (!sizeRead)
		{
			stringstream ss(line.toStdString());
			string temp;
			for (int i = 0; i < 2; i++) {
				getline(ss, temp, Delimiter);
				primitiveNums[i] = stoi(temp);
			}
			sizeRead = true;

			en_start_lineNum = lineNum + 1;

			continue;
		}

		//entities
		if (lineNum < en_start_lineNum + primitiveNums[0]) {

			int nindex;
			string label;
			double x =0.0, y =0.0, z =0.0;

			stringstream ss(line.toStdString());
			string temp;
			vector<string> tokens;
			while (getline(ss, temp, Delimiter)) {
				tokens.push_back(temp);
			}

			if (tokens.size() < 4)
			{
				LogConsole::showError("There is no enough elements!");
				return nullptr;
			}

			if (hasZValue && tokens.size() != 5) {
				LogConsole::showError("There is no enough elements!");
				return nullptr;
			}

			nindex = stoi(tokens[0]);
			label = tokens[1];
			int k = 2;
			while (!isFloat(tokens[k])) {
				label += Delimiter + tokens[k];
				k++;
			}

			x = stod(tokens[k]);
			y = stod(tokens[k+1]);

			if(hasZValue)
				z = stod(tokens[k + 2]);
			else
				z = 0.0;

			Entity *entity = new Entity(Vec3(x, y, z));
			entity->setLabel(label);
			newData->addEntity(entity);
			continue;
		}

		//relationships
		if (lineNum < en_start_lineNum + primitiveNums[0] + primitiveNums[1]) {

			int nindex;
			string label;
			int numVertices;
			vector<Entity*> entitylist;

			stringstream ss(line.toStdString());
			string temp;
			vector<string> tokens;
			while (getline(ss, temp, Delimiter)) {
				tokens.push_back(temp);
			}

			if (tokens.size() == 0)
			{
				LogConsole::showError("There is no enough elements!");
				return nullptr;
			}

			int k = 0;
			nindex = stoi(tokens[k++]);

			int R, G, B;
			if (hasREColors) {
				R = stoi(tokens[k++]);
				G = stoi(tokens[k++]);
				B = stoi(tokens[k++]);
			}

			label = tokens[k++];
			
			while (!is_number(tokens[k])) {
				label += Delimiter + tokens[k];
				k++;
			}

			numVertices = stoi(tokens[k++]);

			for (int i = 0; i < numVertices; i++) {

				int id = stoi(tokens[k++]);

				if (id >= primitiveNums[0]) {
					LogConsole::showError("Relationship %d includes entity out of scope!", i);
					return nullptr;
				}

				Entity *entity = newData->getEntity(id);
				entitylist.push_back(entity);
			}

			double monogon_orientation = 0.0;
			if (entitylist.size() == 1 && tokens.size() > k) {
				monogon_orientation = stod(tokens[k++]);
			}

			/*duplicated nodes in the same polygon is not allowed*/
			bool duplicated = false;
			for (int i = 0; i < entitylist.size(); i++) {
				for (int j = i + 1; j < entitylist.size(); j++) {
					if (entitylist[i] == entitylist[j])
					{
						duplicated = true;
						break;
					}
				}
				if (duplicated)
					break;
			}

			if (duplicated) {
				LogConsole::showError("polygon has duplicated nodes is not allowed");
				continue;
			}


			Relationship *re = new Relationship(entitylist);
			re->setLabel(label);
			if (re->getCardinality() == 1) {
				re->setMonogonRotation(monogon_orientation);
			}

			if (hasREColors)
				re->setColor(Color4_F(R/255.0, G/255.0, B/255.0, 0.5));

			newData->addRelationship(re);
			continue;
		}
	}
	myfile.close();

	return newData;
}

bool FileIO::readERFileLayout(const char* path, ERData *erdata) {

	QFile myfile(path);

	if (!myfile.open(QIODevice::ReadOnly))
	{
		LogConsole::showError("File does not exist.");
		return false;
	}

	int nodeSize = erdata->getEntities().size();
	int polyonSize = erdata->getRelationships().size();

	if (nodeSize == 0) {
		LogConsole::showError("No entities.");
		myfile.close();
		return false;
	}


	QTextStream in(&myfile);

	int lineNum = 0;
	int en_start_lineNum = 0;
	int primitiveNums[2];
	bool sizeRead = false;
	bool hasZValue = true;
	bool hasREColors = false;
	char Delimiter = ' ';

	auto is_number = [](const std::string& s)
	{
		std::string::const_iterator it = s.begin();
		while (it != s.end() && isdigit(*it)) ++it;
		return !s.empty() && it == s.end();
	};

	auto isFloat = [](string myString) {
		std::istringstream iss(myString);
		float f;
		iss >> noskipws >> f; // noskipws considers leading whitespace invalid
							  // Check the entire string was consumed and if either failbit or badbit is set
		return iss.eof() && !iss.fail();
	};

	while (!in.atEnd())
	{
		lineNum++;
		QString line = in.readLine();
		string linestr = line.toStdString();

		if (linestr == "Nary Relationship Data") {
			continue;
		}

		if (linestr == "Nary Relationship Data 2D") {
			hasZValue = false;
			continue;
		}

		if (linestr.find("Comma Delimiter") != string::npos)
		{
			Delimiter = ',';
			continue;
		}

		if (linestr.find("Relationship Colors Saved") != string::npos)
		{
			hasREColors = true;
			continue;
		}

		if (linestr.empty()) {
			continue;
		}

		if (linestr[0] == '#') {
			continue;
		}

		if (!sizeRead)
		{
			stringstream ss(line.toStdString());
			string temp;
			for (int i = 0; i < 2; i++) {
				getline(ss, temp, Delimiter);
				primitiveNums[i] = stoi(temp);
			}
			sizeRead = true;

			en_start_lineNum = lineNum + 1;

			continue;
		}

		if (sizeRead && (primitiveNums[0] != nodeSize)) {
			LogConsole::showError("Incorrect location ER Data, the number of entities are not same.");
			myfile.close();
			return false;
		}
		//entities
		if (lineNum < en_start_lineNum + primitiveNums[0]) {

			int nindex;
			string label;
			double x = 0.0, y = 0.0, z = 0.0;

			stringstream ss(line.toStdString());
			string temp;
			vector<string> tokens;
			while (getline(ss, temp, Delimiter)) {
				tokens.push_back(temp);
			}

			if (tokens.size() < 4)
			{
				LogConsole::showError("There is no enough elements!");
				return false;
			}

			if (hasZValue && tokens.size() != 5) {
				LogConsole::showError("There is no enough elements!");
				return false;
			}

			nindex = stoi(tokens[0]);
			label = tokens[1];
			int k = 2;
			while (!isFloat(tokens[k])) {
				label += Delimiter + tokens[k];
				k++;
			}

			x = stod(tokens[k]);
			y = stod(tokens[k + 1]);

			if (hasZValue)
				z = stod(tokens[k + 2]);
			else
				z = 0.0;

			if (hasZValue)
				z = stod(tokens[4]);

			erdata->getEntity(nindex)->setLocation(Vec3(x, y, z));
			continue;
		}

		//relationships, read monogon rotation
		if (lineNum < en_start_lineNum + primitiveNums[0] + primitiveNums[1]) {

			int nindex;
			string label;
			int numVertices;
			vector<Entity*> entitylist;

			stringstream ss(line.toStdString());
			string temp;
			vector<string> tokens;
			while (getline(ss, temp, Delimiter)) {
				tokens.push_back(temp);
			}

			if (tokens.size() == 0)
			{
				LogConsole::showError("There is no enough elements!");
				return false;
			}

			int k = 0;
			nindex = stoi(tokens[k++]);

			int R, G, B;
			if (hasREColors) {
				R = stoi(tokens[k++]);
				G = stoi(tokens[k++]);
				B = stoi(tokens[k++]);
			}

			label = tokens[k++];

			while (!is_number(tokens[k])) {
				label += Delimiter + tokens[k];
				k++;
			}

			numVertices = stoi(tokens[k++]);

			k += numVertices;

			/*TODO: monogon orientation*/
			double monogon_orientation = 0.0;
			if (numVertices == 1 && tokens.size() > k) {
				monogon_orientation = stod(tokens[k++]);
				erdata->getRelationship(nindex)->setMonogonRotation(monogon_orientation);
			}

			continue;
		}
	}
	myfile.close();

	erdata->update2D();
    
	return true;
}

bool FileIO::writeERFile(const char * path, ERData * merdata)
{
	return writeERFile(path, merdata->getEntities(), merdata->getRelationships(), merdata->FILECOMMENTS);
}

bool FileIO::writeERFile(const char * path, vector<Entity*> enlist, vector<Relationship*> relist, string comment)
{
	FILE *this_file = fopen(path, "w");

	if (this_file)
	{
		fprintf(this_file, "Nary Relationship Data 2D\n");
		fprintf(this_file, "Comma Delimiter\n");
		fprintf(this_file, "Relationship Colors Saved\n");

		int ensize = enlist.size();
		int resize = relist.size();

		/*comments has its own line break*/
		if(comment!= "")
			fprintf(this_file, "#%s\n", comment.c_str());

		fprintf(this_file, "%d,%d\n", ensize, resize);

		for (int i = 0; i < ensize; i++) {
			Entity *v = enlist[i];

			//id	label	x y z
			fprintf(this_file, "%d,%s,%.16f,%.16f\n", i, v->getLabel().c_str(), v->getLocation().x, v->getLocation().y);
		}

		//relationships
		for (int i = 0; i < resize; i++) {
			Relationship *re = relist[i];

			//check if the all the entities of relationship is inisde the enlist
			bool valid = true;
			vector<int> vertexindexlist;
			for (unsigned int j = 0; j < re->getIncidentEntities().size(); j++)
			{
				Entity *en = re->getIncidentEntity(j);
				int res = isInsideList<Entity>(en, enlist);
				if (res == -1) {
					valid = false;
					LogConsole::showError("incorrect relationship");
					break;
				}

				vertexindexlist.push_back(res);
			}

			if (!valid)
				continue;

			//id	label	caridnality		entitiesIds
			fprintf(this_file, "%d", i);
			fprintf(this_file, ",%d,%d,%d", int(re->getColor().R*255.0), int(re->getColor().G * 255.0), int(re->getColor().B * 255.0));
			fprintf(this_file, ",%s,%d", re->getLabel().c_str(), re->getCardinality());

			for (int n = 0; n < re->getCardinality(); n++) {
				fprintf(this_file, ",%d", vertexindexlist[n]);
			}

			if (re->getCardinality() == 1) {
				fprintf(this_file, ",%f", re->getMonogonRotation());
			}

			fprintf(this_file, "\n");
		}

		fclose(this_file);

		return true;
	}
	else
		return false;

}
