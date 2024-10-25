        for (size_t x = 1; x < appWidth - 1; x++)
        {
            for (size_t y = 1; y < appHeight - 1; y++)
            {
                auto a = grid[y][x].x;
                auto b = grid[y][x].y;
                nextGrid[y][x].x = a + (da * LaplaceA(x, y)) - (a * b * b) + (feed * (1 - a)) * dt;
                nextGrid[y][x].y = b + (db * LaplaceB(x, y)) - (a * b * b) + ((k + feed) * b) * dt;
            }
        }