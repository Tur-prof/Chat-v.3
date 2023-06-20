#pragma once
#include <map>
#include <fstream>
#include <filesystem>
#include "Message.h"
#include "User.h"


class Chat {

public:
    Chat() = default;
    void start();
    bool chatOn() const;
    void loginOn();
    bool checkLogin() const;
    void showLoginMenu();
    void showUserMenu();

    bool login();
    void registrUser();
    void showMenuChatPrivate();
    void showMenuChatPublic();
    void showUsersList();
    void showChatPublic();
    void addMessagePublic();
    void showChatPrivate();
    void addMessagePrivate();

    ~Chat() = default;

    std::fstream user_list;
    std::fstream msg_file;

private:

    bool _chatOn = false;
    bool _checkLogin = false;
    std::string _user_online;

    std::map<std::string, bool> _active_user;
};