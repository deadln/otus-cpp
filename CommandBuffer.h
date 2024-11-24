#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

class CommandBufferI
{
public:
    virtual void push_to_buffer(std::string cmd) = 0;
    virtual void output_buffer(std::string prefix) = 0;
    virtual int size() = 0;
};

class ConsoleBuffer : public CommandBufferI
{
    std::vector<std::string> buffer;
public:
    ConsoleBuffer() = default;
    void push_to_buffer(std::string cmd) override;
    void output_buffer(std::string prefix) override;
    int size() override;
};

class LogfileBuffer : public CommandBufferI
{
    std::vector<std::string> buffer;
    std::ofstream out;
    time_t first_cmd;
public:
    LogfileBuffer() = default;
    ~LogfileBuffer();
    void push_to_buffer(std::string cmd) override;
    void output_buffer(std::string prefix) override;
    int size() override;
};