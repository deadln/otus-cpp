#ifndef DIAGRAMS_CONTROLLER_CPP
#define DIAGRAMS_CONTROLLER_CPP
#include <memory>
#include "model.cpp"

class Controller
{
    std::shared_ptr<Workspace> ws_ptr = std::make_shared<Workspace>();
public:
    void create_document()
    {
        ws_ptr->create_document();
    }

    void import_document(std::string filename)
    {
        ws_ptr->import_document(filename);
    }

    void export_document(std::string filename)
    {
        ws_ptr->export_document(filename);
    }

    void add_primitive(){
        ws_ptr->add_primitive();
    }

    void remove_primitive(size_t i){
        ws_ptr->remove_primitive(i);
    }

    std::shared_ptr<Workspace> get_ws_ptr()
    {
        return ws_ptr;
    }
};
#endif