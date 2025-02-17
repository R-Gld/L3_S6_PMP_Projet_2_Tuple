#ifndef T_TUPLE_H
#define T_TUPLE_H

namespace tpl {

  template<typename ... Types>
  class Tuple {
  public:
    /**
     * Default constructor
     */
    Tuple() {

    }

    /**
     * Constructor to initialize values
     */
    Tuple(Types ... default_values) {

    }

    /**
     * Value getter
     */
    template<int index>
    const /* implementation defined */& get() const {

    }

    template<int index>
    /* implementation defined */& get() {

    }

    /**
     * Addition between to tuples
     */
    template <typename ... OtherTypes>
    Tuple</* implementation defined */>& operator+(const Tuple<OtherTypes...>& other) const {

    }

    /**
     * Addition between to tuples - in place
     */
    template <typename ... OtherTypes>
    Tuple</* implementation defined */> operator+=(const Tuple<OtherTypes...>& other) {

    }

    /**
     * Substraction between to tuples
     */
    template <typename ... OtherTypes>
    Tuple</* implementation defined */> operator-(const Tuple<OtherTypes...>& other) const {

    }

    /**
     * Substraction between to tuples - in place
     */
    template <typename ... OtherTypes>
    Tuple</* implementation defined */>& operator-=(const Tuple<OtherTypes...>& other) {

    }

    /**
     * Multiplication between to tuples
     */
    template <typename ... OtherTypes>
    Tuple</* implementation defined */> operator*(const Tuple<OtherTypes...>& other) const {

    }

    /**
     * Multiplication between to tuples - in place
     */
    template <typename ... OtherTypes>
    Tuple</* implementation defined */>& operator*=(const Tuple<OtherTypes...>& other) {

    }

    /**
     * Division between to tuples
     */
    template <typename ... OtherTypes>
    Tuple</* implementation defined */> operator/(const Tuple<OtherTypes...>& other) const {

    }

    /**
     * Division between to tuples - in place
     */
    template <typename ... OtherTypes>
    Tuple</* implementation defined */>& operator/=(const Tuple<OtherTypes...>& other) {

    }

    /**
     * Comparaison operators
     */
    template <typename ... OtherTypes>
    bool operator==(const Tuple<OtherTypes...>& other) const {

    }

    template <typename ... OtherTypes>
    bool operator!=(const Tuple<OtherTypes...>& other) const {

    }

    template <typename ... OtherTypes>
    bool operator<(const Tuple<OtherTypes...>& other) const {

    }

    template <typename ... OtherTypes>
    bool operator<=(const Tuple<OtherTypes...>& other) const {

    }

    template <typename ... OtherTypes>
    bool operator>(const Tuple<OtherTypes...>& other) const {

    }

    template <typename ... OtherTypes>
    bool operator>=(const Tuple<OtherTypes...>& other) const {

    }

    /**
     * Concatenate two tuples
     */
    template <typename ... OtherTypes>
    Tuple</* implementation defined */> operator|(Tuple<OtherTypes...>&& other) {

    }
  };

  /**
   * Helper function to create a tuple
   */
  template <class... Types>
  constexpr Tuple<Types...> makeTuple(Types&&... args) {
    Tuple<Types...> tp(args);
    return tp;
  }

}

#endif // T_TUPLE_H
