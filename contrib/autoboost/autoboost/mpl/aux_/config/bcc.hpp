
#ifndef AUTOBOOST_MPL_AUX_CONFIG_BCC_HPP_INCLUDED
#define AUTOBOOST_MPL_AUX_CONFIG_BCC_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date: 2004-09-02 10:41:37 -0500 (Thu, 02 Sep 2004) $
// $Revision: 24874 $

#include <autoboost/mpl/aux_/config/workaround.hpp>

#if    !defined(AUTOBOOST_MPL_CFG_BCC590_WORKAROUNDS) \
    && !defined(AUTOBOOST_MPL_PREPROCESSING_MODE) \
    && AUTOBOOST_WORKAROUND(__BORLANDC__, >= 0x590) \
    && AUTOBOOST_WORKAROUND(__BORLANDC__, AUTOBOOST_TESTED_AT(0x610))

#   define AUTOBOOST_MPL_CFG_BCC590_WORKAROUNDS

#endif

#endif // AUTOBOOST_MPL_AUX_CONFIG_BCC_HPP_INCLUDED
