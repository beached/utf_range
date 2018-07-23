// The MIT License (MIT)
//
// Copyright (c) 2014-2018 Darrell Wright
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

#include "daw_char_range.h"

#include <cassert>
#include <cstring>
#include <string>

#include <daw/daw_string_view.h>

namespace daw {
	namespace range {
		std::string to_string( CharRange const &str ) {
			return std::string{str.begin( ).base( ),
			                   static_cast<size_t>( std::distance(
			                     str.begin( ).base( ), str.end( ).base( ) ) )};
		}


		daw::string_view to_string_view( utf_string const &str ) {
			return to_string_view( str.char_range( ) );
		}

		std::u32string to_u32string( UTFIterator first, UTFIterator last ) {
			std::u32string result;
			std::transform( first, last, std::back_inserter( result ),
			                []( auto c ) { return static_cast<char32_t>( c ); } );
			return result;
		}

		std::string CharRange::to_raw_u8string( ) const noexcept {
			return std::string( m_begin.base( ), m_end.base( ) );
		}

		std::u32string CharRange::to_u32string( ) const noexcept {
			auto result = std::u32string( );
			daw::algorithm::transform(
			  begin( ), end( ), std::back_inserter( result ),
			  []( auto c ) { return static_cast<char32_t>( c ); } );

			return result;
		}
	} // namespace range

	namespace {
		template<typename Container>
		std::string copy_to_string( Container const &c ) {
			std::string result;
			std::copy( std::begin( c ), std::end( c ), std::back_inserter( result ) );
			return result;
		}

		std::string copy_to_string( char const *const str ) {
			return std::string{str};
		}

	} // namespace

	utf_string::utf_string( )
	  : m_values{}
	  , m_range{daw::range::create_char_range( m_values )} {}

	utf_string::utf_string( daw::string_view other )
	  : m_values{copy_to_string( other )}
	  , m_range{daw::range::create_char_range( m_values )} {}

	utf_string::utf_string( daw::range::CharRange other )
	  : m_values{copy_to_string( other )}
	  , m_range{daw::range::create_char_range( m_values )} {}

	utf_string::utf_string( utf_string const &other )
	  : m_values{other.m_values}
	  , m_range{daw::range::create_char_range( m_values )} {}

	utf_string &utf_string::operator=( utf_string const &rhs ) {
		if( this != &rhs ) {
			m_values = rhs.m_values;
			m_range = daw::range::create_char_range( m_values );
		}
		return *this;
	}

	utf_string::utf_string( char const *other )
	  : m_values{copy_to_string( other )}
	  , m_range{daw::range::create_char_range( m_values )} {}

	utf_string::const_iterator utf_string::begin( ) const {
		return m_range.begin( );
	}

	utf_string::const_iterator utf_string::end( ) const {
		return m_range.end( );
	}

	size_t utf_string::size( ) const {
		return m_range.size( );
	}

	bool utf_string::empty( ) const {
		return m_range.empty( );
	}

	void utf_string::swap( utf_string &rhs ) noexcept {
		using std::swap;
		swap( m_values, rhs.m_values );
		swap( m_range, rhs.m_range );
	}

	void swap( utf_string &lhs, utf_string &rhs ) noexcept {
		lhs.swap( rhs );
	}

	range::CharIterator utf_string::raw_begin( ) const {
		return m_range.raw_begin( );
	}

	range::CharIterator utf_string::raw_end( ) const {
		return m_range.raw_end( );
	}

	utf_string &utf_string::operator=( daw::string_view rhs ) {
		using std::swap;
		utf_string tmp{rhs};
		swap( *this, tmp );
		return *this;
	}

	utf_string &utf_string::operator=( char const *rhs ) {
		using std::swap;
		utf_string tmp{rhs};
		swap( *this, tmp );
		return *this;
	}

	utf_string &utf_string::operator=( std::string const &rhs ) {
		using std::swap;
		utf_string tmp{rhs};
		swap( *this, tmp );
		return *this;
	}

	size_t utf_string::raw_size( ) const {
		return m_range.raw_size( );
	}

	utf_string utf_string::substr( size_t pos, size_t length ) const {
		return utf_string{m_range.substr( pos, length )};
	}

	std::string const &utf_string::to_string( ) const {
		return m_values;
	}

	std::u32string utf_string::to_u32string( ) const {
		return m_range.to_u32string( );
	}

	range::CharRange const &utf_string::char_range( ) const {
		return m_range;
	}

	int utf_string::compare( utf_string const &rhs ) const {
		return m_range.compare( rhs.m_range );
	}

	bool operator==( utf_string const &lhs, utf_string const &rhs ) {
		return lhs.compare( rhs ) == 0;
	}

	bool operator!=( utf_string const &lhs, utf_string const &rhs ) {
		return lhs.compare( rhs ) != 0;
	}

	bool operator<( utf_string const &lhs, utf_string const &rhs ) {
		return lhs.compare( rhs ) < 0;
	}

	bool operator>( utf_string const &lhs, utf_string const &rhs ) {
		return lhs.compare( rhs ) > 0;
	}

	bool operator<=( utf_string const &lhs, utf_string const &rhs ) {
		return lhs.compare( rhs ) <= 0;
	}

	bool operator>=( utf_string const &lhs, utf_string const &rhs ) {
		return lhs.compare( rhs ) >= 0;
	}

	std::string to_string( utf_string const &str ) {
		return str.to_string( );
	}

	daw::string_view to_string_view( utf_string const &str ) {
		return to_string_view( str.char_range( ) );
	}

	std::string from_u32string( std::u32string const &other ) {
		std::string result;
		utf8::unchecked::utf32to8( other.begin( ), other.end( ),
		                           std::back_inserter( result ) );
		return result;
	}
} // namespace daw

std::string to_string( daw::utf_string const &str ) {
	return str.to_string( );
}
