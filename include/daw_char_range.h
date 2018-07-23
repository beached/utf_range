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

#pragma once

#include <iostream>
#include <string>
#include <utf8/unchecked.h>

#include <daw/cpp_17.h>
#include <daw/daw_algorithm.h>
#include <daw/daw_fnv1a_hash.h>
#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>

namespace daw {
	namespace range {
		using CharIterator = char const *;
		using UTFIterator = utf8::unchecked::iterator<CharIterator>;
		using UTFValType = UTFIterator::value_type;

		constexpr size_t hash_sequence( CharIterator first,
		                                CharIterator const last ) noexcept {
			return daw::fnv1a_hash( first, static_cast<size_t>(daw::distance( first, last )) );
		}

		struct CharRange {
			using iterator = UTFIterator;
			using const_iterator = UTFIterator const;
			using reference = UTFIterator::reference;
			using value_type = UTFIterator::value_type;
			using const_reference = value_type const &;
			using difference_type = UTFIterator::difference_type;

		private:
			iterator m_begin{};
			iterator m_end{};
			size_t m_size = 0;

		public:
			constexpr CharRange( ) noexcept = default;

			constexpr CharRange( iterator Begin, iterator End ) noexcept(
			  daw::is_nothrow_copy_constructible_v<iterator> )
			  : m_begin( std::move( Begin ) )
			  , m_end( std::move( End ) )
			  , m_size( static_cast<size_t>( daw::distance( Begin, End ) ) ) {}

			constexpr iterator
			begin( ) noexcept( daw::is_nothrow_copy_constructible_v<iterator> ) {
				return m_begin;
			}

			constexpr const_iterator begin( ) const
			  noexcept( daw::is_nothrow_copy_constructible_v<iterator> ) {
				return m_begin;
			}

			constexpr iterator
			end( ) noexcept( daw::is_nothrow_copy_constructible_v<iterator> ) {
				return m_end;
			}

			constexpr const_iterator end( ) const noexcept {
				return m_end;
			}

			constexpr size_t size( ) const
			  noexcept( daw::is_nothrow_copy_constructible_v<iterator> ) {
				return m_size;
			}

			constexpr bool empty( ) const
			  noexcept( daw::is_nothrow_copy_constructible_v<iterator> ) {
				return m_size == 0;
			}

			constexpr CharRange &operator++( ) noexcept {
				++m_begin;
				--m_size;
				return *this;
			}

			constexpr CharRange operator++( int ) noexcept {
				CharRange result( *this );
				++( *this );
				return result;
			}

			constexpr void advance( size_t const n ) noexcept {
				assert( n <= m_size );
				utf8::unchecked::advance( m_begin, n );
				m_size -= n;
			}

			constexpr void safe_advance( size_t const count ) noexcept {
				if( count <= m_size ) {
					advance( count );
				} else {
					advance( m_size );
				}
			}

			constexpr CharRange &set( iterator Begin, iterator End,
			                          difference_type Size = -1 ) noexcept {
				m_begin = Begin;
				m_end = End;
				if( Size < 0 ) {
					m_size = static_cast<size_t>( daw::distance( m_begin, m_end ) );
				} else {
					m_size = static_cast<size_t>( Size );
				}
				return *this;
			}

			constexpr CharRange &set_begin( iterator Begin,
			                                difference_type Size = -1 ) noexcept {
				return set( Begin, this->m_end, Size );
			}

			constexpr CharRange &set_end( iterator End,
			                              difference_type Size = -1 ) noexcept {
				return set( this->m_begin, End, Size );
			}

			constexpr CharRange &operator+=( size_t const n ) noexcept {
				advance( n );
				return *this;
			}

			constexpr CharIterator raw_begin( ) const noexcept {
				return m_begin.base( );
			}

			constexpr CharIterator raw_end( ) const noexcept {
				return m_end.base( );
			}

			constexpr size_t raw_size( ) const noexcept {
				return static_cast<size_t>(
				  daw::distance( m_begin.base( ), m_end.base( ) ) );
			}

			constexpr CharRange copy( ) const noexcept {
				return *this;
			}

			constexpr CharRange substr( size_t pos, size_t length ) const noexcept {
				assert( pos + length <= size( ) );
				auto result = copy( );
				auto f = result.begin( ) + pos;
				auto l = f + length;
				result.set( std::move( f ), std::move( l ) );
				return result;
			}

			std::u32string to_u32string( ) const noexcept;
			std::string to_raw_u8string( ) const noexcept;

			constexpr int compare( CharRange const &rhs ) const noexcept {
				auto it_lhs = begin( );
				auto it_rhs = rhs.begin( );
				enum compare_results_t {
					less_than = -1,
					equal_to = 0,
					greater_than = 1
				};

				while( it_lhs != end( ) && it_rhs != rhs.end( ) ) {
					auto const l = *it_lhs;
					auto const r = *it_rhs;

					if( l != r ) {
						if( l < r ) {
							return less_than;
						}
						return greater_than;
					}
					++it_lhs;
					++it_rhs;
				}
				if( it_lhs != end( ) ) {
					return greater_than;
				} else if( it_rhs != end( ) ) {
					return less_than;
				}
				return equal_to;
			}

			constexpr daw::string_view to_string_view( ) const noexcept {
				return daw::make_string_view_it( raw_begin( ), raw_end( ) );
			}
		}; // struct CharRange

		constexpr CharRange operator+( CharRange range, size_t const n ) noexcept {
			range.advance( n );
			return range;
		}

		constexpr CharRange create_char_range( UTFIterator first,
		                                       UTFIterator last ) noexcept {
			return {first, last};
		}

