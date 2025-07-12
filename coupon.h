//
// Created by liu on 5/22/25.
//

#ifndef COUPON_H
#define COUPON_H
#include <map>
#include <string>
#include <chrono>

using sys_seconds_tp = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>;


// 优惠券类，用于管理各种优惠券信息
class Coupon {
public:
    // 优惠券信息结构体
    typedef struct couponInfo {
        std::string name; // 优惠券名称
        std::string category; // 优惠券类别
        sys_seconds_tp startTime; // 开始时间
        sys_seconds_tp endTime; // 结束时间
        double ratio; // 折扣比例
    } couponInfo;

    // 优惠券名称结构体，用于 map 的 key
    typedef struct couponName {
        std::string name; // 优惠券名称
        std::string category; // 优惠券类别
        // 小于运算符重载，用于 map 的排序
        bool operator < (const couponName& other) const {
            return name < other.name || (name == other.name && category < other.category);
        }
    } couponName;

    // 构造函数，加载优惠券数据
    Coupon();

    // getters 方法
    // 获取满减优惠的最低消费金额
    [[nodiscard]] double getMinimum() const;
    // 获取满减优惠的折扣金额
    [[nodiscard]] double getDiscount() const;
    // 获取所有优惠券的 map
    [[nodiscard]] std::map<couponName, couponInfo> getCouponMap();
    // 获取指定商品或类别的优惠券折扣信息
    // name: 商品名称
    // category: 商品类别
    // 返回一个 tuple，包含折扣比例、开始时间和结束时间
    [[nodiscard]] std::tuple<double, std::string, std::string> getCouponDiscount(const std::string& name, const std::string& category);

    // setters 方法
    // 设置满减优惠的最低消费金额
    void setMinimum(double min);
    // 设置满减优惠的折扣金额
    void setDiscount(double dis);
    // 添加新的优惠券
    // name: 优惠券名称
    // category: 优惠券类别
    // discount: 折扣比例
    // daysToAdd: 优惠券有效天数
    void addCoupon(const std::string& name, const std::string& category, double discount, int);
    // 清除已过期的优惠券
    void erase_expired_coupon();
    // 将优惠券信息写入文件
    void writeToFile();

    // 设置满减优惠的最低消费金额和折扣金额
    // min: 最低消费金额
    // dis: 折扣金额
    // 返回 true 如果设置成功，否则返回 false
    bool setMJDiscount(double min, double dis);

    // 格式化时间点为本地时间的字符串
    static std::string format_timepoint_local_seconds(const sys_seconds_tp& tp_s) {
        // 将时间点转换为带时区的时间
        auto zt = std::chrono::zoned_time(std::chrono::current_zone(), tp_s);
        // 格式化时间为字符串
        return std::format("{:%Y-%m-%d %H:%M:%S %Z}", zt);
    }

private:
    double minimum = 0; // 满减优惠的最低消费金额
    double discount = 0; // 满减优惠的折扣金额
    std::map<couponName, couponInfo> couponMap; // 存储优惠券信息的 map
};


#endif //COUPON_H
