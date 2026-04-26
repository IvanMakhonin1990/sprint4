#include "metric_impl/cyclomatic_complexity.hpp"
#include <gtest/gtest.h>
#include "file.hpp"
#include "function.hpp"

namespace analyzer::metric::metric_impl {

class CyclomaticComplexityTest : public ::testing::TestWithParam<std::pair<std::string, int>> {};

TEST_P(CyclomaticComplexityTest, CalculatesCorrectly) {
    auto [filename, expected] = GetParam();
    file::File file(filename);
    function::FunctionExtractor extractor;
    auto functions = extractor.Get(file);
    
    ASSERT_FALSE(functions.empty());
    
    CyclomaticComplexityMetric metric;
    auto result = metric.Calculate(functions.front());
    EXPECT_EQ(std::get<int>(result.value), expected);
}

INSTANTIATE_TEST_SUITE_P(
    CyclomaticComplexity,
    CyclomaticComplexityTest,
    ::testing::Values(
        std::make_pair("comments.py", 1),
        std::make_pair("exceptions.py", 3),
        std::make_pair("if.py", 2),
        std::make_pair("loops.py", 3),
        std::make_pair("many_lines.py", 1),
        std::make_pair("many_parameters.py", 2),
        std::make_pair("match_case.py", 4),
        std::make_pair("nested_if.py", 5),
        std::make_pair("simple.py", 2),
        std::make_pair("ternary.py", 3)
    )
);

}  // namespace analyzer::metric::metric_impl
