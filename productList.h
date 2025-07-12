//
// Created by liu on 4/8/25.
//

#ifndef PRODUCTLIST_H
#define PRODUCTLIST_H

#include "product.h"
#include "coupon.h"

// 产品列表类，用于管理所有产品信息
class ProductList {
public:
    // 构造函数，加载产品数据
    explicit ProductList();
    // 打印产品列表
    // coupon: 优惠券对象引用，用于计算并显示应用优惠后的价格
    void printList(Coupon&) const;
    // 添加产品（管理员）
    // name: 产品名称
    // description: 产品描述
    // price: 产品价格
    // inventory: 产品库存
    // category: 产品类别
    void addProduct(const std::string&, const std::string&, double, int, const std::string& category);
    // 将产品信息写入文件
    void writeToFile() const;
    // 编辑产品信息（管理员）
    // name: 产品名称
    // description: 新的产品描述
    // price: 新的产品价格
    // inventory: 新的产品库存
    // category: 新的产品类别
    void editProduct(const std::string&, const std::string&, double, int, const std::string& category);
    // 减少指定产品的库存
    // name: 产品名称
    // quantity: 要减少的数量
    void decreaseInventory(const std::string& name, int quantity);
    // 增加指定产品的库存
    // name: 产品名称
    // quantity: 要增加的数量
    void increaseInventory(const std::string& name, int quantity);
    // 检查产品是否存在
    // name: 产品名称
    // 返回 true 如果产品存在，否则返回 false
    [[nodiscard]] bool hasProduct(const std::string& name) const;
    // 删除产品（管理员）
    // name: 产品名称
    void deleteProduct(const std::string& name);
    // 搜索产品（精确匹配名称、类别、描述或价格）
    // name: 搜索关键字
    // coupon: 优惠券对象引用，用于计算并显示应用优惠后的价格
    void searchProduct(const std::string& name, Coupon&) const;
    // 模糊搜索产品（基于 Levenshtein 距离）
    // name: 搜索关键字
    // coupon: 优惠券对象引用，用于计算并显示应用优惠后的价格
    void fuzzySearchProduct(const std::string& name, Coupon&) const;
    // 获取指定产品的类别
    // name: 产品名称
    // 返回产品类别字符串
    [[nodiscard]] std::string getCategory(const std::string& name) const;
    // 获取指定产品的描述
    // name: 产品名称
    // 返回产品描述字符串
    [[ nodiscard ]] std::string getDescription(const std::string& name) const;
    // 获取指定产品的价格
    // name: 产品名称
    // 返回产品价格
    [[ nodiscard ]] double getPrice(const std::string& name) const;
    // 获取指定产品的库存
    // name: 产品名称
    // 返回产品库存数量
    [[ nodiscard ]] int getInventory(const std::string& name) const;

private:
    std::map<std::string, Product> productMap; // 存储产品的 map，键为产品名称，值为 Product 对象
    std::string fileName; // 存储产品数据的文件名
};


#endif //PRODUCTLIST_H
