#ifndef DIAGRAMS_VIEW_CPP
#define DIAGRAMS_VIEW_CPP
#include "controller.cpp"
#include <memory>

class GUIView
{
    std::unique_ptr<Controller> controller_ptr = std::make_unique<Controller>();
public:
    void create_document_btn()
    {
        controller_ptr->create_document();
    }

    void import_document_btn(std::string filename)
    {
        controller_ptr->import_document(filename);
    }

    void export_document_btn(std::string filename)
    {
        controller_ptr->export_document(filename);
    }

    void create_gp_btn(){
        controller_ptr->add_primitive();
    }

    void remove_gp_btn(size_t i){
        controller_ptr->remove_primitive(i);
    }


};
#endif