#pragma once

#include <iostream>     // cout, ostream
#include <array>        // array
#include <optional>     // optional, nullopt
#include <bitset>       // bitset
#include <string>       // string
#include <algorithm>    // generate_n, shuffle, min
#include <vector>       // vector
#include <iterator>     // back_inserter
#include <utility>      // move, pair
#include <map>

#include <stdint.h>


template<typename T>
constexpr inline T isqrt(T val)
{
    for (T i(0); i < val; i++)
    {
        if (i * i >= val)
            return i;
    }
    return T(0);
}


using fast = uint_fast8_t;
using index = fast; // uint_fast16_t;
using sud  = uint_fast8_t;


template <uint8_t Size>
class grid;


template<uint8_t Size>
void print(const grid<Size>& g, std::ostream& out = std::cout);


template <uint8_t Size>
class bag
{
    std::array<std::bitset<Size>, Size> rows{};
    std::array<std::bitset<Size>, Size> cols{};
    std::array<std::bitset<Size>, Size> blocks{};

    constexpr auto block() const { return grid<Size>::block(); }

    constexpr auto blk_idx(sud x, sud y) const
    {
        return (y / 3) * block() + (x / 3);
    }

    constexpr void _set(sud x, sud y, sud val, bool b)
    {
        rows[y][val - 1] = b;
        cols[x][val - 1] = b;
        blocks[blk_idx(x, y)][val - 1] = b;
    }

public:
    constexpr void set(sud x, sud y, sud val)
    {
        _set(x, y, val, true);
    }

    constexpr void reset(sud x, sud y, sud val)
    {
        _set(x, y, val, false);
    }

    constexpr bool possible(sud x, sud y, sud val) const
    {
        val -= 1;
        return !rows[y][val]
            && !cols[x][val]
            && !blocks[blk_idx(x, y)][val];
    }
};


template <uint8_t Size>
class grid
{
    std::array<sud, Size * Size> data{ 0 };

    constexpr bool validate() const
    {
        auto opts = bag<Size>{};
        for (fast y = 0; y < Size; y++)
        {
            for (fast x = 0; x < Size; x++)
            {
                if (!opts.possible(x, y, at(x, y)))
                    return false;
                opts.set(x, y, at(x, y));
            }
        }
        return true;
    }

public:
    constexpr grid(std::array<sud, Size * Size> data)
        : data(std::move(data))
    { }

    constexpr grid() = default;

    static constexpr auto size() { return Size; }

    constexpr bool check() const
    {
        auto opts = bag<Size>{};
        for (fast y = 0; y < Size; y++)
        {
            for (fast x = 0; x < Size; x++)
            {
                sud val = at(x, y);
                if (val == 0)
                    continue;

                if (!opts.possible(x, y, val))
                    return false;

                opts.set(x, y, val);
            }
        }
        return true;
    }

    constexpr bool is_solved() const
    {
        for (fast i = 0; i < data.size(); i++)
            if (data[i] == 0)
                return false;

        return validate();
    }

    bool possible_at(sud val, sud x, sud y)
    {
        auto prev = at(x, y);
        at(x, y) = val;
        bool ok = check();
        at(x, y) = prev;
        return ok;
    }

    auto solve_rec(index i, grid& gr, bag<Size>& opts) const
        -> std::optional<grid>
    {
        for (; i < gr.data.size(); i++)
        {
            if (gr.data[i] != 0)
                continue;

            fast x = i % Size,
                 y = i / Size;

            for (fast j = 1; j <= Size; j++)
            {
                if (!opts.possible(x, y, j))
                    continue;

                gr.data[i] = j;
                opts.set(x, y, j);

                // // PRINT
                // std::cout << "\033[13A";
                // print(gr);

                auto rec = solve_rec(i + 1, gr, opts);
                if (rec.has_value())
                {
                    return rec;
                }

                opts.reset(x, y, j);
            }

            gr.data[i] = 0;
            return std::nullopt;
        }

        if (gr.validate())
            return { gr };

        return std::nullopt;
    }

    std::optional<grid> solution() const
    {
        auto copy = *this;
        auto opts = bag<Size>{};

        for (int y = 0; y < Size; y++)
            for (int x = 0; x < Size; x++)
                if (copy.at(x, y) != 0)
                    opts.set(x, y, copy.at(x, y));

        return solve_rec(0, copy, opts);
    }

    static constexpr sud block() { return isqrt(Size); }

    constexpr const sud& at(sud x, sud y) const { return data[x + y * Size]; }
    constexpr       sud& at(sud x, sud y)       { return data[x + y * Size]; }
};



template<uint8_t Size>
void print(const grid<Size>& g, std::ostream& out)
{
    auto line = [&]()
    {
        for (fast i = 0; i < g.block(); i++)
            out << "+-"
                << std::string(g.block() * 2, '-');
        out << "+\n";
    };

    line();

    for (fast y = 0; y < Size; y++)
    {
        out << "| ";

        auto* sp = "";
        for (fast x = 0; x < Size; x++)
        {
            out << sp;
            sp = " ";

            int val = static_cast<int>(g.at(x, y));

            val == 0 ? (out << ".") : (out << val);

            if (x % g.block() == g.block() - 1)
                out << " |";
        }

        out << "\n";

        if (y % g.block() == g.block() - 1)
            line();
    }
}


template<uint8_t Size, typename Rand>
grid<Size> generate(int filled, Rand& rand)
{
    auto vals = std::vector<sud>{};
    std::generate_n(std::back_inserter(vals), Size,
                    [n = 1]() mutable { return n++; });

    auto indices = std::vector<std::pair<sud, sud>>{};
    indices.reserve(Size * Size);

    for (int x = 0; x < Size; x++)
        for (int y = 0; y < Size; y++)
            indices.emplace_back(x, y);

    std::shuffle(indices.begin(), indices.end(), rand);

    auto opts = bag<Size>{};
    auto res = grid<Size>{};

    for (int i = 0; i < std::min<int>(filled, indices.size()); i++)
    {
        auto [x, y] = indices[i];

        std::shuffle(vals.begin(), vals.end(), rand);

        for (auto v : vals)
        {
            if (opts.possible(x, y, v))
            {
                res.at(x, y) = v;
                opts.set(x, y, v);
                break;
            }
        }
    }
    return res;
}


