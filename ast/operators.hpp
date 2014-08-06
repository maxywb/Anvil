#ifndef ANVIL_OPERATORS_HPP
#define ANVIL_OPERATORS_HPP

namespace anvil{ 
  namespace operators{

    enum BinaryOperatorType {
      Add,
      Subtract,
      Multiply,
      Divide,
      Modulo,
      GreaterThan,
      LessThan,
      GreaterThanOrEqual,
      LessThanOrEqual,
      Equal,
      NotEqual,
      Xor,
      And,
      Or,
      ShiftRight,
      ShiftLeft,
      Dot,
      Comma
    };
  }
}




#endif
