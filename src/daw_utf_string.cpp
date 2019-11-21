// The MIT License (MIT)
//
// Copyright (c) 2014-2019 Darrell Wright
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and / or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <cassert>
#include <cstring>
#include <string>

#include <daw/daw_string_view.h>
#include <daw/utf_range/daw_utf_string.h>

#include "daw/utf_range/daw_utf_string.h"

namespace daw {
	namespace {
		template<typename Container>
		std::string copy_to_string( Container const &c ) {
			auto result = std::string( );
			std::copy( std::cbegin( c ), std::cend( c ),
			           std::back_inserter( result ) );
			return result;
		}

		std::string copy_to_string( char const *const str ) {
			return std::string( str );
		}

	} // namespace

	utf_string::utf_string( )
	  : m_values( )
	  , m_range( daw::range::create_char_range( m_values ) ) {}

	utf_string::utf_string( daw::string_view other )
	  : m_values( copy_to_string( other ) )
	  , m_range( daw::range::create_char_range( m_values ) ) {}

	utf_string::utf_string( daw::range::utf_range other )
	  : m_values( copy_to_string( other ) )
	  , m_range( daw::range::create_char_range( m_values ) ) {}

	utf_string::utf_string( utf_string const &other )
	  : m_values( other.m_values )
	  , m_range( daw::range::create_char_range( m_values ) ) {}

	utf_string &utf_string::operator=( utf_string const &rhs ) {
		if( this != &rhs ) {
			m_values = rhs.m_values;
			m_range = daw::range::create_char_range( m_values );
		}
		return *this;
	}

	utf_string::utf_string( char const *other )
	  : m_values( copy_to_string( other ) )
	  , m_range( daw::range::create_char_range( m_values ) ) {}

	utf_string::const_iterator utf_string::begin( ) const noexcept {
		return m_range.begin( );
	}

	utf_string::const_iterator utf_string::cbegin( ) const noexcept {
		return m_range.begin( );
	}

	utf_string::const_iterator utf_string::end( ) const noexcept {
		return m_range.end( );
	}

	utf_string::const_iterator utf_string::cend( ) const noexcept {
		return m_range.end( );
	}

	size_t utf_string::size( ) const noexcept {
		return m_range.size( );
	}

	bool utf_string::empty( ) const noexcept {
		return m_range.empty( );
	}

	range::char_iterator utf_string::raw_begin( ) const noexcept {
		return m_range.raw_begin( );
	}

	range::char_iterator utf_string::raw_end( ) const noexcept {
		return m_range.raw_end( );
	}

	utf_string &utf_string::operator=( daw::string_view rhs ) {
		auto tmp = utf_string( rhs );
		using std::swap;
		swap( *this, tmp );
		return *this;
	}

	utf_string &utf_string::operator=( char const *rhs ) {
		auto tmp = utf_string( rhs );
		using std::swap;
		swap( *this, tmp );
		return *this;
	}

	utf_string &utf_string::operator=( std::string const &rhs ) {
		auto tmp = utf_string( rhs );
		using std::swap;
		swap( *this, tmp );
		return *this;
	}

	size_t utf_string::raw_size( ) const noexcept {
		return m_range.raw_size( );
	}

	utf_string utf_string::substr( size_t pos, size_t length ) const {
		return utf_string( m_range.substr( pos, length ) );
	}

	std::string const &utf_string::to_string( ) const noexcept {
		return m_values;
	}

	std::u32string utf_string::to_u32string( ) const {
		return m_range.to_u32string( );
	}

	range::utf_range const &utf_string::utf_range( ) const noexcept {
		return m_range;
	}

	int utf_string::compare( utf_string const &rhs ) const noexcept {
		return m_range.compare( rhs.m_range );
	}

	void utf_string::sort( ) {
		auto result = to_u32string( );
		std::copy( cbegin( ), cend( ), std::back_inserter( result ) );
		std::sort( result.begin( ), result.end( ) );
		m_values.clear( );
		utf8::unchecked::utf32to8( result.cbegin( ), result.cend( ),
		                           std::back_inserter( m_values ) );
		m_range = daw::range::create_char_range( m_values );
	}

	bool operator==( utf_string const &lhs, utf_string const &rhs ) noexcept {
		return lhs.compare( rhs ) == 0;
	}

	bool operator!=( utf_string const &lhs, utf_string const &rhs ) noexcept {
		return lhs.compare( rhs ) != 0;
	}

	bool operator<( utf_string const &lhs, utf_string const &rhs ) noexcept {
		return lhs.compare( rhs ) < 0;
	}

	bool operator>( utf_string const &lhs, utf_string const &rhs ) noexcept {
		return lhs.compare( rhs ) > 0;
	}

	bool operator<=( utf_string const &lhs, utf_string const &rhs ) noexcept {
		return lhs.compare( rhs ) <= 0;
	}

	bool operator>=( utf_string const &lhs, utf_string const &rhs ) noexcept {
		return lhs.compare( rhs ) >= 0;
	}

	std::string to_string( utf_string const &str ) {
		return str.to_string( );
	}

	daw::string_view to_string_view( utf_string const &str ) {
		return to_string_view( str.utf_range( ) );
	}
} // namespace daw

std::string to_string( daw::utf_string const &str ) {
	return str.to_string( );
}

std::string to_string( daw::utf_string &&str ) {
	return std::move( str ).to_string( );
}
