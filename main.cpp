#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

bool comp(std::vector<int> &a, std::vector<int> &b)
{
    if(a[0] < b[0])
        return false;
    else if(a[0] > b[0])
        return true;
    
    if(a[1] < b[1])
        return false;
    else if(a[1] > b[1])
        return true;
    
    if(a[2] < b[2])
        return false;
    else if(a[2] > b[2])
        return true;
    
    if(a[3] < b[3])
        return false;
    return true;
}

int main()
{
    try
    {
        std::vector<std::vector<int> > ip_pool;
        std::vector<std::string> buff1;
        std::vector<int> buff2;
        buff2.resize(4);

        for(std::string line; std::getline(std::cin, line);)
        {
            if(line.length() == 0)
                break;
            std::vector<std::string> v = split(line, '\t');
            buff1 = std::move(split(v.at(0), '.'));
            for(int i = 0; i < 4; i++)
                buff2[i] = std::stoi(buff1[i]);
            ip_pool.push_back(buff2);
        }

        // Reverse lexicographically sort
        std::sort(ip_pool.begin(), ip_pool.end(), comp);

        for(auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
        {
            for(auto ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
            {
                if (ip_part != ip->cbegin())
                {
                    std::cout << ".";

                }
                std::cout << *ip_part;
            }
            std::cout << std::endl;
        }

        for(auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
        {
            if(*ip->cbegin() == 1)
            {
                for(auto ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
                {
                    if (ip_part != ip->cbegin())
                    {
                        std::cout << ".";

                    }
                    std::cout << *ip_part;
                }
                std::cout << std::endl;
            }
        }

        for(auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
        {
            if(*ip->cbegin() == 46 && *(ip->cbegin()+1) == 70)
            {
                for(auto ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
                {
                    if (ip_part != ip->cbegin())
                    {
                        std::cout << ".";

                    }
                    std::cout << *ip_part;
                }
                std::cout << std::endl;
            }
        }

        for(auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
        {
            if(*ip->cbegin() == 46 || *(ip->cbegin()+1) == 46 || *(ip->cbegin()+2) == 46 || *(ip->cbegin()+3) == 46)
            {
                for(auto ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
                {
                    if (ip_part != ip->cbegin())
                    {
                        std::cout << ".";

                    }
                    std::cout << *ip_part;
                }
                std::cout << std::endl;
            }
        }

        
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
