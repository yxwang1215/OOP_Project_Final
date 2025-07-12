//
// Created by liu on 6/1/25.
//
// 统计信息头文件

#ifndef STATISTICS_H
#define STATISTICS_H
// 包含订单列表头文件
#include "orderList.h"
// 包含产品列表头文件
#include "productList.h"
// 包含订单头文件
#include "order.h"

// Statistics 类，用于生成销售统计报告
class Statistics {
public:
    // 构造函数，接收订单列表和产品列表的引用
    // orderList: 订单列表的常量引用
    // productList: 产品列表的常量引用
    Statistics(const OrderList &orderList, const ProductList &productList): orderList(&orderList), productList(&productList) {}
    // 打印统计信息
    void printStatistics() const;
private:
    // 指向订单列表的常量指针
    const OrderList* orderList;
    // 指向产品列表的常量指针
    const ProductList* productList;
};


// STATISTICS_H 宏定义结束
#endif //STATISTICS_H
