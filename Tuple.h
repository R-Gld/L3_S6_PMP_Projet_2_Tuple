#ifndef T_TUPLE_H
#define T_TUPLE_H

#include <cstddef>
#include <utility>

namespace tpl {

  // Forward declaration of the recursive helper.
  template<std::size_t Idx, typename... Types>
  struct Tuple_Impl;

  // Base case: when no types remain.
  template<std::size_t Idx>
  struct Tuple_Impl<Idx> {};

  // Recursive case: store one element and inherit from the tail.
  template<std::size_t Idx, typename HeadType, typename ... TailTypes>
  struct Tuple_Impl<Idx, HeadType, TailTypes...> : Tuple_Impl<Idx+1, TailTypes...> {
  private:
    HeadType value;

  public:
    explicit Tuple_Impl(const HeadType& head, const TailTypes&... tail)
      : Tuple_Impl<Idx+1, TailTypes...>(tail...), value(head) {}

    Tuple_Impl() = default;

    HeadType& getHead() { return value; }
    const HeadType& getHead() const { return value; }

    // Returns the tail (which is itself a Tuple_Impl).
    Tuple_Impl<Idx+1, TailTypes...>& getTail() { return *this; }
    const Tuple_Impl<Idx+1, TailTypes...>& getTail() const { return *this; }
  };

  template<typename ... Types>
  class Tuple : public Tuple_Impl<0, Types...> {
  public:
    // Default constructor.
    Tuple() = default;

    // Constructor to initialize all values.
    explicit Tuple(const Types&... args)
      : Tuple_Impl<0, Types...>(args...) {}

    // Element access by index.
    template<std::size_t Idx>
    auto& get() { return get_impl<Idx>(*this); }
    template<std::size_t Idx>
    const auto& get() const { return get_impl<Idx>(*this); }

