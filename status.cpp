#include "status.h" // 包含 Status 类头文件
#include "user.h" // 包含 User 和 Admin 类头文件
#include <iostream> // 包含标准输入输出流库
#include <format> // 包含格式化库
#include "log.h" // 包含日志库
#include "productList.h" // 包含产品列表头文件

// Status 类的构造函数
Status::Status(): statistics(orderList, productList) {
    // 初始化系统状态为未登录
    this->status = NOT_LOGIN;
    // 初始化当前登录用户名为控
    this->currentUser = "";
    // 初始化当前登录用户基类指针为 nullptr
    this->currentUserBase = nullptr;
}

// 用户登录函数
// username: 用户名
// password: 密码
// 返回 true 如果登录成功，否则返回 false
bool Status::login(const std::string& username, const std::string& password) {
    // 尝试以普通用户身份登录
    auto res = this->user.login(username, password);
    // 如果登录成功（不是 ERROR）
    if (res != UserBase::ERROR) {
        // 设置系统状态为已登录（普通用户）
        this->status = LOGIN;
        // 设置当前登录用户名
        this->currentUser = username;
        // 获取普通用户对象的基类指针
        this->currentUserBase = user.getBasePtr();
        // 返回 true 表示登录成功
        return true;
    }
    // 如果普通用户登录失败，尝试以管理员身份登录
    res = this->admin.login(username, password);
    // 如果登录成功（不是 ERROR）
    if (res != UserBase::ERROR) {
        // 设置系统状态为已登录（管理员）
        this->status = ADMIN;
        // 设置当前登录用户名
        this->currentUser = username;
        // 获取管理员对象的基类指针
        this->currentUserBase = admin.getBasePtr();
        // 返回 true 表示登录成功
        return true;
    }
    // 如果所有登录尝试都失败，返回 false
    return false;
}

// 用户登出函数
void Status::logout() {
    // 设置系统状态为未登录
    this->status = NOT_LOGIN;
    // 清空当前登录用户名
    this->currentUser = "";
    // 将当前登录用户基类指针设置为 nullptr
    this->currentUserBase = nullptr;
}

// 修改当前用户的密码函数
// password: 新密码
// 返回 true 如果修改成功，否则返回 false
bool Status::changePassword(const std::string& password) const {
    // 调用当前登录用户对象的 changePassword 方法修改密码
    return this->currentUserBase->changePassword(currentUser, password);
}

// 获取当前系统状态函数
// 返回 StatusType 枚举值
Status::StatusType Status::getStatus() const {
    // 返回当前系统状态
    return status;
}

// 获取当前登录用户名函数
// 返回用户名字符串
std::string Status::getCurrentUser() const {
    // 返回当前登录用户名
    return currentUser;
}

// 注册新用户函数
// username: 用户名
// password: 密码
// 返回 true 如果注册成功，否则返回 false
bool Status::registerUser(const std::string& username, const std::string& password) {
    // 调用普通用户对象的 registerUser 方法注册用户
    return this->user.registerUser(username, password);
}

// 获取当前登录用户对象指针函数
// 返回 UserBase 指针
UserBase* Status::getUser() const {
    // 返回当前登录用户基类指针
    return this->currentUserBase;
}

// 打印产品列表函数
void Status::printList() {
    // 打印满减优惠券信息
    this->printMJCoupon();
    // 打印产品列表，并应用优惠券信息
    this->productList.printList(this->coupon);
}

// 添加产品函数（管理员权限）
// name: 产品名称
// description: 产品描述
// price: 产品价格
// inventory: 产品库存
// category: 产品类别
void Status::addProduct(const std::string& name, const std::string& description, double price, int inventory,
                        const std::string& category) {
    // 检查当前用户是否为管理员
    if (this->status == ADMIN) {
        // 如果是管理员，调用 Admin 类的 addProduct 方法添加产品
        Admin::addProduct(name, description, price, inventory, category, this->productList);
    }
    else {
        // 如果不是管理员，记录权限不足日志
        // Log::printLog(LogString::PermissionDenied, true);
        logger << true << LogString::PermissionDenied;
    }
}

