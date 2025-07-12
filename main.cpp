#include <iostream>
#include <string>
#include "log.h"
#include "status.h"
#include <regex>
using namespace std;

// 管理员功能函数
// status: Status 类的引用，用于访问和修改系统状态
void adminFunc(Status& status) {
    string option; // 用户输入的选项
    bool is_first = true; // 标记是否是第一次进入循环，用于清空屏幕
    while (true) {
        // 打印管理员面板菜单，如果是第一次进入则清空屏幕
        logger << is_first << LogString::AdminPanel;
        is_first = false; // 之后不再清空屏幕
        cin >> option; // 读取用户输入的选项
        if (option == "0") {
            return; // 返回主菜单
        }
        else if (option == "1") {
            // 修改密码功能
            logger << true << LogString::ChangePassword; // 清空屏幕并打印修改密码提示
            string password; // 新密码
            cin >> password; // 读取新密码
            if (status.changePassword(password)){}; // 调用 Status 类的 changePassword 方法修改密码
        }
        else if (option == "2") {
            // 添加产品功能
            string name, description, category; // 产品名称、描述、类别
            double price; // 产品价格
            int inventory; // 产品库存
            logger << true << LogString::InputProductName; // 清空屏幕并打印输入产品名称提示
            cin >> name; // 读取产品名称
            if (status.hasProduct(name)) {
                // 如果产品已存在
                logger << true << LogString::ProductExist; // 清空屏幕并打印产品已存在提示
                continue; // 继续下一次循环
            }
            logger << LogString::InputProductDescription; // 打印输入产品描述提示
            cin.ignore(); // 忽略缓冲区中的换行符
            getline(cin, description); // 读取产品描述（包含空格）
            logger << LogString::InputProductPrice; // 打印输入产品价格提示

            // 读取产品价格，直到输入有效
            while (cin >> price, cin.fail()) {
                cin.clear(); // 清除错误标志
                cin.ignore(1000, '\n'); // 忽略无效输入
                logger << LogString::Invalid; // 打印无效输入提示
                logger << LogString::InputProductPrice; // 再次打印输入产品价格提示
            }
            logger << LogString::InputProductInventory; // 打印输入产品库存提示
            // 读取产品库存，直到输入有效
            while (cin >> inventory, cin.fail()) {
                cin.clear(); // 清除错误标志
                cin.ignore(1000, '\n'); // 忽略无效输入
                logger << LogString::Invalid; // 打印无效输入提示
                logger << LogString::InputProductInventory; // 再次打印输入产品库存提示
            }
            logger << LogString::InputProductCategory; // 打印输入产品类别提示
            cin >> category; // 读取产品类别
            status.addProduct(name, description, price, inventory, category); // 调用 Status 类的 addProduct 方法添加产品
        }
        else if (option == "3") {
            // 产品列表功能
            status.printList(); // 打印产品列表
            while (true) {
                string option_2; // 用户输入的选项
                cin >> option_2; // 读取用户输入的选项
                if (option_2 == "0") {
                    is_first = true; // 设置为第一次进入循环，以便返回管理员面板时清空屏幕
                    break; // 退出当前循环
                }
            }
        }
        else if (option == "4") {
            // 编辑产品功能
            string name, description, category; // 产品名称、描述、类别
            double price; // 产品价格
            int inventory; // 产品库存
            logger << true << LogString::EditProduct; // 清空屏幕并打印编辑产品提示
            cin >> name; // 读取要编辑的产品名称
            if (!status.hasProduct(name)) {
                // 如果产品不存在
                logger << true << LogString::ProductNotFound; // 清空屏幕并打印产品未找到提示
                continue; // 继续下一次循环
            }
            Log::printLog(LogString::EditProductDesc, false); // 打印编辑产品描述提示
            cin.ignore(); // 忽略缓冲区中的换行符
            getline(cin, description); // 读取新的产品描述
            Log::printLog(LogString::EditProductPrice, false); // 打印编辑产品价格提示
            // 读取新的产品价格，直到输入有效
            while (cin >> price, cin.fail()) {
                cin.clear(); // 清除错误标志
                cin.ignore(1000, '\n'); // 忽略无效输入
                logger << LogString::Invalid; // 打印无效输入提示
                logger << LogString::EditProductPrice; // 再次打印编辑产品价格提示
            }
            Log::printLog(LogString::EditProductInventory, false); // 打印编辑产品库存提示
            // 读取新的产品库存，直到输入有效
            while (cin >> inventory, cin.fail()) {
                cin.clear(); // 清除错误标志
                cin.ignore(1000, '\n'); // 忽略无效输入
                logger << LogString::Invalid; // 打印无效输入提示
                logger << LogString::EditProductInventory; // 再次打印编辑产品库存提示
            }
            Log::printLog(LogString::EditProductCategory, false); // 打印编辑产品类别提示
            cin >> category; // 读取新的产品类别
            status.editProduct(name, description, price, inventory, category); // 调用 Status 类的 editProduct 方法编辑产品
        }
        else if (option == "5") {
            // 删除产品功能
            string name; // 要删除的产品名称
            logger << true << LogString::DeleteProduct; // 清空屏幕并打印删除产品提示
            cin >> name; // 读取要删除的产品名称
            status.deleteProduct(name); // 调用 Status 类的 deleteProduct 方法删除产品
        }
        else if (option == "6") {
            // 修改订单状态功能
            logger << true << LogString::ChangeOrderStatus; // 清空屏幕并打印修改订单状态提示
            string orderId; // 订单ID
            Order::orderStatus order_status = {}; // 订单状态枚举
            string order_status_str; // 订单状态字符串
            cin >> orderId; // 读取订单ID
            logger << LogString::ChangeOrderStatusInputStatus; // 打印输入订单状态提示
            cin >> order_status_str; // 读取订单状态字符串
            // 根据输入的字符串设置订单状态枚举值
            if (order_status_str == "pending") {
                order_status = Order::PENDING;
            }
            else if (order_status_str == "shipped") {
                order_status = Order::SHIPPED;
            }
            else if (order_status_str == "delivered") {
                order_status = Order::DELIVERED;
            }
            else if (order_status_str == "cancelled") {
                order_status = Order::CANCELLED;
            }
            status.changeOrderStatus(orderId, order_status); // 调用 Status 类的 changeOrderStatus 方法修改订单状态
        }
        else if (option == "7") {
            // 订单列表功能
            status.printAllOrders(); // 打印所有订单列表
            while (true) {
                string option_2; // 用户输入的选项
                cin >> option_2; // 读取用户输入的选项
                if (option_2 == "0") {
                    is_first = true; // 设置为第一次进入循环，以便返回管理员面板时清空屏幕
                    break; // 退出当前循环
                }
            }
        }
        else if (option == "8") {
            // 添加满减优惠券功能
            logger << true << LogString::AddThresholdCouponMinimum; // 清空屏幕并打印输入最低阈值提示
            int threshold; // 最低阈值
            // 读取最低阈值，直到输入有效
            while (cin >> threshold, cin.fail()) {
                cin.clear(); // 清除错误标志
                cin.ignore(1000, '\n'); // 忽略无效输入
                logger << LogString::Invalid; // 打印无效输入提示
                logger << LogString::AddThresholdCouponMinimum; // 再次打印输入最低阈值提示
            }
            logger << true << LogString::AddThresholdCouponDiscount; // 清空屏幕并打印输入折扣金额提示
            double discount; // 折扣金额
            // 读取折扣金额，直到输入有效
            while (cin >> discount, cin.fail()) {
                cin.clear(); // 清除错误标志
                cin.ignore(1000, '\n'); // 忽略无效输入
                logger << LogString::Invalid; // 打印无效输入提示
                logger << LogString::AddThresholdCouponDiscount; // 再次打印输入折扣金额提示
            }
            status.setMJCoupon(threshold, discount); // 调用 Status 类的 setMJCoupon 方法设置满减优惠券
        }
        else if (option == "9") {
            // 添加按名称的百分比优惠券功能
            logger << true << LogString::AddPercentCouponName; // 清空屏幕并打印输入名称提示
            string name; // 优惠券名称
            cin >> name; // 读取优惠券名称
            logger << true << LogString::AddPercentRatio; // 清空屏幕并打印输入比例提示
            double discount; // 折扣比例
            // 读取折扣比例，直到输入有效
            while (cin >> discount, cin.fail()) {
                cin.clear(); // 清除错误标志
                cin.ignore(1000, '\n'); // 忽略无效输入
                logger << LogString::Invalid; // 打印无效输入提示
                logger << LogString::AddPercentRatio; // 再次打印输入比例提示
            }
            int duration; // 有效期（秒）
            logger << true << LogString::AddPercentDuration; // 清空屏幕并打印设置有效期提示
            // 读取有效期，直到输入有效
            while (cin >> duration, cin.fail()) {
                cin.clear(); // 清除错误标志
                cin.ignore(1000, '\n'); // 忽略无效输入
                logger << LogString::Invalid; // 打印无效输入提示
                logger << LogString::AddPercentDuration; // 再次打印设置有效期提示
            }
            status.add_coupon(name, "-", discount, duration); // 调用 Status 类的 add_coupon 方法添加优惠券（按名称）
        }
        else if (option == "10") {
            // 添加按类别的百分比优惠券功能
            logger << true << LogString::AddPercentCouponCategory; // 清空屏幕并打印输入类别提示
            string name; // 优惠券类别
            cin >> name; // 读取优惠券类别
            logger << true << LogString::AddPercentRatio; // 清空屏幕并打印输入比例提示
            double discount; // 折扣比例
            // 读取折扣比例，直到输入有效
            while (cin >> discount, cin.fail()) {
                cin.clear(); // 清除错误标志
                cin.ignore(1000, '\n'); // 忽略无效输入
                logger << LogString::Invalid; // 打印无效输入提示
                logger << LogString::AddPercentRatio; // 再次打印输入比例提示
            }
            int duration; // 有效期（秒）
            logger << true << LogString::AddPercentDuration; // 清空屏幕并打印设置有效期提示
            // 读取有效期，直到输入有效
            while (cin >> duration, cin.fail()) {
                cin.clear(); // 清除错误标志
                cin.ignore(1000, '\n'); // 忽略无效输入
                logger << LogString::Invalid; // 打印无效输入提示
                logger << LogString::AddPercentDuration; // 再次打印设置有效期提示
            }
            status.add_coupon("-", name, discount, duration); // 调用 Status 类的 add_coupon 方法添加优惠券（按类别）
        }else if (option == "11") {
            // 统计功能
            status.get_statistics(); // 调用 Status 类的 get_statistics 方法获取统计信息
        }
        else {
            // 无效输入
            logger << true << LogString::Invalid; // 清空屏幕并打印无效输入提示
        }
    }
}

