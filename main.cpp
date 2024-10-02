#include <iostream>
#include <bitset>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <list>
#include <tuple>

/// \brief Домашнее задание по курсу C++ Proffessional от OTUS

namespace sfinae
{
    namespace ip_printer {
        /**
         * \brief Перегрузка функции для целочисленного кодирования IP-адреса. Декодируется в IP-адрес побайтово.
         *
         * \param address IP-адрес в виде целого числа
         */
        template <typename T>
        auto print_ip(T address, long) -> decltype(abs(address))
        {
            std::bitset<sizeof(T)*8> bs{(long long unsigned int)address};
            uint16_t base, num;
            for (size_t i = bs.size() - 1; i >= 7; i -= 8)
            {
                base = 1;
                num = 0;
                for (unsigned int j = i - 7; j <= i; j++)
                {
                    num += bs[j] * base;
                    base *= 2;
                }
                std::cout << num;
                if(i != 7)
                    std::cout << ".";
                else
                    break;
            }
            std::cout << std::endl;
            return 0;
        }

        
        /**
         * \brief Перегрузка функции для строкового формата IP-адреса. Выводится в функции как есть
         *
         * \param address IP-адрес в виде строки
         */
        template <typename T>
        auto print_ip(T address, int) -> decltype(address.c_str()) 
        {
            std::cout << address << std::endl;
            return address.c_str();
        }

        /**
         * \brief Перегрузка функции для контейнерного формата IP-адреса. Элементы контейнера выводятся по очереди через точку.
         *
         * \param address IP-адрес в виде контейнера std::list или std::vector
         */
        template <typename T>
        auto print_ip(T address, long) -> decltype(address.size())
        {
            size_t cont_s = address.size(), i = 0;
            for(auto item:address)
            {
                std::cout << item;
                if(i < cont_s - 1)
                    std::cout << ".";
                i++;
            }
            std::cout << std::endl;
            return address.size();
        }

        // tuple
        template <typename T = std::tuple<int>>
        auto print_ip(T address, int) -> decltype(std::get<0>(address))
        {
            std::cout << "Shnyaga: " << address << std::endl;
            // size_t cont_s = address.size(), i = 0;
            // for(auto item:address)
            // {
            //     std::cout << item;
            //     if(i < cont_s - 1)
            //         std::cout << ".";
            //     i++;
            // }
            // std::cout << std::endl;
            // return address.size();
        }
    }

    /*!
    \brief Выводит входное входную переменную в виде IP адреса. 
    \param[in] address Входной объект, содержащий IP адрес: целочисленный тип(побитовое преобразование), строка(вывод как есть), контейнер(вывод элементов)
    */
    template <typename T>
    auto print_ip(T address)
    {
        ip_printer::print_ip(address, int{});
    }
}

int main()
{
    sfinae::print_ip( int8_t{-1}); // 255
    sfinae::print_ip( int16_t{0} ); // 0.0
    sfinae::print_ip( int32_t{2130706433} ); // 127.0.0.1
    sfinae::print_ip( int64_t{8875824491850138409} );// 123.45.67.89.101.112.131.41
    sfinae::print_ip(std::string{"Hello, World!"}); // Hello, World!
    sfinae::print_ip( std::vector<int>{100, 200, 300, 400} ); // 100.200.300.400
    sfinae::print_ip( std::list<short>{400, 300, 200, 100} ); // 400.300.200.100

    return 0;
}
