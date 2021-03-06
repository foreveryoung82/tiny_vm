import locale


import tiny_ast_exp_kind
import tiny_ast_node_kind
import tiny_ast_stmt_kind
import tiny_op_code as code
import tiny_token
from tiny_op import TinyOp


class TinyOpLabel:
    def __init__(self, entry_idx):
        self.entry_idx = entry_idx

class TinyOpBuffer:
    def __init__(self):
        self._op_list = []
        self._entries = []

    def new_label(self):
        next_entry_idx = len(self._entries)
        label = TinyOpLabel(next_entry_idx)
        self._entries.append(None)

        return label

    def add_label(self, label):
        next_op_addr = self.op_count()
        self._entries[label.entry_idx] = next_op_addr

    def add_op(self, op_code, param=None):
        op_addr = self.op_count()
        self._op_list.append(TinyOp(op_code, param))

        return op_addr

    def op_count(self):
        return len(self._op_list)

    def op_at(self, op_addr):
        return self._op_list[op_addr]

    def entry_at(self, entry_idx):
        return self._entries[entry_idx]

    def op_list(self):
        return self._op_list

class TinyTranslateContext:
    def __init__(self):
        self.buffer = TinyOpBuffer()
        self.ops_with_label = []

class TinyTranslater():
    def translate(self, ast_root):
        context = TinyTranslateContext()
        buf, ops = context.buffer, context.ops_with_label

        self._translate_root(ast_root, context)
        buf.add_op(code.HALT)

        self._convert_label_to_op_address(buf, ops)

        return buf.op_list()

    def _convert_label_to_op_address(self, op_buffer, ops_with_label):
        for op_addr in ops_with_label:
            op = op_buffer.op_at(op_addr)
            label = op.param
            addr = op_buffer.entry_at(label.entry_idx)
            op.param = addr

    def _translate_root(self, ast_root, context):
        stmt_seq = ast_root.first_stmt
        self._translate_stmt_seq(stmt_seq, context)

    def _translate_stmt_seq(self, stmt_seq, context):
        stmt = stmt_seq
        while stmt:
            self._translate_stmt(stmt, context)
            stmt = stmt.next

    def _translate_stmt(self, stmt, context):
        if tiny_ast_stmt_kind.IF==stmt.stmt_kind:
            self._translate_stmt_if(stmt, context)
        elif tiny_ast_stmt_kind.ASSIGN==stmt.stmt_kind:
            self._translate_stmt_assign(stmt, context)
        elif tiny_ast_stmt_kind.REPEAT==stmt.stmt_kind:
            self._translate_stmt_repeat(stmt, context)
        elif tiny_ast_stmt_kind.READ==stmt.stmt_kind:
            self._translate_stmt_read(stmt, context)
        elif tiny_ast_stmt_kind.WRITE==stmt.stmt_kind:
            self._translate_stmt_write(stmt, context)
        else:
            raise Exception()

    def _translate_stmt_if(self, stmt_if, context):
        buf = context.buffer

        l_else = buf.new_label()
        l_end = buf.new_label()
        self._translate_exp(stmt_if.if_part, context)
        jump_else = buf.add_op(code.JUMP_IF_ZERO, l_else)
        context.ops_with_label.append(jump_else)

        self._translate_stmt_seq(stmt_if.then_part, context)
        jump_end = buf.add_op(code.JUMP, l_end)
        context.ops_with_label.append(jump_end)

        buf.add_label(l_else)
        if stmt_if.else_part:
            self._translate_stmt_seq(stmt_if.else_part, context)
        buf.add_label(l_end)

    def _translate_stmt_assign(self, stmt_assign, context):
        buf = context.buffer

        self._translate_exp(stmt_assign.exp, context)

        buf.add_op(code.LOOKUP_OR_NEW, stmt_assign.attr_name)
        buf.add_op(code.VARIABLE_WRITE)

    def _translate_stmt_repeat(self, stmt_repeat, context):
        buf = context.buffer

        l_start = buf.new_label()
        buf.add_label(l_start)

        self._translate_stmt_seq(stmt_repeat.body, context)
        self._translate_exp(stmt_repeat.test, context)
        jump_start = buf.add_op(code.JUMP_IF_ZERO, l_start)
        context.ops_with_label.append(jump_start)

    def _translate_stmt_read(self, stmt_read, context):
        pass

    def _translate_stmt_write(self, stmt_write, context):
        buf = context.buffer
        
        buf.add_op(code.WRITE, stmt_write.attr_name)

    def _translate_exp(self, exp, context):
        kind = exp.exp_kind

        if tiny_ast_exp_kind.IDENTIFIER==kind:
            self._translate_identifier(exp, context)
        elif tiny_ast_exp_kind.NUMBER==kind:
            self._translate_number(exp, context)
        elif tiny_ast_exp_kind.OPERATION==kind:
            self._translate_operation(exp, context)
        else:
            raise Exception()

    def _translate_identifier(self, identifier, context):
        buf = context.buffer

        buf.add_op(code.LOOKUP, identifier.attr_name)
        buf.add_op(code.VARIABLE_READ)

    def _translate_number(self, number, context):
        buf = context.buffer

        numberical_value = locale.atoi(number.attr_value)
        buf.add_op(code.PUSH, numberical_value)

    def _translate_operation(self, operation, context):
        buf = context.buffer

        self._translate_exp(operation.left, context)
        self._translate_exp(operation.right, context)

        token = operation.attr_token
        if tiny_token.PLUS==token:
            buf.add_op(code.PLUS)
        elif tiny_token.MINUS==token:
            buf.add_op(code.MINUS)
        elif tiny_token.MULTIPLY==token:
            buf.add_op(code.MULTIPLY)
        elif tiny_token.DIVIDE==token:
            buf.add_op(code.DIVIDE)
        elif tiny_token.EQUAL==token:
            buf.add_op(code.EQUAL)
        elif tiny_token.LESS_THAN==token:
            buf.add_op(code.LESS_THAN)
        else:
            raise Exception()