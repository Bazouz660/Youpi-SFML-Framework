#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <bitset>
#include <queue>
#include <memory>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>

#include "sfml.hpp"

template<typename T>
using sptr_ref = std::shared_ptr<T>&;

template<typename T>
using const_sptr_ref = const std::shared_ptr<T>&;