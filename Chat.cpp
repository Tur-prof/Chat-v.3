#include <iostream>
#include "Chat.h"
#include "Demo_perms.h"
#include "BadLoginRegistr.h"
#include "BadLoginMessage.h"


#if defined(_WIN32) 
#define PLATFORM_NAME "windows 32-bit" // Windows 32-bit
#elif defined(_WIN64)
#define PLATFORM_NAME "windows 64-bit" // Windows 64-bit
#elif defined(__ANDROID__)
#define PLATFORM_NAME "android"   // Android 
#elif defined(__linux__)
#define PLATFORM_NAME "linux"     // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos и другие
#elif TARGET_OS_IPHONE == 1
#define PLATFORM_NAME "ios"       // Apple iOS
#elif TARGET_OS_MAC == 1
#define PLATFORM_NAME "osx"       // Apple OSX
#else
#define PLATFORM_NAME  "OS not detected" // ОС не распознана
#endif


//Функция установки включения программы
void Chat::start() {
    _chatOn = true;
    std::string login;
    std::string password;
    user_list.open("users.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    fs::permissions("users.txt", fs::perms::owner_all);
    if (!user_list.is_open())
    {
        std::cout << "File openning error!" << std::endl;
    }
    else
    {
        fs::permissions("users.txt", fs::perms::owner_all);
        while (getline(user_list, login, ' ') && getline(user_list, password))
        {
            _active_user[login] = false;
        }
    }
    user_list.close();
    std::cout << "\033[1;31m" << PLATFORM_NAME << std::endl;
}

//Функция возврата включенного состояния программы
bool Chat::chatOn() const
{
    return _chatOn;
}

//Функция установки авторизации пользователя
void Chat::loginOn()
{
    _checkLogin = true;
}

//Функция возврата режима авторизованного пользователя
bool Chat::checkLogin() const
{
    return _checkLogin;
}

//Функция вывода главного меню
void Chat::showLoginMenu()
{
    char operation;

    while (_chatOn && !_checkLogin)
    {
        std::cout << "\033[1;32m" << "(1) Login" << std::endl;
        std::cout << "(2) Regisration" << std::endl;
        std::cout << "(0) Exit" << std::endl;
        std::cout << ">> " << std::endl;
        std::cin >> operation;

        switch (operation)
        {
        case '1':
            if (login())
            {
                _checkLogin = false;
            }
            break;
        case '2':
            registrUser();
            break;
        case '0':
            _chatOn = false;
            break;
        default:
            std::cout << "\033[31m" << "Choose 1 or 2..." << std::endl;
            break;
        }
    }
}

//Функция авторизации пользователя
bool Chat::login()
{
    std::string check_login;
    std::string check_password;
    std::string login;
    std::string password;
    bool is_found = false;
    char operation;

    while (!is_found)
    {
        std::cout << "login: ";
        std::cin >> check_login;
        std::cout << "password: ";
        std::cin >> check_password;
        user_list.open("users.txt", std::fstream::in | std::fstream::out | std::fstream::app);
        if (!user_list.is_open())
        {
            std::cout << "File openning error!" << std::endl;
        }
        else
        {
            while (getline(user_list, login, ' ') && getline(user_list, password))
            {
                if (check_login == login && check_password == password)
                {
                    std::cout << "\033[31m" << "LOGIN SUCCESS" << std::endl;
                    _user_online = check_login;
                    _active_user[_user_online] = true;
                    loginOn();
                    user_list.close();
                    return is_found;
                }
            }
        }
        if (!is_found) {
            std::cout << "\033[31m" << "LOGIN OR PASSWORD FAILED" << std::endl;
            std::cout << "\033[32m" << "Press (0) for exit or press any button to try again" << std::endl;
            user_list.close();
            std::cin >> operation;
            if (operation == '0') {
                break;
            }
        }
    }
    return is_found;
}

//Функция регистрации пользователя
void Chat::registrUser()
{
    std::string login;
    std::string new_login;
    std::string password;
    std::cout << "new login: ";
    std::cin.ignore();
    std::cin >> new_login; 
    try
    {
        if (new_login == "All")
        {
            throw BadLoginRegistr();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return;
    }
    try
    {
        user_list.open("users.txt", std::fstream::in | std::fstream::out | std::fstream::app);
        if (!user_list.is_open())
        {
            std::cout << "File openning error!" << std::endl;
        }
        else
        {
            while (getline(user_list, login, ' ') && getline(user_list, password))
            {
                if (new_login == login)
                {
                    throw BadLoginRegistr();
                }
            }
            user_list.close();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return;
    }
    user_list.open("users.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    if (!user_list.is_open())
    {
        std::cout << "File openning error!" << std::endl;
    }
    else
    {
        std::cout << "password: ";
        std::cin.ignore();
        std::cin >> password;
        User user(new_login, password);
        user_list << user << "\n";
        std::cout << "\033[1;31m" << "REGISTRATION COMPLETED SUCCESSFULLY!" << std::endl;
    }
    _active_user[new_login] = false;
    user_list.close();
}

//Функция вывода меню пользователя
void Chat::showUserMenu()
{
    char operation;
    std::cout << "\033[32m" << "Hi, " << "\033[37m" << _user_online << std::endl;

    while (_checkLogin) {
        std::cout << "\033[36m" << "Menu: (1) Chat Public |" << "\033[35m" << " (2) Chat Private |" << "\033[37m" << " (3) Users |" << "\033[32m" << " (0) Logout";

        std::cout << std::endl
            << "\033[37m" << ">> ";
        std::cin >> operation;

        switch (operation)
        {
        case '1':
            showMenuChatPublic();
            break;
        case '2':
            showMenuChatPrivate();
            break;
        case '3':
            showUsersList();
            break;
        case '0':
            _active_user.at(_user_online) = false;
            _checkLogin = false;
            break;
        default:
            std::cout << "\033[31m" << "Choose 1 or 2 or 3..." << std::endl;
            break;
        }
    }
    std::cout << "\033[36m" <<  "--------------" << std::endl;
}

//Функция вывода меню публичного чата
void Chat::showMenuChatPublic()
{
    bool chat_public_on = true;
    char operation;
    std::cout << "\033[36m" << "--- Chat Public ---" << std::endl;
    while (chat_public_on) {
        std::cout << "\033[36m" << "Menu:  (1) ShowChat |  (2) Add Message |  (0) Back"
            << std::endl
            << ">> ";
        std::cin >> operation;
        
        switch (operation)
        {
        case '1':
            showChatPublic();
            break;
        case '2':
            addMessagePublic();
            break;
        case '0':
            chat_public_on = false;
            break;
        default:
            std::cout << "\033[31m" << "Choose 1 or 2..." << std::endl;
            break;
        }
    }
    std::cout << "\033[36m" << "--------------" << std::endl;
}

//Функция вывода меню приватного чата
void Chat::showMenuChatPrivate()
{
    bool chat_private_on = true;
    std::string from;
    std::string to;
    std::string text;
    char operation;
    
    while (chat_private_on) {
        std::cout << "\033[35m" << "--- Chat Private ---" << std::endl;
        std::cout << "Menu: (1) ShowChat | (2) Add Message | (0) Back"
            << std::endl
            << ">> ";
        std::cin >> operation;
        int _found = 0;

        switch (operation)
        {
        case '1':
            showChatPrivate();
            break;
        case '2':
            addMessagePrivate();
            break;
        case '0':
            chat_private_on = false;
            break;
        default:
            std::cout << "\033[31m" << "Choose 1 or 2..." << std::endl;
            break;
        }
    }
    std::cout << "\033[35m" << "--------------" << std::endl;
}

//Функция вывода списка пользователей
void Chat::showUsersList()
{
    std::string login;
    std::string password;
    std::cout << "\033[37m" << "--- Users ---" << std::endl;
    for (const auto& u : _active_user)
    {
        if (u.first == _user_online) {
            std::cout << "\033[37m" << u.first << "\033[32m" << " online" << "\033[37m" << " (you)" << std::endl;
        }
        else if (u.second == true) {
            std::cout << "\033[37m" << u.first << "\033[32m" << " online" << std::endl;
        }
        else {
            std::cout << "\033[37m" << u.first << "\033[31m" << " offline" << std::endl;
        }
    }
    std::cout << "\033[37m" << "--------------" << std::endl;
}

//Функция вывода публичного чата
void Chat::showChatPublic()
{
    std::string from;
    std::string to;
    std::string text;
    msg_file.open("messages.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    if (!msg_file.is_open())
    {
        std::cout << "File openning error!" << std::endl;
    }
    else
    {
        fs::permissions("messages.txt", fs::perms::owner_all);
        while (getline(msg_file, from, ' ') && getline(msg_file, to, ' ') && getline(msg_file, text))
        {
            if (to == "All")
            {
                std::cout << "From " << from << " To " << to << " : " << text;
                std::cout << std::endl;
            }
        }
    }
    msg_file.close();
}

//Функция добавления публичных сообщений в файл сообщений
void Chat::addMessagePublic() {
    std::string text;
    msg_file.open("messages.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    if (!msg_file.is_open())
        {
            std::cout << "File openning error!" << std::endl;
        }
    else
        {
            std::cout << "Text: ";
            std::cin.ignore();
            std::getline(std::cin, text);
            Message msg(_user_online, "All", text);
            msg_file << msg << "\n";
        }
    msg_file.close();
}

//Функция вывода приватного чата
void Chat::showChatPrivate()
{
    std::string from;
    std::string to;
    std::string text;
    msg_file.open("messages.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    if (!msg_file.is_open())
    {
        std::cout << "File openning error!" << std::endl;
    }
    else
    {
        while (getline(msg_file, from, ' ') && getline(msg_file, to, ' ') && getline(msg_file, text))
        {
            if (to == _user_online || from ==_user_online && to != "All")
            {
                std::cout << "From " << from << " To " << to << " : " << text;
                std::cout << std::endl;
            }
        }
    }
    msg_file.close();
}

//Функция добавления приватных сообщений в файл сообщений
void Chat::addMessagePrivate() {
    std::string login;
    std::string password;
    std::string from;
    std::string to;
    std::string text;
    int _found = 0;
    msg_file.open("messages.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    if (!msg_file.is_open())
    {
        std::cout << "File openning error!" << std::endl;
    }
    else
    {
        std::cout << "To: ";
        std::cin.ignore();
        std::cin >> to;
        try
        {
            for (const auto& u : _active_user)
            {
                if (u.first == to)
                {
                    _found = 1;
                }
            }
            if (_found == 0) {
                throw BadLoginMessage();
            }
            else {
                std::cout << "Text: ";
                std::cin.ignore();
                std::getline(std::cin, text);
                Message msg(_user_online, to, text);
                msg_file << msg << "\n";
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
    msg_file.close();
    user_list.close();
}



        
            