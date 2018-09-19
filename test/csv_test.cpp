#include <src/csv.h>
#include <gtest/gtest.h>

TEST(csv, testloadcsv)
{
    csv::Parser p;

    EXPECT_TRUE(p.loadcsv("test1.csv"));
}

TEST(csv, testParseString_1)
{
    csv::Parser p;

    EXPECT_TRUE(
        p.parseString("1, 2, 3\r\n"
                      "4, 5., 6.1\n")
                );
    EXPECT_EQ(p.rownum(), 2);
    EXPECT_EQ(p.colnum(), 3);

    EXPECT_EQ(p.getInt(0, 0), 1);
    EXPECT_EQ(p.getInt(0, 1), 2);
    EXPECT_EQ(p.getInt(0, 2), 3);
    EXPECT_EQ(p.getInt(1, 0), 4);
    EXPECT_EQ(p.getInt(1, 1), 5);
    EXPECT_EQ(p.getInt(1, 2), 6);
    EXPECT_EQ(p.getChar(1, 0), '4');
}

TEST(csv, testParseString_2)
{
    csv::Parser p;

    EXPECT_TRUE(
        p.parseString("2147483647, 2147483648\r\n"
                      "9223372036854775807, 9223372036854775808\n")
                );
    EXPECT_EQ(p.rownum(), 2);
    EXPECT_EQ(p.colnum(), 2);

    EXPECT_EQ(p.getInt(0, 0), 2147483647);
    EXPECT_THROW(p.getInt(0, 1), std::out_of_range);
    EXPECT_STREQ(p.get(0, 1).c_str(), "2147483648");

    EXPECT_THROW(p.getInt(1, 0), std::out_of_range);
    EXPECT_EQ(p.getInt64(1, 0), 9223372036854775807LL);
    EXPECT_THROW(p.getInt64(1, 1), std::out_of_range);
    EXPECT_EQ(p.getUInt64(1, 1), 9223372036854775808ULL);

    EXPECT_EQ(p.getUInt(0, 1), 2147483648);
}

TEST(csv, testParseString_3)
{
    csv::Parser p;

    EXPECT_TRUE(
        p.parseString(",\r\n"
                      ",\n")
                );
    EXPECT_EQ(p.rownum(), 2);
    EXPECT_EQ(p.colnum(), 2);

    EXPECT_TRUE(
        p.parseString(",,\r\n"
                      ",,\n")
                );
    EXPECT_EQ(p.rownum(), 2);
    EXPECT_EQ(p.colnum(), 3);
}

TEST(csv, testExceptGet)
{
    csv::Parser p;

    EXPECT_THROW(p.getInt(100, 100), std::exception);
    EXPECT_THROW(p.getLong(100, 100), std::exception);
    EXPECT_THROW(p.getInt64(100, 100), std::exception);
    EXPECT_THROW(p.getUInt(100, 100), std::exception);
    EXPECT_THROW(p.getULong(100, 100), std::exception);
    EXPECT_THROW(p.getUInt64(100, 100), std::exception);
    EXPECT_THROW(p.getFloat(100, 100), std::exception);
    EXPECT_THROW(p.getDouble(100, 100), std::exception);
    EXPECT_THROW(p.getChar(100, 100), std::exception);
}
