#include "metric_impl/cyclomatic_complexity.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyzer::metric::metric_impl {
std::string CyclomaticComplexityMetric::Name() const { return kName; }
MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function &f) const {
    // Получаем строковое представление AST (абстрактного синтаксического дерева) функции.
    // Это S-выражение, сгенерированное утилитой tree-sitter, например:
    // "(function_definition name: (identifier) ... (if_statement ...) (for_statement ...))"
    auto &function_ast = f.ast;

    // Список типов узлов AST, каждый из которых увеличивает цикломатическую сложность на 1.
    // Эти узлы соответствуют управляющим конструкциям языка Python:
    // - if / elif
    // - циклы (for, while)
    // - обработка исключений (try, finally)
    // - case в match-выражениях
    // - assert
    // - тернарный оператор (conditional_expression)
    constexpr std::array<std::string_view, 10> complexity_nodes = {
        "(if_statement",           // if
        "(elif_clause",            // elif
        "(for_statement",          // for
        "(while_statement",        // while
        "(try_statement",          // try
        "(except_clause",          // except (catch)
        "(finally_clause",         // finally
        "(case_clause",            // case
        "(assert_statement",       // assert
        "(conditional_expression", // ternary operator
    };

    auto count_node = [&](std::string_view node) {
        auto splits = std::views::split(function_ast, node);
        return std::ranges::distance(splits) - 1;
    };
    auto complexities = complexity_nodes | std::views::transform(count_node);
    
    int complexity = 1;
    auto add_complexity = [&](int c) { complexity += c; };
    std::ranges::for_each(complexities, add_complexity);
    
    return MetricResult::ValueType{complexity};
}
}  // namespace analyzer::metric::metric_impl
