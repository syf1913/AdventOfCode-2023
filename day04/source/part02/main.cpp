#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

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

    // 检查文件是否正确打开
    if (!input_file.is_open())
    {
        std::cerr << "Error opening file: " << input_filename << std::endl;
    }

    std::cout << "Get contents of the input file ... ";

    uint32_t result{0};

    // method 1
    {
        auto getPointsOfOneCard =
            [](const std::vector<std::string> &input_file_lines) {
                // 每张 card(id) 的数量
                // 初始情况下， original card(id) 数量为 1
                std::vector<uint32_t> cards_numbers(input_file_lines.size(), 1);
                uint32_t              sum_numbers_of_cards{0};

                for (const auto &line : input_file_lines)
                {
                    uint32_t              card_id;
                    uint32_t              tmp_int;
                    std::string           tmp_str;
                    std::set<std::string> winning_numbers;

                    std::istringstream i_str_steam(line);

                    i_str_steam >> tmp_str;
                    i_str_steam >> card_id;
                    // i_str_steam >> tmp_str;
                    i_str_steam.ignore(1);

                    bool   is_separated = false;
                    size_t num_of_intersected_numbers{0};

                    while (i_str_steam >> tmp_str)
                    {

                        if (tmp_str == "|")
                        {
                            is_separated = true;
                            continue;
                        }

                        if (is_separated)
                        {
                            // 越过分隔符 |， 即读取 my 数字字符串
                            if (winning_numbers.find(tmp_str) !=
                                winning_numbers.end())
                            {
                                num_of_intersected_numbers++;
                            }
                        }
                        else
                        {
                            // 未到达分隔符 |，即读取 winning 数字字符串
                            // 并将其插入至有序容器 winning_numbers 中
                            winning_numbers.insert(tmp_str);
                        }
                    }

                    for (size_t i = 0; i < num_of_intersected_numbers; ++i)
                    {
                        cards_numbers[card_id + i] +=
                            cards_numbers[card_id - 1];
                    }

                    // 统计所有 card(id)的数量
                    // 包括 original card(id) 和 副本 copies card(id)
                    sum_numbers_of_cards += cards_numbers[card_id - 1];
                }

                return sum_numbers_of_cards;
            };

        std::string              line;
        std::vector<std::string> input_file_lines;
        while (std::getline(input_file, line))
        {
            input_file_lines.push_back(line);
            // break;
        }
        input_file.close();

        result = getPointsOfOneCard(input_file_lines);
    }

    // 获取程序结束执行的时间点
    auto end_time = std::chrono::high_resolution_clock::now();
    // 计算时间差
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - start_time);

    std::cout << "   finished! " << std::endl;
    std::cout << "Result: " << result << std::endl; // right answer: 9236992

    // 输出程序运行时间
    std::cout << "Time taken by this code: " << duration.count()
              << " microseconds" << std::endl;

    return EXIT_SUCCESS;
}