// 编辑产品信息函数（管理员权限）
// name: 产品名称
// description: 新的产品描述
// price: 新的产品价格
// inventory: 新的产品库存
// category: 新的产品类别
void Status::editProduct(const std::string& name, const std::string& description, const double price,
                         const int inventory,
                         const std::string& category) {
    // 检查当前用户是否为管理员
    if (this->status == ADMIN) {
        // 如果是管理员，调用 Admin 类的 editProduct 方法编辑产品
        Admin::editProduct(name, description, price, inventory, category, this->productList);
    }
    else {
        // 如果不是管理员，记录权限不足日志
        // Log::printLog(LogString::PermissionDenied, true);
        logger << true << LogString::PermissionDenied;
    }
}

// 检查产品是否存在函数
// name: 产品名称
// 返回 true 如果产品存在，否则返回 false
bool Status::hasProduct(const std::string& name) const {
    // 调用产品列表对象的 hasProduct 方法检查产品是否存在
    return this->productList.hasProduct(name);
}

// 删除产品函数（管理员权限）
// name: 产品名称
void Status::deleteProduct(const std::string& name) {
    // 检查当前用户是否为管理员
    if (this->status == ADMIN) {
        // 如果是管理员，调用 Admin 类的 deleteProduct 方法删除产品
        Admin::deleteProduct(name, this->productList);
    }
    else {
        // 如果不是管理员，记录权限不足日志
        // Log::printLog(LogString::PermissionDenied, true);
        logger << true << LogString::PermissionDenied;
    }
}

// 搜索产品函数（精确匹配）
// name: 产品名称
void Status::searchProduct(const std::string& name) {
    // 调用产品列表对象的 searchProduct 方法搜索产品，并应用优惠券信息
    this->productList.searchProduct(name, coupon);
}

// 模糊搜索产品函数
// name: 产品名称关键字
void Status::fuzzySearchProduct(const std::string& name) {
    // 调用产品列表对象的 fuzzySearchProduct 方法进行模糊搜索，并应用优惠券信息
    this->productList.fuzzySearchProduct(name, coupon);
}

// 添加产品到购物车函数
// name: 产品名称
// quantity: 数量
void Status::addCart(const std::string& name, const int quantity) {
    // 检查数量是否有效（大于 0）
    if (quantity <= 0) {
        // 如果数量无效，记录无效输入日志
        logger << true << LogString::Invalid;
        // 返回
        return;
    }
    // 检查产品是否存在
    if (!productList.hasProduct(name)) {
        // 如果产品不存在，记录产品不存在日志
        logger << true << LogString::ProductNotExist;
        // 返回
        return;
    }
    // 检查当前用户是否已登录（普通用户）
    if (this->status == LOGIN) {
        // 如果已登录，调用普通用户对象的 addCart 方法添加产品到购物车
        this->user.addCart(currentUser, name, quantity);
    }
    else {
        // 如果未登录或管理员，记录权限不足日志
        logger << true << LogString::PermissionDenied;
    }
}

// 编辑购物车产品数量函数
// name: 产品名称
// quantity: 新的数量
void Status::editCart(const std::string& name, const int quantity) {
    // 检查新数量是否有效（小于等于 0）
    if (quantity <= 0) {
        // 如果新数量无效，记录提示用户是否删除该产品
        logger << true << LogString::WantToDelete;
        // 用于读取用户输入
        std::string op;
        // 读取用户输入
        std::cin >> op;
        // 如果用户选择是 (y/Y)
        if (op == "y" || op == "Y") {
            // 调用 deleteCart 方法删除该产品
            this->deleteCart(name);
            // 返回
            return;
        }
        // 如果用户选择否，记录日志（表示用户取消操作）
        logger << true;
        // 返回
        return;
    }
    // 检查当前用户是否已登录（普通用户）
    if (this->status == LOGIN) {
        // 如果已登录，调用普通用户对象的 editCart 方法编辑购物车产品数量
        this->user.editCart(currentUser, name, quantity);
    }
    else {
        // 如果未登录或管理员，记录权限不足日志
        logger << true << LogString::PermissionDenied;
    }
}

// 从购物车删除产品函数
// name: 产品名称
void Status::deleteCart(const std::string& name) {
    // 检查当前用户是否已登录（普通用户）
    if (this->status == LOGIN) {
        // 如果已登录，调用普通用户对象的 deleteCart 方法从购物车删除产品
        this->user.deleteCart(currentUser, name);
    }
    else {
        // 如果未登录或管理员，记录权限不足日志
        logger << true << LogString::PermissionDenied;
    }
}