// 用户订单功能函数
// status: Status 类的引用，用于访问和修改系统状态
void userOrderFunc(Status& status) {
    string option; // 用户输入的选项
    bool is_first = true; // 标记是否是第一次进入循环，用于清空屏幕
    while (true) {
        logger << is_first << LogString::UserOrderPanel; // 打印用户订单面板菜单
        is_first = false; // 之后不再清空屏幕
        cin >> option; // 读取用户输入的选项
        if (option == "0") {
            return; // 返回用户面板
        }
        if (option == "1") {
            // 创建订单功能
            logger << true << LogString::InputOrderName; // 清空屏幕并打印输入订单产品名称提示
            string name; // 产品名称
            cin >> name; // 读取产品名称
            logger << LogString::InputOrderQua; // 打印输入订单产品数量提示
            int quantity; // 产品数量
            // 读取产品数量，直到输入有效
            while (cin >> quantity, cin.fail()) {
                cin.clear(); // 清除错误标志
                cin.ignore(1000, '\n'); // 忽略无效输入
                logger << LogString::Invalid; // 打印无效输入提示
                logger << LogString::InputOrderQua; // 再次打印输入订单产品数量提示
            }
            string address; // 收货地址
            logger << LogString::InputOrderAddress; // 打印输入地址提示
            cin.ignore(); // 忽略缓冲区中的换行符
            getline(cin, address); // 读取收货地址
            status.addOrder({{name, quantity}}, address); // 调用 Status 类的 addOrder 方法创建订单
        }
        else if (option == "2") {
            // 订单列表功能
            status.printOrder(); // 打印当前用户的订单列表
            logger << LogString::ExitList; // 打印退出列表提示
            while (true) {
                string option_2; // 用户输入的选项
                cin >> option_2; // 读取用户输入的选项
                if (option_2 == "0") {
                    is_first = true; // 设置为第一次进入循环，以便返回用户订单面板时清空屏幕
                    break; // 退出当前循环
                }
            }
        }
        else if (option == "3") {
            // 修改订单地址功能
            logger << true << LogString::ChangeAddressId; // 清空屏幕并打印输入要修改地址的订单ID提示
            string OrderId; // 订单ID
            cin >> OrderId; // 读取订单ID
            string address; // 新的收货地址
            logger << LogString::InputOrderAddress; // 打印输入地址提示
            cin.ignore(); // 忽略缓冲区中的换行符
            getline(cin, address); // 读取新的收货地址
            status.changeOrderAddress(OrderId, address); // 调用 Status 类的 changeOrderAddress 方法修改订单地址
        }
        else if (option == "4") {
            // 删除订单功能
            logger << true << LogString::DeleteOrder; // 清空屏幕并打印输入要删除的订单ID提示
            string OrderId; // 订单ID
            cin >> OrderId; // 读取订单ID
            status.deleteOrder(OrderId); // 调用 Status 类的 deleteOrder 方法删除订单
        }
        else if (option == "5") {
            // 取消订单功能
            logger << true << LogString::CancelOrder; // 清空屏幕并打印输入要取消的订单ID提示
            string OrderId; // 订单ID
            cin >> OrderId; // 读取订单ID
            status.cancelOrder(OrderId); // 调用 Status 类的 cancelOrder 方法取消订单
        }
        else if (option == "6") {
            // 从购物车创建订单功能
            logger << true << LogString::CreateOrderFromCart; // 清空屏幕并打印从购物车创建订单提示
            string op; // 用户输入的选项
            cin >> op; // 读取用户输入的选项
            if (op == "y" || op == "Y") {
                // 如果用户选择从购物车创建订单
                string address; // 收货地址
                logger << LogString::InputOrderAddress; // 打印输入地址提示
                cin.ignore(); // 忽略缓冲区中的换行符
                getline(cin, address); // 读取收货地址
                status.addOrderFromCart(address); // 调用 Status 类的 addOrderFromCart 方法从购物车创建订单
            }
        }
        else {
            // 无效输入
            logger << true << LogString::Invalid; // 清空屏幕并打印无效输入提示
        }
    }
}

