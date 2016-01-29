/*!
@file
Defines `boost::hana::hash`.

@copyright Jason Rice 2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_HASH_HPP
#define BOOST_HANA_HASH_HPP

#include <boost/hana/fwd/hash.hpp>

#include <boost/hana/concept/hashable.hpp>
#include <boost/hana/concept/integral_constant.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>


BOOST_HANA_NAMESPACE_BEGIN
    //! @cond
    template <typename X>
    constexpr auto hash_t::operator()(X&& x) const {
        using Tag = typename hana::tag_of<X>::type;
        using Hash = BOOST_HANA_DISPATCH_IF(hash_impl<Tag>,
            hana::Hashable<Tag>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(hana::Hashable<Tag>::value,
        "hana::hash(x) requires 'x' to be Hashable");
    #endif

        return Hash::apply(static_cast<X&&>(x));
    }
    //! @endcond

    template <typename Tag, bool condition>
    struct hash_impl<Tag, when<condition>> : default_ {
        template <typename X>
        static constexpr auto apply(X&& x) {
            return hana::decltype_(static_cast<X&&>(x));
        }
    };

    template <>
    struct hash_impl<hana::type_tag> {
        template <typename X>
        static constexpr auto apply(X const&) {
            return hana::type_c<typename X::type>;
        }
    };

    namespace detail {
        template <bool is_signed, typename X, typename = void>
        struct hash_integral_helper;

        template <typename X>
        struct hash_integral_helper<true, X,
            typename std::enable_if<
                !std::is_same<decltype(X::value), char const>::value
            >::type> {
            static constexpr auto apply() {
                return hana::type_c<hana::integral_constant<signed long, X::value>>;
            }
        };

        template <typename X>
        struct hash_integral_helper<true, X,
            typename std::enable_if<
                std::is_same<decltype(X::value), char const>::value
            >::type> {
            static constexpr auto apply() {
                return hana::type_c<X>;
            }
        };

        template <typename X>
        struct hash_integral_helper<false, X,
            typename std::enable_if<
                !std::is_same<decltype(X::value), unsigned char const>::value
                && !std::is_same<decltype(X::value), char const>::value
            >::type> {
            static constexpr auto apply() {
                //return hana::type_c<hana::integral_constant<unsigned long, X::value>>;
                return hana::type_c<hana::integral_constant<decltype(X::value), X::value>>;
            }
        };
        template <typename X>
        struct hash_integral_helper<false, X,
            typename std::enable_if<
                std::is_same<decltype(X::value), unsigned char const>::value
                || std::is_same<decltype(X::value), char const>::value
            >::type> {
            static constexpr auto apply() {
                return hana::type_c<X>;
            }
        };

    }

    template <typename Tag>
    struct hash_impl<Tag, when<hana::IntegralConstant<Tag>::value>> {
        template <typename X>
        static constexpr auto apply(X const&) {
            return detail::hash_integral_helper<
                std::is_signed<decltype(X::value)>::value, X>::apply();
        }

        // fixes use of pointers to members as IntegralConstant
        template <typename T, typename X, T X::* v>
        static constexpr auto apply(hana::integral_constant<T X::*, v> const p) {
            return hana::decltype_(p);
        }
    };

BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_HASH_HPP