// 清空购物车函数
void Status::clearCart() {
    // 检查当前用户是否已登录（普通用户）
    if (this->status == LOGIN) {
        // 如果已登录，调用普通用户对象的 clearCart 方法清空购物车
        this->user.clearCart(currentUser);
    }
    else {
        // 如果未登录或管理员，记录权限不足日志
        logger << true << LogString::PermissionDenied;
    }
}

// 打印购物车内容函数
void Status::printCart() {
    // 检查当前用户是否已登录（普通用户）
    if (this->status == LOGIN) {
        double sumprice = 0; // 初始化总价格
        int sumqua = 0; // 初始化总数量
        // 获取当前用户的购物车列表
        auto cart = this->user.getCartList(currentUser);
        // 记录购物车头部日志
        logger << true << LogString::CartHead;
        // 遍历购物车中的产品
        for (const auto& [name, quantity] : cart) {
            // 打印产品信息（名称、类别、描述、价格、数量）
            logger << Log::paddingEnd(15, name) << Log::paddingEnd(10, productList.getCategory(name)) <<
                Log::paddingEnd(20, productList.getDescription(name)) << Log::paddingEnd(
                    10, std::format("{:.2f}", productList.getPrice(name))) << Log::paddingEnd(
                    10, std::to_string(quantity)) << "\n";
            // 累加总数量
            sumqua += quantity;
            // 累加总价格
            sumprice += productList.getPrice(name) * quantity;
        }
        // 记录购物车尾部、总数量和总价格日志
        logger << LogString::CartTail << LogString::SumQua << sumqua << "\t" << LogString::SumPrice << sumprice << "\n"
            << LogString::ExitList;
    }
    else {
        // 如果未登录或管理员，记录权限不足日志
        logger << true << LogString::PermissionDenied;
    }
}

// 创建订单函数
// items: 订单包含的商品列表
// address: 收货地址
// 返回 true 如果订单创建成功，否则返回 false
bool Status::addOrder(const std::vector<std::pair<std::string, int>>& items, const std::string& address) {
    // 检查当前用户是否已登录（普通用户）
    if (this->status == LOGIN) {
        bool enough = true; // 标志，指示库存是否充足
        // 遍历订单中的商品项
        for (const auto& [name, quantity] : items) {
            // 检查产品是否存在
            if (!this->productList.hasProduct(name)) {
                // 如果产品不存在，记录产品不存在日志
                logger << true << LogString::ProductNotExist;
                enough = false; // 设置库存不足标志
                break; // 中断循环
            }
            // 检查产品库存是否充足
            if (this->productList.getInventory(name) < quantity) {
                // 如果库存不足，记录库存不足日志
                logger << true << LogString::ProductNotEnough;
                logger << true << LogString::ProductNotEnough; // 再次记录，可能是为了强调
                enough = false; // 设置库存不足标志
                break; // 中断循环
            }
        }
        // 如果库存充足
        if (enough) {
            // 调用订单列表对象的 addOrder 方法创建订单
            if (orderList.addOrder(this->currentUser, items, address, coupon)) {
                // 如果订单创建成功，减少产品库存
                for (const auto& [name, quantity] : items) {
                    this->productList.decreaseInventory(name, quantity);
                }
                // 记录添加订单成功日志
                logger << true << LogString::AddOrderSuccess;
                // 返回 true 表示订单创建成功
                return true;
            }
        }
        // 如果库存不足或订单创建失败，记录日志
        logger << true;
        // 返回 false 表示订单创建失败
        return false;
    }
    // 如果未登录或管理员，记录权限不足日志
    logger << true << LogString::PermissionDenied;
    // 返回 false
    return false;
}

