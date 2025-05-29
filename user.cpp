#include "user.h"
#include "log.h"
#include <fstream>
#include <utility>
#include "productList.h"
#include <sstream>
#include "cart.h"
#include <iostream>

UserBase::UserBase(std::string fileName) {
    hasInit = false;
    this->fileName = std::move(fileName);
    std::ifstream userFile(this->fileName);
    if (!userFile.is_open()) {
        // Log::printLog(LogString::FileNotFound, true);
        logger << true << LogString::FileNotFound;
        return;
    }
    std::string username, password, role;
    while (userFile >> username >> password >> role) {
        userStruct user = {"", UserBase::ERROR, Cart()};
        user.password = password;
        if (role == "admin") {
            user.role = ADMIN;
        }
        else {
            user.role = USER;
        }
        userFile.ignore();
        std::string cart;
        getline(userFile, cart);
        std::stringstream ss(cart);
        std::string name;
        int quantity;
        logger << cart << " :1145" << username;
        while (ss >> name >> quantity) {
            user.cart.addProduct(name, quantity);
        }
        userList[username] = user;
    }
    hasInit = true;
    userFile.close();
}

UserBase::userType UserBase::login(const std::string& username, const std::string& password) {
    if (userList.contains(username) && userList[username].password == password) {
        if (userList[username].role == ADMIN) {
            return ADMIN;
        }
        else if (userList[username].role == USER) {
            return USER;
        }
    }
    // Log::printLog(LogString::WrongPassword, true);
    logger << true << LogString::WrongPassword;
    return ERROR;
}

bool UserBase::changePassword(const std::string& username, const std::string& password) {
    if (userList[username].password == password) {
        // Log::printLog(LogString::PasswordSame, true);
        logger << true << LogString::PasswordSame;
        return false;
    }
    userList[username].password = password;
    writeToFile();
    // Log::printLog(LogString::ChangePasswordSuccess, true);
    logger << true << LogString::ChangePasswordSuccess;
    return true;
}

void UserBase::writeToFile() {
    std::ofstream userFile(this->fileName);
    if (!userFile.is_open()) {
        logger << true << LogString::FileNotFound;
        return;
    }
    for (const auto& [username, user] : userList) {
        userFile << username << " " << user.password << " ";
        if (user.role == ADMIN) {
            userFile << "admin";
        }
        else {
            userFile << "user";
        }
        userFile << std::endl;
        userFile << user.cart.toString();
        userFile << std::endl;
    }
    userFile.close();
}

bool User::registerUser(const std::string& username, const std::string& password) {
    if (userList.contains(username)) {
        logger << true << LogString::UserExist;
        return false;
    }
    const userStruct user = {password, USER, Cart()};
    userList[username] = user;
    writeToFile();
    logger << true << LogString::RegisterSuccess;
    return true;
}

void Admin::addProduct(const std::string& name, const std::string& description, const double price, const int inventory,
                       const std::string& category, ProductList& productList) {
    productList.addProduct(name, description, price, inventory, category);
}

void Admin::editProduct(const std::string& name, const std::string& description, const double price,
                        const int inventory,
                        const std::string& category, ProductList& productList) {
    productList.editProduct(name, description, price, inventory, category);
}

void Admin::deleteProduct(const std::string& name, ProductList& productList) {
    productList.deleteProduct(name);
}

void User::addCart(const std::string& username, const std::string& name, const int quantity) {
    if (userList[username].cart.hasProduct(name)) {
        logger << false << LogString::CartExist;
        std::string op;
        std::cin >> op;
        if (op == "y" || op == "Y") {
            userList[username].cart.editProduct(name, userList[username].cart.getQuantity(name) + quantity);
            logger << true << LogString::AddCartSuccess;
        }
        else {
            logger << true;
        }
        return;
    }
    userList[username].cart.addProduct(name, quantity);
    logger << true << LogString::AddCartSuccess;
    this->writeToFile();
}

void User::editCart(const std::string& username, const std::string& name, const int quantity) {
    if (!userList[username].cart.hasProduct(name)) {
        logger << true << LogString::CartNotExist;
        return;
    }
    userList[username].cart.editProduct(name, quantity);
    logger << true << LogString::EditCartSuccess;
    this->writeToFile();
}

void User::deleteCart(const std::string& username, const std::string& name) {
    if (!userList[username].cart.hasProduct(name)) {
        logger << name << " " << LogString::CartNotExist;
        return;
    }
    userList[username].cart.removeProduct(name);
    this->writeToFile();
    logger << name << " " << LogString::DeleteCartSuccess;
}

void User::clearCart(const std::string& username) {
    userList[username].cart.clearCart();
    this->writeToFile();
}

std::vector<std::pair<std::string, int>> User::getCartList(const std::string& username) {
    return userList[username].cart.getCartList();
}
