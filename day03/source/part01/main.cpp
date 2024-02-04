#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

void mapProjectedByNumbersAndSymbols(
    const std::vector<std::string>                      &input_file_lines,
    std::map<std::array<int, 2>, char>                  &symbol_map,
    std::map<std::array<int, 2>, std::vector<uint32_t>> &number_map)
{
    // 遍历input文件内容的所有行
    for (int i = 0; i < input_file_lines.size(); ++i)
    {
        std::vector<char>               number_char;
        std::vector<std::array<int, 2>> number_position;

        // 遍历文件每行的所有字符
        for (int j = 0; j < input_file_lines[i].size(); ++j)
        {

            // 如果是数字则记录数字及其上下位置坐标
            if (std::isdigit(input_file_lines[i][j]))
            {
                // 如果是数字的第一位，则还要记录其左上和左下的位置坐标
                if (number_position.empty())
                {
                    number_position.push_back(std::array<int, 2>{i - 1, j - 1});
                    number_position.push_back(std::array<int, 2>{i, j - 1});
                    number_position.push_back(std::array<int, 2>{i + 1, j - 1});
                }
                number_char.push_back(input_file_lines[i][j]);
                number_position.push_back(std::array<int, 2>{i, j});
                number_position.push_back(std::array<int, 2>{i - 1, j});
                number_position.push_back(std::array<int, 2>{i + 1, j});
            }
            else
            {
                // 如果当前字符不是数字，且number_char不为空，则表示数字字符串结束
                if (!number_char.empty())
                {
                    // 将数字字符串转换为数字
                    uint32_t number = std::stoul(
                        std::string(number_char.begin(), number_char.end()));

                    // 记录数字字符串后一位字符之上下位置坐标
                    number_position.push_back(std::array<int, 2>{i - 1, j});
                    number_position.push_back(std::array<int, 2>{i, j});
                    number_position.push_back(std::array<int, 2>{i + 1, j});

                    for (const auto &position : number_position)
                    {
                        // 如果某个位置对应的坐标已在number_map中，
                        // 则将其追加到number_map中

                        // only in c++17: if(init-statement)
                        // in selection statement

                        // if (auto number_map_iter = number_map.find(position);
                        //     number_map_iter != number_map.end())
                        // {
                        //     number_map_iter->second.push_back(number);
                        // }

                        if (number_map.find(position) != number_map.end())
                        {
                            number_map.find(position)->second.push_back(number);
                        }
                        else
                        {
                            // 否则，在number_map对应坐标位置处映射首个数字(字符串)
                            // 使用map的默认构造函数
                            // map[{std::array<int, 2> position}] =
                            // std::vector<uint32_t> or {number} =>
                            // vector<uint32_t>
                            number_map[{position}] = {number};
                        }
                    }
                    // 数字字符串结束，重置数字字符串及其位置坐标容器，
                    // 以确保下一个数字字符串迭代正确
                    number_char.clear();
                    number_position.clear();
                }

                // 如果当前字符不是数字，且number_char为空，
                // 即表示现在不是记录数字字符串的结束，
                // 则将其映射到symbol_map中
                if (input_file_lines[i][j] != '.')
                {
                    symbol_map[{i, j}] = input_file_lines[i][j];
                }
            }
        } // end loop j
    }     // end loop i
}

int main(int argc, char *argv[])
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

    // 检查文件是否正确打卡
    if (!input_file.is_open())
    {
        std::cerr << "Error opening file: " << input_filename << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Get contents of the input file ...";
    uint32_t result(0);

    std::string              line;
    std::vector<std::string> input_file_lines;
    while (std::getline(input_file, line))
    {
        // 将input文件中的所有行内容存储到一个vector中
        input_file_lines.push_back(line);
    }

    // input_file_lines.insert(input_file_lines.begin(),
    //                         std::string(input_file_lines[0].size(), '.'));

    // 必须在input文件内容的每一行最后添加一个dot字符，
    // 作为数字字符串的结束字符，
    // 否则，无法正确解析符合要求的数字字符串
    std::transform(input_file_lines.begin(),
                   input_file_lines.end(),
                   input_file_lines.begin(),
                   [](std::string &row) { return std::move(row) + '.'; });

    // 手动关闭文件
    input_file.close();

    std::map<std::array<int, 2>, char>                  symbol_map;
    std::map<std::array<int, 2>, std::vector<uint32_t>> number_map;

    mapProjectedByNumbersAndSymbols(input_file_lines, symbol_map, number_map);

    for (const auto &pos_symbol : symbol_map)
    {
        // only in c++17: if(init-statement) in selection statement
        // if (const auto number_map_iter = number_map.find(position);
        //     number_map_iter != number_map.end())
        if (number_map.find(pos_symbol.first) != number_map.end())
        {
            //! WARNING: 这里假设每个数字字符串有且仅有一个相邻的非dot字符
            // 非dot字符对应坐标位置处的所有数字字符串求和
            result += std::accumulate(
                number_map.find(pos_symbol.first)->second.cbegin(),
                number_map.find(pos_symbol.first)->second.cend(),
                uint32_t{0});
        }
    }

    // 获取程序结束执行的时间点
    auto end_time = std::chrono::high_resolution_clock::now();
    // 计算时间差
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - start_time);

    std::cout << "   finished ! " << std::endl;
    std::cout << "Result: " << result << std::endl; // right answer: 546312

    // 输出程序运行时间
    std::cout << "Time taken by this code: " << duration.count()
              << " microseconds" << std::endl;

    return EXIT_SUCCESS;
}