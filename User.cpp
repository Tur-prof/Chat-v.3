#include "User.h"
#include <fstream>

//Создание пользователя с заданными параметрами
User::User(const std::string& login, const std::string& password) : _login(login), _password(password)
{
}

std::ostream& operator << (std::ostream& out, const User& u)
{
    out << u._login << " " << u._password;
    return out;
}