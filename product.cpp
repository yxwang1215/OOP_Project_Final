//
// Created by liu on 4/8/25.
//

#include "product.h"
#include <iomanip>
#include <sstream>
#include "log.h"
#include "coupon.h"
#include <tuple>
#include "order.h"


// 将产品信息转换为简略字符串表示（包含优惠后价格）
// coupon: 优惠券对象引用，用于计算应用优惠后的价格
// 返回产品的简略信息字符串
std::string Product::toStringShort(Coupon &coupon) const {
    std::stringstream ss; // 用于格式化价格的字符串流
    std::stringstream ss2; // 用于格式化折扣信息的字符串流
    // 获取适用于该产品的优惠券折扣比例
    const double ratio = std::get<0>(coupon.getCouponDiscount(info.name, info.category));
    // 将应用折扣后的价格格式化到 ss 字符串流中，保留两位小数
    ss << std::fixed << std::setprecision(2) << info.price * ratio;
    // 如果存在折扣（折扣比例不为 1.0），则格式化折扣信息到 ss2 字符串流中
    if (ratio != 1.0) {
        ss2 << "(" << std::fixed << std::setprecision(0) << 100 * (1 - ratio) << "% discount)";
    }
    // 构建最终的简略产品信息字符串，使用 Log::paddingEnd 进行格式化对齐
    return Log::paddingEnd(30, info.name + ss2.str()) + // 产品名称 + 折扣信息，填充到 30 个字符宽度
        Log::paddingEnd(10, info.category) + // 产品类别，填充到 10 个字符宽度
        Log::paddingEnd(20, info.description) + // 产品描述，填充到 20 个字符宽度
        Log::paddingEnd(10, ss.str()) + // 应用折扣后的价格，填充到 10 个字符宽度
        Log::paddingEnd(5, std::to_string(info.inventory)) + // 产品库存，填充到 5 个字符宽度
        '\n'; // 换行
}
