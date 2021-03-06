from stack_machine import StackMachine
from tiny_lexer import TinyLexer
from tiny_parser import TinyParser
from tiny_translater import TinyTranslater
from tiny_tree_view import TinyTreeView


class Console():
    def __init__(self):
        pass

    def process(self):
        while True:
            source_text = input('Enter TINY program:\n')
            if source_text=='exit':
                break

            # front end
            lexer = TinyLexer(source_text)
            parser = TinyParser()
            syntax_tree = parser.parse(lexer)

            tree_view = TinyTreeView(syntax_tree)
            tree_view.print()

            # back end
            translater = TinyTranslater()
            op_list = translater.translate(syntax_tree)

            # virtual machine
            machine = StackMachine()
            machine.run(op_list)
