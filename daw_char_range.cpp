// The MIT License (MIT)
//
// Copyright (c) 2014-2016 Darrell Wright
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
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

namespace daw {
	namespace range {
		CharRange::CharRange( ):
			m_begin{ nullptr },
			m_end{ nullptr },
			m_size{ 0 } { }

		CharRange::CharRange( CharRange::iterator Begin, CharRange::iterator End ):
			m_begin( Begin ),
			m_end( End ),
			m_size( static_cast<size_t>(std::distance( Begin, End )) ) { }

		CharRange::~CharRange( ) { }

		CharRange::iterator CharRange::begin( ) {
			return m_begin;
		}

		CharRange::const_iterator CharRange::begin( ) const {
			return m_begin;
		}

		CharRange::iterator CharRange::end( ) {
			return m_end;
		}

		CharRange::const_iterator CharRange::end( ) const {
			return m_end;
		}

		size_t CharRange::size( ) const {
			return m_size;
		}
		
		bool CharRange::empty( ) const {
			return m_size == 0;
		}

		CharRange & CharRange::operator++( ) {
			++m_begin;
			--m_size;
			return *this;
		}

		CharRange CharRange::operator++( int ) {
			CharRange result( *this );
			++(*this);
			return result;
		}

		void CharRange::advance( size_t const n ) {
			assert( n <= m_size );
			utf8::unchecked::advance( m_begin, n );
			m_size -= n;
		}

		void CharRange::safe_advance( size_t const count ) {
			if( count <= m_size ) {
				advance( count );
			} else {
				advance( m_size );
			}
		}

		CharRange & CharRange::set( CharRange::iterator Begin, CharRange::iterator End, CharRange::difference_type Size ) {
			m_begin = Begin;
			m_end = End;
			if( Size < 0 ) {
				m_size = static_cast<size_t>(std::distance( m_begin, m_end ));
			} else {
				m_size = static_cast<size_t>(Size);
			}
			return *this;
		}

		CharRange & CharRange::set_begin( CharRange::iterator Begin, CharRange::difference_type Size ) {
			return set( Begin, this->m_end, Size );
		}

		CharRange & CharRange::set_end( CharRange::iterator End, CharRange::difference_type Size ) {
			return set( this->m_begin, End, Size );
		}

		CharRange operator+( CharRange range, size_t const n ) {
			range.advance( n );
			return range;
		}

		CharRange & CharRange::operator+=( size_t const n ) { 
			advance( n );
			return *this;
		}

		CharIterator CharRange::raw_begin( ) const {
			return m_begin.base( );
		}

		CharIterator CharRange::raw_end( ) const {
			return m_end.base( );
		}

		size_t CharRange::raw_size( ) const {
			return static_cast<size_t>(std::distance( m_begin.base( ), m_end.base( ) ));
		}

		bool at_end( CharRange const & range ) {
			return range.size( ) == 0;
		}

		
		CharRange CharRange::copy( ) const {
			return *this;
		}

		CharRange CharRange::substr( size_t pos, size_t length ) const {
			assert( pos + length <= size( ) );
			auto result = copy( );
			auto f = result.begin( ) + pos;
			auto l = f + length;
			return result.set( f, l );
		}


		std::u32string CharRange::to_u32string( ) const {
			std::u32string result;
			std::transform( begin( ), end( ), std::back_inserter( result ), []( auto c ) {
				return static_cast<char32_t>( c );
			});
			
			return result;
		}

		size_t hash_sequence( CharIterator first, CharIterator const last ) {
			// FNV-1a hash function for bytes in [fist, last], see http://www.isthe.com/chongo/tech/comp/fnv/index.html
#if defined(_WIN64) || defined(__amd64__)
			static_assert(sizeof( size_t ) == 8, "This code is for 64-bit size_t.");
			size_t const fnv_offset_basis = 14695981039346656037ULL;
			size_t const fnv_prime = 1099511628211ULL;

#elif defined(_WIN32) || defined(__i386__)
			static_assert(sizeof( size_t ) == 4, "This code is for 32-bit size_t.");
			size_t const fnv_offset_basis = 2166136261U;
			size_t const fnv_prime = 16777619U;
#else
#error "Unknown platform for hash"
#endif 

			auto result = fnv_offset_basis;
			for( ; first <= last; ++first ) {
				result ^= static_cast<size_t>(*first);
				result *= fnv_prime;
			}
			return result;
		}

		CharRange create_char_range( UTFIterator first, UTFIterator last ) {
			return { first, last };
		}

		CharRange create_char_range( boost::string_ref str ) {
			UTFIterator it_begin( str.begin( ) );
			UTFIterator it_end( str.end( ) );
			return { it_begin, it_end };
		}
		
