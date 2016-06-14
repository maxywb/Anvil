#pragma once


#include "../assert.hpp"

#include <iostream>
#include <array>
#include <limits>
#include <utility>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <list>
#include <string.h>
#include <sys/mman.h>
namespace {


void make_memory_executable(void* m, size_t const size) {
  if (mprotect(m, size, PROT_READ | PROT_EXEC) == -1) {
    ASSERT(false, "failed to mark executable");
  }
}

}

namespace anvil {
// ATTN: move near code that emits this type of function
// ATTN: make class hierarchy of "executable items"
typedef int64_t (*Function)(const int64_t);

class Executor
{
 private:
 public:

  int64_t execute(std::pair<Function,size_t> function_info,int64_t const & argument)
  {
    Function function = function_info.first;
    size_t const function_size = function_info.second;

    make_memory_executable(reinterpret_cast<void*>(function), function_size);
    return function(argument);
    
  }

};
    
}

