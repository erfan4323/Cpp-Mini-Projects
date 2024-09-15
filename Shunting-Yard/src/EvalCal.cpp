#include "../lib/EvalCal.h"



std::deque<Symbol> Evaluate(std::string expression)
{

    std::unordered_map<char, Operator> mapOps;
    mapOps['/'] = { 4, 2 };
    mapOps['*'] = { 3, 2 };
    mapOps['+'] = { 2, 2 };
    mapOps['-'] = { 1, 2 };

    std::deque<Symbol> stkHolding;
    std::deque<Symbol> stkOutput;

    Symbol symPrevious = { "0", Symbol::Type::Literal_Numeric, { 0, 0 } };
    int pass = 0;

    for (const char c : expression)
    {
        if (std::isdigit(c))
        {
            stkOutput.push_back({ std::string(1, c), Symbol::Type::Literal_Numeric });
            symPrevious = stkOutput.back();
        }
        else if (c == '(')
        {
            stkHolding.push_front({ std::string(1, c), Symbol::Type::Par_Open });
            symPrevious = stkHolding.front();
        }
        else if (c == ')')
        {
            while (!stkHolding.empty() && stkHolding.front().type != Symbol::Type::Par_Open)
            {
                stkOutput.push_back(stkHolding.front());
                stkHolding.pop_front();
            }

            if (stkHolding.empty())
            {
                std::cerr << "Missing '('" << std::endl;
                return std::deque<Symbol>();
            }

            stkHolding.pop_front();

            symPrevious = { std::string(1, c), Symbol::Type::Par_Close };
        }
        else if (mapOps.find(c) != mapOps.end())
        {
            Operator newOp = mapOps[c];

            if (c == '-' || c == '+')
            {
                if (symPrevious.type != Symbol::Type::Literal_Numeric
                    && symPrevious.type != Symbol::Type::Par_Close
                    || pass == 0)
                {
                    newOp.arguments = 1;
                    newOp.precedence = 100;
                }
            }

            while (!stkHolding.empty() && stkHolding.front().type != Symbol::Type::Par_Open)
            {
                if (stkHolding.front().type == Symbol::Type::Operator)
                {
                    const auto& topOp = stkHolding.front().op;

                    if (topOp.precedence >= newOp.precedence)
                    {
                        stkOutput.push_back(stkHolding.front());
                        stkHolding.pop_front();
                    }
                    else
                    {
                        break;
                    }

                }
            }

            stkHolding.push_front({ std::string(1, c), Symbol::Type::Operator, newOp });

            symPrevious = stkHolding.front();

        }
        else
        {
            std::cerr << "Unknown symbol: " << c << std::endl;
            return std::deque<Symbol>();
        }

        pass++;

    }

    while (!stkHolding.empty())
    {
        stkOutput.push_back(stkHolding.front());
        stkHolding.pop_front();
    }

}
double Calculate(std::deque<Symbol> stkOutput)
{

    std::deque<double> stkSolve;

    for (const auto& inst : stkOutput)
    {
        switch (inst.type)
        {
        case Symbol::Type::Literal_Numeric:
        {
            stkSolve.push_front(std::stod(inst.symbol));
            break;
        }
        case Symbol::Type::Operator:
        {
            std::vector<double> args(inst.op.arguments);
            for (uint8_t a = 0; a < inst.op.arguments; a++)
            {
                if (stkSolve.empty())
                {
                    std::cerr << "Empty expression";
                }
                else
                {

                    args[a] = stkSolve[0];
                    stkSolve.pop_front();
                }
            }
            double result = 0.0;
            if (inst.op.arguments == 2)
            {
                if (inst.symbol[0] == '/') result = args[1] / args[0];
                if (inst.symbol[0] == '*') result = args[1] * args[0];
                if (inst.symbol[0] == '+') result = args[1] + args[0];
                if (inst.symbol[0] == '-') result = args[1] - args[0];
            }

            if (inst.op.arguments == 1)
            {
                if (inst.symbol[0] == '+') result = +args[0];
                if (inst.symbol[0] == '-') result = -args[0];
            }

            stkSolve.push_front(result);

            break;
        }
        default:
            break;
        }
    }

    return stkSolve[0];

}