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

#include <string>

#include "daw/utf_range/daw_utf_range.h"

namespace daw {
	namespace range {
		std::string to_string( utf_range const &str ) {
			return std::string{str.begin( ).base( ),
			                   static_cast<size_t>( std::distance(
			                     str.begin( ).base( ), str.end( ).base( ) ) )};
		}

		std::u32string to_u32string( utf_iterator first, utf_iterator last ) {
			std::u32string result;
			std::transform( first, last, std::back_inserter( result ),
			                []( auto c ) { return static_cast<char32_t>( c ); } );
			return result;
		}

		std::string utf_range::to_raw_u8string( ) const noexcept {
			return std::string( m_begin.base( ), m_end.base( ) );
		}

		std::u32string utf_range::to_u32string( ) const noexcept {
			auto result = std::u32string( );
			daw::algorithm::transform(
			  begin( ), end( ), std::back_inserter( result ),
			  []( auto c ) { return static_cast<char32_t>( c ); } );

			return result;
		}
	} // namespace range

	std::string from_u32string( std::u32string const &other ) {
		std::string result;
		utf8::unchecked::utf32to8( other.begin( ), other.end( ),
		                           std::back_inserter( result ) );
		return result;
	}
} // namespace daw
