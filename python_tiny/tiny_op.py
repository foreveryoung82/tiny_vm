import tiny_op_code


class TinyOp:
    def __init__(self, code=tiny_op_code.NOP, param=None):
        self.code = code
        self.param = param

