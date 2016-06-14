#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>
#include <numeric>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <sys/mman.h>

#include "CodeManager.hpp"
#include "Executor.hpp"

enum class Register {
  EAX,
    EBX,
    ECX,
    EDX,
    ESI,
    EDI
      
  

};

uint8_t get_hex(Register const & r)
{
  switch(r){
    case EAX:
      return 0x;
    case EBX:
      return 0x;
    case ECX:
      return 0x;
    case EDX:
      return 0x;
    case ESI:
      return 0x;
    case ED:
      return 0x;
  }
}

uint8_t * emit_mov(anvil::CodeManager & cm, Register & left, Register & right)
{
  uint8_t left_hex = 0;
  uint8_t right_hex = 0;

  switch()


}

// %rdi, %rsi, %rdx, %rcx, %r8 and %r9 in order 
std::pair<anvil::Function,size_t> emit_add_2_functions(anvil::CodeManager & cm) {
  //{ 0x48, 0x89, 0xC7, 0x48, 0x89, 0xDE, 0x48, 0x01, 0xD8, 0xC3 } 
  std::array<uint8_t, 3> mov = {0x48, 0x89, 0xf8};                   // mov %rdi, %rax
  std::array<uint8_t, 4> add = {0x48, 0x83, 0xc0, 0x04};             // add $4, %rax
  std::array<uint8_t, 1> ret = {0xc3};                               // ret

  uint8_t * entry = cm.add_instruction(mov);
  (void)cm.add_instruction(add);
  (void)cm.add_instruction(ret);

  return std::make_pair(reinterpret_cast<anvil::Function>(entry.first), entry.second);
}

int main(int argc, char** argv) {

  anvil::CodeManager code_manager;
  anvil::Executor executor;

  std::pair<anvil::Function,size_t> f = emit_function(code_manager);

  int64_t const result = executor.execute(f, 12);

  std::cout << result << std::endl;

  return 0;
}