		CharRange create_char_range( CharIterator first, CharIterator last ) {
			UTFIterator it_begin( first );
			UTFIterator it_end( last );
			return { it_begin, it_end };
		}

		CharRange create_char_range( CharIterator first ) { 
			return create_char_range( first, first + strlen( first ) );
		}

		bool operator==( CharRange const & first, CharRange const & second ) {
			return std::equal( first.begin( ), first.end( ), second.begin( ), second.end( ) );
		}

		bool operator==( CharRange const & first, boost::string_ref const & second ) {
			return std::equal( first.begin( ), first.end( ), second.begin( ), second.end( ), []( UTFValType const & lhs, char const & rhs ) {
				return static_cast<char>(lhs) == rhs;
			} );
		}

		void clear( CharRange & str ) {
			str.advance( str.size( ) );
		}

		std::string to_string( CharRange const & str ) {
			return std::string { str.begin( ).base( ), static_cast<size_t>(std::distance( str.begin( ).base( ), str.end( ).base( ) )) };
		}

		std::ostream& operator<<( std::ostream & os, CharRange const & value ) {
			for( auto it = value.begin( ).base( ); it != value.end( ).base( ); ++it ) {
				os << *it;
			}
			return os;
		}

		boost::string_ref to_string_ref( CharRange const & str ) {
			auto it_begin = str.begin( ).base( );
			auto it_end = str.end( ).base( );

			return { it_begin, static_cast<size_t>(std::distance( it_begin, it_end )) };
		}

		boost::string_ref to_string_ref( utf_string const & str ) {
			return to_string_ref( str.char_range( ) );
		}

		bool operator<( CharRange const & lhs, CharRange const & rhs ) {
			return std::lexicographical_compare( lhs.begin( ), lhs.end( ), rhs.begin( ), rhs.end( ) );
		}

		std::u32string to_u32string( UTFIterator first, UTFIterator last ) {
			std::u32string result;
			std::transform( first, last, std::back_inserter( result ), []( auto c ) {
				return static_cast<char32_t>( c );
			} );
			return result;
		}
	}	// namespace range

	namespace {
		template<typename Container>
		std::string copy_to_string( Container const & c ) {
			std::string result;
			std::copy( std::begin( c ), std::end( c ), std::back_inserter( result ) );
			return result;
		}
	}

	utf_string::utf_string( ):
			m_values{ },
			m_range{ daw::range::create_char_range( m_values ) } { }

	utf_string::utf_string( boost::string_ref other ):
			m_values{ copy_to_string( other ) },
			m_range{ daw::range::create_char_range( m_values ) } { }

	utf_string::utf_string( daw::range::CharRange other ):
			m_values{ copy_to_string( other ) },
			m_range{ daw::range::create_char_range( m_values ) } { }

	utf_string::utf_string( utf_string const & other ):
			m_values{ other.m_values },
			m_range{ daw::range::create_char_range( m_values ) } { }

	utf_string & utf_string::operator=( utf_string const & rhs ) {
		if( this != &rhs ) {
			m_values = rhs.m_values;
			m_range = daw::range::create_char_range( m_values );
		}
		return *this;
	}

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

	range::CharIterator utf_string::raw_begin( ) const {
		return m_range.raw_begin( );
	}

	range::CharIterator utf_string::raw_end( ) const {
		return m_range.raw_end( );
	}

	size_t utf_string::raw_size( ) const {
		return m_range.raw_size( );
	}

	utf_string utf_string::substr( size_t pos, size_t length ) const {
		return utf_string{ m_range.substr( pos, length ) };
	}

	std::string const & utf_string::to_string( ) const {
		return m_values;
	}

	std::u32string utf_string::to_u32string( ) const {
		return m_range.to_u32string( );
	}

	range::CharRange const & utf_string::char_range( ) const {
		return m_range;
	}

	bool operator<( utf_string const & lhs, utf_string const & rhs ) {
		return lhs.to_string( ) < rhs.to_string( );
	}

	bool operator==( utf_string const & lhs, utf_string const & rhs ) {
		return lhs.to_string( ) == rhs.to_string( );
	}

	std::string to_string( utf_string const & str ) {
		return str.to_string( );
	}

	boost::string_ref to_string_ref( utf_string const & str ) {
		return to_string_ref( str.char_range( ) );
	}

	std::ostream & operator<<( std::ostream & os, utf_string const & value ) {
		os << value.char_range( );
		return os;
	}


	std::string from_u32string( std::u32string const & other ) {
		std::string result;
		utf8::unchecked::utf32to8( other.begin( ), other.end( ), std::back_inserter( result ) );
		return result;
	}
}	// namespace daw
