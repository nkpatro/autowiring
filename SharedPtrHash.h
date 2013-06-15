// Copyright (c) 2010 - 2013 Leap Motion. All rights reserved. Proprietary and confidential.
#ifndef _SHARED_PTR_HASH_H
#define _SHARED_PTR_HASH_H
#include SHARED_PTR_HEADER
#include <memory>

/// <summary>
/// A simple hash class that allows the use of a shared pointer in a hash_set
/// </summary>
template<class T>
class SharedPtrHash
{
public:
  static const size_t bucket_size = 1;
  static const size_t min_buckets = 8;

  size_t operator()(const std::shared_ptr<T>& _Key) const {
    return (size_t)_Key.get();
  }

  bool operator()(const std::shared_ptr<T>& lh, const std::shared_ptr<T>& rh) const {
    return lh == rh;
  }
};

#endif