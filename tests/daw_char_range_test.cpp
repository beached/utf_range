// The MIT License (MIT)
//
// Copyright (c) 2016 Darrell Wright
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

#define BOOST_TEST_MODULE char_range_test 

#include <boost/test/unit_test.hpp>
#include <iostream>

#include "daw_char_range.h"

BOOST_AUTO_TEST_CASE( char_range_test_001 ) {
	std::string a = u8"Приве́т नमस्ते שָׁלוֹם";
	auto rng = daw::range::create_char_range( a );
	for( auto c: rng ) {
		std::cout << c << std::endl;
	}
}

BOOST_AUTO_TEST_CASE( utf_string_test_001 ) {
	daw::utf_string b = u8"Приве́т नमस्ते שָׁלוֹם";
	for( auto c: b ) {
		std::cout << c << std::endl;
	}
}

BOOST_AUTO_TEST_CASE( utf_comparison_test_001 ) {
	daw::utf_string const a = u8"a";
	daw::utf_string const aa = u8"aa";
	daw::utf_string const bb = u8"bb";

	BOOST_REQUIRE( a == a );
	BOOST_REQUIRE( a != bb );
	BOOST_REQUIRE( a < aa );
	BOOST_REQUIRE( a < bb );
	BOOST_REQUIRE( aa < bb );
	BOOST_REQUIRE( bb > aa );
}

