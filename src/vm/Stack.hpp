/// Implementation of J1 stack with limited depth.
#ifndef STACK_HPP
#define STACK_HPP

#include <cstdint>
#include <optional>
#include <stack>
#include <vector>

template <typename T> class Stack {
  public:
    Stack(size_t);

    /// Push element on a stack
    /// @return 0 if success, -1 if max depth exceeds
    int Push(T);

    std::vector<T> ToSeq();

    bool Empty();
    size_t Depth();
    std::optional<T> Top();
    /// Helper for N (second item on the stack)
    std::optional<T> Next();
    std::optional<T> Pop();

    /// Helper to swap T (top of stack) and N (second item on the stack)
    /// @return 0 if success, -1 otherwise
    int SwapTN();

  private:
    std::stack<T> _s;
    /// Max depth
    size_t _depth;
};

#endif // STACK_HPP
