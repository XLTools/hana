/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/sandbox/searchable_set.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/cnumber/constant.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::cnumber<int, i>;

BOOST_HANA_CONSTANT_ASSERT(union_(singleton(x<0>), singleton(x<0>)) == singleton(x<0>));
BOOST_HANA_CONSTANT_ASSERT(union_(singleton(x<0>), singleton(x<1>)) == doubleton(x<0>, x<1>));
BOOST_HANA_CONSTANT_ASSERT(union_(singleton(x<0>), doubleton(x<0>, x<1>)) == doubleton(x<0>, x<1>));

int main() { }
