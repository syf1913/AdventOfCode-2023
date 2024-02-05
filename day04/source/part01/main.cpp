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
    /*
    // method 1
    {
        auto getPointsOfOneCard = [](const std::string &line) {
            uint32_t    card_id;
            uint32_t    tmp_int;
            uint32_t    points{0};
            std::string tmp_str;

            std::vector<uint32_t> winning_numbers;
            std::vector<uint32_t> my_numbers;

            std::istringstream i_str_steam(line);

            i_str_steam >> tmp_str;
            i_str_steam >> card_id;
            i_str_steam >> tmp_str;

            // std::cout << "\nwinning_numbers: ";
            for (size_t i = 0; i < 10; ++i)
            {
                i_str_steam >> tmp_int;
                winning_numbers.push_back(tmp_int);
                // debug
                // std::cout << tmp_int << " ";
            }
            // debug
            // std::cout << std::endl;

            i_str_steam >> tmp_str;

            // std::cout << "my_numbers: ";
            for (size_t i = 0; i < 25; ++i)
            {
                i_str_steam >> tmp_int;
                my_numbers.push_back(tmp_int);

                // debug
                // std::cout << tmp_int << " ";
            }
            // std::cout << std::endl;

            // 对两个容器进行排序
            std::sort(winning_numbers.begin(), winning_numbers.end());
            std::sort(my_numbers.begin(), my_numbers.end());

            std::vector<uint32_t> intersected_numbers;
            std::set_intersection(winning_numbers.cbegin(),
                                  winning_numbers.cend(),
                                  my_numbers.cbegin(),
                                  my_numbers.cend(),
                                  std::back_inserter(intersected_numbers));

            //   debug
            // std::cout << "intersected_numbers: ";
            // for (auto &inter_number : intersected_numbers)
            // {
            //     std::cout << inter_number << " ";
            // }
            // std::cout << std::endl;

            if (intersected_numbers.size() == 0)
            {
                //
                return points;
            }
            else
            {
                points = static_cast<uint32_t>(
                    std::pow(uint32_t{2}, intersected_numbers.size() - 1));
                return points;
            }
        };

        std::string line;
        while (std::getline(input_file, line))
        {
            result += getPointsOfOneCard(line);
            // break;
        }
    }
    */

    // method 2
    {
        auto getPointsOfOneCard = [](const std::string &line) {
            uint32_t              card_id;
            uint32_t              tmp_int;
            uint32_t              points{0};
            std::string           tmp_str;
            std::set<std::string> winning_numbers;

            std::istringstream i_str_steam(line);

            i_str_steam >> tmp_str;
            i_str_steam >> card_id;
            // i_str_steam >> tmp_str;
            i_str_steam.ignore(1);

            bool     is_separated = false;
            uint32_t num_of_intersected_numbers{0};

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
                    if (winning_numbers.find(tmp_str) != winning_numbers.end())
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

            if (num_of_intersected_numbers == 0)
            {
                // 如果没有相同的数字字符串，则返回0
                return uint32_t{0};
            }
            else
            {
                // 将 1 左移 (num_of_intersected_numbers - 1) 位
                // 左移操作相当于将1乘以2的(num_of_intersected_numbers - 1)次方

                // return static_cast<uint32_t>(
                //     (1 << (num_of_intersected_numbers - 1)));

                return (uint32_t{1} << (num_of_intersected_numbers - 1));
            }
        };

        std::string line;
        while (std::getline(input_file, line))
        {
            result += getPointsOfOneCard(line);
            // break;
        }
    }

    input_file.close();

    // 获取程序结束执行的时间点
    auto end_time = std::chrono::high_resolution_clock::now();
    // 计算时间差
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - start_time);

    std::cout << "   finished! " << std::endl;
    std::cout << "Result: " << result << std::endl; // right answer: 23028

    // 输出程序运行时间
    std::cout << "Time taken by this code: " << duration.count()
              << " microseconds" << std::endl;

    return EXIT_SUCCESS;
}