
//  (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes,
//      Howard Hinnant and John Maddock 2000.
//  (C) Copyright Mat Marcus, Jesse Jones and Adobe Systems Inc 2001

//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

//    Fixed is_pointer, is_reference, is_const, is_volatile, is_same,
//    is_member_pointer based on the Simulated Partial Specialization work
//    of Mat Marcus and Jesse Jones. See  http://opensource.adobe.com or
//    http://groups.yahoo.com/group/autoboost/message/5441
//    Some workarounds in here use ideas suggested from "Generic<Programming>:
//    Mappings between Types and Values"
//    by Andrei Alexandrescu (see http://www.cuj.com/experts/1810/alexandr.html).


#ifndef AUTOBOOST_TT_IS_SAME_HPP_INCLUDED
#define AUTOBOOST_TT_IS_SAME_HPP_INCLUDED

#include <autoboost/type_traits/config.hpp>
// should be the last #include
#include <autoboost/type_traits/detail/bool_trait_def.hpp>

namespace autoboost {


AUTOBOOST_TT_AUX_BOOL_TRAIT_DEF2(is_same,T,U,false)
AUTOBOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC2_1(typename T,is_same,T,T,true)
#if AUTOBOOST_WORKAROUND(__BORLANDC__, < 0x600)
// without this, Borland's compiler gives the wrong answer for
// references to arrays:
AUTOBOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC2_1(typename T,is_same,T&,T&,true)
#endif


} // namespace autoboost

#include <autoboost/type_traits/detail/bool_trait_undef.hpp>

#endif  // AUTOBOOST_TT_IS_SAME_HPP_INCLUDED

