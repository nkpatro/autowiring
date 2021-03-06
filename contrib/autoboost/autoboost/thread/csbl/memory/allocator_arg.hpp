// Copyright (C) 2013 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// 2013/10 Vicente J. Botet Escriba
//   Creation.

#ifndef AUTOBOOST_CSBL_MEMORY_ALLOCATOR_ARG_HPP
#define AUTOBOOST_CSBL_MEMORY_ALLOCATOR_ARG_HPP

#include <autoboost/thread/csbl/memory/config.hpp>

// 20.7.6, allocator argument tag
#if defined AUTOBOOST_NO_CXX11_ALLOCATOR
#include <autoboost/container/scoped_allocator.hpp>

namespace autoboost
{
  namespace csbl
  {
    using ::autoboost::container::allocator_arg_t;
    using ::autoboost::container::allocator_arg;
  }
}
#else
namespace autoboost
{
  namespace csbl
  {
    using ::std::allocator_arg_t;
    using ::std::allocator_arg;
  }
}
#endif // AUTOBOOST_NO_CXX11_ALLOCATOR
namespace autoboost
{
  using ::autoboost::csbl::allocator_arg_t;
  using ::autoboost::csbl::allocator_arg;
}
#endif // header
