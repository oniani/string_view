/*
 * MIT License
 *
 * Copyright (c) 2022 David Oniani
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef STRING_VIEW_HPP
#define STRING_VIEW_HPP

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <ios>
#include <iostream>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <string>

// NOTE: Implemented according to https://en.cppreference.com/w/cpp/string/basic_string_view
template <class CharT, class Traits = std::char_traits<CharT>>
class basic_string_view {
 private:
  // Type aliases {{{

  using Traits_type = Traits;
  using value_type = CharT;

  using pointer = value_type*;
  using const_pointer = const value_type*;
  using reference = value_type&;
  using const_reference = const value_type&;

  using iterator = const_pointer;
  using const_iterator = iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = reverse_iterator;

  using size_type = size_t;
  using difference_type = ptrdiff_t;

  // }}}

  // Helpers {{{

  static constexpr size_type npos = size_type(-1);
  static constexpr size_type k_max_size = std::numeric_limits<size_type>::max();

  constexpr static size_type __strlen(const_pointer data) {
    return data == nullptr ? 0 : Traits_type::length(data);
  }

  // }}}

  // Fields {{{

  const_pointer m_data;
  size_type m_size;

  // }}}

 public:
  // Constructors and assignment {{{

  /**
   * @brief Default constructor. Constructs an empty basic_string_view. After construction, `data()`
   * is equal to `nullptr`, and `size()` is equal to `0`.
   */
  constexpr basic_string_view() noexcept : m_data(nullptr), m_size(0) {}

  /**
   * @brief Copy constructor. Constructs a view of the same content as other. After construction,
   * `data()` is equal to `other.data()`, and `size()` is equal to `other.size()`.
   */
  constexpr basic_string_view(const basic_string_view& other) noexcept = default;

  /**
   * @brief Constructs a view of the first count characters of the character array starting with the
   * element pointed by `s`. `s` can contain null characters. The behavior is undefined if `[s,
   * s+count)` is not a valid range (even though the constructor may not access any of the elements
   * of this range). After construction, `data()` is equal to s, and `size()` is equal to count.
   */
  constexpr basic_string_view(const CharT* s, size_type count) : m_data(s), m_size(count) {}

  /**
   * @brief Constructs a view of the null-terminated character string pointed to by `s`, not
   * including the terminating null character. The length of the view is determined as if by
   * `Traits::length(s)`. The behavior is undefined if `[s, s+Traits::length(s))` is not a valid
   * range. After construction, `data()` is equal to `s`, and `size()` is equal to
   * `Traits::length(s)`.
   */
  constexpr basic_string_view(const CharT* s) : m_data(s), m_size(__strlen(s)) {}

  // NOT YET IMPLEMENTED
  template <class It, class End>
  constexpr basic_string_view(It first, End last);

  // NOT YET IMPLEMENTED
  template <class R>
  constexpr basic_string_view(R&& r);

  /**
   * @brief `basic_string_view` cannot be constructed from `nullptr`.
   */
  constexpr basic_string_view(std::nullptr_t) = delete;

  // }}}

  // operator= {{{

  /**
   * @brief Replaces the view with that of `view`.
   * @param view view to copy.
   * @return `*this`.
   */
  constexpr basic_string_view& operator=(const basic_string_view& view) noexcept = default;

  // }}}

  // Iterators {{{

  /**
   * @brief Returns an iterator to the first character of the view.
   * @return `const_iterator` to the first character.
   */
  constexpr const_iterator begin() const noexcept { return m_data; }

  /**
   * @brief Returns an iterator to the first character of the view.
   * @return `const_iterator` to the first character.
   */
  constexpr const_iterator cbegin() const noexcept { return begin(); }

  /**
   * @brief Returns an iterator to the character following the last character of the view. This
   * character acts as a placeholder, attempting to access it results in undefined behavior.
   * @return `const_iterator` to the character following the last character.
   */
  constexpr const_iterator end() const noexcept { return m_data + m_size; }

  /**
   * @brief Returns an iterator to the character following the last character of the view. This
   * character acts as a placeholder, attempting to access it results in undefined behavior.
   * @return `const_iterator` to the character following the last character.
   */
  constexpr const_iterator cend() const noexcept { return end(); }

  /**
   * @brief Returns a reverse iterator to the first character of the reversed view. It corresponds
   * to the last character of the non-reversed view.
   * @return `const_reverse_iterator` to the first character.
   */
  constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }

  /**
   * @brief Returns a reverse iterator to the first character of the reversed view. It corresponds
   * to the last character of the non-reversed view.
   * @return `const_reverse_iterator` to the first character.
   */
  constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }

  /**
   * @brief Returns a reverse iterator to the character following the last character of the reversed
   * view. It corresponds to the character preceding the first character of the non-reversed view.
   * This character acts as a placeholder, attempting to access it results in undefined behavior.
   * @return `const_reverse_iterator` to the character following the last character.
   */
  constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

  /**
   * @brief Returns a reverse iterator to the character following the last character of the reversed
   * view. It corresponds to the character preceding the first character of the non-reversed view.
   * This character acts as a placeholder, attempting to access it results in undefined behavior.
   * @return `const_reverse_iterator` to the character following the last character.
   */
  constexpr const_reverse_iterator crend() const noexcept { return rend(); }

  // }}}

  // Element access {{{

  /**
   * @brief Returns a const reference to the character at specified location pos. No bounds checking
   * is performed: the behavior is undefined if `pos >= size()`.
   * @param pos position of the character to return.
   * @return const reference to the requested character.
   */
  constexpr const_reference operator[](size_type pos) const { return m_data[pos]; }

  /**
   * @brief Returns a const reference to the character at specified location pos. Bounds checking is
   * performed, exception of type `std::out_of_range` will be thrown on invalid access.
   * @param pos position of the character to return.
   */
  constexpr const_reference at(size_type pos) const {
    return (pos < m_size) ? m_data[pos] : throw std::out_of_range("Out of range.");
  }

  /**
   * @brief Returns reference to the first character in the view. The behavior is undefined if
   * `empty() == true`.
   * @return reference to the first character, equivalent to operator[](0).
   */
  constexpr const_reference front() const { return m_data[0]; }

  /**
   * @brief Returns reference to the last character in the view. The behavior is undefined if
   * `empty() == true`.
   * @return reference to the last character, equivalent to `operator[](size() - 1)`.
   */
  constexpr const_reference back() const { return m_data[m_size - 1]; }

  /**
   * @brief Returns a pointer to the underlying character array. The pointer is such that the range
   * `[data(); data() + size())` is valid and the values in it correspond to the values of the view.
   * @return a pointer to the underlying character array.
   */
  constexpr const_pointer data() const noexcept { return m_data; }

  // }}}

  // Capacity {{{

  /**
   * @brief Returns the number of `CharT` elements in the view.
   * @return the number of `CharT` elements in the view.
   */
  constexpr size_type size() const noexcept { return m_size; }

  /**
   * @brief Returns the number of `CharT` elements in the view.
   * @return the number of `CharT` elements in the view.
   */
  constexpr size_type length() const noexcept { return m_size; }

  /**
   * @brief The largest possible number of char-like objects that can be referred to by a
   * `basic_string_view`.
   * @return maximum number of characters.
   */
  constexpr size_type max_size() const noexcept { return k_max_size; }

  /**
   * @brief Checks if the view has no characters, i.e. `whether size() == 0`.
   * @return `true` if the view is empty, `false` otherwise.
   */
  [[nodiscard]] constexpr bool empty() const noexcept { return m_size == 0; }

  // }}}

  // Modifiers {{{

  /**
   * @brief Moves the start of the view forward by n characters.
   * @param n number of characters to remove from the start of the view.
   */
  constexpr void remove_prefix(size_type n) {
    m_data += n;
    m_size -= n;
  }

  /**
   * @brief Moves the end of the view back by n characters.
   * @param n number of characters to remove from the end of the view.
   */
  constexpr void remove_suffix(size_type n) { m_size -= n; }

  /**
   * @brief Exchanges the view with that of `v`.
   * @param s the string view for swapping.
   */
  constexpr void swap(basic_string_view& v) noexcept {
    auto other = *this;
    *this = v;
    v = other;
  }

  // }}}

  // Operations {{{

  // copy {{{

  /**
   * @brief Copies the substring `[pos, pos + rcount)` to the character array pointed to by dest,
   * where `rcount` is the smaller of count and `size() - pos`.
   * @param dest Pointer to the destination character string.
   * @param count Requested substring length.
   * @param pos Position of the first character.
   * @return Number of characters copied.
   */
  constexpr size_type copy(CharT* dest, size_type count, size_type pos = 0) const {
    if (m_size < pos) {
      throw std::out_of_range("Out of range");
    }
    const size_type rcount = std::min(count, m_size - pos);
    Traits_type::copy(dest, m_data + pos, rcount);
    return rcount;
  }

  // }}}

  // substr {{{

  /**
   * @brief Returns a view of the substring [pos, pos + rcount), where rcount is the smaller of
   * count and `size() - pos`.
   * @param pos Position of the first character.
   * @param count Requested length.
   * @return View of the substring `[pos, pos + rcount)`.
   */
  constexpr basic_string_view substr(size_type pos = 0, size_type count = npos) const {
    if (m_size < pos) {
      throw std::out_of_range("Out of range");
    }
    const size_type rcount = std::min(count, m_size - pos);
    return (rcount <= 0) ? basic_string_view() : basic_string_view(m_data + pos, rcount);
  }

  // }}}

  // compare {{{

  /**
   * @brief The length `rlen` of the sequences to compare is the smaller of `size()` and `v.size()`.
   * The function compares the two views by calling `Traits::compare(data(), v.data(), rlen)`, and
   * returns a value according to the following table.
   * @param v view to compare.
   * @return negative value if this view is less than the other character sequence, zero if the both
   * character sequences are equal, positive value if this view is greater than the other character
   * sequence.
   */
  constexpr int compare(basic_string_view v) const noexcept {
    const size_t rlen = std::min(m_size, v.m_size);
    const int comparison = Traits_type::compare(m_data, v.m_data, rlen);
    if (comparison != 0) {
      return comparison;
    }
    if (m_size == v.m_size) {
      return 0;
    }
    return m_size < v.m_size ? -1 : 1;
  }

  /**
   * @brief Equivalent to `substr(pos1, count1).compare(v)`.
   * @param pos1 position of the first character in this view to compare.
   * @param count1 number of characters of this view to compare.
   * @param v view to compare.
   * @return negative value if this view is less than the other character sequence, zero if the both
   * character sequences are equal, positive value if this view is greater than the other character
   * sequence.
   */
  constexpr int compare(size_type pos1, size_type count1, basic_string_view v) const {
    return substr(pos1, count1).compare(v);
  }

  /**
   * @brief Equivalent to `substr(pos1, count1).compare(v.substr(pos2, count2))`.
   * @param pos1 position of the first character in this view to compare.
   * @param count1 number of characters of this view to compare.
   * @param v view to compare.
   * @param pos2 position of the first character of the given view to compare.
   * @param count2 number of characters of the given view to compare.
   * @return negative value if this view is less than the other character sequence, zero if the both
   * character sequences are equal, positive value if this view is greater than the other character
   * sequence.
   */
  constexpr int compare(size_type pos1, size_type count1, basic_string_view v, size_type pos2,
                        size_type count2) const {
    return substr(pos1, count1).compare(v.substr(pos2, count2));
  }

  /**
   * @brief Equivalent to `compare(basic_string_view(s))`.
   * @param s pointer to the character string to compare to.
   * @return negative value if this view is less than the other character sequence, zero if the both
   * character sequences are equal, positive value if this view is greater than the other character
   * sequence.
   */
  constexpr int compare(const CharT* s) const { return compare(basic_string_view(s)); }

  /**
   * @brief Equivalent to `substr(pos1, count1).compare(basic_string_view(s))`.
   * @param pos1 position of the first character in this view to compare.
   * @param count1 number of characters of this view to compare.
   * @param s pointer to the character string to compare to.
   * @return negative value if this view is less than the other character sequence, zero if the both
   * character sequences are equal, positive value if this view is greater than the other character
   * sequence.
   */
  constexpr int compare(size_type pos1, size_type count1, const CharT* s) const {
    return substr(pos1, count1).compare(basic_string_view(s));
  }

  /**
   * @brief Equivalent to `substr(pos1, count1).compare(basic_string_view(s, count2))`.
   * @param pos1 position of the first character in this view to compare.
   * @param count1 number of characters of this view to compare.
   * @param s pointer to the character string to compare to.
   * @param count2 number of characters of the given view to compare.
   * @return negative value if this view is less than the other character sequence, zero if the both
   * character sequences are equal, positive value if this view is greater than the other character
   * sequence.
   */
  constexpr int compare(size_type pos1, size_type count1, const CharT* s, size_type count2) const {
    return substr(pos1, count1).compare(basic_string_view(s, count2));
  }

  // }}}

  // starts_with {{{

  /**
   * @brief Checks if the string view begins with the given prefix, where the prefix is a string
   * view. Effectively returns `substr(0, sv.size()) == sv`.
   * @param sv a string view which may be a result of implicit conversion from `std::basic_string`.
   * @return `true` if the string view begins with the provided prefix, `false` otherwise.
   */
  constexpr bool starts_with(basic_string_view sv) const noexcept {
    return m_size >= sv.m_size && substr(0, sv.m_size) == sv;
  }

  /**
   * @brief Checks if the string view begins with the given prefix, where the prefix is a single
   * character. Effectively returns `!empty() && Traits::eq(front(), c)`.
   * @param c a single character.
   * @return `true` if the string view begins with the provided prefix, `false` otherwise.
   */
  constexpr bool starts_with(value_type c) const noexcept {
    return !empty() && Traits_type::eq(front(), c);
  }

  /**
   * @brief Checks if the string view begins with the given prefix, where the prefix is a
   * null-terminated character string. Effectively returns `starts_with(basic_string_view(s))`.
   * @param s a null-terminated character string.
   * @return `true` if the string view begins with the provided prefix, `false` otherwise.
   */
  constexpr bool starts_with(const_pointer s) const {
    return starts_with(basic_string_view<CharT, Traits>(s));
  }

  // }}}

  // ends_with {{{

  /**
   * @brief Checks if the string view ends with the given suffix, where the suffix is a string view.
   * Effectively returns `size() >= sv.size() && compare(size() - sv.size(), npos, sv) == 0`.
   * @param sv a string view which may be a result of implicit conversion from `std::basic_string`.
   * @return `true` if the string view ends with the provided suffix, `false` otherwise.
   */
  constexpr bool ends_with(basic_string_view sv) const noexcept {
    return m_size >= sv.m_size && substr(m_size - sv.m_size, npos) == sv;
  }

  /**
   * @brief Checks if the string view ends with the given suffix, where the suffix is a single
   * character. Effectively returns `!empty() && Traits::eq(back(), c)`.
   * @param c a single character.
   * @return `true` if the string view ends with the provided suffix, `false` otherwise.
   */
  constexpr bool ends_with(CharT c) const noexcept {
    return !empty() && Traits_type::eq(back(), c);
  }

  /**
   * @brief Checks if the string view ends with the given suffix, where the suffix is a
   * null-terminated character string. Effectively returns `ends_with(basic_string_view(s))`.
   * @param s a null-terminated character string.
   * @return `true` if the string view ends with the provided suffix, `false` otherwise.
   */
  constexpr bool ends_with(const CharT* s) const {
    return ends_with(basic_string_view<CharT, Traits>(s));
  }

  // }}}

  // find {{{

  /**
   * @brief Finds the first substring equal to the given character sequence. Finds the first
   * occurence of v in this view, starting at position pos.
   * @param v view to search for.
   * @param pos position at which to start the search.
   * @return Position of the first character of the found substring, or `npos` if no such substring
   * is found.
   */
  constexpr size_type find(basic_string_view v, size_type pos = 0) const noexcept {
    if (empty() && v.empty() && pos == 0) {
      return 0;
    }

    if (pos > m_size || v.m_size > (m_size - pos)) {
      return npos;
    }

    while (pos + v.m_size <= m_size) {
      if (Traits_type::compare(m_data + pos, v.m_data, v.m_size) == 0) {
        return pos;
      }
      pos++;
    }

    return npos;
  }

  /**
   * @brief Finds the first substring equal to the given character sequence. Equivalent to
   * `find(basic_string_view(std::addressof(ch), 1), pos)`.
   * @param ch view to search for.
   * @param pos position at which to start the search.
   * @return Position of the first character of the found substring, or `npos` if no such substring
   * is found.
   */
  constexpr size_type find(CharT ch, size_type pos = 0) const noexcept {
    return find(basic_string_view(&ch, 1), pos);
  }

  /**
   * @brief Finds the first substring equal to the given character sequence. Equivalent to
   * @param ch view to search for.
   * @param pos position at which to start the search.
   * @return Position of the first character of the found substring, or `npos` if no such substring
   * is found.
   */
  constexpr size_type find(const CharT* s, size_type pos, size_type count) const {
    return find(basic_string_view(s, count), pos);
  }

  /**
   * @brief Finds the first substring equal to the given character sequence. Equivalent to
   * @param ch view to search for.
   * @param pos position at which to start the search.
   * @return Position of the first character of the found substring, or `npos` if no such substring
   * is found.
   */
  constexpr size_type find(const CharT* s, size_type pos = 0) const {
    return find(basic_string_view(s), pos);
  }

  // }}}

  // }}}

  // The rest of the class stuff {{{

  // rfind {{{

  size_type rfind(basic_string_view s, size_type pos = npos) const noexcept {
    if (s.empty()) {
      return std::min(pos, m_size);
    }
    if (s.m_size > m_size) {
      return npos;
    }
    pos = std::min(pos, m_size - s.m_size);
    while (pos != npos) {
      if (Traits_type::compare(m_data + pos, s.m_data, s.m_size) == 0) {
        return pos;
      }

      pos--;
    }

    return npos;
  }

  size_type rfind(value_type c, size_type pos = npos) const noexcept {
    return rfind(basic_string_view(&c, 1), pos);
  }

  size_type rfind(const_pointer s, size_type pos, size_type n) const {
    return rfind(basic_string_view(s, n), pos);
  }

  size_type rfind(const_pointer s, size_type pos = npos) const {
    return rfind(basic_string_view(s), pos);
  }

  // }}}

  // find_first_of {{{

  size_type find_first_of(basic_string_view s, size_type pos = 0) const noexcept {
    while (pos < m_size) {
      if (Traits_type::find(s.m_data, s.m_size, m_data[pos]) != nullptr) {
        return pos;
      }

      pos++;
    }

    return npos;
  }

  size_type find_first_of(value_type c, size_type pos = 0) const noexcept {
    return find_first_of(basic_string_view(&c, 1), pos);
  }

  size_type find_first_of(const_pointer s, size_type pos, size_type n) const {
    return find_first_of(basic_string_view(s, n), pos);
  }

  size_type find_first_of(const_pointer s, size_type pos = 0) const {
    return find_first_of(basic_string_view(s), pos);
  }

  // }}}

  // find_last_of {{{

  size_type find_last_of(basic_string_view s, size_type pos = npos) const noexcept {
    if (empty()) {
      return npos;
    }

    pos = std::min(pos, m_size - 1);
    while (pos != npos) {
      if (Traits_type::find(s.m_data, s.m_size, m_data[pos]) != nullptr) {
        return pos;
      }

      pos--;
    }

    return npos;
  }

  size_type find_last_of(value_type c, size_type pos = npos) const noexcept {
    return find_last_of(basic_string_view(&c, 1), pos);
  }

  size_type find_last_of(const_pointer s, size_type pos, size_type n) const {
    return find_last_of(basic_string_view(s, n), pos);
  }

  size_type find_last_of(const_pointer s, size_type pos = npos) const {
    return find_last_of(basic_string_view(s), pos);
  }

  // }}}

  // find_first_not_of {{{

  size_type find_first_not_of(basic_string_view s, size_type pos = 0) const noexcept {
    while (pos < m_size) {
      if (Traits_type::find(s.m_data, s.m_size, m_data[pos]) == nullptr) {
        return pos;
      }

      pos++;
    }

    return npos;
  }

  size_type find_first_not_of(value_type c, size_type pos = 0) const noexcept {
    return find_first_not_of(basic_string_view(&c, 1), pos);
  }

  size_type find_first_not_of(const_pointer s, size_type pos, size_type n) const {
    return find_first_not_of(basic_string_view(s, n), pos);
  }

  size_type find_first_not_of(const_pointer s, size_type pos = 0) const {
    return find_first_not_of(basic_string_view(s), pos);
  }

  // }}}

  // find_last_not_of {{{

  size_type find_last_not_of(basic_string_view s, size_type pos = npos) const noexcept {
    if (empty()) {
      return npos;
    }

    pos = std::min(pos, m_size - 1);
    while (pos != npos) {
      if (Traits_type::find(s.m_data, s.m_size, m_data[pos]) == nullptr) {
        return pos;
      }

      pos--;
    }

    return npos;
  }

  size_type find_last_not_of(value_type c, size_type pos = npos) const noexcept {
    return find_last_not_of(basic_string_view(&c, 1), pos);
  }

  size_type find_last_not_of(const_pointer s, size_type pos, size_type n) const {
    return find_last_not_of(basic_string_view(s, n), pos);
  }

  size_type find_last_not_of(const_pointer s, size_type pos = npos) const {
    return find_last_not_of(basic_string_view(s), pos);
  }

  // }}}

  // compare friend functions {{{

  // We need these to be friend non-member functions because we want to compare
  // operands with different types (for example string_view vs const char*).
  // https://stackoverflow.com/a/3850120
  friend inline bool operator==(basic_string_view a, basic_string_view b) noexcept {
    return a.compare(b) == 0;
  }

  friend inline bool operator!=(basic_string_view a, basic_string_view b) noexcept {
    return a.compare(b) != 0;
  }

  friend inline bool operator<(basic_string_view a, basic_string_view b) noexcept {
    return a.compare(b) < 0;
  }

  friend inline bool operator>(basic_string_view a, basic_string_view b) noexcept {
    return a.compare(b) > 0;
  }

  friend inline bool operator<=(basic_string_view a, basic_string_view b) noexcept {
    return a.compare(b) <= 0;
  }

  friend inline bool operator>=(basic_string_view a, basic_string_view b) noexcept {
    return a.compare(b) >= 0;
  }

  // }}}

  // Printing {{{

  friend std::ostream& operator<<(std::ostream& os, basic_string_view s) {
    std::ostream::sentry sentry{os};

    if (!sentry) {
      return os;
    }

    // Ideas from:
    // https://stackoverflow.com/q/39653508
    os.put('"');

    size_t padding = 0;
    char filler = os.fill();
    if (s.size() < static_cast<unsigned long>(os.width())) {
      padding = os.width() - s.size();
    }

    bool align_left = os.flags() & std::ios_base::left;
    if (padding > 0 && !align_left) {
      while (--padding) {
        os.put(filler);
      }
    }
    os.write(s.data(), s.size());
    if (padding > 0 && align_left) {
      while (--padding) {
        os.put(filler);
      }
    }
    os.put('"');

    os.width(0);

    return os;
  }

  // }}}

  // }}}
};

