#include "connectivity.h"

#include <random>

#include "gtest/gtest.h"

TEST(Connectivity, PostSynapticNeuronsInRange)
{
  std::random_device rd;
  std::mt19937 gen(rd());

  SparseProjection sparseProjection;
  build_fixed_probability_connector(1000, 1000, 0.1, sparseProjection, false, gen);

  unsigned int expectedNumConnections = 1000 * 1000 * 0.1;
  EXPECT_LT(expectedNumConnections * 0.8, sparseProjection.connN);
  EXPECT_GT(expectedNumConnections * 1.2, sparseProjection.connN);
}