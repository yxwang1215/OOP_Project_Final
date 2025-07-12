//
// Created by liu on 4/26/25.
//

#ifndef CART_H
#define CART_H

#include <utility>
#include <vector>
#include <string>

// 购物车类，用于管理用户的购物车内容
class Cart {
    // 存储购物车中的商品列表，每个元素是一个pair，包含商品名称和数量
    std::vector<std::pair<std::string, int>> cartList;

public:
    // 默认构造函数，创建一个空的购物车
    explicit Cart() = default;
    // 获取购物车中的商品列表
    // 返回一个包含商品名称和数量对的向量
    [[ nodiscard ]] std::vector<std::pair<std::string, int>> getCartList() const;
    // 检查购物车是否包含某个商品
    // name: 商品名称
    // 如果购物车包含该商品则返回true，否则返回false
    [[nodiscard]] bool hasProduct(const std::string& name) const;
    // 向购物车添加商品
    // name: 商品名称
    // quantity: 商品数量
    void addProduct(const std::string& name, int quantity);
    // 从购物车中移除商品
    // name: 要移除的商品名称
    void removeProduct(const std::string& name);
    // 编辑购物车中商品的数量
    // name: 商品名称
    // quantity: 新的数量
    void editProduct(const std::string& name, int quantity);
    // 获取购物车中某个商品的数量
    // name: 商品名称
    // 返回商品的数量，如果商品不存在则返回0
    [[nodiscard]] int getQuantity(const std::string& name) const;
    // 清空购物车
    void clearCart();
    // 将购物车内容转换为字符串表示
    // 返回购物车内容的字符串
    [[nodiscard]] std::string toString() const;
};


#endif //CART_H
