# Tiny脚本语言解释器

1. 安装
    - clone或下载仓库到本地， 用vs2019编译项目`tiny`和`tiny_console`

2. 使用
    - 运行`tiny_console`项目（C++实现的解释器） 或 运行`python_tiny`项目（python实现的解释器）
    - 运行后， 输入脚本语言允许的语句， 就会输出执行结果。C++版本会还会输出AST和翻译后的VM代码。

3. 示例
                
                i:=1; j:=1; repeat i:=i+1; j:=j+i until i=10; write j   
    - 输入上述代码， 虚拟机会计算1+2+3...+10并最后输出结果55

4. 词法

                _pattern = r'(?P<_0_white_space>[\s]+)|' \
                    r'(?P<_0_newline>\n)|' \
                    r'(?P<_1>if)|' \
                    r'(?P<_2>then)|' \
                    r'(?P<_3>else)|' \
                    r'(?P<_4>end)|' \
                    r'(?P<_5>repeat)|' \
                    r'(?P<_6>until)|' \
                    r'(?P<_7>read)|' \
                    r'(?P<_8>write)|' \
                    r'(?P<_101>[+])|' \
                    r'(?P<_102>[-])|' \
                    r'(?P<_103>[*])|' \
                    r'(?P<_104>[/])|' \
                    r'(?P<_105>[=])|' \
                    r'(?P<_106>[<])|' \
                    r'(?P<_107>[(])|' \
                    r'(?P<_108>[)])|' \
                    r'(?P<_109>[;])|' \
                    r'(?P<_110>[:][=])|' \
                    r'(?P<_201>[\d]+)|' \
                    r'(?P<_202>[\w]+)|'

5. 语法

                    program ->stmt_sequence
                    
                    stmt_sequence ->statement {';' statement}
                    
                    stmt ->if_stmt | repeat_stmt | assign_stmt | read_stmt | write_stmt

                    stmt_if ->IF exp THEN stat_sequence [ELSE stmt-sequence] END

                    stmt_repeat ->REPEAT stat_sequence UNTIL exp

                    stmt_assign ->IDENTIFIER ':=' exp

                    stmt_read ->READ identifier

                    stmt_write ->WRITE exp

                    exp ->simple_exp [COMPARISION_OP simple_exp]

                    COMPARISION_OP ->'<' | '='

                    simple_exp ->term {ADD_OP term}

                    ADD_OP ->'+' |'-'

                    term ->factor {MUL_OP factor}

                    MUL_OP ->'*' | '/'

                    factor ->'(' exp ')' | NUMBER | IDENTIFIER

6. 开发状态
    - 语法功能过于简单
    - 完全没有错误处理
    - 部分词法、语法设计不合理

