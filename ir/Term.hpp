

#ifndef ANVIL_IR_TERM_HPP
#define ANVIL_IR_TERM_HPP


#include <string>
#include <list>


namespace anvil{

  class Term 
  {
  private:
    std::string m_operation;
    std::list<std::string> m_operands;

  public:
    Term(std::string operation, std::list<std::string> operands)
      : m_operation(operation), m_operands(operands) {}
    

    std::string & getFirstArgument()
    {

      return m_operands.front();
    }

    std::string print();
  };

}

#endif
