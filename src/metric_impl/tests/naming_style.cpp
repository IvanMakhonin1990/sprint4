#include "metric_impl/naming_style.hpp"
#include <gtest/gtest.h>
#include "file.hpp"
#include "function.hpp"

namespace analyzer::metric::metric_impl {

class NamingStyleTest : public ::testing::TestWithParam<std::pair<std::string, std::string>> {};

TEST_P(NamingStyleTest, CalculatesCorrectly) {
    auto [filename, expected] = GetParam();
    file::File file(filename);
    function::FunctionExtractor extractor;
    auto functions = extractor.Get(file);
    
    ASSERT_FALSE(functions.empty());
    
    NamingStyleMetric metric;
    auto result = metric.Calculate(functions.front());
    EXPECT_EQ(std::get<std::string>(result.value), expected);
}

INSTANTIATE_TEST_SUITE_P(
    NamingStyle,
    NamingStyleTest,
    ::testing::Values(
        std::make_pair("comments.py", "Lower Case"),
        std::make_pair("exceptions.py", "Snake Case"),
        std::make_pair("if.py", "Snake Case"),
        std::make_pair("loops.py", "Snake Case"),
        std::make_pair("many_lines.py", "Snake Case"),
        std::make_pair("many_parameters.py", "Snake Case"),
        std::make_pair("match_case.py", "Unknown"),
        std::make_pair("nested_if.py", "Pascal Case"),
        std::make_pair("simple.py", "Snake Case"),
        std::make_pair("ternary.py", "Unknown")
    )
);

}  // namespace analyzer::metric::metric_impl
