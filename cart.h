//
// Created by liu on 4/26/25.
//

#ifndef CART_H
#define CART_H

#include <utility>
#include <vector>
#include <string>

class Cart {
    std::vector<std::pair<std::string, int>> cartList;

public:
    explicit Cart() = default;
    [[ nodiscard ]] std::vector<std::pair<std::string, int>> getCartList() const;
    [[nodiscard]] bool hasProduct(const std::string& name) const;
    void addProduct(const std::string& name, int quantity);
    void removeProduct(const std::string& name);
    void editProduct(const std::string& name, int quantity);
    [[nodiscard]] int getQuantity(const std::string& name) const;
    void clearCart();
    [[nodiscard]] std::string toString() const;
};


#endif //CART_H
