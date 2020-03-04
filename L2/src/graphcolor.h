#pragma once
#include <L2.h>
#include <map>
#include <unordered_set>

using namespace std;

namespace L2 {
	
	pair<vector<string>,map<string,int>> graphcolor(map<string,unordered_set<string>> graph);
}
