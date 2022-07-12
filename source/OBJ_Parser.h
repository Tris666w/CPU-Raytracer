#pragma once
#include <vector>
#include "EMath.h"
#include <string>

using namespace Elite;
class OBJ_Parser
{
public:
	static void ParseOBJ(const std::string& path, std::vector<FPoint3>& verticesVect, std::vector<int>& indecesVect);
	static void ParseOBJ(const std::string& path, std::vector<FPoint3>& verticesVect, std::vector<int>& indecesVect, std::vector<FVector3>& normalsVect);

private:
};
