#include "metric_impl/parameters_count.hpp"
#include <gtest/gtest.h>
#include "file.hpp"
#include "function.hpp"

namespace analyzer::metric::metric_impl {

class ParametersCountTest : public ::testing::TestWithParam<std::pair<std::string, int>> {};

TEST_P(ParametersCountTest, CalculatesCorrectly) {
    auto [filename, expected] = GetParam();
    file::File file(filename);
    function::FunctionExtractor extractor;
    auto functions = extractor.Get(file);
    
    ASSERT_FALSE(functions.empty());
    
    CountParametersMetric metric;
    auto result = metric.Calculate(functions.front());
    EXPECT_EQ(std::get<int>(result.value), expected);
}

INSTANTIATE_TEST_SUITE_P(
    ParametersCount,
    ParametersCountTest,
    ::testing::Values(
        std::make_pair("comments.py", 2),
        std::make_pair("exceptions.py", 0),
        std::make_pair("if.py", 1),
        std::make_pair("loops.py", 1),
        std::make_pair("many_lines.py", 1),
        std::make_pair("many_parameters.py", 5),
        std::make_pair("match_case.py", 1),
        std::make_pair("nested_if.py", 2),
        std::make_pair("simple.py", 0),
        std::make_pair("ternary.py", 1)
    )
);

}  // namespace analyzer::metric::metric_impl
