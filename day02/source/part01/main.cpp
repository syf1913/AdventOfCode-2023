#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

/**------------------------------------------------------------------------
 * *                                INFO
 *   1. 使用 istringstream::operator>>() 来读取文件中的特定的数字和字符串；
 *   2. 使用Lambda表达式来快速 step 1 的操作，当然也可以使用普通函数来实现；
 *   3. Lambda表达式能够作为std::function 传递给std::accumulate()来实现累加
 *      操作，从而省略 while 循环，但是采用此方法，程序的运行时间略高于普通
 *      while循环，因为前者同样需要一个while循环，来存储inputfile的所有行内容，
 *      至一个string容器中；
 *------------------------------------------------------------------------**/

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

    // method 1:
    {
        // 使用while循环实现累加操作
        // 定义一个读取特定数字和字符串的Lambda表达式
        auto getGameIdAndSpecialString = [](const std::string &line) {
            uint32_t    gameId;
            uint32_t    tmp_int;
            std::string tmp_str;

            std::istringstream i_str_steam(line);

            i_str_steam >> tmp_str;
            i_str_steam >> gameId;
            i_str_steam.ignore(1);

            while (i_str_steam >> tmp_int)
            {
                i_str_steam >> tmp_str;
                if ((tmp_str[0] == 'r' && tmp_int > 12) ||
                    (tmp_str[0] == 'g' && tmp_int > 13) ||
                    (tmp_str[0] == 'b' && tmp_int > 14))
                {
                    return uint32_t(0);
                }
            }

            return gameId;
        };

        std::string line;
        while (std::getline(input_file, line))
        {
            result += getGameIdAndSpecialString(line);
            // break;
        }
    }

    // method 2:
    {
        // 使用std::accumulate()函数实现累加操作
        // 定义一个读取特定数字和字符串的Lambda表达式
        // auto getGameIdAndSpecialString = [](auto              &sum,
        //                                     const std::string &line) {
        //     uint32_t    gameId;
        //     uint32_t    tmp_int;
        //     std::string tmp_str;

        //     std::istringstream i_str_steam(line);

        //     i_str_steam >> tmp_str;
        //     i_str_steam >> gameId;
        //     i_str_steam.ignore(1);

        //     while (i_str_steam >> tmp_int)
        //     {
        //         i_str_steam >> tmp_str;
        //         if ((tmp_str[0] == 'r' && tmp_int > 12) ||
        //             (tmp_str[0] == 'g' && tmp_int > 13) ||
        //             (tmp_str[0] == 'b' && tmp_int > 14))
        //         {
        //             // return uint32_t{0};
        //             return sum;
        //         }
        //     }

        //     // return gameId;
        //     return sum + gameId;
        // };

        // std::string              line;
        // std::vector<std::string> input_file_lines;
        // while (std::getline(input_file, line))
        // {
        //     input_file_lines.push_back(line);
        //     // break;
        // }

        // 使用cbegin()和cend()，确保input_file_lines不会被修改
        // result = std::accumulate(input_file_lines.cbegin(),
        //                          input_file_lines.cend(),
        //                          uint32_t(0),
        //                          getGameIdAndSpecialString);
    }

    input_file.close();

    // 获取程序结束执行的时间点
    auto end_time = std::chrono::high_resolution_clock::now();
    // 计算时间差
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - start_time);

    std::cout << "   finished ! " << std::endl;
    std::cout << "Result: " << result << std::endl; // right answer: 2285

    // 输出程序运行时间
    std::cout << "Time taken by this code: " << duration.count()
              << " microseconds" << std::endl;

    return EXIT_SUCCESS;
}