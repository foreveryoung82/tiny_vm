# keywords
IF = 1 # if
THEN = 2 # then
ELSE = 3 # else
END = 4 # end
REPEAT = 5 # repeat
UNTIL = 6 # until
READ = 7 # read
WRITE = 8 # write

# operators
PLUS = 101  # +
MINUS = 102 # -
MULTIPLY = 103 # *
DIVIDE = 104 # /
EQUAL = 105 # =
LESS_THAN = 106 # <
LEFT_BRACE = 107 # (
RIGHT_BRACE = 108 # )
SEMICOLON = 109 # ;
ASSIGN = 110 # :=

# literal and identifier
NUMBER = 201 # 1 or more digits
IDENTIFIER = 202 # 1 or more letters

EOS = 301 # end of whole string
ERROR = 302 # error when scaning token

# for debug dump info
TOKEN_TO_STRING = {
    1:'IF',
    2:'THEN',
    3:'ELSE',
    4:'END',
    5:'REPEAT',
    6:'UNTIL',
    7:'READ',
    8:'WRITE',

    101:'PLUS',
    102:'MINUS',
    103:'MULTIPLY',
    104:'DIVIDE',
    105:'EQUAL',
    106:'LESS_THAN',
    107:'LEFT_BRACE',
    108:'RIGHT_BRACE',
    109:'SEMICOLON',
    110:'ASSIGN',

    201:'NUMBER',
    202:'IDENTIFIER',

    301:'EOS',
    302:'ERROR',
}