# control flow
# (JUMP, label): unconditional jump
#   PC = label
JUMP = 1


# (JUMP_IF_NOT_ZERO, label): jump if not 0
#   if pop():
#       PC = label.op_address
JUMP_IF_NOT_ZERO = 2

# (JUMP_IF_ZERO, label): jump if 0
#   if not pop():
#       PC = label.op_address
JUMP_IF_ZERO = 3


# name lookup
# (LOOKUP, name): loopup name string and push id
#   id = NAME.get(name)
#   push(id)
LOOKUP = 101

# (LOOKUP_OR_NEW, name): if name exsits, works like LOOKUP;
#                        if not, create new variable with name first.
#   id = NAME.get(name)
#   if not id:
#       id = new_variable(name)
#       NAME[name] = id
#   push(id)
LOOKUP_OR_NEW = 102 


# variable access
# (VARIABLE_READ, None): read from variable which id is S[-1]
#   id = pop()
#   push(VARIABLE[id])
VARIABLE_READ = 201

# (VARIABLE_WRITE, None): write to variable which id is S[-1], value is S[-2]
#   id = pop()
#   VARIABLE[id] = pop()
VARIABLE_WRITE = 202


# stack manipulation
# (PUSH, object): push object to stack top
#   push(object)
PUSH = 301

# (POP, None): pop stack
#   pop()
POP = 302


# arithmetic operation
# (PLUS, None): s[-2:] = s[-2]+s[-1]
PLUS = 401

# (MINUS, None): s[-2:] = s[-2]-s[-1]
MINUS = 402

# (MULTIPLY, None): s[-2:] = s[-2]*s[-1]
MULTIPLY = 403

# (DIVIDE, None): s[-2:] = s[-2]/s[-1]
DIVIDE = 404


# comparision operation
# (EQUAL, None): compare if S[-1]==S[-2] and push result
# S[-2:] = S[-2]==S[-1]
EQUAL = 501

# (LESS_THAN)
# (LESS_THAN, None): compare if S[-2]<S[-1] and push result
# S[-2:] = S[-2]<S[-1]
LESS_THAN = 502


# I/O operations
# (READ, name): read number into variable by name
# READ = 601

# (WRITE, name): write variable by name to screen
WRITE = 602

# no operation (only for debug)
# (NOP, None)
NOP = 1001


# halt virtual machine
# (HALT, None)
HALT = 1101