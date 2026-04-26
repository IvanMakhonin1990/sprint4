#include "metric_accumulator_impl/categorical_accumulator.hpp"
#include <gtest/gtest.h>

namespace analyzer::metric_accumulator::metric_accumulator_impl {

TEST(CategoricalAccumulatorTest, ThrowsBeforeFinalize) {
    CategoricalAccumulator acc;
    EXPECT_THROW(acc.Get(), std::runtime_error);
}

TEST(CategoricalAccumulatorTest, AccumulateSingleCategory) {
    CategoricalAccumulator acc;
    metric::MetricResult mr{"test", "Snake Case"};
    acc.Accumulate(mr);
    acc.Finalize();
    auto result = acc.Get();
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result.at("Snake Case"), 1);
}

TEST(CategoricalAccumulatorTest, AccumulateMultipleCategories) {
    CategoricalAccumulator acc;
    acc.Accumulate({"test", "Snake Case"});
    acc.Accumulate({"test", "Snake Case"});
    acc.Accumulate({"test", "Pascal Case"});
    acc.Finalize();
    auto result = acc.Get();
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result.at("Snake Case"), 2);
    EXPECT_EQ(result.at("Pascal Case"), 1);
}

TEST(CategoricalAccumulatorTest, ResetClearsState) {
    CategoricalAccumulator acc;
    acc.Accumulate({"test", "Snake Case"});
    acc.Finalize();
    
    acc.Reset();
    EXPECT_THROW(acc.Get(), std::runtime_error);
}

TEST(CategoricalAccumulatorTest, ReaccumulateAfterReset) {
    CategoricalAccumulator acc;
    acc.Accumulate({"test", "Pascal Case"});
    acc.Finalize();
    
    acc.Reset();
    acc.Accumulate({"test", "Camel Case"});
    acc.Accumulate({"test", "Lower Case"});
    acc.Finalize();
    auto result = acc.Get();
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result.at("Camel Case"), 1);
    EXPECT_EQ(result.at("Lower Case"), 1);
    EXPECT_TRUE(result.find("Pascal Case") == result.end());
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl
