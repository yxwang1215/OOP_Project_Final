//
// Created by liu on 4/7/25.
//

#ifndef LOG_H
#define LOG_H
#include <string>

// 日志类，用于控制台输出日志信息
// 日志类，用于控制台输出日志信息
class Log {
public:
    // 静态方法，打印日志信息并可选择清空屏幕
    // log: 要打印的日志字符串
    // clear: 是否清空屏幕 (true 清空，false 不清空)
    static void printLog(const std::string&, bool clear);
    // 重载 << 运算符，用于输出字符串
    // 返回当前 Log 对象引用，支持链式调用
    Log& operator<<(const std::string&);
    // 重载 << 运算符，用于控制是否清空屏幕
    // 返回当前 Log 对象引用，支持链式调用
    Log& operator<<(bool);
    // 重载 << 运算符，用于输出 C 风格字符串
    // 返回当前 Log 对象引用，支持链式调用
    Log& operator<<(const char*);
    // 重载 << 运算符，用于输出字符
    // 返回当前 Log 对象引用，支持链式调用
    Log& operator<<(char);
    // 重载 << 运算符，用于输出整数
    // 返回当前 Log 对象引用，支持链式调用
    Log& operator<<(int);
    // 重载 << 运算符，用于输出双精度浮点数
    // 返回当前 Log 对象引用，支持链式调用
    Log& operator<<(double);
    // 静态方法，在字符串末尾填充空格以达到指定长度
    // len: 目标长度
    // str: 原始字符串
    // 返回填充后的字符串
    static std::string paddingEnd(int len, const std::string&);
};

// 日志字符串命名空间，包含各种用于控制台输出的字符串常量
namespace LogString
{
    // 退出时的艺术字
    const std::string goodbye_art = R"(
  _____                 _ _
 / ____|               | | |
| |  __  ___   ___   __| | |__  _   _  ___
| | |_ |/ _ \ / _ \ / _` | '_ \| | | |/ _ \
| |__| | (_) | (_) | (_| | |_) | |_| |  __/
 \_____|\___/ \___/ \__,_|_.__/ \__, |\___|
                                 __/ |
                                |___/
    )";
    // 主菜单
    const std::string MENU = "welcome to use this system\n"
        "1. login\n"
        "2. register\n"
        "3. browse products\n"
        "4. search products\n"
        "5. fuzzy search products\n"
        "6. list cart\n"
        "7. add cart\n"
        "8. edit cart\n"
        "9. delete cart\n"
        "0. exit\n";
    // 登录提示
    const std::string Login = "please login\n"
        "please input your username and password(input - to exit)\n";
    // 用户名输入提示
    const std::string Username = "username: ";
    // 密码输入提示
    const std::string Password = "password: ";
    // 换行符
    const std::string changeLine = "\n";
    // 退出信息
    const std::string Exit = "thank you for using this system\n"
        "goodbye\n" + goodbye_art + "\n";

    // 错误密码提示
    const std::string WrongPassword = "\033[1;31musername or password is wrong\033[0m\n";
    // 注册提示
    const std::string Register = "please register\n"
        "please input your username and password\n";
    // 用户已存在提示
    const std::string UserExist = "\033[1;31muser already exist\033[0m\n";
    // 用户不存在提示
    const std::string UserNotExist = "\033[1;31muser not exist\033[0m\n";
    // 注册成功提示
    const std::string RegisterSuccess = "\033[1;32mregister success\033[0m\n";
    // 管理员面板菜单
    const std::string AdminPanel = "welcome to admin panel\n"
        "1. change password\n"
        "2. add product\n"
        "3. product list\n"
        "4. edit product\n"
        "5. delete product\n"
        "6. change order status\n"
        "7. order list\n"
        "8. add threshold coupon\n"
        "9. add percentage coupon by name\n"
        "10. add percentage coupon by category\n"
        "11. statistics\n"
        "0. logout\n";
    // 修改密码提示
    const std::string ChangePassword = "please input your new password\n";
    // 修改密码成功提示
    const std::string ChangePasswordSuccess = "\033[1;32mchange password success\033[0m\n";
    // 文件未找到提示
    const std::string FileNotFound = "\033[1;31mfile not found\033[0m\n";
    // 密码相同提示
    const std::string PasswordSame = "\033[1;31mpassword is the same\033[0m\n";
    // 用户面板菜单
    const std::string UserPanel = "welcome to user panel\n"
        "1. change password\n"
        "2. product list\n"
        "3. search product\n"
        "4. fuzzy search product\n"
        "5. list cart\n"
        "6. add cart\n"
        "7. edit cart\n"
        "8. delete cart\n"
        "9. order\n"
        "0. logout\n";
    // 用户订单面板菜单
    const std::string UserOrderPanel = "welcome to order panel\n"
        "1. create order\n"
        "2. order list\n"
        "3. edit order\n"
        "4. delete order\n"
        "5. cancel order\n"
        "6. create order from cart\n"
        "0. exit\n";
    // 后端面板菜单
    const std::string BackendPanel = "welcome to backend panel\n"
        "Details not yet implemented\n"
        "0. exit\n";
    // 购买面板菜单
    const std::string BuyPanel = "welcome to buy panel\n"
        "Details not yet implemented\n"
        "0. exit\n";

    // 产品列表面板菜单
    const std::string ProductPanel = "welcome to Product List\n"
        "Details not yet implemented\n"
        "0. exit\n";
    // 无效输入提示
    const std::string Invalid = "\033[1;31minvalid input\033[0m\n";

    // 产品列表头部
    const std::string ProductHead = "Name                          category  desc                price     inv       \n"
                                    "--------------------------------------------------------------------------------\n";
    // 未授权提示
    const std::string Authorization = "you are not authorized\n";
    // 权限不足提示
    const std::string PermissionDenied = "permission denied\n";
    // 输入产品名称提示
    const std::string InputProductName = "please input product name: ";
    // 输入产品类别提示
    const std::string InputProductCategory = "please input product category: ";
    // 输入产品描述提示
    const std::string InputProductDescription = "please input product description: ";
    // 输入产品价格提示
    const std::string InputProductPrice = "please input product price: ";
    // 输入产品库存提示
    const std::string InputProductInventory = "please input product inventory: ";
    // 产品已存在提示
    const std::string ProductExist = "\033[1;31mproduct already exist\033[0m\n";
    // 添加产品成功提示
    const std::string AddProductSuccess = "\033[1;32madd product success\033[0m\n";
    // 产品未找到提示
    const std::string ProductNotFound = "\033[1;31mproduct not found\033[0m\n";
    // 退出列表提示
    const std::string ExitList = "\nPress 0 to exit\n";
    // 编辑产品成功提示
    const std::string EditProductSuccess = "\033[1;32medit product success\033[0m\n";
    // 编辑产品名称提示
    const std::string EditProduct = "please input product name to edit: ";
    // 编辑产品描述提示
    const std::string EditProductDesc = "please input product description(- for no change): ";
    // 编辑产品价格提示
    const std::string EditProductPrice = "please input product price(-1 for no change): ";
    // 编辑产品库存提示
    const std::string EditProductInventory = "please input product inventory(-1 for no change): ";
    // 编辑产品类别提示
    const std::string EditProductCategory = "please input product category(- for no change): ";
    // 删除产品名称提示
    const std::string DeleteProduct = "please input product name to delete: ";
    // 删除产品成功提示
    const std::string DeleteProductSuccess = "\033[1;32mdelete product success\033[0m\n";
    // 搜索产品名称提示
    const std::string SearchProduct = "please input product name to search: ";
    // 购物车已存在产品提示
    const std::string CartExist = "\033[1;31 is already in cart\033[0m\nAdd it anyway?(y/n): ";
    // 购物车产品不存在提示
    const std::string CartNotExist = "\033[1;31mProduct not exist in cart\033[0m\n";
    // 产品不存在提示
    const std::string ProductNotExist = "\033[1;31mProduct not exist\033[0m\n";
    // 购物车头部
    const std::string CartHead = "Name           category  desc                price     quantity      \n";
    // 购物车尾部
    const std::string CartTail = "---------------------------------------------------------------------\n";
    // 总数量提示
    const std::string SumQua = "Total quantity: ";
    // 总价格提示
    const std::string SumPrice = "Total price: ";
    // 添加购物车成功提示
    const std::string AddCartSuccess = "\033[1;32madd cart success\033[0m\n";
    // 输入购物车产品名称提示
    const std::string InputCartName = "please input product name: ";
    // 输入购物车产品数量提示
    const std::string InputCartQua = "please input product quantity: ";
    // 编辑购物车成功提示
    const std::string EditCartSuccess = "\033[1;32medit cart success\033[0m\n";
    // 编辑购物车产品名称提示
    const std::string EditCart = "please input product name to edit: ";
    // 编辑购物车产品数量提示
    const std::string EditCartQua = "please input product quantity(-1 for no change): ";
    // 删除购物车产品名称提示
    const std::string DeleteCart = "please input product name to delete: (split by ,)";
    // 删除购物车成功提示
    const std::string DeleteCartSuccess = "\033[1;32mdelete cart success\033[0m\n";
    // 确认删除提示
    const std::string WantToDelete = "want to delete this product?(y/n): ";
    // 需要登录提示
    const std::string NeedLogin = "\033[1;31mplease login first\033[0m\n";

    // 订单相关字符串
    // 订单ID标签
    const std::string OrderLabelId = "Order ID: ";
    // 时间标签
    const std::string OrderLabelTime = "Time: ";
    // 状态标签
    const std::string OrderLabelStatus = "Status: ";
    // 订单状态：待处理
    const std::string OrderStatusPending = "Pending";
    // 订单状态：已发货
    const std::string OrderStatusShipped = "Shipped";
    // 订单状态：已送达
    const std::string OrderStatusDelivered = "Delivered";
    // 订单商品标签
    const std::string OrderLabelItems = "Items: ";
    // 订单商品名称标签
    const std::string OrderItemName = "\t- Name: ";
    // 订单商品数量标签
    const std::string OrderItemQuantity = ", Quantity: ";
    // 订单总计标签
    const std::string OrderLabelTotal = "Total: ";
    // 订单地址标签
    const std::string OrderLabelAddress = "Address: ";

    // 订单未找到提示
    const std::string OrderNotFound = "\033[1;31mOrder not found\033[0m\n";
    // 产品库存不足提示
    const std::string ProductNotEnough = "\033[1;31mproduct not enough\033[0m\n";
    // 删除订单成功提示
    const std::string DeleteOrderSuccess = "\033[1;32mdelete order success\033[0m\n";
    // 删除订单失败提示
    const std::string DeleteOrderFail = "\033[1;31myou can only delete delivered order\033[0m\n";
    // 编辑地址成功提示
    const std::string EditAddressSuccess = "\033[1;32medit address success\033[0m\n";
    // 编辑地址失败提示
    const std::string EditAddressFail = "\033[1;31myou can only edit pending order\033[0m\n";
    // 订单不存在提示
    const std::string OrderNotExist = "\033[1;31mOrder not exist\033[0m\n";
    // 取消订单成功提示
    const std::string CancelOrderSuccess = "\033[1;32mcancel order success\033[0m\n";
    // 取消订单失败提示
    const std::string CancelOrderFail = "\033[1;31myou can only cancel pending order\033[0m\n";
    // 订单头部
    const std::string OrderHead = "your order\n---------------------------------------------------------\n";
    // 分隔线
    const std::string Hr = "---------------------------------------------------------\n";
    // 输入订单产品名称提示
    const std::string InputOrderName = "please input product name: ";
    // 输入订单产品数量提示
    const std::string InputOrderQua = "please input product quantity: ";
    // 输入订单地址提示
    const std::string InputOrderAddress = "please input address: ";
    // 下单成功提示
    const std::string OrderSuccess = "\033[1;32morder success\033[0m\n";
    // 下单失败提示
    const std::string OrderFail = "\033[1;31morder fail\033[0m\n";
    // 输入要修改地址的订单ID提示
    const std::string ChangeAddressId = "please input order id to change: (you can only change pending order)";
    // 输入要修改的地址提示
    const std::string ChangeAddress = "please input address to change (you can only change pending order): ";
    // 输入要删除的订单ID提示
    const std::string DeleteOrder = "please input order id to delete (you can only delete delivered order): ";
    // 输入要取消的订单ID提示
    const std::string CancelOrder = "please input order id to cancel (you can only cancel pending order): ";
    // 从购物车创建订单提示
    const std::string CreateOrderFromCart = "do you want to buy items in cart?(y/n): ";
    // 输入要修改状态的订单ID提示
    const std::string ChangeOrderStatus = "please input order id to change status: ";
    // 输入要修改的订单状态提示
    const std::string ChangeOrderStatusInputStatus =
        "please input order status (pending, shipped, delivered, cancelled): ";
    // 修改订单状态成功提示
    const std::string ChangeOrderStatusSuccess = "\033[1;32mchange order status success\033[0m\n";
    // 添加订单成功提示
    const std::string AddOrderSuccess = "\033[1;32madd order success\033[0m\n";
    // 购物车为空提示
    const std::string EmptyCart = "your cart is empty\n";
    // 提交订单ID提示
    const std::string SubmitOrderId = "Your order id is: ";
    // 添加满减优惠券最低阈值提示
    const std::string AddThresholdCouponMinimum = "please input the minimum threshold: ";
    // 添加满减优惠券折扣金额提示
    const std::string AddThresholdCouponDiscount = "please input the discount: ";
    // 添加满减优惠券成功提示
    const std::string AddThresholdCouponSuccess = "\033[1;32madd threshold coupon success\033[0m\n";
    // 添加满减优惠券失败提示
    const std::string AddThresholdCouponFail = "\033[1;31madd threshold coupon fail\033[0m\n";
    // 添加百分比优惠券成功提示
    const std::string AddPercentCouponSuccess = "\033[1;32madd percent coupon success\033[0m\n";
    // 添加百分比优惠券失败提示
    const std::string AddPercentCouponFail = "\033[1;31madd percent coupon fail\033[0m\n";
    // 添加百分比优惠券名称提示
    const std::string AddPercentCouponName = "please input the name: ";
    // 添加百分比优惠券类别提示
    const std::string AddPercentCouponCategory = "please input the category: ";
    // 添加百分比优惠券比例提示
    const std::string AddPercentRatio = "please input the ratio: ";
    // 添加百分比优惠券持续时间提示
    const std::string AddPercentDuration = "please set the offer duration (seconds): ";
    // 统计信息写入成功提示
    const std::string StatisticsWriteSuccess = "\033[1;32mwrite statistics to statistics.csv success\033[0m\n";
}


inline Log logger;

#endif //LOG_H
