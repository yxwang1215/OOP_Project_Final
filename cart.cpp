//
// Created by liu on 4/26/25.
//

#include "cart.h"
#include <vector>
#include <string>
#include <iostream>
#include <ranges>

// 获取购物车中的商品列表
// 返回一个包含商品名称和数量对的向量
std::vector<std::pair<std::string, int>> Cart::getCartList() const {
    // 返回内部存储的购物车列表
    return cartList;
}

// 向购物车添加商品
// name: 商品名称
// quantity: 商品数量
void Cart::addProduct(const std::string& name, const int quantity) {
    // 将商品名称和数量作为一个pair添加到购物车列表中
    cartList.emplace_back(name, quantity);
}

// 从购物车中移除商品
// name: 要移除的商品名称
void Cart::removeProduct(const std::string& name) {
    // 遍历购物车列表
    for (auto it = cartList.begin(); it != cartList.end(); ++it) {
        // 如果找到匹配的商品名称
        if (it->first == name) {
            // 移除该商品
            cartList.erase(it);
            // 找到后即可退出循环
            break;
        }
    }
}

// 清空购物车
void Cart::clearCart() {
    // 清除购物车列表中的所有元素
    cartList.clear();
}

// 获取购物车中某个商品的数量
// name: 商品名称
// 返回商品的数量，如果商品不存在则返回0
int Cart::getQuantity(const std::string& name) const {
    // 遍历购物车列表
    for (const auto& [fst, snd] : cartList) {
        // 如果找到匹配的商品名称
        if (fst == name) {
            // 返回该商品的数量
            return snd;
        }
    }
    // 如果商品不存在，返回0
    return 0;
}

// 编辑购物车中商品的数量
// name: 商品名称
// quantity: 新的数量
void Cart::editProduct(const std::string& name, const int quantity) {
    // 遍历购物车列表
    for (auto& [fst, snd] : cartList) {
        // 如果找到匹配的商品名称
        if (fst == name) {
            // 更新商品的数量
            snd = quantity;
            // 找到后即可退出循环
            break;
        }
    }
}

// 将购物车内容转换为字符串表示
// 返回购物车内容的字符串，格式为 "商品名称 数量 商品名称 数量 ..."
std::string Cart::toString() const {
    // 初始化结果字符串
    std::string result;
    // 遍历购物车列表
    for (const auto& [fst, snd] : cartList) {
        // 将商品名称和数量以及空格添加到结果字符串
        result += fst + " " + std::to_string(snd) + " ";
    }
    // 返回结果字符串
    return result;
}

// 检查购物车是否包含某个商品
// name: 商品名称
// 如果购物车包含该商品则返回true，否则返回false
bool Cart::hasProduct(const std::string& name) const {
    // 遍历购物车列表中的商品名称
    for (const auto& fst : cartList | std::views::keys) {
        // 如果找到匹配的商品名称
        if (fst == name) {
            // 返回true表示找到
            return true;
        }
    }
    // 如果遍历完没有找到，返回false
    return false;
}
