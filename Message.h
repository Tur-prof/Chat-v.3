#pragma once
#include <string>

class Message
{
    std::string _from;
    std::string _to;
    std::string _text;

public:
    Message(const std::string& from, const std::string& to,const std::string& text);

    friend std::istream& operator >> (std::istream&, Message&);
    friend std::ostream& operator << (std::ostream&, const Message&);

    ~Message() = default;
};