		constexpr CharRange create_char_range( daw::string_view str ) noexcept {
			auto it_begin = UTFIterator( str.begin( ) );
			auto it_end = UTFIterator( str.end( ) );
			return {std::move( it_begin ), std::move( it_end )};
		}

		template<size_t N>
		constexpr CharRange create_char_range( char const( &str )[N] ) noexcept {
			return { str, N-1 };
		}

		constexpr CharRange create_char_range( CharIterator first,
		                                       CharIterator last ) noexcept {
			auto it_begin = UTFIterator( first );
			auto it_end = UTFIterator( last );
			return {std::move( it_begin ), std::move( it_end )};
		}

		constexpr CharRange create_char_range( CharIterator first ) noexcept {
			auto sv = daw::string_view( first );
			return create_char_range( sv );
		}

		constexpr bool operator==( CharRange const &lhs,
		                           CharRange const &rhs ) noexcept {
			return lhs.compare( rhs ) == 0;
		}

		constexpr bool operator==( CharRange const &lhs,
		                           daw::string_view const &rhs ) noexcept {
			return lhs == create_char_range( rhs );
		}

		constexpr bool operator!=( CharRange const &lhs,
		                           CharRange const &rhs ) noexcept {
			return lhs.compare( rhs ) != 0;
		}

		constexpr bool operator<( CharRange const &lhs,
		                          CharRange const &rhs ) noexcept {
			return lhs.compare( rhs ) < 0;
		}

		constexpr bool operator>( CharRange const &lhs,
		                          CharRange const &rhs ) noexcept {
			return lhs.compare( rhs ) > 0;
		}

		constexpr bool operator<=( CharRange const &lhs,
		                           CharRange const &rhs ) noexcept {
			return lhs.compare( rhs ) <= 0;
		}

		constexpr bool operator>=( CharRange const &lhs,
		                           CharRange const &rhs ) noexcept {
			return lhs.compare( rhs ) >= 0;
		}

		constexpr void clear( CharRange &str ) noexcept {
			str.advance( str.size( ) );
		}

		std::string to_string( CharRange const &str );

		template<typename OStream,
		         std::enable_if_t<daw::traits::is_ostream_like_v<OStream, char>,
		                          std::nullptr_t> = nullptr>
		OStream &operator<<( OStream &os, CharRange const &value ) {
			for( auto it = value.begin( ).base( ); it != value.end( ).base( );
			     ++it ) {
				os << *it;
			}
			return os;
		}

		constexpr daw::string_view to_string_view( CharRange const &str ) noexcept {
			return str.to_string_view( );
		}

		constexpr bool at_end( CharRange const &range ) noexcept {
			return range.size( ) == 0;
		}

		std::u32string to_u32string( UTFIterator first, UTFIterator last );

	} // namespace range

	std::string from_u32string( std::u32string const &other );

	struct utf_string {
		using iterator = range::UTFIterator;
		using const_iterator = range::UTFIterator const;
		using reference = range::UTFIterator::reference;
		using value_type = range::UTFIterator::value_type;
		using const_reference = value_type const &;
		using difference_type = range::UTFIterator::difference_type;

	private:
		std::string m_values;
		daw::range::CharRange m_range;

	public:
		utf_string( );
		utf_string( daw::string_view other );
		utf_string( daw::range::CharRange other );
		utf_string( utf_string const &other );
		utf_string( char const *other );
		utf_string &operator=( utf_string const &rhs );
		utf_string &operator=( daw::string_view rhs );
		utf_string &operator=( char const *rhs );
		utf_string &operator=( std::string const &rhs );

		~utf_string( ) = default;

		utf_string( utf_string && ) = default;
		utf_string &operator=( utf_string && ) = default;

		void swap( utf_string &rhs ) noexcept;

		const_iterator begin( ) const;
		const_iterator end( ) const;
		size_t size( ) const;
		bool empty( ) const;
		range::CharIterator raw_begin( ) const;
		range::CharIterator raw_end( ) const;
		size_t raw_size( ) const;
		utf_string substr( size_t pos, size_t length ) const;

		std::string const &to_string( ) const;
		std::u32string to_u32string( ) const;
		range::CharRange const &char_range( ) const;
		int compare( utf_string const &rhs ) const;
	}; // utf_string

	void swap( utf_string &lhs, utf_string &rhs ) noexcept;

	bool operator==( utf_string const &lhs, utf_string const &rhs );
	bool operator!=( utf_string const &lhs, utf_string const &rhs );
	bool operator<( utf_string const &lhs, utf_string const &rhs );
	bool operator>( utf_string const &lhs, utf_string const &rhs );
	bool operator<=( utf_string const &lhs, utf_string const &rhs );
	bool operator>=( utf_string const &lhs, utf_string const &rhs );
	std::string to_string( utf_string const &str );
	daw::string_view to_string_view( utf_string const &str );

	template<typename OStream,
	         std::enable_if_t<daw::traits::is_ostream_like_v<OStream, char>,
	                          std::nullptr_t> = nullptr>
	OStream &operator<<( OStream &os, utf_string const &value ) {
		os << value.char_range( );
		return os;
	}

} // namespace daw

std::string to_string( daw::utf_string const &str );

namespace std {
	template<>
	struct hash<daw::range::CharRange> {
		constexpr size_t operator( )( daw::range::CharRange const &value ) const noexcept {
			return daw::range::hash_sequence( value.begin( ).base( ),
			                                  value.end( ).base( ) );
		}
	};

	template<>
	struct hash<daw::utf_string> {
		std::hash<std::string> m_hash;
		size_t operator( )( daw::utf_string const &value ) const noexcept {
			return m_hash( value.to_string( ) );
		}
	};
} // namespace std
