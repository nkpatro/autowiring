//
// detail/winrt_resolve_op.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef AUTOBOOST_ASIO_DETAIL_WINRT_RESOLVE_OP_HPP
#define AUTOBOOST_ASIO_DETAIL_WINRT_RESOLVE_OP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <autoboost/asio/detail/config.hpp>

#if defined(AUTOBOOST_ASIO_WINDOWS_RUNTIME)

#include <autoboost/asio/detail/addressof.hpp>
#include <autoboost/asio/detail/bind_handler.hpp>
#include <autoboost/asio/detail/fenced_block.hpp>
#include <autoboost/asio/detail/handler_alloc_helpers.hpp>
#include <autoboost/asio/detail/handler_invoke_helpers.hpp>
#include <autoboost/asio/detail/winrt_async_op.hpp>
#include <autoboost/asio/ip/basic_resolver_iterator.hpp>
#include <autoboost/asio/error.hpp>

#include <autoboost/asio/detail/push_options.hpp>

namespace autoboost {
namespace asio {
namespace detail {

template <typename Protocol, typename Handler>
class winrt_resolve_op :
  public winrt_async_op<
    Windows::Foundation::Collections::IVectorView<
      Windows::Networking::EndpointPair^>^>
{
public:
  AUTOBOOST_ASIO_DEFINE_HANDLER_PTR(winrt_resolve_op);

  typedef typename Protocol::endpoint endpoint_type;
  typedef autoboost::asio::ip::basic_resolver_query<Protocol> query_type;
  typedef autoboost::asio::ip::basic_resolver_iterator<Protocol> iterator_type;

  winrt_resolve_op(const query_type& query, Handler& handler)
    : winrt_async_op<
        Windows::Foundation::Collections::IVectorView<
          Windows::Networking::EndpointPair^>^>(
            &winrt_resolve_op::do_complete),
      query_(query),
      handler_(AUTOBOOST_ASIO_MOVE_CAST(Handler)(handler))
  {
  }

  static void do_complete(io_service_impl* owner, operation* base,
      const autoboost::system::error_code&, std::size_t)
  {
    // Take ownership of the operation object.
    winrt_resolve_op* o(static_cast<winrt_resolve_op*>(base));
    ptr p = { autoboost::asio::detail::addressof(o->handler_), o, o };

    AUTOBOOST_ASIO_HANDLER_COMPLETION((o));

    iterator_type iterator = iterator_type();
    if (!o->ec_)
    {
      try
      {
        iterator = iterator_type::create(
            o->result_, o->query_.hints(),
            o->query_.host_name(), o->query_.service_name());
      }
      catch (Platform::Exception^ e)
      {
        o->ec_ = autoboost::system::error_code(e->HResult,
            autoboost::system::system_category());
      }
    }

    // Make a copy of the handler so that the memory can be deallocated before
    // the upcall is made. Even if we're not about to make an upcall, a
    // sub-object of the handler may be the true owner of the memory associated
    // with the handler. Consequently, a local copy of the handler is required
    // to ensure that any owning sub-object remains valid until after we have
    // deallocated the memory here.
    detail::binder2<Handler, autoboost::system::error_code, iterator_type>
      handler(o->handler_, o->ec_, iterator);
    p.h = autoboost::asio::detail::addressof(handler.handler_);
    p.reset();

    // Make the upcall if required.
    if (owner)
    {
      fenced_block b(fenced_block::half);
      AUTOBOOST_ASIO_HANDLER_INVOCATION_BEGIN((handler.arg1_, handler.arg2_));
      autoboost_asio_handler_invoke_helpers::invoke(handler, handler.handler_);
      AUTOBOOST_ASIO_HANDLER_INVOCATION_END;
    }
  }

private:
  query_type query_;
  Handler handler_;
};

} // namespace detail
} // namespace asio
} // namespace autoboost

#include <autoboost/asio/detail/pop_options.hpp>

#endif // defined(AUTOBOOST_ASIO_WINDOWS_RUNTIME)

#endif // AUTOBOOST_ASIO_DETAIL_WINRT_RESOLVE_OP_HPP
