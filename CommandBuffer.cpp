#include "CommandBuffer.h"

void ConsoleBuffer::push_to_buffer(std::string cmd)
{
    buffer.push_back(cmd);
}

void ConsoleBuffer::output_buffer(std::string prefix="")
{
    if(buffer.size() == 0)
        return;
    std::cout << prefix << ": ";
    for (unsigned int i = 0; i < buffer.size(); i++)
    {
        std::cout << buffer[i];
        if(i < buffer.size() - 1)
            std::cout << ", ";
    }
    std::cout << std::endl;
    buffer.clear();
}

int ConsoleBuffer::size()
{
    return buffer.size();
}


LogfileBuffer::~LogfileBuffer()
{
    out.close();
}

void LogfileBuffer::push_to_buffer(std::string cmd)
{
    if(buffer.size() == 0)
        first_cmd = std::time(0);
    buffer.push_back(cmd);
}

void LogfileBuffer::output_buffer(std::string prefix="")
{
    if(buffer.size() == 0)
        return;
    out.open(prefix + std::to_string(first_cmd) + std::string(".log"));
    out << prefix << ": ";
    for (unsigned int i = 0; i < buffer.size(); i++)
    {
        out << buffer[i];
        if(i < buffer.size() - 1)
            out << ", ";
    }
    out << std::endl;
    out.close();
    buffer.clear();
}

int LogfileBuffer::size()
{
    return buffer.size();
}