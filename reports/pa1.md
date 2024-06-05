# PA1必答题
## RTFM (RISCV)
https://riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf
1. Page23 Chapter2.2
2. Load upper immediate Page26
3. https://riscv.org/wp-content/uploads/2016/07/Tue0900_RISCV-20160712-Interrupts.pdf Page11
## shell命令
统计行数
```shell
find . -type f \( -name "*.h" -o -name "*.c" \) -exec cat {} + | wc -l
```
切换到PA0:
```shell
git checkout pa0
```

### gcc命令
Wall用于启用所有警告，警告未使用的变量、为初始化的变量、不可到达的代码等等。Werror用于将所有警告看作错误。使用它们有利于让我们提前解决一些未定义的错误，保证代码编写质量。
