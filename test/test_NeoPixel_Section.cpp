#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../NeoPixel_Section.h"

TEST_CASE("PatternCompleted is 0") {
  NeoPixel_Section section(NULL, 0, 10, NULL);

  REQUIRE(section.PatternCompleted == 0);
}
