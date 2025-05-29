#ifndef USERBASE_H
#define USERBASE_H

#include <string>
#include <map>
#include "productList.h"
#include <vector>
#include "cart.h"

class UserBase {
public:
    enum userType {
        ADMIN,
        USER,
        ERROR,
    };

    struct userStruct {
        std::string password;
        userType role;
        Cart cart;
    };

    bool hasInit;
    explicit UserBase(std::string);
    userType login(const std::string&, const std::string&);
    bool changePassword(const std::string&, const std::string&);
    void writeToFile();
    std::map<std::string, userStruct> userList;
    std::string fileName;
    virtual ~UserBase() = default;
};

class User final : private UserBase {
public:
    User(): UserBase("user.txt") {}
    bool registerUser(const std::string&, const std::string&);
    using UserBase::login;
    using UserBase::changePassword;
    using UserBase::writeToFile;
    void addCart(const std::string&, const std::string&, int);
    void deleteCart(const std::string&, const std::string&);
    void clearCart(const std::string&);
    void editCart(const std::string&, const std::string&, int);
    std::vector<std::pair<std::string, int>> getCartList(const std::string&);

    UserBase* getBasePtr() {
        return this;
    }

    ~User() override = default;
};

class Admin final : private UserBase {
public:
    Admin(): UserBase("admin.txt") {}
    using UserBase::login;
    using UserBase::changePassword;

    UserBase* getBasePtr() {
        return this;
    }

    ~Admin() override = default;
    static void addProduct(const std::string&, const std::string&, double, int, const std::string&, ProductList&);
    static void editProduct(const std::string&, const std::string&, double, int, const std::string&, ProductList&);
    static void deleteProduct(const std::string&, ProductList&);
};


#endif //USERBASE_H
