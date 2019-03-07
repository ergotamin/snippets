// !c++
// #pragma once
//-------------------------------------
// (c) 2018 MIT License Marcel Bobolz
// <mailto:ergotamin.source@gmail.com>
//-------------------------------------
#
// [**`foreach`**](github.com/ergotamin/snippets)
// **`{`** ``` Fp: []( auto &p ){  p++; } ``` **`}`**
template<typename Tp, typename Fp>
constexpr Fp foreach(Tp _begin, Tp end_, Fp func)
{
    for (; _begin != end_; ++_begin)
        func(*_begin);
    return func;
}
//
