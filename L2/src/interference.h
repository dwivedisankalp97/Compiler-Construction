#pragma once

#include <L2.h>
#include <map>
#include <unordered_set>

using namespace std;

namespace L2 {
  map<string,unordered_set<string>> interference_graph(Function *f);
}
