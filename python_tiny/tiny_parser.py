import tiny_token
from tiny_lexer import TinyLexer
from tiny_ast_tree import TinyASTNodeRoot
from tiny_ast_tree import TinyASTNodeStmtIf
from tiny_ast_tree import TinyASTNodeStmtAssign
from tiny_ast_tree import TinyASTNodeStmtRepeat
from tiny_ast_tree import TinyASTNodeStmtRead
from tiny_ast_tree import TinyASTNodeStmtWrite
from tiny_ast_tree import TinyASTNodeExpIdentifier
from tiny_ast_tree import TinyASTNodeExpNumber
from tiny_ast_tree import TinyASTNodeExpOperation


class TinyParser():
    def __init__(self):
        self._lexer = None
        self._value_stack = None

    def parse(self, lexer):
        self._lexer = lexer
        self._lexer.next()

        return self._program()

    def _is_comparision_operator(self, token):
        return (token==tiny_token.EQUAL) or (token==tiny_token.LESS_THAN)

    def _is_add_operator(self, token):
        return (token==tiny_token.PLUS) or (token==tiny_token.MINUS)

    def _is_multiply_operator(self, token):
        return (token==tiny_token.MULTIPLY) or (token==tiny_token.DIVIDE)

    def _match(self, expected_token):
        lexer = self._lexer
        token = lexer.token()

        if token!=expected_token:
            raise Exception()
        lexer.next()

    # ->stmt_sequence
    def _program(self):
        root = TinyASTNodeRoot()
        root.first_stmt = self._stmt_sequence()
        return root

    # ->statement {';' statement}
    def _stmt_sequence(self):
        lexer = self._lexer

        first = self._statment()
        previous = first

        while lexer.token()==tiny_token.SEMICOLON:
            self._match(tiny_token.SEMICOLON)
            current = self._statment()
            previous.next = current
            previous = current

        return first

    # ->if_stmt | repeat_stmt | assign_stmt | read_stmt | write_stmt
    def _statment(self):
        lexer = self._lexer
        token = lexer.token()
        stmt = None

        if token==tiny_token.IF:
            stmt = self._if_stmt()
        elif token==tiny_token.REPEAT:
            stmt = self._repeat_stmt()
        elif token==tiny_token.IDENTIFIER:
            stmt = self._assign_stmt()
        elif token==tiny_token.READ:
            stmt = self._read_stmt()
        elif token==tiny_token.WRITE:
            stmt = self._write_stmt()
        else:
            raise Exception()

        return stmt

    # ->IF exp THEN stat_sequence [ELSE stmt-sequence] END
    def _if_stmt(self):
        lexer = self._lexer
        if_stmt = TinyASTNodeStmtIf()

        self._match(tiny_token.IF)
        if_stmt.if_part = self._exp()

        self._match(tiny_token.THEN)
        if_stmt.then_part = self._stmt_sequence()

        if lexer.token()==tiny_token.ELSE:
            self._match(tiny_token.ELSE)
            if_stmt.else_part = self._stmt_sequence()

        self._match(tiny_token.END)

        return if_stmt

    # ->REPEAT stat_sequence UNTIL exp
    def _repeat_stmt(self):
        repeat_stmt = TinyASTNodeStmtRepeat()

        self._match(tiny_token.REPEAT)
        repeat_stmt.body = self._stmt_sequence()

        self._match(tiny_token.UNTIL)
        repeat_stmt.test = self._exp()

        return repeat_stmt

    # ->IDENTIFIER ':=' exp
    def _assign_stmt(self):
        assign_stmt = TinyASTNodeStmtAssign()

        assign_stmt.attr_name = self._identifier().attr_name
        self._match(tiny_token.ASSIGN)
        assign_stmt.exp = self._exp()
        
        return assign_stmt

    # ->READ identifier
    def _read_stmt(self):
        read_stmt = TinyASTNodeStmtRead()

        self._match(tiny_token.READ)
        read_stmt.attr_name = self._identifier().attr_name

        return read_stmt

    # ->WRITE exp
    def _write_stmt(self):
        write_stmt = TinyASTNodeStmtWrite()

        self._match(tiny_token.WRITE)
        write_stmt.attr_name = self._identifier().attr_name

        return write_stmt

    # ->simple_exp [COMPARISION_OP simple_exp]
    def _exp(self):
        left = self._simple_exp()

        token = self._lexer.token()
        if self._is_comparision_operator(token):
            op_exp = TinyASTNodeExpOperation()
            op_exp.attr_token = self._comparision_op()
            op_exp.left = left
            op_exp.right = self._simple_exp()
            left = op_exp

        return left

    # ->'<' | '='
    def _comparision_op(self):
        token = self._lexer.token()
        
        if not self._is_comparision_operator(token):
            raise Exception()

        self._match(token)

        return token

    # ->term {ADD_OP term}
    def _simple_exp(self):
        lexer = self._lexer

        left = self._term()
        while True:
            token = lexer.token()
            if self._is_add_operator(token):
                op_exp = TinyASTNodeExpOperation()
                op_exp.left = left
                op_exp.attr_token= self._add_op()
                op_exp.right = self._term()
                left = op_exp
            else:
                break
        
        return left

    # ->'+' |'-'
    def _add_op(self):
        token = self._lexer.token()
        
        if not self._is_add_operator(token):
            raise Exception()

        self._match(token)

        return token

    # ->factor {MUL_OP factor}
    def _term(self):
        lexer = self._lexer
        
        left = self._factor()
        while True:
            token = lexer.token()
            if self._is_multiply_operator(token):
                op_exp = TinyASTNodeExpOperation()
                op_exp.left = left
                op_exp.attr_token= self._mul_op()
                op_exp.right = self._factor()
                left = op_exp
            else:
                break

        return left

    # ->'*' | '/'
    def _mul_op(self):
        token = self._lexer.token()
        
        if not self._is_multiply_operator(token):
            raise Exception()

        self._match(token)

        return token

    # ->'(' exp ')' | NUMBER | IDENTIFIER
    def _factor(self):
        token = self._lexer.token()
        value = self._lexer.value()
        factor = None

        if token==tiny_token.LEFT_BRACE:
            self._match(tiny_token.LEFT_BRACE)
            factor = self._exp()
            self._match(tiny_token.RIGHT_BRACE)
        elif token==tiny_token.NUMBER:
            factor = TinyASTNodeExpNumber()
            factor.attr_value = value
            self._match(tiny_token.NUMBER)
        elif token==tiny_token.IDENTIFIER:
            factor = self._identifier()
        else:
            raise Exception()

        return factor

    def _identifier(self):
        id_exp = TinyASTNodeExpIdentifier()
        id_exp.attr_name = self._lexer.value()
        self._match(tiny_token.IDENTIFIER)

        return id_exp
