
// unit test include
#include "catch_amalgamated.hpp"

#include "static_resource_assert_api.h"

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

TEST_CASE( "StaticResourceAssert Test" )
{
	SECTION( "STATIC_RESOURCE_FIND" )
	{
		STATIC_RESOURCE_FIND("subRes");

		STATIC_REQUIRE(STATIC_RESOURCE_FIND("subRes"));
		STATIC_REQUIRE(STATIC_RESOURCE_FIND("subRes - Copy"));
		STATIC_REQUIRE(STATIC_RESOURCE_FIND("richText1 - Copy.rtf"));
		STATIC_REQUIRE(STATIC_RESOURCE_FIND("richText1.rtf"));
		STATIC_REQUIRE(STATIC_RESOURCE_FIND("someText - Copy.txt"));
		STATIC_REQUIRE(STATIC_RESOURCE_FIND("someText.txt"));
		STATIC_REQUIRE(STATIC_RESOURCE_FIND("texture1 - Copy.bmp"));
		STATIC_REQUIRE(STATIC_RESOURCE_FIND("texture1.bmp"));

		STATIC_REQUIRE(STATIC_RESOURCE_FIND("subRes/Test"));
		STATIC_REQUIRE(STATIC_RESOURCE_FIND("subRes/someText2234.txt"));
		STATIC_REQUIRE(STATIC_RESOURCE_FIND("subRes/texture134234.bmp"));
		STATIC_REQUIRE(STATIC_RESOURCE_FIND("subRes/Test/Test - Copy"));
		STATIC_REQUIRE(STATIC_RESOURCE_FIND("subRes/Test/someText2234 - Copy.txt"));
		STATIC_REQUIRE(STATIC_RESOURCE_FIND("subRes/Test/texture134234 - Copy.bmp"));
		STATIC_REQUIRE(STATIC_RESOURCE_FIND("subRes/Test/Test - Copy/New Bitmap Image.bmp"));

		STATIC_REQUIRE_FALSE(STATIC_RESOURCE_FIND("richText1234234234234.rtf"));
		STATIC_REQUIRE_FALSE(STATIC_RESOURCE_FIND("subRes/Test/texture134234 - Copy222.bmp"));
	};

	SECTION("STATIC_RESOURCE_FIND_utf8")
	{
		constexpr bool val = STATIC_RESOURCE_FIND("こんにちは世界.txt");
		STATIC_REQUIRE(val);
		STATIC_REQUIRE(STATIC_RESOURCE_FIND("こんにちは世界.txt"));
		STATIC_REQUIRE(STATIC_RESOURCE_FIND("πfs.c"));
	};

	SECTION( "STATIC_RESOURCE_ASSERT" )
	{
		STATIC_RESOURCE_ASSERT("subRes");

		STATIC_RESOURCE_ASSERT("subRes");
		STATIC_RESOURCE_ASSERT("subRes - Copy");
		STATIC_RESOURCE_ASSERT("richText1 - Copy.rtf");
		STATIC_RESOURCE_ASSERT("richText1.rtf");
		STATIC_RESOURCE_ASSERT("someText - Copy.txt");
		STATIC_RESOURCE_ASSERT("someText.txt");
		STATIC_RESOURCE_ASSERT("texture1 - Copy.bmp");
		STATIC_RESOURCE_ASSERT("texture1.bmp");

		STATIC_RESOURCE_ASSERT("subRes/Test");
		STATIC_RESOURCE_ASSERT("subRes/someText2234.txt");
		STATIC_RESOURCE_ASSERT("subRes/texture134234.bmp");
		STATIC_RESOURCE_ASSERT("subRes/Test/Test - Copy");
		STATIC_RESOURCE_ASSERT("subRes/Test/someText2234 - Copy.txt");
		STATIC_RESOURCE_ASSERT("subRes/Test/texture134234 - Copy.bmp");
		STATIC_RESOURCE_ASSERT("subRes/Test/Test - Copy/New Bitmap Image.bmp");
	};

	SECTION("STATIC_RESOURCE_ASSERT_utf8")
	{
		STATIC_RESOURCE_ASSERT("こんにちは世界.txt");
		STATIC_RESOURCE_ASSERT("πfs.c");
	};
}

