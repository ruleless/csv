#include <src/csv.h>
#include <gtest/gtest.h>

TEST(csv, testloadcsv)
{
    csv::Parser p;

    EXPECT_TRUE(p.loadcsv("test1.csv"));
}

TEST(csv, testParseString)
{
    csv::Parser p;

    EXPECT_TRUE(
        p.parseString("col11, col12, col13\n"
                      "col21, col22, col23\n")
                );
    EXPECT_EQ(p.rownum(), 2);
    EXPECT_EQ(p.colnum(), 3);
}
