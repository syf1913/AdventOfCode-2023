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

    // Part 2 Solution
    uint64_t result{ UINT64_MAX };
    {
        std::vector<std::array<uint64_t, size_t{ 2 }> > seeds;
        std::istringstream iss{ input_file_lines[0] };
        iss.ignore(6);
        uint64_t tmp_seed, tmp_length;
        while (iss >> tmp_seed >> tmp_length)
        {
            seeds.push_back({ tmp_seed, tmp_length });
        }

        for (const auto& seed : seeds)
        {
            for (uint64_t i = 0; i < seed[1]; ++i)
            {
                uint64_t tmp_seed = seed[0] + i;
                std::vector<uint64_t> to_skip;
                for (const auto& map : maps)
                {
                    uint64_t min_skip{ UINT64_MAX };
                    for (const auto& array_in_map : map)
                    {
                        if (tmp_seed >= array_in_map[1] && tmp_seed < array_in_map[1] + array_in_map[2])
                        {
                            to_skip.push_back(array_in_map[1] + array_in_map[2] - tmp_seed - 1);
                            tmp_seed = (tmp_seed - array_in_map[1]) + array_in_map[0];
                            break;
                        }
                        else
                        {
                            min_skip = std::min(min_skip, array_in_map[1] - tmp_seed - 1);
                        }
                    }

                    if (min_skip < UINT64_MAX)
                    {
                        to_skip.push_back(min_skip);
                    }
                }

                if (!to_skip.empty())
                {
                    i += *std::min_element(to_skip.cbegin(), to_skip.cend());
                }

                result = std::min(result, tmp_seed);
            }
        }
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