#include "metric_impl/code_lines_count.hpp"
#include <gtest/gtest.h>
#include "file.hpp"
#include "function.hpp"

namespace analyzer::metric::metric_impl {

class CodeLinesCountTest : public ::testing::TestWithParam<std::pair<std::string, int>> {};

TEST_P(CodeLinesCountTest, CalculatesCorrectly) {
    auto [filename, expected] = GetParam();
    file::File file(filename);
    function::FunctionExtractor extractor;
    auto functions = extractor.Get(file);
    
    ASSERT_FALSE(functions.empty());
    
    CodeLinesCountMetric metric;
    auto result = metric.Calculate(functions.front());
    EXPECT_EQ(std::get<int>(result.value), expected);
}

INSTANTIATE_TEST_SUITE_P(
    CodeLinesCount,
    CodeLinesCountTest,
    ::testing::Values(
        std::make_pair("comments.py", 5),
        std::make_pair("exceptions.py", 7),
        std::make_pair("if.py", 4),
        std::make_pair("loops.py", 7),
        std::make_pair("many_lines.py", 2),
        std::make_pair("many_parameters.py", 2),
        std::make_pair("match_case.py", 8),
        std::make_pair("nested_if.py", 9),
        std::make_pair("simple.py", 7),
        std::make_pair("ternary.py", 2)
    )
);

}  // namespace analyzer::metric::metric_impl
