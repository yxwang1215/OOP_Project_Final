//
// Created by liu on 4/26/25.
//

#include "cart.h"
#include <vector>
#include <string>
#include <iostream>
#include <ranges>

std::vector<std::pair<std::string, int>> Cart::getCartList() const {
    return cartList;
}

void Cart::addProduct(const std::string& name, const int quantity) {
    cartList.emplace_back(name, quantity);
}

void Cart::removeProduct(const std::string& name) {
    for (auto it = cartList.begin(); it != cartList.end(); ++it) {
        if (it->first == name) {
            cartList.erase(it);
            break;
        }
    }
}

void Cart::clearCart() {
    cartList.clear();
}

int Cart::getQuantity(const std::string& name) const {
    for (const auto& [fst, snd] : cartList) {
        if (fst == name) {
            return snd;
        }
    }
    return 0;
}

void Cart::editProduct(const std::string& name, const int quantity) {
    for (auto& [fst, snd] : cartList) {
        if (fst == name) {
            snd = quantity;
            break;
        }
    }
}

std::string Cart::toString() const {
    std::string result;
    for (const auto& [fst, snd] : cartList) {
        result += fst + " " + std::to_string(snd) + " ";
    }
    return result;
}

bool Cart::hasProduct(const std::string& name) const {
    for (const auto& fst : cartList | std::views::keys) {
        if (fst == name) {
            return true;
        }
    }
    return false;
}
