#pragma once

#include <cstdint>
#include <string>

struct Operator
{
    uint8_t precedence = 0;
    uint8_t arguments = 0;
};

struct Symbol
{
    std::string symbol = "";

    enum class Type : uint8_t
    {
        Unknown,
        Literal_Numeric,
        Operator,
        Par_Open,
        Par_Close,
    } type = Type::Unknown;

    Operator op;
};