// 用户功能函数
// status: Status 类的引用，用于访问和修改系统状态
void userFunc(Status& status) {
    string option; // 用户输入的选项
    bool is_first = true; // 标记是否是第一次进入循环，用于清空屏幕
    while (true) {
        // 打印用户面板菜单，如果是第一次进入则清空屏幕
        logger << is_first << LogString::UserPanel;
        is_first = false; // 之后不再清空屏幕
        cin >> option; // 读取用户输入的选项
        if (option == "0") {
            return; // 返回主菜单
        }
        else if (option == "1") {
            // 修改密码功能
            logger << true << LogString::ChangePassword; // 清空屏幕并打印修改密码提示
            string password; // 新密码
            cin >> password; // 读取新密码
            status.changePassword(password); // 调用 Status 类的 changePassword 方法修改密码
        }
        else if (option == "2") {
            // 产品列表功能
            status.printList(); // 打印产品列表
            while (true) {
                string option_2; // 用户输入的选项
                cin >> option_2; // 读取用户输入的选项
                if (option_2 == "0") {
                    is_first = true; // 设置为第一次进入循环，以便返回用户面板时清空屏幕
                    break; // 退出当前循环
                }
            }
        }
        else if (option == "3") {
            // 搜索产品功能
            string name; // 要搜索的产品名称
            logger << true << LogString::SearchProduct; // 清空屏幕并打印搜索产品提示
            cin >> name; // 读取要搜索的产品名称
            status.searchProduct(name); // 调用 Status 类的 searchProduct 方法搜索产品
        }
        else if (option == "4") {
            // 模糊搜索产品功能
            string name; // 要模糊搜索的产品名称
            logger << true << LogString::SearchProduct; // 清空屏幕并打印搜索产品提示
            cin >> name; // 读取要模糊搜索的产品名称
            status.fuzzySearchProduct(name); // 调用 Status 类的 fuzzySearchProduct 方法模糊搜索产品
        }
        else if (option == "5") {
            // 购物车列表功能
            status.printCart(); // 打印购物车列表
            while (true) {
                string option_2; // 用户输入的选项
                cin >> option_2; // 读取用户输入的选项
                if (option_2 == "0") {
                    is_first = true; // 设置为第一次进入循环，以便返回用户面板时清空屏幕
                    break; // 退出当前循环
                }
            }
        }
        else if (option == "6") {
            // 添加购物车功能
            string name; // 产品名称
            int quantity; // 产品数量
            logger << true << LogString::InputCartName; // 清空屏幕并打印输入购物车产品名称提示
            cin >> name; // 读取产品名称
            logger << LogString::InputCartQua; // 打印输入购物车产品数量提示
            // 读取产品数量，直到输入有效
            while (cin >> quantity, cin.fail()) {
                cin.clear(); // 清除错误标志
                cin.ignore(1000, '\n'); // 忽略无效输入
                logger << LogString::Invalid; // 打印无效输入提示
                logger << LogString::InputCartQua; // 再次打印输入购物车产品数量提示
            }
            status.addCart(name, quantity); // 调用 Status 类的 addCart 方法添加产品到购物车
        }
        else if (option == "7") {
            // 编辑购物车功能
            string name; // 产品名称
            int quantity; // 产品数量
            logger << true << LogString::EditCart; // 清空屏幕并打印编辑购物车提示
            cin >> name; // 读取要编辑的产品名称
            logger << LogString::EditCartQua; // 打印编辑购物车产品数量提示
            // 读取产品数量，直到输入有效
            while (cin >> quantity, cin.fail()) {
                cin.clear(); // 清除错误标志
                cin.ignore(1000, '\n'); // 忽略无效输入
                logger << LogString::Invalid; // 打印无效输入提示
                logger << LogString::EditCartQua; // 再次打印编辑购物车产品数量提示
            }
            status.editCart(name, quantity); // 调用 Status 类的 editCart 方法编辑购物车中的产品数量
        }
        else if (option == "8") {
            // 删除购物车产品功能
            string name; // 要删除的产品名称（可能包含多个，用逗号分隔）
            logger << true << LogString::DeleteCart; // 清空屏幕并打印删除购物车产品提示
            cin.ignore(); // 忽略缓冲区中的换行符
            getline(cin, name); // 读取要删除的产品名称字符串
            logger << true; // 清空屏幕
            // 使用正则表达式分割产品名称字符串，并逐个删除购物车中的产品
            const std::regex token_re(R"(\s*([^,]+?)\s*(?:,|$))");
            for (std::sregex_iterator it(name.begin(), name.end(), token_re), end; it != end; ++it) {
                status.deleteCart((*it)[1].str()); // 调用 Status 类的 deleteCart 方法删除购物车中的产品
            }
        }
        else if (option == "9") {
            // 订单功能
            userOrderFunc(status); // 调用用户订单功能函数
            is_first = true; // 设置为第一次进入循环，以便返回用户面板时清空屏幕
        }
        else {
            // 无效输入
            logger << true << LogString::Invalid; // 清空屏幕并打印无效输入提示
        }
    }
}


