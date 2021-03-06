import tiny_op_code as op_code


OP_LIST_MAX_LEN = 65536

class StackMachine():
    def __init__(self):
        self._stack = []
        self._variable = []
        self._name = {}
        self._next = 0

        self._code_to_proc = {
            op_code.JUMP:self._execute_jump,
            op_code.JUMP_IF_NOT_ZERO:self._execute_jump_if_not_zero,
            op_code.JUMP_IF_ZERO:self._execute_jump_if_zero,

            op_code.LOOKUP:self._execute_lookup,
            op_code.LOOKUP_OR_NEW:self._execute_lookup_or_new,

            op_code.VARIABLE_READ:self._execute_variable_read,
            op_code.VARIABLE_WRITE:self._execute_variable_write,

            op_code.PUSH:self._execute_push,
            op_code.POP:self._execute_pop,

            op_code.PLUS:self._execute_plus,
            op_code.MINUS:self._execute_minus,
            op_code.MULTIPLY:self._execute_multiply,
            op_code.DIVIDE:self._execute_divide,

            op_code.EQUAL:self._execute_equal,
            op_code.LESS_THAN:self._execute_less_than,

            op_code.WRITE:self._execute_write,
            
            op_code.NOP:self._execute_nop,
            op_code.HALT:self._execute_halt,
        }

    # OP_LIST_MAX_LEN>=len(op_list)>=1 
    # op_list[0] is global entry
    # op_list[-1] is global exit and must be HALT
    def run(self, op_list):
        op_count = len(op_list)
        while self._next<op_count:
            next_op = op_list[self._next]
            jump_addr = self._execute_op(next_op)
            self._next = jump_addr or (self._next+1)

    def _new_variable(self):
        var_idx = len(self._variable)
        self._variable.append(None)

        return var_idx

    def _execute_op(self, op):
        proc = self._code_to_proc[op.code]
        return proc(op)

    def _execute_jump(self, op):
        return op.param

    def _execute_jump_if_not_zero(self, op):
        if 0!=self._stack.pop():
            return op.param

    def _execute_jump_if_zero(self, op):
        if 0==self._stack.pop():
            return op.param

    def _execute_lookup(self, op):
        id = self._name[op.param]
        self._stack.append(id)

    def _execute_lookup(self, op):
        id = self._name[op.param]
        self._stack.append(id)

    def _execute_lookup_or_new(self, op):
        name = op.param
        id = self._name.get(name)
        if not id:
            id = self._new_variable()
            self._name[name] = id

        self._stack.append(id)

    def _execute_variable_read(self, op):
        id = self._stack.pop()
        self._stack.append(self._variable[id])

    def _execute_variable_write(self, op):
        id = self._stack.pop()
        self._variable[id] = self._stack.pop()

    def _execute_push(self, op):
        self._stack.append(op.param)

    def _execute_pop(self, op):
        self._stack.pop()

    def _execute_plus(self, op):
        b = self._stack.pop()
        a = self._stack.pop()
        self._stack.append(a+b)

    def _execute_minus(self, op):
        b = self._stack.pop()
        a = self._stack.pop()
        self._stack.append(a-b)

    def _execute_multiply(self, op):
        b = self._stack.pop()
        a = self._stack.pop()
        self._stack.append(a*b)

    def _execute_divide(self, op):
        b = self._stack.pop()
        a = self._stack.pop()
        self._stack.append(a/b)

    def _execute_equal(self, op):
        b = self._stack.pop()
        a = self._stack.pop()
        self._stack.append(a==b)

    def _execute_less_than(self, op):
        b = self._stack.pop()
        a = self._stack.pop()
        self._stack.append(a<b)

    def _execute_write(self, op):
        name = op.param
        id = self._name[name]
        var = self._variable[id]

        print(var)

    def _execute_nop(self, op):
        pass

    def _execute_halt(self, op):
        self._next = OP_LIST_MAX_LEN

    