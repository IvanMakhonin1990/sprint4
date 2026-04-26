#include "metric_accumulator_impl/sum_average_accumulator.hpp"
#include <gtest/gtest.h>

namespace analyzer::metric_accumulator::metric_accumulator_impl {

TEST(SumAverageAccumulatorTest, ThrowsBeforeFinalize) {
    SumAverageAccumulator acc;
    EXPECT_THROW(acc.Get(), std::runtime_error);
}

TEST(SumAverageAccumulatorTest, AccumulateSingleValue) {
    SumAverageAccumulator acc;
    metric::MetricResult mr{"test", 10};
    acc.Accumulate(mr);
    acc.Finalize();
    auto result = acc.Get();
    EXPECT_EQ(result.sum, 10);
    EXPECT_DOUBLE_EQ(result.average, 10.0);
}

TEST(SumAverageAccumulatorTest, AccumulateMultipleValues) {
    SumAverageAccumulator acc;
    acc.Accumulate({"test", 10});
    acc.Accumulate({"test", 20});
    acc.Accumulate({"test", 30});
    acc.Finalize();
    auto result = acc.Get();
    EXPECT_EQ(result.sum, 60);
    EXPECT_DOUBLE_EQ(result.average, 20.0);
}

TEST(SumAverageAccumulatorTest, ResetClearsState) {
    SumAverageAccumulator acc;
    acc.Accumulate({"test", 10});
    acc.Finalize();
    
    acc.Reset();
    EXPECT_THROW(acc.Get(), std::runtime_error);
}

TEST(SumAverageAccumulatorTest, ReaccumulateAfterReset) {
    SumAverageAccumulator acc;
    acc.Accumulate({"test", 10});
    acc.Finalize();
    
    acc.Reset();
    acc.Accumulate({"test", 5});
    acc.Accumulate({"test", 15});
    acc.Finalize();
    auto result = acc.Get();
    EXPECT_EQ(result.sum, 20);
    EXPECT_DOUBLE_EQ(result.average, 10.0);
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl
