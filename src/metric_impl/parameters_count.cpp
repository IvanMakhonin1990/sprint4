#include "metric_impl/parameters_count.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyzer::metric::metric_impl {
std::string CountParametersMetric::Name() const { return kName; }

MetricResult::ValueType CountParametersMetric::CalculateImpl(const function::Function &f) const {
    auto &function_ast = f.ast;
    // 1. Находим начало блока параметров
    const std::string parameters_marker = "(parameters";
    size_t params_start = function_ast.find(parameters_marker);
    if (params_start == std::string::npos) {
        return MetricResult::ValueType{0};
    }

    // 2. Находим конец блока параметров (балансировка скобок)
    int balance = 0;
    auto it = std::ranges::find_if(function_ast | std::views::drop(params_start), [&balance](char c) {
        if (c == '(') {
            balance++;
        } else if (c == ')') {
            balance--;
        }
        return balance == 0;
    });

    if (it == function_ast.end()) {
        return MetricResult::ValueType{0};
    }
    size_t params_end = std::distance(function_ast.begin(), it.base());

    // 3. Извлекаем подстроку с параметрами
    std::string_view params_block(function_ast.data() + params_start, params_end - params_start);

    // 4. Считаем параметры (идентификаторы или pattern-ы)
    const std::string id_marker = "(identifier";
    auto splits = std::views::split(params_block, id_marker);
    return MetricResult::ValueType{static_cast<int>(std::ranges::distance(splits) - 1)};
}

}  // namespace analyzer::metric::metric_impl
