#pragma once
#include TYPE_TRAITS_HEADER

class AutoFactoryBase
{
public:
  /// <summary>
  /// Returns type information for the proposed 
  /// </summary>
  virtual const type_info& GetType(void) const = 0;
};

template<class U>
static std::true_type sfinae(std::integral_constant<int, sizeof(U())>*);

template<class U>
static std::false_type sfinae(...);

template<class T>
class is_default_constructible:
  decltype(sfinae(nullptr))
{
};

/// <summary>
/// This is a marker wrapper for classes which must be constructed by a factory
/// </summary>
/// <remarks>
/// A context may contain, at most, one factory for a particular Product.  Introduction of
/// a factory into a context enables the construction of the Product type, and the
/// product's construction must have been impossible without the factory's presence.  This
/// can be done by making the product an abstract type, or by eliminating the trivial
/// constructor from the product.
///
///
/// </remarks>
template<class Product>
class AutoFactory
{
public:
  virtual Product* operator()(void) = 0;
};

/// <summary>
/// Utility type which simply invokes the static method "New" on the product type
/// </summary>
/// <remarks>
/// This method is useful in situations where the type in question defines a static
/// method "New" which can construct instances of the product type.
/// </remarks>
template<class Product>
class AutoStaticFactory:
  public AutoFactory<Product>
{
public:
  Product* operator()(void) {return Product::New();}
};