// 主函数
// 主函数
int main() {
    // logger << "aaa"; // 调试语句，已注释
    Status status; // 创建 Status 对象，用于管理系统状态、用户、产品、订单等
    std::istream::sync_with_stdio(false); // 取消 C++ 标准流与 C 标准流的同步，提高输入输出效率
    cin.tie(nullptr); // 解除 cin 与 cout 的绑定，提高输入效率
    string option; // 用户输入的选项
    bool is_first = true; // 标记是否是第一次进入循环，用于清空屏幕
    while (true) {
        logger << is_first << LogString::MENU; // 打印主菜单，如果是第一次进入则清空屏幕
        is_first = false; // 之后不再清空屏幕
        cin >> option; // 读取用户输入的选项
        if (option == "0") {
            // 退出系统
            Log::printLog(LogString::Exit, true); // 清空屏幕并打印退出信息
            logger << true << LogString::Exit; // 再次清空屏幕并打印退出信息（冗余，可优化）
            return 0; // 退出程序
        }
        if (option == "1") {
            // 登录功能
            bool is_first_2 = true; // 标记是否是第一次进入登录循环，用于清空屏幕
            while (true) {
                logger << is_first_2 << LogString::Login; // 打印登录提示，如果是第一次进入则清空屏幕
                is_first_2 = false; // 之后不再清空屏幕
                string username, password; // 用户名和密码
                logger << LogString::Username; // 打印输入用户名提示
                cin >> username; // 读取用户名
                if (username == "-") {
                    is_first = true; // 设置为第一次进入主菜单循环，以便返回主菜单时清空屏幕
                    break; // 退出登录循环
                }
                logger << LogString::Password; // 打印输入密码提示
                cin >> password; // 读取密码
                if (status.login(username, password)) {
                    // 如果登录成功
                    if (status.getStatus() == Status::ADMIN) {
                        // 如果是管理员
                        adminFunc(status); // 调用管理员功能函数
                    }
                    else if (status.getStatus() == Status::LOGIN) {
                        // 如果是普通用户
                        userFunc(status); // 调用用户功能函数
                    }
                    status.logout(); // 用户或管理员退出登录
                    is_first = true; // 设置为第一次进入主菜单循环，以便返回主菜单时清空屏幕
                    break; // 退出登录循环
                }
            }
        }
        else if (option == "2") {
            // 注册功能
            logger << true << LogString::Register; // 清空屏幕并打印注册提示
            string username, password; // 用户名和密码
            logger << LogString::Username; // 打印输入用户名提示
            cin >> username; // 读取用户名
            logger << LogString::Password; // 打印输入密码提示
            cin >> password; // 读取密码
            status.registerUser(username, password); // 调用 Status 类的 registerUser 方法注册用户
        }
        else if (option == "3") {
            // 浏览产品功能
            status.printList(); // 打印产品列表
            while (true) {
                string option_2; // 用户输入的选项
                cin >> option_2; // 读取用户输入的选项
                if (option_2 == "0") {
                    is_first = true; // 设置为第一次进入主菜单循环，以便返回主菜单时清空屏幕
                    break; // 退出当前循环
                }
            }
        }
        else if (option == "4") {
            // 搜索产品功能
            string name; // 要搜索的产品名称
            logger << true << LogString::SearchProduct; // 清空屏幕并打印搜索产品提示
            cin >> name; // 读取要搜索的产品名称
            status.searchProduct(name); // 调用 Status 类的 searchProduct 方法搜索产品
        }
        else if (option == "5") {
            // 模糊搜索产品功能
            string name; // 要模糊搜索的产品名称
            logger << true << LogString::SearchProduct; // 清空屏幕并打印搜索产品提示
            cin >> name; // 读取要模糊搜索的产品名称
            status.fuzzySearchProduct(name); // 调用 Status 类的 fuzzySearchProduct 方法模糊搜索产品
        }
        else if (option == "6" || option == "7" || option == "8" || option == "9") {
            // 购物车和订单相关功能，需要登录后才能访问
            logger << true << LogString::NeedLogin; // 清空屏幕并打印需要登录提示
        }
        else {
            // 无效输入
            logger << true << LogString::Invalid; // 清空屏幕并打印无效输入提示
        }
    }
}