// 从购物车创建订单函数
// address: 收货地址
void Status::addOrderFromCart(const std::string& address) {
    // 检查当前用户是否已登录（普通用户）
    if (this->status == LOGIN) {
        // 检查购物车是否为空
        if (this->user.getCartList(this->currentUser).empty()) {
            // 如果购物车为空，记录购物车为空日志
            logger << true << LogString::EmptyCart;
            // 返回
            return;
        }
        // 调用 addOrder 方法从购物车创建订单
        if (addOrder(this->user.getCartList(this->currentUser), address)) {
            // 如果订单创建成功，清空购物车
            this->user.clearCart(this->currentUser);
        }
    }
    else {
        // 如果未登录或管理员，记录权限不足日志
        logger << true << LogString::PermissionDenied;
    }
}

// 删除订单函数（仅可以删除已收货的订单）
// orderId: 订单ID
void Status::deleteOrder(const std::string& orderId) {
    // 检查当前用户是否已登录（普通用户）且是订单的拥有者
    if (this->status == LOGIN && this->currentUser == this->orderList.getOrderUser(orderId)) {
        // 检查订单是否存在
        if (!this->orderList.hasOrder(orderId)) {
            // 如果订单不存在，记录订单不存在日志
            logger << true << LogString::OrderNotExist;
            // 返回
            return;
        }
        // 检查订单状态是否为已收货
        if (this->orderList.getOrderStatus(orderId) == Order::DELIVERED) {
            // 如果是已收货状态，调用订单列表对象的 deleteOrder 方法删除订单
            orderList.deleteOrder(orderId);
            // 记录删除订单成功日志
            logger << true << LogString::DeleteOrderSuccess;
        }
        else {
            // 如果不是已收货状态，记录删除订单失败日志
            logger << true << LogString::DeleteOrderFail;
        }
    }
    else {
        // 如果未登录、管理员或不是订单拥有者，记录权限不足日志
        logger << true << LogString::PermissionDenied;
    }
}

// 修改订单状态函数（管理员权限）
// orderId: 订单ID
// status: 新的订单状态
void Status::changeOrderStatus(const std::string& orderId, const Order::orderStatus status) {
    // 检查当前用户是否为管理员
    if (this->status == ADMIN) {
        // 检查订单是否存在
        if (!this->orderList.hasOrder(orderId)) {
            // 如果订单不存在，记录订单不存在日志
            logger << true << LogString::OrderNotExist;
            // 返回
            return;
        }
        // 调用订单列表对象的 changeOrderStatus 方法修改订单状态
        this->orderList.changeOrderStatus(orderId, status, 0); // 这里的 0 可能表示没有额外的参数
        // 记录修改订单状态成功日志
        logger << true << LogString::ChangeOrderStatusSuccess;
    }
    else {
        // 如果不是管理员，记录权限不足日志
        logger << true << LogString::PermissionDenied;
    }
}

// 取消订单函数（仅可以取消待发货的订单）
// orderId: 订单ID
void Status::cancelOrder(const std::string& orderId) {
    // 检查当前用户是否已登录（普通用户）且是订单的拥有者
    if (this->status == LOGIN && this->currentUser == this->orderList.getOrderUser(orderId)) {
        // 检查订单是否存在
        if (!this->orderList.hasOrder(orderId)) {
            // 如果订单不存在，记录订单不存在日志
            logger << true << LogString::OrderNotExist;
            // 返回
            return;
        }
        // 检查订单状态是否为待发货
        if (this->orderList.getOrderStatus(orderId) == Order::PENDING) {
            // 如果是待发货状态，修改订单状态为已取消
            this->orderList.changeOrderStatus(orderId, Order::CANCELLED, 0); // 这里的 0 可能表示没有额外的参数
            // 增加产品库存（将订单中的产品数量加回库存）
            for (const auto& [name, quantity] : this->orderList.getOrderItems(orderId)) {
                this->productList.increaseInventory(name, quantity);
            }
            // 记录取消订单成功日志
            logger << true << LogString::CancelOrderSuccess;
        }
        else {
            // 如果不是待发货状态，记录取消订单失败日志
            logger << true << LogString::CancelOrderFail;
        }
    }
    else {
        // 如果未登录、管理员或不是订单拥有者，记录权限不足日志
        logger << true << LogString::PermissionDenied;
    }
}

