#include <boost/test/unit_test.hpp>
#include <iostream>

struct F {
    F():i(0) { std::cout << "setup" << std::endl; }
    ~F() { std::cout << "teardown" << std::endl; }

    int i;
};

BOOST_AUTO_TEST_SUITE( test )

BOOST_FIXTURE_TEST_CASE( test_case1, F ) {
    BOOST_CHECK( i == 0 );
    ++i;
}

BOOST_AUTO_TEST_CASE( test_case2 ) {
    BOOST_REQUIRE( 2 > 1 );
}

BOOST_AUTO_TEST_CASE( test_case3 ) {
    int i = 1;
    BOOST_CHECK_EQUAL( i, 1 );
    ++i;
}

BOOST_AUTO_TEST_SUITE_END()