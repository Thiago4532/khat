#include "rng.hpp"

std::mt19937 rng(std::random_device {}());