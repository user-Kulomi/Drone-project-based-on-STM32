#ifndef COM_DEBUG_H
#define COM_DEBUG_H

#include "usart.h"
#include "stdio.h"

//日志输出打印非常耗时，通过比特率可计算出打印10字节需要耗时大概1ms，会严重影响飞机飞行
//所以需要设置一个日志打印开关：
#define DEBUG_PRINTF_ON 1

#ifdef DEBUG_PRINTF_ON
//实现带有文件名和行号的日志打印（写成宏定义）：
#define debug_printf(format, ...) printf("[%s:%d]" format, __FILE__, __LINE__, ##__VA_ARGS__)
//解释：format代表用户要传入的带有占位符的字符串，诸如"a = %d\n, b = %s\n"此类，__FILE__是C标准内置宏
//__VA_ARGS__ 也是一种宏，用于将可变参数打包，代表format里面所有占位符代表的值，##代表即使不传参数也不会报逗号错误
//示例：debug_printf("a = %d\n, b = %s\n", 10, "hello")等同于printf("[%s:%d]a = %d\n, b = %s\n",__FILE__,__LINE__,10,"hello")
//占位符%d的值是__LINE__(当前源代码的行号),而方括号后面的占位符%d和%s的值分别是10和"hello"
#else
//如果开关关闭，则通过将宏函数定义为空操作，关闭日志打印
#define debug_printf(format, ...) (void)0
#endif // DEBUG_PRINTF_ON

#endif // COM_DEBUG_H
