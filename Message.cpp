#include "Message.h"
#include <iostream>
#include <fstream>

//Создание сообщения с заданными параметрами
Message::Message(const std::string& from, const std::string& to, const std::string& text) : _from(from), _to(to), _text(text)
{
}

std::istream& operator >> (std::istream& in, Message& msg)
{
    in >> msg._from >> msg._to >> msg._text;
    return in;
}

std::ostream& operator << (std::ostream& out, const Message& msg)
{
    out << msg._from << " " << msg._to << " " << msg._text;
    return out;
}
