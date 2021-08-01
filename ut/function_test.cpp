#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>

#include "tl/component/function_traits.hh"
#include "tl/component/logger.h"
class test {
 public:
  void info() {}
  void operator()() {}
};
int f() { return 0; }

struct Foo {
  Foo(int num) : num_(num) {}
  void print_add(int i) const { std::cout << num_ + i << '\n'; }
  int num_;
};

void print_num(int i) { std::cout << i << '\n'; }

struct PrintNum {
  void operator()(int i) const { std::cout << i << '\n'; }
};

int test_function() {
  // 存储自由函数
  std::function<void(int)> f_display = print_num;
  f_display(-9);

  // 存储 lambda
  std::function<void()> f_display_42 = []() { print_num(42); };
  f_display_42();

  // 存储到 std::bind 调用的结果
  std::function<void(int)> f_display_31337 =
      std::bind(print_num, std::placeholders::_1);
  f_display_31337(31337);
  // bind 绑定 print_num(int) 的第一个参数为 31338
  std::function<void()> f_display_31338 = std::bind(print_num, 31338);
  f_display_31338();

  // 存储到成员函数的调用
  std::function<void(Foo&, int)> f_add_display = &Foo::print_add;
  Foo foo(314159);
  f_add_display(foo, 1);
  // f_add_display(314159, 1); // 这里会隐式构造，const rvlaue

  // 存储到数据成员访问器的调用
  std::function<int(Foo const&)> f_num = &Foo::num_;
  std::cout << "num_: " << f_num(foo) << '\n';
  std::function<int()> f_num100 = []() { return 100; };
  std::cout << "cout 100 func : " << f_num100() << std::endl;

  // 存储到成员函数及对象的调用
  using std::placeholders::_1;
  std::function<void(int)> f_add_display2 = std::bind(&Foo::print_add, foo, _1);
  f_add_display2(2);

  // 存储到成员函数和对象指针的调用
  std::function<void(int)> f_add_display3 =
      std::bind(&Foo::print_add, &foo, _1);
  f_add_display3(3);

  // 存储到函数对象的调用
  std::function<void(int)> f_display_obj = PrintNum();
  f_display_obj(18);
  return 0;
}

int main() {
  using namespace tl::blacklist;
  auto f1 = [](int a, double b, std::string c) {
    LOG_ERROR("a = {}, b = {}, c = {}", a, b, c);
    return a * b;
  };
  LOG_INFO("f1 result = {}", f1(10, 23.4, "mutiple"));
  using f1type = function_traits<decltype(f1)>::return_type;
  using tuptype1 = function_traits<decltype(f1)>::args_as_tuple;
  tuptype1 t1;
  LOG_INFO("tuptype1 type = {}, size = {}", typeid(t1).name(),
           std::tuple_size<tuptype1>{});
  if constexpr (std::is_same<f1type, decltype(f1)>::value) {
    LOG_INFO("type same");
  } else {
    LOG_INFO("type different");
    LOG_INFO("f1type type name : {}", typeid(f1type{}).name());
    LOG_INFO("f1 type name : {}", typeid(f1).name());
    std::function<double(int, int, int)> f2;
    LOG_INFO("decltypef1 type name : {}", typeid(f2).name());
  }

  function_traits<decltype(f1)> df1;
  LOG_INFO("xixi");
  function_traits<void()> df2;
  LOG_INFO("xinxin");
  function_traits<decltype(&test::info)> df3;
  LOG_INFO("xinxin1");
  function_traits<decltype(f)> df4;
  LOG_INFO("xinxin2");
  function_traits<decltype(test())> df5;
  LOG_INFO("xinxin3");
  function_traits<decltype(std::function<void(int, int, double)>{})> df6;
  function_traits<decltype(f1)>::get_type<1> a{};
  function_traits<void(int, long, double)>::get_type<1> b{};
  // LOG_INFO("f1type get<1> : {}", a);
  // LOG_INFO("<void(int,int,double)> get<1> : {}, type is {}", b,
  // typeid(b).name());
  test_function();
  return 0;
}
