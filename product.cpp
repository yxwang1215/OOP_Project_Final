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

std::string Product::toStringShort(const Coupon &coupon) const {
    std::stringstream ss;
    std::stringstream ss2;
    const double ratio = std::get<0>(coupon.getCouponDiscount(info.name, info.category));
    ss << std::fixed << std::setprecision(2) << info.price * ratio;
    if (ratio != 1.0) {
        ss2 << "(" << std::fixed << std::setprecision(0) << 100 * (1 - ratio) << "% discount)";
    }
    return Log::paddingEnd(30, info.name + ss2.str()) + Log::paddingEnd(10, info.category) + Log::paddingEnd(20, info.description) +
        Log::paddingEnd(10, ss.str()) + Log::paddingEnd(5, std::to_string(info.inventory)) + '\n';
}
