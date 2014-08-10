/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/fusion.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/integral.hpp>

#include "helper.hpp"
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto is_even = [](auto x) {
    return x % 2 == 0;
};

int main() {
    with_nonassociative_forward_sequences([=](auto container) {
        // all
        {
            BOOST_HANA_CONSTANT_ASSERT(all(container(), is_even));
            BOOST_HANA_RUNTIME_ASSERT(!all(container(1), is_even));
            BOOST_HANA_RUNTIME_ASSERT( all(container(2), is_even));
            BOOST_HANA_RUNTIME_ASSERT( all(container(2, 4), is_even));
            BOOST_HANA_RUNTIME_ASSERT(!all(container(1, 2), is_even));
            BOOST_HANA_RUNTIME_ASSERT(!all(container(1, 3), is_even));
            BOOST_HANA_RUNTIME_ASSERT(!all(container(1, 3, 4), is_even));
        }

        // any
        {
            BOOST_HANA_CONSTANT_ASSERT(!any(container(), is_even));
            BOOST_HANA_RUNTIME_ASSERT( !any(container(1), is_even));
            BOOST_HANA_RUNTIME_ASSERT(  any(container(2), is_even));
            BOOST_HANA_RUNTIME_ASSERT(  any(container(1, 2), is_even));
            BOOST_HANA_RUNTIME_ASSERT( !any(container(1, 3), is_even));
            BOOST_HANA_RUNTIME_ASSERT(  any(container(1, 3, 4), is_even));
        }

        // none
        {
            BOOST_HANA_CONSTANT_ASSERT(none(container(), is_even));
            BOOST_HANA_RUNTIME_ASSERT( none(container(1), is_even));
            BOOST_HANA_RUNTIME_ASSERT(!none(container(2), is_even));
            BOOST_HANA_RUNTIME_ASSERT(!none(container(1, 2), is_even));
            BOOST_HANA_RUNTIME_ASSERT( none(container(1, 3), is_even));
            BOOST_HANA_RUNTIME_ASSERT(!none(container(1, 3, 4), is_even));
        }

        // count
        {
            BOOST_HANA_RUNTIME_ASSERT(count(container(), is_even) == 0);
            BOOST_HANA_RUNTIME_ASSERT(count(container(1), is_even) == 0);
            BOOST_HANA_RUNTIME_ASSERT(count(container(2), is_even) == 1);
            BOOST_HANA_RUNTIME_ASSERT(count(container(1, 2), is_even) == 1);
            BOOST_HANA_RUNTIME_ASSERT(count(container(1, 2, 3), is_even) == 1);
            BOOST_HANA_RUNTIME_ASSERT(count(container(1, 2, 3, 4), is_even) == 2);
        }

        // foldl
        {
            BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});
            constexpr auto s = detail::number<>(0);
            BOOST_HANA_RUNTIME_ASSERT(equal(foldl(container(), s, f), s));
            BOOST_HANA_RUNTIME_ASSERT(equal(foldl(container(1), s, f), f(s, 1)));
            BOOST_HANA_RUNTIME_ASSERT(equal(foldl(container(1, '2'), s, f), f(f(s, 1), '2')));
            BOOST_HANA_RUNTIME_ASSERT(equal(foldl(container(1, '2', 3.3), s, f), f(f(f(s, 1), '2'), 3.3)));
            BOOST_HANA_RUNTIME_ASSERT(equal(foldl(container(1, '2', 3.3, 4.4f), s, f), f(f(f(f(s, 1), '2'), 3.3), 4.4f)));
        }

        // foldr
        {
            BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});
            constexpr auto s = detail::number<>(0);
            BOOST_HANA_RUNTIME_ASSERT(equal(foldr(container(), s, f), s));
            BOOST_HANA_RUNTIME_ASSERT(equal(foldr(container(1), s, f), f(1, s)));
            BOOST_HANA_RUNTIME_ASSERT(equal(foldr(container(1, '2'), s, f), f(1, f('2', s))));
            BOOST_HANA_RUNTIME_ASSERT(equal(foldr(container(1, '2', 3.3), s, f), f(1, f('2', f(3.3, s)))));
            BOOST_HANA_RUNTIME_ASSERT(equal(foldr(container(1, '2', 3.3, 4.4f), s, f), f(1, f('2', f(3.3, f(4.4f, s))))));
        }

        // length
        {
            BOOST_HANA_CONSTANT_ASSERT(length(container()) == size_t<0>);
            BOOST_HANA_CONSTANT_ASSERT(length(container(1)) == size_t<1>);
            BOOST_HANA_CONSTANT_ASSERT(length(container(1, '2')) == size_t<2>);
            BOOST_HANA_CONSTANT_ASSERT(length(container(1, '2', 3.3)) == size_t<3>);
        }
    });
}
