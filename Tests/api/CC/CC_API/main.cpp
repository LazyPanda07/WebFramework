#include "gtest/gtest.h"

extern "C"
{
#include "cases.h"
}

TEST(API, ConfigConstructors)
{
	bool configuration;
	bool basePath;
	bool rawConfiguration;

	configConstructors(&configuration, &basePath, &rawConfiguration);

	ASSERT_FALSE(configuration);
	ASSERT_FALSE(basePath);
	ASSERT_FALSE(rawConfiguration);
}

TEST(API, ConfigOverrideString)
{
	bool assertTrue;

	configOverrideString(&assertTrue);

	ASSERT_TRUE(assertTrue);
}

TEST(API, ConfigOverrideInteger)
{
	bool assertTrue;

	configOverrideInteger(&assertTrue);

	ASSERT_TRUE(assertTrue);
}

TEST(API, ConfigOverrideBool)
{
	bool assertTrue;

	configOverrideBool(&assertTrue);

	ASSERT_TRUE(assertTrue);
}

TEST(API, ConfigOverrideStringArray)
{
	bool assertFalse;

	configOverrideStringArray(&assertFalse);

	ASSERT_FALSE(assertFalse);
}

TEST(API, ConfigOverrideIntegerArray)
{
	bool assertFalse;

	configOverrideIntegerArray(&assertFalse);

	ASSERT_FALSE(assertFalse);
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);

	initialize();

	return RUN_ALL_TESTS();
}
