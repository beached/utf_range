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

#pragma once

#include <boost/utility/string_view.hpp>
#include <iostream>
#include <string>
#include <utf8/unchecked.h>

namespace daw {
	namespace range {
			using CharIterator = char const *;
			using UTFIterator = utf8::unchecked::iterator<CharIterator>;
			using UTFValType = UTFIterator::value_type;

			size_t hash_sequence( CharIterator first, CharIterator const last );

			struct CharRange {
				using iterator = UTFIterator;
				using const_iterator = UTFIterator const;
				using reference = UTFIterator::reference;
				using value_type = UTFIterator::value_type;
				using const_reference = value_type const &;
				using difference_type = UTFIterator::difference_type;
			private:
				iterator m_begin;
				iterator m_end;
				size_t m_size;
			public:
			  CharRange( ) noexcept; 

			  ~CharRange( );
			  CharRange( CharRange const & ) = default;
			  CharRange &operator=( CharRange const & ) = default;
			  CharRange( CharRange && ) = default;
			  CharRange &operator=( CharRange && ) = default;
			  CharRange( iterator Begin, iterator End );

			  iterator begin( ) noexcept;
			  const_iterator begin( ) const noexcept;
			  iterator end( ) noexcept;
			  const_iterator end( ) const noexcept;
			  size_t size( ) const noexcept;
			  bool empty( ) const noexcept;

			  CharRange &operator++( );
			  CharRange operator++( int );
			  void advance( size_t const n );
			  void safe_advance( size_t const count );
			  CharRange &set( iterator Begin, iterator End, difference_type Size = -1 );
			  CharRange &set_begin( iterator Begin, difference_type Size = -1 );
			  CharRange &set_end( iterator End, difference_type Size = -1 );
			  CharRange &operator+=( size_t const n );
			  CharIterator raw_begin( ) const;
			  CharIterator raw_end( ) const;
			  size_t raw_size( ) const;
			  CharRange copy( ) const;
			  CharRange substr( size_t pos, size_t length ) const;
			  std::u32string to_u32string( ) const;
			  std::string to_raw_u8string( ) const;
			  int compare( CharRange const &rhs ) const;
			  boost::string_view to_string_view( ) const;
			};	// struct CharRange


			CharRange operator+( CharRange range, size_t const n );


			CharRange create_char_range( UTFIterator const first,  UTFIterator const last );
			CharRange create_char_range( boost::string_view str );
			CharRange create_char_range( CharIterator first, CharIterator last );
			CharRange create_char_range( CharIterator first );

			bool operator==( CharRange const & lhs, CharRange const & rhs );
			bool operator==( CharRange const & lhs, boost::string_view const & rhs );
			bool operator!=( CharRange const & lhs, CharRange const & rhs );
			bool operator<( CharRange const & lhs, CharRange const & rhs );
			bool operator>( CharRange const & lhs, CharRange const & rhs );
			bool operator<=( CharRange const & lhs, CharRange const & rhs );
			bool operator>=( CharRange const & lhs, CharRange const & rhs );

			void clear( CharRange & str );
			std::string to_string( CharRange const & str );

			std::ostream& operator<<( std::ostream & os, CharRange const & value );
			
			boost::string_view to_string_view( CharRange const & str );

			bool at_end( CharRange const & range );
			std::u32string to_u32string( UTFIterator first, UTFIterator last );

	}	// namespace range

	std::string from_u32string( std::u32string const & other );

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
		utf_string( boost::string_view other );
		utf_string( daw::range::CharRange other );
		utf_string( utf_string const & other );
		utf_string( char const * other );
		utf_string & operator=( utf_string const & rhs );
		utf_string & operator=( boost::string_view rhs );
		utf_string & operator=( char const * rhs );
		utf_string & operator=( std::string const & rhs );

		~utf_string( );
		utf_string( utf_string && ) = default;
		utf_string & operator=( utf_string && ) = default;

		void swap( utf_string & rhs ) noexcept;

		const_iterator begin( ) const;
		const_iterator end( ) const;
		size_t size( ) const;
		bool empty( ) const;
		range::CharIterator raw_begin( ) const;
		range::CharIterator raw_end( ) const;
		size_t raw_size( ) const;
		utf_string substr( size_t pos, size_t length ) const;

		std::string const & to_string( ) const;
		std::u32string to_u32string( ) const;
		range::CharRange const & char_range( ) const;
		int compare( utf_string const & rhs ) const;
	};	// utf_string

	void swap( utf_string & lhs, utf_string & rhs ) noexcept;

	bool operator==( utf_string const & lhs, utf_string const & rhs );
	bool operator!=( utf_string const & lhs, utf_string const & rhs );
	bool operator<( utf_string const & lhs, utf_string const & rhs );
	bool operator>( utf_string const & lhs, utf_string const & rhs );
	bool operator<=( utf_string const & lhs, utf_string const & rhs );
	bool operator>=( utf_string const & lhs, utf_string const & rhs );
	std::string to_string( utf_string const & str );
	boost::string_view to_string_view( utf_string const & str );
	std::ostream & operator<<( std::ostream & os, utf_string const & value );
}	// namespace daw

std::string to_string( daw::utf_string const & str );

namespace std {
	template<>
	struct hash<daw::range::CharRange> {
		size_t operator()( daw::range::CharRange const & value ) const {
			return daw::range::hash_sequence( value.begin( ).base( ), value.end( ).base( ) );
		}
	};

	template<>
	struct hash<daw::utf_string> {
		std::hash<std::string> m_hash;
		size_t operator()( daw::utf_string const & value ) const {
			return m_hash( value.to_string( ) );
		}
	};
}
