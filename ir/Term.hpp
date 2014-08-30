

#ifndef ANVIL_IR_TERM_HPP
#define ANVIL_IR_TERM_HPP


#include <string>
#include <vector>


namespace anvil{

    namespace operations {
        enum TermOperation {
            Add,
            Multiply,
            LessThan,
            Jump,
            JumpIfEqual,
            Label
        };
    }

    class Term 
    {
      private:
        operations::TermOperation m_operation;
        std::vector<std::string> m_operands;

      public:
        Term(operations::TermOperation operation, std::vector<std::string> operands)
            : m_operation(operation), m_operands(operands) {}
    

        typedef std::vector<std::string>::const_iterator const_iterator;

        const_iterator begin()
        {
            return m_operands.begin();
        }

        const_iterator end()
        {
            return m_operands.end();
        }

        std::vector<std::string> getOperands()
        {
            return m_operands;
        }

        std::string & getFirstArgument()
        {

            return m_operands.front();
        }


        operations::TermOperation getOperation()
        {
            return m_operation;
        }

        std::string print();
    };

}

#endif
