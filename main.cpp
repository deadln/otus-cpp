#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "CommandBuffer.h"

int main(int argc, char *argv[])
{
    int N = atoi(argv[argc - 1]);
    int par_count = 0;  // Счётчик скобок
    std::unique_ptr<CommandBufferI> comm_buf = std::make_unique<ConsoleBuffer>();
    std::unique_ptr<CommandBufferI> file_log = std::make_unique<LogfileBuffer>();
    std::string prefix = "bulk";
    std::string open_par = "{";
    std::string close_par = "}";

    for(std::string line; std::getline(std::cin, line);)
    {
        if(line.length() == 0)
            break;
        if(line == open_par)
        {
            if(par_count == 0)
            {
                comm_buf->output_buffer(prefix);
                file_log->output_buffer(prefix);
            }
            par_count++;
        }
        else if(line == close_par)
        {
            if(par_count == 1)
            {
                comm_buf->output_buffer(prefix);
                file_log->output_buffer(prefix);
            }
            par_count--;
            continue;
        }
        else
        {
            comm_buf->push_to_buffer(line);
            file_log->push_to_buffer(line);
        }
            
        if(par_count == 0 && comm_buf->size() == N)
        {
            comm_buf->output_buffer(prefix);
            file_log->output_buffer(prefix);
        }
    }
    if(par_count == 0)
    {
        comm_buf->output_buffer(prefix);
        file_log->output_buffer(prefix);
    }
    return 0;
}
