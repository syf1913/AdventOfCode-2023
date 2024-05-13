#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

int
main(int argc, char* argv[])
{
    // 获取程序开始执行的时间点
    auto start_time = std::chrono::high_resolution_clock::now();

    std::string input_filename;
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " input_filename" << std::endl;
        exit(EXIT_FAILURE);
    }

    input_filename = argv[1];
    std::fstream input_file(input_filename, std::ios::in);

    // 检查文件是否正确打开
    if (!input_file.is_open())
    {
        std::cerr << "Error opening file: " << input_filename << std::endl;
    }

    std::cout << "Get contents of the input file ... ";

    std::vector<std::string> input_file_lines;
    std::string line;
    while (std::getline(input_file, line))
    {
        input_file_lines.push_back(line);
    }
    input_file.close();

    // 从 input 本文内容的第一行读取 seeds
    std::vector<uint64_t> seeds;
    std::istringstream iss{ input_file_lines[0] };
    iss.ignore(6);
    uint64_t tmp_seed;
    while (iss >> tmp_seed)
    {
        seeds.push_back(tmp_seed);
    }
    // debug
    // std::for_each(seeds.cbegin(), seeds.cend(), [](const uint64_t& seed) { std::cout << seed << " "; });
    // std::cout << std::endl;

    std::vector<std::vector<std::array<uint64_t, size_t{ 3 }> > > maps;
    {
        // 添加空字符，使其进入 if 分支
        input_file_lines.push_back("");

        std::vector<std::array<uint64_t, size_t{ 3 }> > map;
        for (size_t nline = 3; nline < input_file_lines.size(); ++nline)
        {
            if (input_file_lines[nline].empty())
            {
                maps.push_back(map);
                map.clear();
                // 跳过字符说明行
                nline++;
                continue;
            }

            std::istringstream iss{ input_file_lines[nline] };
            uint64_t destination, source, length;
            iss >> destination >> source >> length;
            map.push_back({ destination, source, length });
        }

        // 这里加入 {}，会释放临时变量 map
    }

    // Part 1 Solution
    uint64_t result{};
    {
        auto seeds_copy = seeds;
        for (const auto& map : maps)
        {
            for (auto& seed : seeds_copy)
            {
                for (const auto& array_in_map : map)
                {
                    if (seed >= array_in_map[1] && seed < array_in_map[1] + array_in_map[2])
                    {
                        /*
                         * seed 与 source 之间的差值 映射到 destination + 该差值
                         * 并且随着最外层循环，map : maps
                         * 在此处直接修改 seed 值，即可实现 seed-to-soil，soil-to-fertilizer ...
                         */

                        // seed += array_in_map[0] - array_in_map[1];
                        seed = (seed - array_in_map[1]) + array_in_map[0];
                        break;
                    }
                }
            }
        }

        result = *std::min_element(seeds_copy.cbegin(), seeds_copy.cend());
    }

    // 获取程序结束执行的时间点
    auto end_time = std::chrono::high_resolution_clock::now();
    // 计算时间差
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "   finished! " << std::endl;
    std::cout << "Result: " << result << std::endl; // right answer: 551761867

    // 输出程序运行时间
    std::cout << "Time taken by this code: " << duration.count() << " microseconds" << std::endl;

    return EXIT_SUCCESS;
}