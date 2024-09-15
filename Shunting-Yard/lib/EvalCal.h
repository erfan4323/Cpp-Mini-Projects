#pragma once

#include <deque>
#include <vector>
#include <string>
#include <cstdint> 
#include <iostream>
#include <unordered_map>

#include "Types.h"

std::deque<Symbol> Evaluate(std::string expression);

double Calculate(std::deque<Symbol> stkOutput);