
#include <iostream>
#include <map>
#include <set>
#include <memory>
#include <cassert>
#include <vector>
#include <tuple>

template <class T, T def>
class Matrix  // Класс разреженной матрицы, которая является Proxy для std::map
{
    class IndexProxy  //  Proxy-класс матрицы для реализации двойных скобок
    {
        std::shared_ptr<std::map<std::pair<int, int>, T>> matrix;
        int index_i;
    public:
        IndexProxy(std::shared_ptr<std::map<std::pair<int, int>, T>> m) {matrix = m;}
        void set_i(int i) {index_i = i;}
        
        T &operator[](int index_j)
        {
            std::pair<int, int> p(index_i, index_j);
            if(matrix->find(p) == matrix->end())
                (*matrix)[p] = def;
            return (*matrix)[p];
        }
    };

    class iterator : public std::tuple<int, int, T>  // Proxy-класс для итератора std::map для возврата кортежа при прохождении по циклу for
    {
    public:
        typename std::map<std::pair<int, int>, T>::iterator internal_iterator;

        iterator(typename std::map<std::pair<int, int>, T>::iterator i)
        {
            internal_iterator = i;
        }

        void operator++()
        {
            internal_iterator++;
        }

        bool operator!=(Matrix::iterator i)
        {
            return internal_iterator != i.internal_iterator;
        }

        std::tuple<int, int, T> operator*()
        {
            return std::make_tuple(internal_iterator->first.first, internal_iterator->first.second, internal_iterator->second);
        }

    };

    std::shared_ptr<std::map<std::pair<int, int>, T>> matrix;
    std::unique_ptr<IndexProxy> ip;
    std::shared_ptr<std::set<std::pair<int, int>>> keys;

    void clear_defaults()
    {
        std::vector<std::pair<int, int>> del_keys;
        for(auto item : *matrix)
            if(item.second == def)
                del_keys.push_back(item.first);
        for(auto key : del_keys)
            matrix->erase(key);
    }
public:
    Matrix()
    {
        matrix = std::make_shared<std::map<std::pair<int, int>, T>>();
        ip = std::make_unique<IndexProxy>(matrix);//, keys);
        keys = std::make_shared<std::set<std::pair<int, int>>>();
    }

    IndexProxy &operator[](int i)
    {
        ip->set_i(i);
        return *ip;
    }

    int size()
    {
        clear_defaults();
        return matrix->size();
    }

    Matrix::iterator begin()
    {
        clear_defaults();
        auto it = Matrix::iterator(matrix->begin());
        std::get<0>(it) = it.internal_iterator->first.first;
        std::get<1>(it) = it.internal_iterator->first.second;
        std::get<2>(it) = it.internal_iterator->second;
        return it;
    }


    Matrix::iterator end()
    {
        clear_defaults();
        auto it = Matrix::iterator(matrix->end());
        return it;
    }

};

int main()
{
    // бесконечная матрица int заполнена значениями -1
    // Matrix<int, -1> matrix2;
    // assert(matrix2.size() == 0); // все ячейки свободны
    // auto a = matrix2[0][0];
    // assert(a == -1);
    // assert(matrix2.size() == 0);
    // matrix2[100][100] = 314;
    // assert(matrix2[100][100] == 314);
    // assert(matrix2.size() == 1);
    // for(auto c: matrix2)
    // {
    //     int x;
    //     int y;
    //     int v;
    //     std::tie(x, y, v) = c;
    //     std::cout << x << y << v << std::endl;
    // }
    // std::cout << std::endl;

    Matrix<int, 0> matrix;
    for (int i = 0; i <= 9; i++)
    {
        matrix[i][i] = i;
        matrix[i][9-i] = 9-i;
    }

    for (int i = 1; i <= 8; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << matrix.size() << std::endl;
    
    for(auto c: matrix)
    {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << "M[" << x << "][" << y << "] = " << v << std::endl;
    }

    ((matrix[100][100] = 314) = 0) = 217;
    std::cout << matrix[100][100] << std::endl;
    return 0;
}
