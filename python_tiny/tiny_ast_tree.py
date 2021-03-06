import tiny_ast_node_kind
import tiny_ast_stmt_kind
import tiny_ast_exp_kind


class TinyASTNode:
    def __init__(self, node_kind):
        self.node_kind = node_kind

    def first_kind(self):
        return self.node_kind

    def second_kind(self):
        return None

    def sibling(self):
        return None

    def children(self):
        return ()

class TinyASTNodeRoot(TinyASTNode):
    def __init__(self):
        self.node_kind = tiny_ast_node_kind.ROOT
        self.first_stmt = None

    def children(self):
        return (self.first_stmt,)

class TinyASTNodeStmt(TinyASTNode):
    def __init__(self, stmt_kind):
        TinyASTNode.__init__(self, tiny_ast_node_kind.STMT)
        self.stmt_kind = stmt_kind
        self.next = None

    def second_kind(self):
        return self.stmt_kind

    def sibling(self):
        return self.next

class TinyASTNodeStmtIf(TinyASTNodeStmt):
    def __init__(self):
        TinyASTNodeStmt.__init__(self, tiny_ast_stmt_kind.IF)
        self.if_part = None
        self.then_part = None
        self.else_part = None

    def children(self):
        return (self.if_part, self.then_part, self.else_part)

class TinyASTNodeStmtAssign(TinyASTNodeStmt):
    def __init__(self):
        TinyASTNodeStmt.__init__(self, tiny_ast_stmt_kind.ASSIGN)
        self.attr_name = None
        self.exp = None

    def children(self):
        return (self.exp,)

class TinyASTNodeStmtRepeat(TinyASTNodeStmt):
    def __init__(self):
        TinyASTNodeStmt.__init__(self, tiny_ast_stmt_kind.REPEAT)
        self.body = None
        self.test = None

    def children(self):
        return (self.body, self.test)

class TinyASTNodeStmtRead(TinyASTNodeStmt):
    def __init__(self):
        TinyASTNodeStmt.__init__(self, tiny_ast_stmt_kind.READ)
        self.attr_name = None

class TinyASTNodeStmtWrite(TinyASTNodeStmt):
    def __init__(self):
        TinyASTNodeStmt.__init__(self, tiny_ast_stmt_kind.WRITE)
        self.attr_name = None

class TinyASTNodeExp(TinyASTNode):
    def __init__(self, exp_kind):
        TinyASTNode.__init__(self, tiny_ast_node_kind.EXP)
        self.exp_kind = exp_kind

    def second_kind(self):
        return self.exp_kind

class TinyASTNodeExpIdentifier(TinyASTNodeExp):
    def __init__(self):
        TinyASTNodeExp.__init__(self, tiny_ast_exp_kind.IDENTIFIER)
        self.attr_name = None

class TinyASTNodeExpNumber(TinyASTNodeExp):
    def __init__(self):
        TinyASTNodeExp.__init__(self, tiny_ast_exp_kind.NUMBER)
        self.attr_value = None

class TinyASTNodeExpOperation(TinyASTNodeExp):
    def __init__(self):
        TinyASTNodeExp.__init__(self, tiny_ast_exp_kind.OPERATION)
        self.attr_token = None
        self.left = None
        self.right = None

    def children(self):
        return (self.left, self.right)