// Specializations {{{

template <class CharT, class Traits>
constexpr
    typename basic_string_view<CharT, Traits>::size_type basic_string_view<CharT, Traits>::npos;

template <class CharT, class Traits>
constexpr typename basic_string_view<CharT, Traits>::size_type
    basic_string_view<CharT, Traits>::k_max_size;

using string_view = basic_string_view<char>;
using u16string_view = basic_string_view<char16_t>;
using u32string_view = basic_string_view<char32_t>;
using wstring_view = basic_string_view<wchar_t>;

inline string_view operator"" _sv(const char* data, std::size_t size) noexcept {
  return string_view(data, size);
}

inline u16string_view operator"" _sv(const char16_t* data, std::size_t size) noexcept {
  return u16string_view(data, size);
}

inline u32string_view operator"" _sv(const char32_t* data, std::size_t size) noexcept {
  return u32string_view(data, size);
}

inline wstring_view operator"" _sv(const wchar_t* data, std::size_t size) noexcept {
  return wstring_view(data, size);
}

// }}}

// namespace std {{{

namespace std {
  template <>
  struct hash<::string_view> {
    size_t operator()(::string_view s) const {
      // Extracted from
      // https://stackoverflow.com/a/19411888
      return std::_Hash_impl::hash(s.data(), s.size());
    }
  };
}  // namespace std

// }}}

#endif  // STRING_VIEW_HPP
