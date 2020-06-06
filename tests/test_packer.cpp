#include <boost/test/unit_test.hpp>
#include <iostream>
#include "cpp_texture_packer.h"


BOOST_AUTO_TEST_SUITE( test_cpp_texture_packer )

BOOST_AUTO_TEST_CASE( test_cpp_texture_packer_case ) 
{
    auto packer = CppTexturePacker::TexturePacker(
        128,
        128,
        true,
        true,
        2,
        2,
        2,
        true,
        1,
        2
    );
    packer.pack(TEST_DATA_PATH, "test_output", "./");
}

BOOST_AUTO_TEST_SUITE_END()