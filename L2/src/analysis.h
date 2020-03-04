#pragma once

#include <L2.h>
#include <vector>
#include <string>

using namespace std;

namespace L2 {
  vector<vector<vector<string>>> liveness_analysis(Function *f);
}
