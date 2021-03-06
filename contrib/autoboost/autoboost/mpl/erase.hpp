
#ifndef AUTOBOOST_MPL_ERASE_HPP_INCLUDED
#define AUTOBOOST_MPL_ERASE_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <autoboost/mpl/erase_fwd.hpp>
#include <autoboost/mpl/sequence_tag.hpp>
#include <autoboost/mpl/aux_/erase_impl.hpp>
#include <autoboost/mpl/aux_/na_spec.hpp>
#include <autoboost/mpl/aux_/lambda_support.hpp>
#include <autoboost/mpl/aux_/config/msvc_typename.hpp>

namespace autoboost { namespace mpl {

template<
      typename AUTOBOOST_MPL_AUX_NA_PARAM(Sequence)
    , typename AUTOBOOST_MPL_AUX_NA_PARAM(First)
    , typename AUTOBOOST_MPL_AUX_NA_PARAM(Last)
    >
struct erase
    : erase_impl< typename sequence_tag<Sequence>::type >
        ::template apply< Sequence,First,Last >
{
    AUTOBOOST_MPL_AUX_LAMBDA_SUPPORT(3,erase,(Sequence,First,Last))
};

AUTOBOOST_MPL_AUX_NA_SPEC(3,erase)

}}

#endif // AUTOBOOST_MPL_ERASE_HPP_INCLUDED
