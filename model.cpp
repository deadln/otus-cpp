#ifndef DIAGRAMS_MODEL_CPP
#define DIAGRAMS_MODEL_CPP
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <string>


class GraphPrimitive
{
    std::string data;
public:
    GraphPrimitive() = default;
    GraphPrimitive(std::string gp_str) {data = gp_str;}
    std::string to_string() {return data;}
    void render() {}
};

class IWorkspace
{
public:
    IWorkspace(){};
    virtual void create_document()=0;
    virtual void import_document(std::string filename)=0;
    virtual void export_document(std::string filename)=0;
    virtual void add_primitive()=0;
    virtual void remove_primitive(size_t i)=0;
    virtual std::vector<GraphPrimitive> get_gp_list()=0;
};

class Workspace : public IWorkspace
{
    std::vector<GraphPrimitive> gp_ws;
    std::ifstream in;  // Поток in будем использовать для чтения
    std::ofstream out; // Поток out будем использовать для записи
public:
    Workspace() = default;
    void create_document()  // создание нового документа
    {
        gp_ws.clear();
    }

    void import_document(std::string filename)
    {
        in.open(filename);
        std::string s;
        while ( getline(in,s))
            gp_ws.push_back(std::move(GraphPrimitive(s)));
    }

    void export_document(std::string filename)
    {
        out.open(filename);
        for (auto gp : gp_ws)
        {
            out << gp.to_string() << "\n";
        }
    }

    void add_primitive()  // создание графического примитива
    {
        gp_ws.push_back(std::move(GraphPrimitive()));
    }

    void remove_primitive(size_t i)  // удаление графического примитива
    {
        gp_ws.erase(gp_ws.begin()+i);
    }
    
    std::vector<GraphPrimitive> get_gp_list() // TODO: сделать вовзрат ссылки на вектор (если это не ссылка сама по себе)
    {
        return gp_ws;
    }
};
#endif