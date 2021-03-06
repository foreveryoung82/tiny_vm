import re
import locale


import tiny_token


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

_regexp = re.compile(_pattern, re.ASCII)

class TinyLexer():
    def __init__(self, source_text):
        self._src = source_text
        self._token = tiny_token.ERROR
        self._value = None
        self._pos = 0
        self._line_no = 1

    def next(self):
        while self._pos<len(self._src):
            match = _regexp.match(self._src, self._pos)
            if not match:
                return (tiny_token.ERROR, None)
            
            self._pos = match.end()

            group_name = match.lastgroup

            if group_name=='_0_white_space':
                continue
            elif group_name=='_0_new_line':
                self._line_no+=1
                continue
            else:
                token = locale.atoi(group_name[1:])
                value = match.group(group_name)
                self._token = token
                self._value = value
                return (self._token, self._value)

        self._token = tiny_token.EOS
        self._value = 'EOS'
        return (self._token, self._value)

    def token(self):
        return self._token

    def value(self):
        return self._value

    def pos(self):
        return self._pos

    def line_no(self):
        return self._line_no
