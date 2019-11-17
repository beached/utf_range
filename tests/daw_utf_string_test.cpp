// The MIT License (MIT)
//
// Copyright (c) 2016-2018 Darrell Wright
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

#include <iostream>

#include <daw/daw_benchmark.h>

#include "daw/utf_range/daw_utf_string.h"

void  utf_string_test_001 ( ) {
	daw::utf_string b = u8"Приве́т नमस्ते שָׁלוֹם";
	for( auto c : b ) {
		std::cout << c << '\n';
	}
}

void  utf_comparison_test_001 ( ) {
	daw::utf_string const a = u8"a";
	daw::utf_string const aa = u8"aa";
	daw::utf_string const bb = u8"bb";

	daw::expecting( a == a );
	daw::expecting( a != bb );
	daw::expecting( a < aa );
	daw::expecting( a < bb );
	daw::expecting( aa < bb );
	daw::expecting( bb > aa );
}

void  utf_string_sort_001 ( ) {
	daw::utf_string tst_01 = u8"Приве́т नमस्ते שָׁלוֹם";
	auto const tst_02 = tst_01;
	tst_01.sort( );
	daw::expecting( !tst_01.empty( ) );
	daw::expecting( tst_01.size( ) == tst_02.size( ) );
	daw::expecting( tst_01 != tst_02 );
	std::cout << "orig:   " << tst_02 << "\nlexigraphically\nsorted: " << tst_01 << '\n';
}

void  utf_string_sort_002 ( ) {
	daw::utf_string tst_01 = u8"zyxwvutsrqponmlkjihgfedcba";
	auto const tst_02 = tst_01;
	tst_01.sort( );
	daw::expecting( !tst_01.empty( ) );
	daw::expecting( tst_01.size( ) == tst_02.size( ) );
	daw::expecting( tst_01 != tst_02 );
	std::cout << "orig:   " << tst_02 << "\nlexigraphically\nsorted: " << tst_01 << '\n';
}

int main( ) {
	utf_string_test_001( );
	utf_comparison_test_001( );
	utf_string_sort_001( );
	utf_string_sort_002( );
}

