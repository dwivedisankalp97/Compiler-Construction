#pragma once

#include <L2.h>

namespace L2 {
  Program parse_file (char *fileName);
  Program parse_function_file (char *fileName);
  Program_spill parse_spill_file (char *fileName);
}