// 打印当前用户订单函数
void Status::printOrder() {
    // 检查当前用户是否已登录（普通用户）
    if (this->status == LOGIN) {
        // 获取当前用户的订单列表
        const auto order = this->orderList.getOrderList(this->currentUser);
        // 记录订单头部日志
        logger << true << LogString::OrderHead;
        // 遍历用户订单ID
        for (const auto& orderId : order) {
            // 打印单个订单信息
            orderList.printOrder(orderId);
            // 打印分隔线
            logger << LogString::Hr;
        }
    }
    else {
        // 如果未登录或管理员，记录权限不足日志
        logger << true << LogString::PermissionDenied;
    }
}

// 修改订单地址函数（仅可以修改待发货的订单）
// orderId: 订单ID
// address: 新的收货地址
void Status::changeOrderAddress(const std::string& orderId, const std::string& address) {
    // 检查当前用户是否已登录（普通用户）且是订单的拥有者
    if (this->status == LOGIN && this->currentUser == this->orderList.getOrderUser(orderId)) {
        // 检查订单是否存在
        if (!this->orderList.hasOrder(orderId)) {
            // 如果订单不存在，记录订单不存在日志
            logger << true << LogString::OrderNotExist;
            // 返回
            return;
        }
        // 检查订单状态是否为待发货
        if (this->orderList.getOrderStatus(orderId) == Order::PENDING) {
            // 如果是待发货状态，调用订单列表对象的 setOrderAddress 方法修改订单地址
            if (orderList.setOrderAddress(orderId, address)) {
                // 如果修改成功，记录修改地址成功日志
                logger << true << LogString::EditAddressSuccess;
            }
            else {
                // 如果修改失败，记录修改地址失败日志
                logger << true << LogString::EditAddressFail;
            }
        }
        else {
            // 如果不是待发货状态，记录修改地址失败日志
            logger << true << LogString::EditAddressFail;
        }
    }
    // 如果未登录、管理员或不是订单拥有者，不执行任何操作（权限不足已在其他地方处理）
}

// 打印所有订单函数（管理员权限）
void Status::printAllOrders() const {
    // 检查当前用户是否为管理员
    if (this->status == ADMIN) {
        // 记录日志（可能用于标记开始打印所有订单）
        logger << true;
        // 调用订单列表对象的 printAllOrders 方法打印所有订单
        orderList.printAllOrders();
        // 记录日志（可能用于标记结束打印所有订单）
        logger << LogString::ExitList;
    }
    else {
        // 如果不是管理员，记录权限不足日志
        logger << true << LogString::PermissionDenied;
    }
}

// 打印满减优惠券信息函数
void Status::printMJCoupon() const {
    // 记录日志（可能用于标记开始打印优惠券信息）
    logger << true;
    // 获取满减优惠券的最低消费金额
    const double mini = this->coupon.getMinimum();
    // 获取满减优惠券的折扣金额
    if (const double disc = this->coupon.getDiscount(); mini != 0 && disc != 0) {
        // 如果存在有效的满减优惠券，打印优惠信息
        logger << "spend " << mini << " or more to get " << disc << " off\n";
    }
}

// 添加优惠券函数
// name: 优惠券名称（按名称添加时使用）
// category: 优惠券类别（按类别添加时使用）
// discount: 折扣比例或金额
// daysToAdd: 有效期（天）
void Status::add_coupon(const std::string& name, const std::string& category, double discount, int daysToAdd) {
    // 检查当前用户是否为管理员
    if (this->status == ADMIN) {
        // 如果是管理员，调用优惠券对象的 addCoupon 方法添加优惠券
        this->coupon.addCoupon(name, category, discount, daysToAdd);
        // 记录添加百分比优惠券成功日志
        logger << true << LogString::AddPercentCouponSuccess;
    }else {
        // 如果不是管理员，记录权限不足日志
        logger << true << LogString::PermissionDenied;
    }
}

// 设置满减优惠券函数
// minimum: 最低消费金额
// discount: 折扣金额
void Status::setMJCoupon(const double minimum, const double discount) {
    // 检查当前用户是否为管理员
    if (this->status == ADMIN) {
        // 如果是管理员，调用优惠券对象的 setMJDiscount 方法设置满减优惠券
        this->coupon.setMJDiscount(minimum, discount);
    }
}

// 获取统计信息并打印函数
void Status::get_statistics() const {
    // 调用统计信息对象的 printStatistics 方法打印统计信息
    statistics.printStatistics();
}
