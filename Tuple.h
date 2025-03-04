#ifndef T_TUPLE_H
#define T_TUPLE_H

#include <cstddef>
#include <utility>

namespace tpl {
  template<std::size_t Idx, typename... Types>
  struct Tuple_Impl;

  template<std::size_t Idx>
  struct Tuple_Impl<Idx> {};

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

    Tuple_Impl<Idx+1, TailTypes...>& getTail() { return *this; }
    const Tuple_Impl<Idx+1, TailTypes...>& getTail() const { return *this; }
  };

  template<typename ... Types>
  struct Tuple : Tuple_Impl<0, Types...> {
    Tuple() = default;

    /**
     * Construct a tuple with the given arguments.
     * The line <code>template<typename NotUsedType = void, typename = std::enable_if_t<(sizeof...(Types) > 0), NotUsedType>></code>
     * is used to ensure that the constructor is only enabled if the tuple has at least one type to avoid override
     * the default constructor with no parameter. (SFINAE)
     *
     * @tparam NotUsedType a dummy type to enable the constructor
     * @param args the arguments to initialize the tuple
     */
    template<typename NotUsedType = void, typename = std::enable_if_t<(sizeof...(Types) > 0), NotUsedType>>
    explicit Tuple(const Types&... args) : Tuple_Impl<0, Types...>(args...) {}

    template<std::size_t Idx>
    auto& get() { return get_impl<Idx>(*this); }

    template<std::size_t Idx>
    const auto& get() const { return get_impl<Idx>(*this); }

    template <typename ... OtherTypes>
    auto operator+(const Tuple<OtherTypes...>& other) const {
      return plus_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    template <typename ... OtherTypes>
    auto& operator+=(const Tuple<OtherTypes...>& other) {
      return plus_eq_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    template <typename ... OtherTypes>
    auto operator-(const Tuple<OtherTypes...>& other) const {
      return minus_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    template <typename ... OtherTypes>
    auto& operator-=(const Tuple<OtherTypes...>& other) {
      return minus_eq_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    template <typename ... OtherTypes>
    auto operator*(const Tuple<OtherTypes...>& other) const {
      return times_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    template <typename ... OtherTypes>
    auto& operator*=(const Tuple<OtherTypes...>& other) {
      return times_eq_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    template <typename ... OtherTypes>
    auto operator/(const Tuple<OtherTypes...>& other) const {
      return divide_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    template <typename ... OtherTypes>
    auto& operator/=(const Tuple<OtherTypes...>& other) {
      return divide_eq_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    template <typename ... OtherTypes>
    bool operator==(const Tuple<OtherTypes...>& other) const {
      return equals_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    template <typename ... OtherTypes>
    bool operator!=(const Tuple<OtherTypes...>& other) const {
      return !equals_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    /**
     * Using the lexicographic comparison as said <a href="https://en.cppreference.com/w/cpp/utility/tuple/operator_cmp">here</a>.
     * @tparam OtherTypes the types of the other tuple
     * @param other the other tuple
     * @return true if the current tuple is lexicographically less than the other tuple
     */
    template <typename ... OtherTypes>
    bool operator<(const Tuple<OtherTypes...>& other) const {
      return lexic_compare(other);
    }

    /**
     * Using the lexicographic comparison as said <a href="https://en.cppreference.com/w/cpp/utility/tuple/operator_cmp">here</a>.
     * @tparam OtherTypes the types of the other tuple
     * @param other the other tuple
     * @return true if the current tuple is lexicographically less than or equal to the other tuple
     */
    template <typename ... OtherTypes>
    bool operator<=(const Tuple<OtherTypes...>& other) const {
      return !(other < *this);
    }

    /**
     * Using the lexicographic comparison as said <a href="https://en.cppreference.com/w/cpp/utility/tuple/operator_cmp">here</a>.
     * @tparam OtherTypes the types of the other tuple
     * @param other the other tuple
     * @return true if the current tuple is lexicographically greater than the other tuple
     */
    template <typename ... OtherTypes>
    bool operator>(const Tuple<OtherTypes...>& other) const {
      return other < *this;
    }

    /**
     * Using the lexicographic comparison as said <a href="https://en.cppreference.com/w/cpp/utility/tuple/operator_cmp">here</a>.
     * @tparam OtherTypes the types of the other tuple
     * @param other the other tuple
     * @return true if the current tuple is lexicographically greater than or equal to the other tuple
     */
    template <typename ... OtherTypes>
    bool operator>=(const Tuple<OtherTypes...>& other) const {
      return !(*this < other);
    }

    template <typename ... OtherTypes>
    auto operator|(Tuple<OtherTypes...>&& other) {
      return concat_impl(
        std::move(*this), std::move(other),
        std::make_index_sequence<sizeof...(Types)>{},
        std::make_index_sequence<sizeof...(OtherTypes)>{}
      );
    }


  private:
    template<std::size_t Idx, typename TupleImpl_SubType>
    static auto& get_impl(TupleImpl_SubType& t) {
      if constexpr (Idx == 0) {
        return t.getHead();
      } else {
        return get_impl<Idx - 1>(t.getTail());
      }
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    auto plus_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) const {
      return Tuple<decltype(this->get<Idx>() + other.template get<Idx>())...>(
        (this->get<Idx>() + other.template get<Idx>())...
      );
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    auto& plus_eq_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) {
      ((this->get<Idx>() += other.template get<Idx>()), ...);
      return *this;
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    auto minus_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) const {
      return Tuple<decltype(this->get<Idx>() - other.template get<Idx>())...>(
        (this->get<Idx>() - other.template get<Idx>())...
      );
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    auto& minus_eq_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) {
      ((this->get<Idx>() -= other.template get<Idx>()), ...);
      return *this;
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    auto times_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) const {
      return Tuple<decltype(this->get<Idx>() * other.template get<Idx>())...>(
        (this->get<Idx>() * other.template get<Idx>())...
      );
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    auto& times_eq_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) {
      ((this->get<Idx>() *= other.template get<Idx>()), ...);
      return *this;
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    auto divide_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) const {
      return Tuple<decltype(this->get<Idx>() / other.template get<Idx>())...>(
        (this->get<Idx>() / other.template get<Idx>())...
      );
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    auto& divide_eq_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) {
      ((this->get<Idx>() /= other.template get<Idx>()), ...);
      return *this;
    }

    template<std::size_t... Idx, typename ... OtherTypes>
    bool equals_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) const {
      return ((this->get<Idx>() == other.template get<Idx>()) && ...);
    }

    template<std::size_t... IL, std::size_t... IR, typename TupleL, typename TupleR>
    static auto concat_impl(TupleL&& lhs, TupleR&& rhs, std::index_sequence<IL...>, std::index_sequence<IR...>) {
      return Tuple<
        decltype(std::forward<TupleL>(lhs).template get<IL>()) ...,
        decltype(std::forward<TupleR>(rhs).template get<IR>()) ...
      >(
        std::forward<TupleL>(lhs).template get<IL>()...,
        std::forward<TupleR>(rhs).template get<IR>()...
      );
    }

    /**
     * Function used to compare two tuples lexicographically (cf <a href="https://en.cppreference.com/w/cpp/utility/tuple/operator_cmp">cppreference.com/...</a>).
     * @tparam I the index of the element to compare
     * @tparam OtherTypes the types inside the rhs tuple
     * @param other the rhs tuple
     * @return true if the lhs tuple is lexicographically less than the rhs tuple
     */
    template<std::size_t I = 0, typename... OtherTypes>
    constexpr bool lexic_compare(const Tuple<OtherTypes...>& other) const {
      if constexpr (I < sizeof...(Types)) {
        if (this->get<I>() < other.template get<I>())
          return true;
        if (other.template get<I>() < this->get<I>())
          return false;
        return lexic_compare<I+1>(other);
      } else {
        return false;
      }
    }

  };

  /**
   * Use the perfect forwarding to avoid
   * @tparam Types the types inside the tuple
   * @param args the values to put insides of the tuple
   * @return a tuple made with the given values
   */
  template <class... Types>
  constexpr auto makeTuple(Types&&... args) {
    return Tuple<std::decay_t<Types>...>(std::forward<Types>(args)...);
  }



}

#endif // T_TUPLE_H
