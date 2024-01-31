#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// 包含 isdigit 的头文件
#include <cctype>

void extractNumbers(const std::string &line, std::vector<int> &digits_in_line)
{
    // 创建一个有序映射
    std::map<int, std::string> wordToNumber = {
        {1, "one"  },
        {2, "two"  },
        {3, "three"},
        {4, "four" },
        {5, "five" },
        {6, "six"  },
        {7, "seven"},
        {8, "eight"},
        {9, "nine" },
    };

    for (auto char_iter = line.begin(); char_iter != line.end(); ++char_iter)
    {
        if (std::isdigit(*char_iter))
        {
            std::string char_to_string_tmp = std::string(1, *char_iter);
            int         string_to_int_tmp  = std::stoi(char_to_string_tmp);

            digits_in_line.push_back(string_to_int_tmp);
            continue;
        }
        else if (std::isalpha(*char_iter))
        {
            // 计算当前字符在字符串中的索引
            std::size_t index = std::distance(line.begin(), char_iter);

            for (auto map_iter = wordToNumber.begin();
                 map_iter != wordToNumber.end();
                 ++map_iter)
            {
                // c++11标准中，substr() 无需显示进行越界检查
                if ((line.substr(index, map_iter->second.size())) ==
                    map_iter->second)
                {
                    digits_in_line.push_back(map_iter->first);
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    std::string input_filename;
    if ((argc != 2))
    {
        std::cout << "Usage: " << argv[0] << " <input_file>" << std::endl;
        exit(EXIT_FAILURE);
    }

    input_filename = argv[1];
    // std::cout << "Input file: " << input_file << std::endl;

    std::ifstream input_file(input_filename);

    // 检查文件是否正确打开
    if (!input_file.is_open())
    {
        std::cerr << "Error opening file: " << input_filename << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Get contents of the file ...";

    std::string      line;
    int              result = 0;
    std::vector<int> digits_in_line;
    while (std::getline(input_file, line))
    {
        // std::cout << line << std::endl;

        // 提取每行文本中的所有数字
        extractNumbers(line, digits_in_line);

        // 检查包含文本中数字的容器是否为空
        if (!digits_in_line.empty())
        {
            // 将提取出的数字组成一个两位数
            result += (digits_in_line.front() * 10 + digits_in_line.back());
        }
        else
        {
            std::cout << "      \n No digits found in line: " << line
                      << std::endl;
            continue;
        }

        digits_in_line.clear();
    }

    input_file.close();

    std::cout << "\nFinished ... " << std::endl;
    std::cout << "Result: " << result << std::endl; // right answer: 54591

    return 0;
}