#pragma once

#include <iostream>
#include <utility>
#include <array>
#include <limits>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <list>
#include <string.h>
#include <sys/mman.h>

namespace {
void* allocate_writable_memory(size_t size) {
  void* ptr = mmap(0, size,
                   PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (ptr == (void*)-1) {
    perror("mmap");
    return NULL;
  }
  return ptr;
}

}

namespace anvil {


class CodeManager
{
 private:
  static size_t const BLOCK_SIZE = 1024;

  std::list<uint8_t*> code_blocks_;
  uint8_t * current_ = nullptr;
  size_t offset_ = 0;


  void allocate_block()
  {
    uint8_t * block_ptr = reinterpret_cast<uint8_t*>(allocate_writable_memory(BLOCK_SIZE));

    code_blocks_.push_back(block_ptr);
    current_ = block_ptr;
    offset_ = 0;

  }

 public:
  ~CodeManager()
  {
    for (auto block : code_blocks_) {
      (void)munmap(&block, BLOCK_SIZE);
    }
  }

  CodeManager()
  {
    allocate_block();
  }

  template<size_t bytes>
  uint8_t* add_instruction(std::array<uint8_t, bytes> const instruction)
  {
    
    if ((BLOCK_SIZE - offset_) < bytes) {
      allocate_block();
    }

    uint8_t * start = &current_[offset_];

    std::memcpy(start, &instruction[0], bytes);

    offset_ += bytes;

    return start;
  }
  

};

}



