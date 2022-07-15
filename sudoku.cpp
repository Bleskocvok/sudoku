
#include <iostream>
#include <array>
#include <optional>
#include <bitset>
#include <string>
#include <random>
#include <algorithm>
#include <set>
#include <vector>
#include <iterator>
#include <utility>
#include <cassert>

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

    // void validate(sud x, sud y, sud val) const
    // {
    //     assert(val != 0 && val <= Size);
    //     assert(x >= 0 && x < Size);
    //     assert(y >= 0 && y < Size);
    // }

    constexpr auto blk_idx(sud x, sud y) const
    {
        return (y / 3) * block() + (x / 3);
    }

    void _set(sud x, sud y, sud val, bool b)
    {
        rows[y][val - 1] = b;
        cols[x][val - 1] = b;
        blocks[blk_idx(x, y)][val - 1] = b;
    }

public:
    void set(sud x, sud y, sud val)
    {
        _set(x, y, val, true);
    }

    void reset(sud x, sud y, sud val)
    {
        _set(x, y, val, false);
    }

    bool possible(sud x, sud y, sud val) const
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

    template<typename Obj, typename F1, typename F2>
    bool check_lambda(Obj& bits, F1 cond, F2 chk) const
    {
        for (int_fast8_t i = 0; i < Size; i++)
        {
            for (int_fast8_t x = 0; x < Size; x++)
            {
                if (!cond(at(x, i)))
                    return false;
            }
            if (!chk(bits))
                return false;
            bits.reset();

            for (int_fast8_t y = 0; y < Size; y++)
            {
                if (!cond(at(i, y)))
                    return false;
            }
            if (!chk(bits))
                return false;
            bits.reset();
        }

        for (int_fast8_t i = 0; i < block(); i++)
        {
            for (int_fast8_t j = 0; j < block(); j++)
            {
                for (int_fast8_t y = 0; y < block(); y++)
                {
                    for (int_fast8_t x = 0; x < block(); x++)
                    {
                        if (!cond(at(x + i * block(), y + j * block())))
                            return false;
                    }
                }
                if (!chk(bits))
                    return false;
                bits.reset();
            }
        }

        return true;
    }

    bool check_at(sud x, sud y) const
    {
        auto bits = std::bitset<Size>{ 0 };
        auto cond = [&](auto v)
        {
            if (v == 0)
                return true;
            if (bits.test(v - 1))
                return false;
            bits.set(v - 1);
            return true;
        };

        for (fast i = 0; i < Size; i++)
        {
            if (!cond(at(x, i)))
                return false;
        }
        bits.reset();

        for (fast i = 0; i < Size; i++)
        {
            if (!cond(at(i, y)))
                return false;
        }
        bits.reset();

        fast i = x / block(),
                    j = y / block();

        for (fast y = 0; y < block(); y++)
        {
            for (fast x = 0; x < block(); x++)
            {
                if (!cond(at(x + i * block(), y + j * block())))
                    return false;
            }
        }
        bits.reset();

        return true;
    }

public:
    constexpr grid(std::array<sud, Size * Size> data)
        : data(std::move(data))
    { }

    constexpr grid() = default;

    bool check() const
    {
        auto bits = std::bitset<Size>{ 0 };
        auto cond = [&](auto v)
        {
            if (v == 0)
                return true;
            if (bits.test(v - 1))
                return false;
            bits.set(v - 1);
            return true;
        };
        return check_lambda(bits, cond, [](const auto&) { return true; });
    }

    bool possible_at(sud val, sud x, sud y)
    {
        auto prev = at(x, y);
        at(x, y) = val;
        bool ok = check();
        at(x, y) = prev;
        return ok;
    }

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


int main(int argc, char** argv)
{
    // int filled = 35;
    // if (argc < 2)
    // {
    //     std::cerr << "usage: " << argv[0] << " [FILLED]\n";
    //     std::cerr << "using default value: " << filled << "\n";
    // }
    // else
    // {
    //     filled = std::stoi(argv[1]);
    // }

    // auto seeder = std::random_device{};
    // auto seed = seeder();
    // auto rand = std::mt19937{ seed };

    // std::cout << "seed: " << seed << "\n";

    // unsigned long long i = 0;
    // while (true)
    // {
    //     auto gr = generate<9>(filled, rand);
    //     if (gr.check())
    //     {
    //         auto sol = gr.solution();
    //         if (sol)
    //         {
    //             std::cout << "input:\n";
    //             print(gr);
    //             std::cout << "solution:\n";
    //             print(*sol);
    //             break;
    //         }
    //     }

    //     // std::cout << "failed\n";
    //     // print(gr);
    //     if (i % 10000 == 0)
    //         std::cout << i << std::endl;
    //     i++;
    // }
    // return 0;

    // grid<9> ahoj(
    // {
    //     0, 6, 2,  0, 0, 7,  0, 4, 3,
    //     0, 0, 0,  0, 5, 6,  1, 0, 0,
    //     0, 7, 0,  3, 0, 0,  5, 9, 0,

    //     0, 0, 0,  0, 4, 0,  2, 0, 0,
    //     1, 9, 7,  0, 8, 0,  4, 6, 5,
    //     0, 0, 6,  0, 7, 0,  0, 0, 0,

    //     0, 8, 3,  0, 0, 2,  0, 5, 0,
    //     0, 0, 5,  7, 3, 0,  0, 0, 0,
    //     4, 2, 0,  5, 0, 0,  7, 3, 0,
    // });

    // sud _ = 0;
    // grid<9> ahoj(
    // {
    //     _, _, 8,  _, _, 7,  4, _, _,
    //     _, 3, _,  _, 8, _,  _, 5, _,
    //     _, _, 5,  3, _, 6,  _, _, _,

    //     6, _, _,  _, 1, _,  _, _, _,
    //     _, _, _,  _, 2, 4,  _, 7, 9,
    //     _, _, _,  _, _, _,  8, _, _,

    //     _, 1, _,  _, 4, _,  5, _, _,
    //     _, 7, 6,  8, _, _,  1, _, 2,
    //     _, _, _,  7, _, _,  _, _, _,
    // });

    sud _ = 0;
    grid<9> ahoj(
    {
        _, _, _,  _, _, _,  _, _, 5,
        _, _, _,  _, _, _,  _, _, _,
        2, _, _,  _, _, _,  _, _, _,

        _, _, _,  5, 2, _,  _, _, _,
        _, _, _,  6, _, _,  _, _, _,
        _, _, _,  _, _, 9,  _, _, _,

        _, _, _,  _, _, _,  _, _, _,
        _, _, 1,  _, _, 5,  _, _, _,
        _, _, 8,  _, _, 1,  _, _, _,
    });

    print(ahoj);

    auto r = ahoj.solution();

    if (r)
    {
        print(*r);
        std::cout << r->validate() << std::endl;
    }
    else
    {
        std::cout << "no solution\n";
    }

    // std::cout << ahoj.check() << std::endl;
    // std::cout << ahoj.validate() << std::endl;
    return 0;
}
