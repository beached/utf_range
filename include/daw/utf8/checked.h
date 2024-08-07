// Copyright 2006 Nemanja Trifunovic
// Modified by Darrell Wright 2018
/*
Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "core.h"

#include <daw/daw_exception.h>

#include <ciso646>
#include <stdexcept>

namespace daw::utf8 {
	// Exceptions that may be thrown from the library functions.
	class invalid_code_point {
		uint32_t cp;

	public:
		invalid_code_point( uint32_t Cp ) noexcept
		  : cp( Cp ) {}

		char const *what( ) const noexcept {
			return "Invalid code point";
		}

		uint32_t code_point( ) const noexcept {
			return cp;
		}
	};

	class invalid_utf8 {
		uint8_t u8;

	public:
		invalid_utf8( uint8_t u ) noexcept
		  : u8( u ) {}

		char const *what( ) const noexcept {
			return "Invalid UTF-8";
		}

		uint8_t utf8_octet( ) const noexcept {
			return u8;
		}
	};

	class invalid_utf16 {
		uint16_t u16;

	public:
		invalid_utf16( uint16_t u ) noexcept
		  : u16( u ) {}

		char const *what( ) const noexcept {
			return "Invalid UTF-16";
		}

		uint16_t utf16_word( ) const noexcept {
			return u16;
		}
	};

	struct not_enough_room {
		not_enough_room( ) noexcept = default;

		char const *what( ) const noexcept {
			return "Not enough space";
		}
	};

	/// The library API - functions intended to be called by the users

	template<typename octet_iterator>
	constexpr octet_iterator append( uint32_t cp, octet_iterator result ) {
		daw::exception::precondition_check<invalid_code_point>(
		  utf8::internal::is_code_point_valid( cp ), cp );

		if( cp < 0x80 ) // one octet
			*( result++ ) = static_cast<uint8_t>( cp );
		else if( cp < 0x800 ) { // two octets
			*( result++ ) = static_cast<uint8_t>( ( cp >> 6 ) | 0xc0 );
			*( result++ ) = static_cast<uint8_t>( ( cp & 0x3f ) | 0x80 );
		} else if( cp < 0x10000 ) { // three octets
			*( result++ ) = static_cast<uint8_t>( ( cp >> 12 ) | 0xe0 );
			*( result++ ) = static_cast<uint8_t>( ( ( cp >> 6 ) & 0x3f ) | 0x80 );
			*( result++ ) = static_cast<uint8_t>( ( cp & 0x3f ) | 0x80 );
		} else { // four octets
			*( result++ ) = static_cast<uint8_t>( ( cp >> 18 ) | 0xf0 );
			*( result++ ) = static_cast<uint8_t>( ( ( cp >> 12 ) & 0x3f ) | 0x80 );
			*( result++ ) = static_cast<uint8_t>( ( ( cp >> 6 ) & 0x3f ) | 0x80 );
			*( result++ ) = static_cast<uint8_t>( ( cp & 0x3f ) | 0x80 );
		}
		return result;
	}

	template<typename octet_iterator, typename output_iterator>
	constexpr output_iterator
	replace_invalid( octet_iterator start, octet_iterator end,
	                 output_iterator out, uint32_t replacement ) {
		while( start != end ) {
			auto sequence_start = start;
			auto err_code = utf8::internal::validate_next( start, end );

			switch( err_code ) {
			case internal::UTF8_OK:
				for( auto it = sequence_start; it != start; ++it ) {
					*out++ = *it;
				}
				break;
			case internal::NOT_ENOUGH_ROOM:
				daw::exception::daw_throw<not_enough_room>( );
			case internal::INVALID_LEAD:
				out = utf8::append( replacement, out );
				++start;
				break;
			case internal::INCOMPLETE_SEQUENCE:
			case internal::OVERLONG_SEQUENCE:
			case internal::INVALID_CODE_POINT:
				out = utf8::append( replacement, out );
				++start;
				// just one replacement mark for the sequence
				while( start != end && utf8::internal::is_trail( *start ) ) {
					++start;
				}
				break;
			}
		}
		return out;
	}

	template<typename octet_iterator, typename output_iterator>
	constexpr output_iterator replace_invalid( octet_iterator start,
	                                           octet_iterator end,
	                                           output_iterator out ) noexcept {
		uint32_t const replacement_marker = utf8::internal::mask16( 0xFFFD );
		return utf8::replace_invalid( start, end, out, replacement_marker );
	}

	template<typename octet_iterator>
	constexpr uint32_t next( octet_iterator &it, octet_iterator end ) {
		uint32_t cp = 0;
		auto const err_code = utf8::internal::validate_next( it, end, cp );

		switch( err_code ) {
		case internal::utf_error::UTF8_OK:
			break;
		case internal::utf_error::NOT_ENOUGH_ROOM:
			daw::exception::daw_throw<not_enough_room>( );
		case internal::utf_error::INVALID_LEAD:
		case internal::utf_error::INCOMPLETE_SEQUENCE:
		case internal::utf_error::OVERLONG_SEQUENCE:
			daw::exception::daw_throw<invalid_utf8>( static_cast<uint8_t>( *it ) );
		case internal::utf_error::INVALID_CODE_POINT:
			daw::exception::daw_throw<invalid_code_point>( cp );
		}
		return cp;
	}

	template<typename octet_iterator>
	constexpr uint32_t peek_next( octet_iterator it, octet_iterator end ) {
		return utf8::next( it, end );
	}

	template<typename octet_iterator>
	constexpr uint32_t prior( octet_iterator &it, octet_iterator start ) {
		// can't do much if it == start
		daw::exception::precondition_check<not_enough_room>( it != start );

		auto end = it;
		// Go back until we hit either a lead octet or start
		while( utf8::internal::is_trail( *( --it ) ) ) {
			// error - no lead byte in the sequence
			daw::exception::precondition_check<invalid_utf8>( it != start, *it );
		}
		return utf8::peek_next( it, end );
	}

	/// Deprecated in versions that include "prior"
	template<typename octet_iterator>
	constexpr uint32_t previous( octet_iterator &it, octet_iterator pass_start ) {
		auto end = it;

		while( utf8::internal::is_trail( *( --it ) ) ) {
			// error - no lead byte in the sequence
			daw::exception::precondition_check<invalid_utf8>( it != pass_start, *it );
		}
		octet_iterator temp = it;
		return utf8::next( temp, end );
	}

	template<typename octet_iterator, typename distance_type>
	constexpr void advance( octet_iterator &it, distance_type n,
	                        octet_iterator end ) {
		for( distance_type i = 0; i < n; ++i ) {
			utf8::next( it, end );
		}
	}

	template<typename octet_iterator>
	constexpr auto distance( octet_iterator first, octet_iterator last ) ->
	  typename std::iterator_traits<octet_iterator>::difference_type {

		typename std::iterator_traits<octet_iterator>::difference_type dist = 0;

		for( ; first < last; ++dist ) {
			utf8::next( first, last );
		}
		return dist;
	}

	template<typename u16bit_iterator, typename octet_iterator>
	constexpr octet_iterator utf16to8( u16bit_iterator start, u16bit_iterator end,
	                                   octet_iterator result ) {

		while( start != end ) {
			int32_t cp = utf8::internal::mask16( *start++ );
			// Take care of surrogate pairs first

			if( utf8::internal::is_lead_surrogate( cp ) ) {
				daw::exception::precondition_check<invalid_utf16>(
				  start != end, static_cast<uint16_t>( cp ) );
				uint32_t trail_surrogate = utf8::internal::mask16( *start++ );
				daw::exception::precondition_check<invalid_utf16>(
				  utf8::internal::is_trail_surrogate( trail_surrogate ),
				  static_cast<uint16_t>( trail_surrogate ) );

				cp = ( cp << 10 ) + static_cast<int32_t>( trail_surrogate +
				                                          internal::SURROGATE_OFFSET );
			} else if( utf8::internal::is_trail_surrogate( cp ) ) {
				// Lone trail surrogate
				daw::exception::daw_throw<invalid_utf16>( static_cast<uint16_t>( cp ) );
			}
			result = utf8::append( cp, result );
		}
		return result;
	}

	template<typename u16bit_iterator, typename octet_iterator>
	constexpr u16bit_iterator utf8to16( octet_iterator start, octet_iterator end,
	                                    u16bit_iterator result ) {
		while( start != end ) {
			auto cp = utf8::next( start, end );
			if( cp > 0xFFFF ) { // make a surrogate pair
				*result++ =
				  static_cast<uint16_t>( ( cp >> 10 ) + internal::LEAD_OFFSET );
				*result++ = static_cast<uint16_t>( ( cp & 0x3ff ) +
				                                   internal::TRAIL_SURROGATE_MIN );
			} else {
				*result++ = static_cast<uint16_t>( cp );
			}
		}
		return result;
	}

	template<typename octet_iterator, typename u32bit_iterator>
	constexpr octet_iterator utf32to8( u32bit_iterator start, u32bit_iterator end,
	                                   octet_iterator result ) {
		while( start != end ) {
			result = utf8::append( *( start++ ), result );
		}

		return result;
	}

	template<typename octet_iterator, typename u32bit_iterator>
	constexpr u32bit_iterator utf8to32( octet_iterator start, octet_iterator end,
	                                    u32bit_iterator result ) {
		while( start != end ) {
			( *result++ ) = utf8::next( start, end );
		}

		return result;
	}

	// The iterator class
	template<typename octet_iterator>
	class iterator {
		octet_iterator it;
		octet_iterator range_start;
		octet_iterator range_end;

	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = uint32_t;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type *;
		using reference = value_type &;

		constexpr iterator( ) noexcept(
		  std::is_nothrow_default_constructible_v<octet_iterator> ) = default;

		constexpr explicit iterator( octet_iterator const &octet_it,
		                             octet_iterator const &rangestart,
		                             octet_iterator const &rangeend )
		  : it( octet_it )
		  , range_start( rangestart )
		  , range_end( rangeend ) {}

		constexpr explicit iterator( octet_iterator const &octet_it,
		                             octet_iterator const &rangeend )
		  : it( octet_it )
		  , range_start( octet_it )
		  , range_end( rangeend ) {}

		// the default "big three" are OK
		constexpr octet_iterator base( ) const
		  noexcept( std::is_nothrow_copy_constructible_v<octet_iterator> ) {
			return it;
		}

		iterator begin( ) const {
			return *this;
		}

		iterator end( ) const {
			return iterator( range_end, range_start, range_end );
		}

		constexpr value_type operator*( ) const {
			octet_iterator temp = it;
			return utf8::next( temp, range_end );
		}

		constexpr bool operator==( const iterator &rhs ) const {
			daw::exception::precondition_check<std::logic_error>(
			  range_start == rhs.range_start and range_end == rhs.range_end,
			  "Comparing utf-8 iterators defined with different ranges" );
			return ( it == rhs.it );
		}

		constexpr bool operator!=( const iterator &rhs ) const {
			return not( operator==( rhs ) );
		}

		constexpr iterator &operator++( ) {
			utf8::next( it, range_end );
			return *this;
		}

		constexpr iterator operator++( int ) {
			iterator temp = *this;
			utf8::next( it, range_end );
			return temp;
		}

		constexpr iterator &operator--( ) {
			utf8::prior( it, range_start );
			return *this;
		}

		constexpr iterator operator--( int ) {
			iterator temp = *this;
			utf8::prior( it, range_start );
			return temp;
		}
	}; // class iterator
	template<typename octet_iterator>
	iterator( octet_iterator ) -> iterator<octet_iterator>;
} // namespace daw::utf8
