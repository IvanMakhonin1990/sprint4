#include "metric_accumulator_impl/average_accumulator.hpp"
#include <gtest/gtest.h>

namespace analyzer::metric_accumulator::metric_accumulator_impl {

TEST(AverageAccumulatorTest, ThrowsBeforeFinalize) {
    AverageAccumulator acc;
    EXPECT_THROW(acc.Get(), std::runtime_error);
}

TEST(AverageAccumulatorTest, AccumulateSingleValue) {
    AverageAccumulator acc;
    metric::MetricResult mr{"test", 10};
    acc.Accumulate(mr);
    acc.Finalize();
    EXPECT_DOUBLE_EQ(acc.Get(), 10.0);
}

TEST(AverageAccumulatorTest, AccumulateMultipleValues) {
    AverageAccumulator acc;
    acc.Accumulate({"test", 10});
    acc.Accumulate({"test", 20});
    acc.Accumulate({"test", 30});
    acc.Finalize();
    EXPECT_DOUBLE_EQ(acc.Get(), 20.0);
}

TEST(AverageAccumulatorTest, ResetClearsState) {
    AverageAccumulator acc;
    acc.Accumulate({"test", 10});
    acc.Finalize();
    EXPECT_DOUBLE_EQ(acc.Get(), 10.0);
    
    acc.Reset();
    EXPECT_THROW(acc.Get(), std::runtime_error);
}

TEST(AverageAccumulatorTest, ReaccumulateAfterReset) {
    AverageAccumulator acc;
    acc.Accumulate({"test", 10});
    acc.Finalize();
    
    acc.Reset();
    acc.Accumulate({"test", 50});
    acc.Accumulate({"test", 100});
    acc.Finalize();
    EXPECT_DOUBLE_EQ(acc.Get(), 75.0);
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl
