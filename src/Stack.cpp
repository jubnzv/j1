#include <algorithm>
#include <cstdint>
#include <stack>
#include <stdexcept>

#include "Stack.hpp"

template <> Stack<uint16_t>::Stack(size_t depth) : _depth(depth) {}

template <> int Stack<uint16_t>::Push(uint16_t v)
{
    if (_s.size() > _depth) {
        return -1;
    }
    _s.push(v);
    return 0;
}

template <> std::vector<uint16_t> Stack<uint16_t>::ToSeq()
{
    auto v = std::vector<uint16_t>();
    auto ns = std::stack<uint16_t>();

    while (!_s.empty()) {
        v.push_back(_s.top());
        _s.pop();
    }

    for (auto const &vv : v) {
        ns.push(vv);
    }
    _s = ns;

    return v;
}

template <> bool Stack<uint16_t>::Empty() { return _s.empty(); }

template <> size_t Stack<uint16_t>::Depth() { return _s.size(); }

template <> std::optional<uint16_t> Stack<uint16_t>::Top()
{
    if (_s.empty()) {
        return std::optional<uint16_t>();
    }
    return std::optional<uint16_t>(_s.top());
}

template <> std::optional<uint16_t> Stack<uint16_t>::Next()
{
    if (_s.size() < 2) {
        return std::optional<uint16_t>();
    }
    auto t = _s.top();
    _s.pop();
    auto n = _s.top();
    _s.push(t);
    return std::optional<uint16_t>(n);
}

template <> std::optional<uint16_t> Stack<uint16_t>::Pop()
{
    if (_s.empty()) {
        return std::optional<uint16_t>();
    }
    auto ret = _s.top();
    _s.pop();
    return std::optional<uint16_t>(ret);
}

template <> int Stack<uint16_t>::SwapTN()
{
    if (_s.size() < 2) {
        return -1;
    }

    auto t = _s.top();
    _s.pop();
    std::swap(t, _s.top());
    _s.push(t);

    return 0;
}
