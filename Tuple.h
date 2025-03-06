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

    /**
     * In the implementation of the `tpl::Tuple::plus_impl` function, the `std::index_sequence<sizeof...(Types)>` given here is used to generate a sequence of indices
     * @tparam OtherTypes the types of the elements contained in the other tuple
     * @param other the other tuple
     * @return a new tuple containing the result of operation between the two tuples
     */
    template <typename ... OtherTypes>
    auto operator+(const Tuple<OtherTypes...>& other) const {
      return plus_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    /**
     * In the implementation of the `tpl::Tuple::plus_eq_impl` function, the `std::index_sequence<sizeof...(Types)>` is used to generate a sequence of indices
     * @tparam OtherTypes the types of the elements contained in the other tuple
     * @param other the other tuple
     * @return a new tuple containing the result of operation between the two tuples
     */
    template <typename ... OtherTypes>
    auto& operator+=(const Tuple<OtherTypes...>& other) {
      return plus_eq_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    /**
     * see `tpl::Tuple::operator+` for more details on the implementation with the `std::index_sequence`
     * @tparam OtherTypes the types of the elements contained in the other tuple
     * @param other the other tuple
     * @return a new tuple containing the result of operation between the two tuples
     */
    template <typename ... OtherTypes>
    auto operator-(const Tuple<OtherTypes...>& other) const {
      return minus_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    /**
     * see `tpl::Tuple::operator+=` for more details on the implementation with the `std::index_sequence`
     * @tparam OtherTypes the types of the elements contained in the other tuple
     * @param other the other tuple
     * @return a new tuple containing the result of operation between the two tuples
     */
    template <typename ... OtherTypes>
    auto& operator-=(const Tuple<OtherTypes...>& other) {
      return minus_eq_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    /**
     * see `tpl::Tuple::operator+` for more details on the implementation with the `std::index_sequence`
     * @tparam OtherTypes the types of the elements contained in the other tuple
     * @param other the other tuple
     * @return a new tuple containing the result of operation between the two tuples
     */
    template <typename ... OtherTypes>
    auto operator*(const Tuple<OtherTypes...>& other) const {
      return times_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    /**
     * see `tpl::Tuple::operator+=` for more details on the implementation with the `std::index_sequence`
     * @tparam OtherTypes the types of the elements contained in the other tuple
     * @param other the other tuple
     * @return a new tuple containing the result of operation between the two tuples
     */
    template <typename ... OtherTypes>
    auto& operator*=(const Tuple<OtherTypes...>& other) {
      return times_eq_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    /**
     * see `tpl::Tuple::operator+` for more details on the implementation with the `std::index_sequence`
     * @tparam OtherTypes the types of the elements contained in the other tuple
     * @param other the other tuple
     * @return a new tuple containing the result of operation between the two tuples
     */
    template <typename ... OtherTypes>
    auto operator/(const Tuple<OtherTypes...>& other) const {
      return divide_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    /**
     * see `tpl::Tuple::operator+=` for more details on the implementation with the `std::index_sequence`
     * @tparam OtherTypes the types of the elements contained in the other tuple
     * @param other the other tuple
     * @return a new tuple containing the result of operation between the two tuples
     */
    template <typename ... OtherTypes>
    auto& operator/=(const Tuple<OtherTypes...>& other) {
      return divide_eq_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    /**
     * see `tpl::Tuple::operator+` for more details on the implementation with the `std::index_sequence`
     * @tparam OtherTypes the types of the elements contained in the other tuple
     * @param other the other tuple
     * @return true if the two tuples are equals element by element, false otherwise
     */
    template <typename ... OtherTypes>
    bool operator==(const Tuple<OtherTypes...>& other) const {
      return equals_impl(other, std::make_index_sequence<sizeof...(Types)>{});
    }

    /**
     * see `tpl::Tuple::operator+` for more details on the implementation with the `std::index_sequence`
     * @tparam OtherTypes the types of the elements contained in the other tuple
     * @param other the other tuple
     * @return false if the two tuples are equals element by element, true otherwise
     */
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


    /**
     * In the definition of `operator+`, `std::make_index_sequence<sizeof...(Types)>` generate a sequence of index from 0 to `sizeof...(Types) - 1`
     * stored in an object of type `std::index_sequence`<br>
     *
     * The pack of indexes (`Idx...`) is used to access the elements of the tuples (`this` and `other`).
     * There is only one pack because `this` and `other` should have the same size, as said in the instructions.
     * Here, the type of each new element is given by the function `decltype`, for example, this Google test execute successfully:
     * <code>
    // We calculate the type of the result of the operation using decltype, here we have a double to not lose the precision.
    TEST(decltype, explained) {
      constexpr int i = 1;
      constexpr double d = 5.0;
      constexpr bool res = std::is_same_v<decltype(i + d), double>;
      EXPECT_EQ(res, true);
    }
     * </code>
     *
     * So we create a new Tuple, between `<` and `>` we calculate the new type of element, and between `(` and `)` we calculate the result of the operation.
     * And we do this for every couple of elements in the tuples using the `...` operator.
     *
     * @tparam Idx A `std:size_t...`. A pack of index generated by `std::index_sequence`.
     * @tparam OtherTypes The pack of types corresponding to the elements of the tuple given in arguments.
     * @param other the other tuple used to do the operation
     * @return A new tuple containing the result of the operation.
     */
    template<std::size_t... Idx, typename ... OtherTypes>
    auto plus_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) const {
      return Tuple<decltype(this->get<Idx>() + other.template get<Idx>())...>(
        (this->get<Idx>() + other.template get<Idx>())...
      );
    }

    /**
     * see tpl::Tuple::plus_impl for the full explanation of std::index_sequence.
     * Here we don't use `decltype` because we don't need to calculate the type of the new tuple because we only modify the current one.
     * @tparam Idx A `std:size_t...`. A pack of index generated by `std::index_sequence`.
     * @tparam OtherTypes The pack of types corresponding to the elements of the tuple given in arguments.
     * @param other the other tuple used to do the operation
     * @return A new tuple containing the result of the operation.
     */
    template<std::size_t... Idx, typename ... OtherTypes>
    auto& plus_eq_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) {
      ((this->get<Idx>() += other.template get<Idx>()), ...);
      return *this;
    }

    /**
     * see tpl::Tuple::plus_impl for the full explanation of std::index_sequence.
     * @tparam Idx A `std:size_t...`. A pack of index generated by `std::index_sequence`.
     * @tparam OtherTypes The pack of types corresponding to the elements of the tuple given in arguments.
     * @param other the other tuple used to do the operation
     * @return A new tuple containing the result of the operation.
     */
    template<std::size_t... Idx, typename ... OtherTypes>
    auto minus_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) const {
      return Tuple<decltype(this->get<Idx>() - other.template get<Idx>())...>(
        (this->get<Idx>() - other.template get<Idx>())...
      );
    }

    /**
     * see tpl::Tuple::plus_eq_impl for the full explanation of std::index_sequence.
     * @tparam Idx A `std:size_t...`. A pack of index generated by `std::index_sequence`.
     * @tparam OtherTypes The pack of types corresponding to the elements of the tuple given in arguments.
     * @param other the other tuple used to do the operation
     * @return A new tuple containing the result of the operation.
     */
    template<std::size_t... Idx, typename ... OtherTypes>
    auto& minus_eq_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) {
      ((this->get<Idx>() -= other.template get<Idx>()), ...);
      return *this;
    }

    /**
     * see tpl::Tuple::plus_impl for the full explanation of std::index_sequence.
     * @tparam Idx A `std:size_t...`. A pack of index generated by `std::index_sequence`.
     * @tparam OtherTypes The pack of types corresponding to the elements of the tuple given in arguments.
     * @param other the other tuple used to do the operation
     * @return A new tuple containing the result of the operation.
     */
    template<std::size_t... Idx, typename ... OtherTypes>
    auto times_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) const {
      return Tuple<decltype(this->get<Idx>() * other.template get<Idx>())...>(
        (this->get<Idx>() * other.template get<Idx>())...
      );
    }

    /**
     * see tpl::Tuple::plus_eq_impl for the full explanation of std::index_sequence.
     * @tparam Idx A `std:size_t...`. A pack of index generated by `std::index_sequence`.
     * @tparam OtherTypes The pack of types corresponding to the elements of the tuple given in arguments.
     * @param other the other tuple used to do the operation
     * @return A new tuple containing the result of the operation.
     */
    template<std::size_t... Idx, typename ... OtherTypes>
    auto& times_eq_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) {
      ((this->get<Idx>() *= other.template get<Idx>()), ...);
      return *this;
    }

    /**
     * see tpl::Tuple::plus_impl for the full explanation of std::index_sequence.
     * @tparam Idx A `std:size_t...`. A pack of index generated by `std::index_sequence`.
     * @tparam OtherTypes The pack of types corresponding to the elements of the tuple given in arguments.
     * @param other the other tuple used to do the operation
     * @return A new tuple containing the result of the operation.
     */
    template<std::size_t... Idx, typename ... OtherTypes>
    auto divide_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) const {
      return Tuple<decltype(this->get<Idx>() / other.template get<Idx>())...>(
        (this->get<Idx>() / other.template get<Idx>())...
      );
    }

    /**
     * see tpl::Tuple::plus_eq_impl for the full explanation of std::index_sequence.
     * @tparam Idx A `std:size_t...`. A pack of index generated by `std::index_sequence`.
     * @tparam OtherTypes The pack of types corresponding to the elements of the tuple given in arguments.
     * @param other the other tuple used to do the operation
     * @return A new tuple containing the result of the operation.
     */
    template<std::size_t... Idx, typename ... OtherTypes>
    auto& divide_eq_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) {
      ((this->get<Idx>() /= other.template get<Idx>()), ...);
      return *this;
    }

    /**
     * see tpl::Tuple::plus_impl for the full explanation of std::index_sequence.
     * Here we do not use `decltype` because we only return a bool.
     * @tparam Idx A `std:size_t...`. A pack of index generated by `std::index_sequence`.
     * @tparam OtherTypes The pack of types corresponding to the elements of the tuple given in arguments.
     * @param other the other tuple used to do the operation
     * @return A new tuple containing the result of the operation.
     */
    template<std::size_t... Idx, typename ... OtherTypes>
    bool equals_impl(const Tuple<OtherTypes...>& other, std::index_sequence<Idx...>) const {
      return ((this->get<Idx>() == other.template get<Idx>()) && ...);
    }

    /**
     * see tpl::Tuple::plus_impl for the global explanation of `std::index_sequence`.
     * Here we have two `std::index_sequence` because the tuples can have different sizes. So one `std::index_sequence` for each tuple.
     * @tparam IL A `std:size_t...`. A pack of index generated by `std::index_sequence` for the `lhs` tuple.
     * @tparam IR A `std:size_t...`. A pack of index generated by `std::index_sequence` for the `rhs` tuple.
     * @tparam TupleL The type of the `lhs` tuple containing the types of the elements contained in the `lhs` tuple given in argument of the function.
     * @tparam TupleR The type of the `rhs` tuple containing the types of the elements contained in the `rhs` tuple given in argument of the function.
     * @param lhs A rvalue reference to the `lhs` tuple.
     * @param rhs A rvalue reference to the `rhs` tuple.
     * @return A new tuple containing the concatenation of the elements contained in `lhs` and `rhs`.
     */
    template<std::size_t... IL, std::size_t... IR, typename TupleL, typename TupleR>
    static auto concat_impl(TupleL&& lhs, TupleR&& rhs, std::index_sequence<IL...>, std::index_sequence<IR...>) {
      return Tuple<
        std::decay_t<decltype(std::forward<TupleL>(lhs).template get<IL>())> ...,
        std::decay_t<decltype(std::forward<TupleR>(rhs).template get<IR>())> ...
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
      constexpr std::size_t left_size = sizeof...(Types);
      constexpr std::size_t right_size = sizeof...(OtherTypes);
      if constexpr (I < left_size && I < right_size) {
        if (this->get<I>() < other.template get<I>())
          return true;
        if (other.template get<I>() < this->get<I>())
          return false;
        return lexic_compare<I+1>(other);
      } else {
        return left_size < right_size;
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
  constexpr Tuple<std::decay_t<Types>...> makeTuple(Types&&... args) {
    return Tuple<std::decay_t<Types>...>(std::forward<Types>(args)...);
  }



}

#endif // T_TUPLE_H
