#include "OBJ_Parser.h"
#include <fstream>
#include <sstream>

void OBJ_Parser::ParseOBJ(const std::string& path, std::vector<FPoint3>& verticesVect, std::vector<int>& indecesVect)
{
	verticesVect.clear();
	indecesVect.clear();

	std::string currentLine{};
	std::ifstream input;
	input.open(path);
	while (!input.eof())
	{
		std::getline(input, currentLine);
		if (currentLine[0] == 'v')
		{
			std::stringstream buffer{ currentLine };
			std::string v0, v1, v2;
			std::getline(buffer, v0, ' ');
			std::getline(buffer, v0, ' ');

			std::getline(buffer, v0, ' ');
			std::getline(buffer, v1, ' ');
			std::getline(buffer, v2, ' ');

			verticesVect.push_back(FPoint3(std::stof(v0), std::stof(v1), std::stof(v2)));
		}
		else if (currentLine[0] == 'f')
		{
			std::stringstream buffer{ currentLine };
			std::string f0, f1, f2;
			std::getline(buffer, f0, ' ');

			std::getline(buffer, f0, ' ');
			std::getline(buffer, f1, ' ');
			std::getline(buffer, f2, ' ');

			//Faces start at 1 in .obj -> -1 to correct
			indecesVect.push_back(std::stoi(f0) - 1);
			indecesVect.push_back(std::stoi(f1) - 1);
			indecesVect.push_back(std::stoi(f2) - 1);
		}
	}
}

void OBJ_Parser::ParseOBJ(const std::string& path, std::vector<FPoint3>& verticesVect, std::vector<int>& indecesVect, std::vector<FVector3>& normalsVect)
{
	verticesVect.clear();
	indecesVect.clear();
	normalsVect.clear();

	std::string currentLine{};
	std::ifstream input;
	input.open(path);
	while (!input.eof())
	{
		std::getline(input, currentLine);
		if (currentLine[0] == 'v')
		{
			std::stringstream buffer{ currentLine };
			std::string v0, v1, v2;
			std::getline(buffer, v0, ' ');
			std::getline(buffer, v0, ' ');

			std::getline(buffer, v0, ' ');
			std::getline(buffer, v1, ' ');
			std::getline(buffer, v2, ' ');

			verticesVect.push_back(FPoint3(std::stof(v0), std::stof(v1), std::stof(v2)));
		}
		else if (currentLine[0] == 'f')
		{
			std::stringstream buffer{ currentLine };
			std::string f0, f1, f2;
			std::getline(buffer, f0, ' ');

			std::getline(buffer, f0, ' ');
			std::getline(buffer, f1, ' ');
			std::getline(buffer, f2, ' ');

			//Faces start at 1 in .obj -> -1 to correct
			indecesVect.push_back(std::stoi(f0) - 1);
			indecesVect.push_back(std::stoi(f1) - 1);
			indecesVect.push_back(std::stoi(f2) - 1);
		}
	}
	for (uint64_t index = 0; index < indecesVect.size(); index += 3)
	{
		int index1{ indecesVect[index] };
		int index2{ indecesVect[index + 1] };
		int index3{ indecesVect[index + 2] };

		FVector3 a = verticesVect[index2] - verticesVect[index1];
		FVector3 b = verticesVect[index3] - verticesVect[index1];

		normalsVect.push_back(GetNormalized(Cross(a, b)));
	}
}