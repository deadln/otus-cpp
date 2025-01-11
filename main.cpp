#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cctype>
#include <set>
#include <map>
#include <unordered_map>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include "md5.hpp"

std::vector<std::string> inp_vec = {std::string(".")}, exl_vec;
std::set<std::string> exl_set;

std::vector<std::string> files;
std::unordered_map<std::string, std::vector<std::string>> files_hash;
int max_depth = 0;
long unsigned int min_size = 1;
int block_size = 1;
std::string mask;
boost::regex rx;
boost::smatch what;
int hash_type = 0;  // 0: std::hash; 1: md5;

namespace po = boost::program_options;

std::string str_to_lower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                [](unsigned char c){ return std::tolower(c); });
                return s;
}

// Рекурсивный поиск файлов и папок
void recursive_file_search(std::string dir_path, int depth)
{
    if(exl_set.find(dir_path) != exl_set.end())
    {
        return;
    }
    boost::filesystem::directory_iterator begin(dir_path);
    boost::filesystem::directory_iterator end;
    for (; begin != end; ++ begin) {

        boost::filesystem::file_status fs = 
        boost::filesystem::status(*begin);{
        switch (fs.type()) {
            case boost::filesystem::regular_file:
                if(boost::filesystem::file_size(boost::filesystem::path(*begin)) >= min_size && 
                (mask.length() == 0 || boost::regex_match( str_to_lower(begin->path().filename().string()), what, rx)))
                {
                    files.push_back(boost::filesystem::path(*begin).string());
                    files_hash[boost::filesystem::path(*begin).string()] = std::vector<std::string>();
                }
                break;
            case boost::filesystem::symlink_file:
                // Пропускаем
                break;
            case boost::filesystem::directory_file: 
                if(depth < max_depth)
                    recursive_file_search(boost::filesystem::path(*begin).string(), depth + 1);
                break;
            default: 
                break;
        }
    } /*for*/
} /*main*/
}

// Чтение блока из файла
std::string read_block_from_file(std::string file, long int pos)
{
    FILE * pFile;
    char c;
    std::string res;
    pFile=fopen(file.c_str(), "r");
    fseek(pFile, pos, SEEK_SET);
    for(int i = 0; i < block_size; i++)
    {
        c = fgetc(pFile);
        if(c == EOF)
        {
            if(i == 0)
            {
                fclose(pFile);
                return res;

            }
            res += '\0';
        }
        else
            res += c;
    }
    fclose(pFile);
    return res;
}

// Хэш от строки
std::string get_hash(std::string s)
{
    if(hash_type == 0)
    {
        std::hash<std::string> str_hash;
        return std::to_string(str_hash(s));
    }
    else if(hash_type == 1)
    {
        return MD5(s).hexdigest();
    }
    return 0;
}

// Сравнение файлов, оптимизированное по чтению с диска
bool compare_files(std::string file1, std::string file2)
{
    long unsigned int i = 0;
    std::string str_block, empty_str;
    std::string block1, block2, empty_hash = get_hash(empty_str);
    for(;;)
    {
        // Проверяем, имеется ли уже вычисленный блок
        if(i >= files_hash[file1].size())  // Вычисляем хэш при отсутствии
        {
            str_block = read_block_from_file(file1, i * block_size);
            files_hash[file1].push_back(get_hash(str_block));
        }

        // Проверяем, имеется ли уже вычисленный блок
        if(i >= files_hash[file2].size())  // Вычисляем хэш при отсутствии
        {
            str_block = read_block_from_file(file2, i * block_size);
            files_hash[file2].push_back(get_hash(str_block));
        }

        block1 = files_hash[file1][i];
        block2 = files_hash[file2][i];
        if(block1 == block2)
        {
            if(block1 == empty_hash)
                return true;
        }
        else
        {
            return false;
        }

        i++;
    }
}

int main(int argc, const char *argv[])
{
    try {
        po::options_description desc{"Options"};
        desc.add_options()
                ("help,h", "ShowHelp")
                ("input,i", po::value<std::vector<std::string> >()->multitoken(), "Input directories to check")
                ("exclude,e", po::value<std::vector<std::string> >()->multitoken(), "Exclude directories")
                ("depth,d", po::value<int>()->default_value(0), "Depth of bulk check")
                ("min_size,m", po::value<long unsigned int>()->default_value(1), "Minimal size of checked file (bytes)")
                ("mask", po::value<std::string>(), "Mask for checked files, which is register independent")
                ("size,s", po::value<int>(), "Block size (S)")
                ("hash", po::value<std::string>(), "Hash algorithm {std, md5}")
                ;

        po::variables_map vm;
        po::store(parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            std::cout << desc << '\n';
            return 0;
        }
        
        if (vm.count("depth"))
        {
            max_depth = vm["depth"].as<int>();
        }
        if (vm.count("min_size"))
        {
            min_size = vm["min_size"].as<long unsigned int>();
        }
        if (vm.count("mask"))
        {
            mask = vm["mask"].as<std::string>();
            std::transform(mask.begin(), mask.end(), mask.begin(),
                [](unsigned char c){ return std::tolower(c); });
            rx = mask;
        }
        if (vm.count("size"))
        {
            block_size = vm["size"].as<int>();
        }
            
        if (vm.count("hash"))
        {
            std::string h = vm["hash"].as<std::string>();
            if(h == "std")
                hash_type = 0;
            else if (h == "md5")
                hash_type = 1;
            else
            {
                std::cout << "Wrong hash option!\n";
                std::cout << desc << '\n';
                return 0;
            }
        }
        if (vm.count("input"))
        {
            inp_vec = vm["input"].as<std::vector<std::string> >();
            for(long unsigned int i = 0; i < inp_vec.size(); i++)
            {
                if(inp_vec[i][inp_vec[i].length() - 1] == '/')
                    inp_vec[i] = inp_vec[i].substr(0, inp_vec[i].length() - 1);
            }
        }
        if (vm.count("exclude"))
        {
            exl_vec = vm["exclude"].as<std::vector<std::string>>();
            for(long unsigned int i = 0; i < exl_vec.size(); i++)
            {
                if(exl_vec[i][exl_vec[i].length() - 1] == '/')
                    exl_vec[i] = exl_vec[i].substr(0, exl_vec[i].length() - 1);
                exl_set.insert(exl_vec[i]);
            }
        }
            
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    
    // Сперва находим все файлы
    for(auto dir : inp_vec)
    {
        recursive_file_search(dir, 0);
    }

    // Затем начинаем их сравнивать
    std::map<std::vector<std::string>, std::set<std::string>> result;
    for(size_t i = 0; i < files.size(); i++)
    {
        for(size_t j = i + 1; j < files.size(); j++)
        {
            if(compare_files(files[i], files[j]))
            {
                result[files_hash[files[i]]].insert(files[i]);
                result[files_hash[files[j]]].insert(files[j]);
            }
        }
    }

    // Вывод результатов
    for(auto h : result)
    {
        for(auto f : h.second)
        {
            std::cout << f << std::endl;
        }
        std::cout << "\n"; 
    }
    return 0;
}
