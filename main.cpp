#include <iostream>
#include <memory>
#include "controller.cpp"

class IGUIView
{
public:
    IGUIView(){};
    virtual void create_document_btn()=0;
    virtual void import_document_btn(std::string filename)=0;
    virtual void export_document_btn(std::string filename)=0;
    virtual void create_gp_btn()=0;
    virtual void remove_gp_btn(size_t i)=0;
    virtual void render_gp(GraphPrimitive gp)=0;
    virtual void render_all_gp()=0;
};

class GUIView : public IGUIView
{
    std::unique_ptr<Controller> controller_ptr;
    std::shared_ptr<Workspace> ws_ptr;
public:
    GUIView()
    {
        controller_ptr = std::make_unique<Controller>();
        ws_ptr = controller_ptr->get_ws_ptr();
    }
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

    void render_gp(GraphPrimitive gp){std::cout << "Rendering graphic primitive\n" << gp.to_string() << std::endl;}

    void render_all_gp()
    {
        auto gp_list = ws_ptr->get_gp_list();
        for(auto gp : gp_list)
        {
            render_gp(gp);
        }
    }
};

int main()
{
    std::unique_ptr<IGUIView> window_ptr(new GUIView);
    while(true)
        window_ptr->render_all_gp();
    return 0;
}
