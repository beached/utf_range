// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/header_libraries
//

#pragma once

#include "../utf8/unchecked.h"
#include "daw_utf_range.h"

#include <daw/cpp_17.h>
#include <daw/daw_algorithm.h>
#include <daw/daw_fnv1a_hash.h>
#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>

#include <daw/stdinc/move_fwd_exch.h>
#include <iostream>
#include <string>

namespace daw {
	namespace details {
		template<typename Container>
		inline std::string copy_to_string( Container const &c ) {
			auto result = std::string( );
			std::copy( std::cbegin( c ), std::cend( c ),
			           std::back_inserter( result ) );
			return result;
		}

		inline std::string copy_to_string( char const *const str ) {
			return std::string( str );
		}
	} // namespace details

	struct utf_string {
		using iterator = range::utf_iterator;
		using const_iterator = range::utf_iterator const;
		using reference = range::utf_iterator::reference;
		using value_type = range::utf_iterator::value_type;
		using const_reference = value_type const &;
		using difference_type = range::utf_iterator::difference_type;

	private:
		std::string m_values = { };
		daw::range::utf_range m_range = daw::range::create_char_range( m_values );

	public:
		utf_string( ) = default;

		template<size_t N>
		utf_string( char const ( &str )[N] )
		  : m_values( str, N - 1 )
		  , m_range( daw::range::create_char_range( m_values ) ) {}

		template<size_t N>
		utf_string &operator=( char const ( &str )[N] ) {
			m_values = str;
			m_range = daw::range::create_char_range( m_values );
			return *this;
		}

		inline utf_string( daw::string_view other )
		  : m_values( details::copy_to_string( other ) )
		  , m_range( daw::range::create_char_range( m_values ) ) {}

		inline utf_string( daw::range::utf_range other )
		  : m_values( details::copy_to_string( other ) )
		  , m_range( daw::range::create_char_range( m_values ) ) {}

		inline utf_string( char const *other )
		  : m_values( details::copy_to_string( other ) )
		  , m_range( daw::range::create_char_range( m_values ) ) {}

		[[nodiscard]] inline const_iterator begin( ) const noexcept {
			return m_range.begin( );
		}

		[[nodiscard]] inline const_iterator cbegin( ) const noexcept {
			return m_range.begin( );
		}

		[[nodiscard]] inline const_iterator end( ) const noexcept {
			return m_range.end( );
		}

		[[nodiscard]] inline const_iterator cend( ) const noexcept {
			return m_range.end( );
		}

		[[nodiscard]] inline size_t size( ) const noexcept {
			return m_range.size( );
		}

		[[nodiscard]] inline bool empty( ) const noexcept {
			return m_range.empty( );
		}

		[[nodiscard]] inline range::char_iterator raw_begin( ) const noexcept {
			return m_range.raw_begin( );
		}

		[[nodiscard]] inline range::char_iterator raw_end( ) const noexcept {
			return m_range.raw_end( );
		}

		inline utf_string &operator=( daw::string_view rhs ) {
			auto tmp = utf_string( rhs );
			using std::swap;
			swap( *this, tmp );
			return *this;
		}

		inline utf_string &operator=( char const *rhs ) {
			auto tmp = utf_string( rhs );
			using std::swap;
			swap( *this, tmp );
			return *this;
		}

		inline utf_string &operator=( std::string const &rhs ) {
			auto tmp = utf_string( rhs );
			using std::swap;
			swap( *this, tmp );
			return *this;
		}

		[[nodiscard]] inline size_t raw_size( ) const noexcept {
			return m_range.raw_size( );
		}

		[[nodiscard]] inline utf_string substr( size_t pos, size_t length ) const {
			return utf_string( m_range.substr( pos, length ) );
		}

		[[nodiscard]] inline std::string const &to_string( ) const &noexcept {
			return m_values;
		}

		[[nodiscard]] inline std::string to_string( ) &&noexcept {
			auto result = std::move( m_values );
			m_range = range::create_char_range( m_values );
			return std::move( m_values );
		}

		[[nodiscard]] inline std::u32string to_u32string( ) const {
			return m_range.to_u32string( );
		}

		[[nodiscard]] inline range::utf_range const &utf_range( ) const noexcept {
			return m_range;
		}

		[[nodiscard]] inline int compare( utf_string const &rhs ) const noexcept {
			return m_range.compare( rhs.m_range );
		}

		inline void sort( ) {
			auto result = to_u32string( );
			std::copy( cbegin( ), cend( ), std::back_inserter( result ) );
			std::sort( result.begin( ), result.end( ) );
			m_values.clear( );
			utf8::unchecked::utf32to8( result.cbegin( ), result.cend( ),
			                           std::back_inserter( m_values ) );
			m_range = daw::range::create_char_range( m_values );
		}

		[[nodiscard]] friend inline bool
		operator==( utf_string const &lhs, utf_string const &rhs ) noexcept {
			return lhs.compare( rhs ) == 0;
		}

		[[nodiscard]] friend inline bool
		operator!=( utf_string const &lhs, utf_string const &rhs ) noexcept {
			return lhs.compare( rhs ) != 0;
		}

		[[nodiscard]] friend inline bool
		operator<( utf_string const &lhs, utf_string const &rhs ) noexcept {
			return lhs.compare( rhs ) < 0;
		}

		[[nodiscard]] friend inline bool
		operator>( utf_string const &lhs, utf_string const &rhs ) noexcept {
			return lhs.compare( rhs ) > 0;
		}

		[[nodiscard]] friend inline bool
		operator<=( utf_string const &lhs, utf_string const &rhs ) noexcept {
			return lhs.compare( rhs ) <= 0;
		}

		[[nodiscard]] friend inline bool
		operator>=( utf_string const &lhs, utf_string const &rhs ) noexcept {
			return lhs.compare( rhs ) >= 0;
		}
	}; // utf_string

	inline std::string to_string( utf_string const &str ) {
		return str.to_string( );
	}

	inline daw::string_view to_string_view( utf_string const &str ) {
		return to_string_view( str.utf_range( ) );
	}

	template<typename OStream,
	         std::enable_if_t<daw::traits::is_ostream_like_v<OStream, char>,
	                          std::nullptr_t> = nullptr>
	OStream &operator<<( OStream &os, utf_string const &value ) {
		os << value.utf_range( );
		return os;
	}
} // namespace daw

inline std::string to_string( daw::utf_string const &str ) {
	return str.to_string( );
}

inline std::string to_string( daw::utf_string &&str ) {
	return std::move( str ).to_string( );
}

namespace std {
	template<>
	struct hash<daw::utf_string> {
		inline size_t operator( )( daw::utf_string const &value ) const noexcept {
			return std::hash<std::string>{ }( value.to_string( ) );
		}
	};
} // namespace std
