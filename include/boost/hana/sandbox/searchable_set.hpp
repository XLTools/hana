/*!
@file
Defines `boost::hana::SearchableSet`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SANDBOX_SEARCHABLE_SET_HPP
#define BOOST_HANA_SANDBOX_SEARCHABLE_SET_HPP

#include <boost/hana/applicative/mcd.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/monad/flatten_mcd.hpp>
#include <boost/hana/searchable/mcd.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! A `Monad` for searching infinite sets in finite time.
    //!
    //! Taken from http://math.andrej.com/2008/11/21/a-haskell-monad-for-infinite-search-in-finite-time/.
    struct SearchableSet;

    template <typename Find, typename = operators<Comparable, Monad>>
    struct _sset {
        Find find;
        using hana_datatype = SearchableSet;
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto searchable_set = [](auto pred) {
        return _sset<decltype(pred)>{pred};
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto singleton = [](auto x) {
        return searchable_set([=](auto p) { return x; });
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto doubleton = [](auto x, auto y) {
        return searchable_set([=](auto p) {
            return if_(p(x), x, y);
        });
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto union_ = [](auto xs, auto ys) {
        return flatten(doubleton(xs, ys));
    };

    template <>
    struct Comparable::instance<SearchableSet, SearchableSet>
        : Comparable::equal_mcd
    {
        template <typename Xs, typename Ys>
        static constexpr auto equal_impl(Xs xs, Ys ys)
        { return and_(subset(xs, ys), subset(ys, xs)); }
    };

    template <>
    struct Functor::instance<SearchableSet> : Functor::fmap_mcd {
        template <typename F, typename Set>
        static constexpr auto fmap_impl(F f, Set set) {
            return searchable_set([=](auto q) {
                return f(set.find([=](auto x) { return q(f(x)); }));
            });
        }
    };

    template <>
    struct Applicative::instance<SearchableSet> : Applicative::mcd {
        template <typename X>
        static constexpr auto lift_impl(X x)
        { return singleton(x); }

        template <typename F, typename Set>
        static constexpr auto ap_impl(F fset, Set set) {
            return flatten(fmap(
                [=](auto f) { return fmap(f, set); },
                fset
            ));
        }
    };

    template <>
    struct Monad::instance<SearchableSet> : Monad::flatten_mcd<SearchableSet> {
        template <typename Set>
        static constexpr auto flatten_impl(Set set) {
            return searchable_set([=](auto p) {
                return set.find([=](auto set) {
                    return any(set, p);
                }).find(p);
            });
        }
    };

    template <>
    struct Searchable::instance<SearchableSet> : Searchable::mcd {
        template <typename Set, typename Pred>
        static constexpr auto find_impl(Set set, Pred p) {
            auto x = set.find(p);
            return if_(p(x), just(x), nothing);
        }

        template <typename Set, typename Pred>
        static constexpr auto any_impl(Set set, Pred p) {
            return p(set.find(p));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SANDBOX_SEARCHABLE_SET_HPP
