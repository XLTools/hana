/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/sandbox/searchable_set.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/number/comparable.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::number<>(i);

int main() {
    BOOST_HANA_CONSTEXPR_ASSERT( subset(singleton(x<0>), singleton(x<0>)));
    BOOST_HANA_CONSTEXPR_ASSERT(!subset(singleton(x<1>), singleton(x<0>)));

    BOOST_HANA_CONSTEXPR_ASSERT( subset(singleton(x<0>), doubleton(x<0>, x<1>)));
    BOOST_HANA_CONSTEXPR_ASSERT( subset(singleton(x<1>), doubleton(x<0>, x<1>)));
    BOOST_HANA_CONSTEXPR_ASSERT(!subset(singleton(x<2>), doubleton(x<0>, x<1>)));

    BOOST_HANA_CONSTEXPR_ASSERT( subset(doubleton(x<0>, x<1>), doubleton(x<0>, x<1>)));
    BOOST_HANA_CONSTEXPR_ASSERT(!subset(doubleton(x<0>, x<2>), doubleton(x<0>, x<1>)));
    BOOST_HANA_CONSTEXPR_ASSERT(!subset(doubleton(x<2>, x<3>), doubleton(x<0>, x<1>)));
}