    // Addition between two tuples.
    template <typename ... OtherTypes>
    auto operator+(const Tuple<OtherTypes...>& other) const {
      static_assert(sizeof...(OtherTypes) == sizeof...(Types), "Tuples must have the same size.");
      return plus_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    // Addition between two tuples – in place.
    template <typename ... OtherTypes>
    auto& operator+=(const Tuple<OtherTypes...>& other) {
      static_assert(sizeof...(OtherTypes) == sizeof...(Types), "Tuples must have the same size.");
      plus_eq_impl(other, std::make_index_sequence<sizeof...(Types)>{});
      return *this;
    }


    // The following operators are placeholders for future implementation:
    template <typename ... OtherTypes>
    auto operator-(const Tuple<OtherTypes...>& other) const {
      static_assert(sizeof...(OtherTypes) == sizeof...(Types), "Tuples must have the same size.");
      return minus_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    template <typename ... OtherTypes>
    auto& operator-=(const Tuple<OtherTypes...>& other) {
      static_assert(sizeof...(OtherTypes) == sizeof...(Types), "Tuples must have the same size.");
      minus_eq_impl(other, std::make_index_sequence<sizeof...(Types)>{});
      return *this;
    }


    template <typename ... OtherTypes>
    auto operator*(const Tuple<OtherTypes...>& other) const {
      static_assert(sizeof...(OtherTypes) == sizeof...(Types), "Tuples must have the same size.");
      return times_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    template <typename ... OtherTypes>
    auto& operator*=(const Tuple<OtherTypes...>& other) {
      static_assert(sizeof...(OtherTypes) == sizeof...(Types), "Tuples must have the same size.");
      times_eq_impl(other, std::make_index_sequence<sizeof...(Types)>{});
      return *this;
    }


    template <typename ... OtherTypes>
    auto operator/(const Tuple<OtherTypes...>& other) const {
      static_assert(sizeof...(OtherTypes) == sizeof...(Types), "Tuples must have the same size.");
      return divide_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    template <typename ... OtherTypes>
    auto& operator/=(const Tuple<OtherTypes...>& other) {
      static_assert(sizeof...(OtherTypes) == sizeof...(Types), "Tuples must have the same size.");
      divide_eq_impl(other, std::make_index_sequence<sizeof...(Types)>{});
      return *this;
    }

    template <typename ... OtherTypes>
    bool operator==(const Tuple<OtherTypes...>& other) const {
      static_assert(sizeof...(OtherTypes) == sizeof...(Types), "Tuples must have the same size.");
      return equals_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    template <typename ... OtherTypes>
    bool operator!=(const Tuple<OtherTypes...>& other) const {
      static_assert(sizeof...(OtherTypes) == sizeof...(Types), "Tuples must have the same size.");
      return !equals_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }


    template <typename ... OtherTypes>
    bool operator<(const Tuple<OtherTypes...>& other) const {
      static_assert(sizeof...(OtherTypes) == sizeof...(Types), "Tuples must have the same size.");
      return inferior_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    template <typename ... OtherTypes>
    bool operator<=(const Tuple<OtherTypes...>& other) const {
      static_assert(sizeof...(OtherTypes) == sizeof...(Types), "Tuples must have the same size.");
      return inferior_or_equal_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    template <typename ... OtherTypes>
    bool operator>(const Tuple<OtherTypes...>& other) const {
      static_assert(sizeof...(OtherTypes) == sizeof...(Types), "Tuples must have the same size.");
      return superior_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    template <typename ... OtherTypes>
    bool operator>=(const Tuple<OtherTypes...>& other) const {
      static_assert(sizeof...(OtherTypes) == sizeof...(Types), "Tuples must have the same size.");
      return superior_or_equal_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    /*
    template <typename ... OtherTypes>
    Tuple operator|(Tuple<OtherTypes...>&& other) { }
    */

  private:
    // Recursive helper for get(): if Idx==0, return the head; otherwise, recurse on the tail.
    template<std::size_t Idx, typename TupleImpl_SubType>
    static auto& get_impl(TupleImpl_SubType& t) {
      if constexpr (Idx == 0) {
        return t.getHead();
      } else {
        return get_impl<Idx - 1>(t.getTail());
      }
    }

    // Helper for operator+: builds a new Tuple with element-wise addition.
    template<std::size_t... Idx, typename ... OtherTypes>
    auto plus_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) const {
      return Tuple<decltype(this->get<Idx>() + other.template get<Idx>())...>(
        (this->get<Idx>() + other.template get<Idx>())...
      );
    }

    // Helper for operator+=: performs element-wise addition in place.
    template<std::size_t... Idx, typename ... OtherTypes>
    void plus_eq_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) {
      ((this->get<Idx>() += other.template get<Idx>()), ...);
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    auto minus_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) const {
      return Tuple<decltype(this->get<Idx>() - other.template get<Idx>())...>(
        (this->get<Idx>() + other.template get<Idx>())...
      );
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    void minus_eq_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) {
      ((this->get<Idx>() -= other.template get<Idx>()), ...);
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    auto times_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) const {
      return Tuple<decltype(this->get<Idx>() - other.template get<Idx>())...>(
        (this->get<Idx>() * other.template get<Idx>())...
      );
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    void times_eq_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) {
      ((this->get<Idx>() *= other.template get<Idx>()), ...);
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    auto divide_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) const {
      return Tuple<decltype(this->get<Idx>() - other.template get<Idx>())...>(
        (this->get<Idx>() / other.template get<Idx>())...
      );
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    void divide_eq_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) {
      ((this->get<Idx>() /= other.template get<Idx>()), ...);
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    bool equals_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) const {
      return ((this->get<Idx>() == other.template get<Idx>()) && ...);
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    bool inferior_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) const {
      return ((this->get<Idx>() < other.template get<Idx>()) && ...);
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    bool inferior_or_equal_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) const {
      return ((this->get<Idx>() <= other.template get<Idx>()) && ...);
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    bool superior_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) const {
      return ((this->get<Idx>() > other.template get<Idx>()) && ...);
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    bool superior_or_equal_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) const {
      return ((this->get<Idx>() >= other.template get<Idx>()) && ...);
    }
  };

  // Helper function to create a tuple (uses perfect forwarding).
  template <class... Types>
  constexpr Tuple<Types...> makeTuple(Types&&... args) {
    return Tuple<Types...>(std::forward<Types>(args)...);
  }

}

#endif // T_TUPLE_H
