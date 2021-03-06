from xml.etree import ElementTree
from collections import deque


import tiny_token
import tiny_ast_node_kind as node_kind
import tiny_ast_stmt_kind as stmt_kind
import tiny_ast_exp_kind as exp_kind


ATTR_PREFIX = 'attr_'

ATTR_PREFIX_LEN = len(ATTR_PREFIX)

class TinyTreeView():
    def __init__(self, ast):
        self._root = None
        self._node_processor_entries = {
            (node_kind.ROOT, None):'ROOT',
            (node_kind.STMT, stmt_kind.IF):'STMT_IF',
            (node_kind.STMT, stmt_kind.ASSIGN):'STMT_ASSIGN',
            (node_kind.STMT, stmt_kind.REPEAT):'STMT_REPEAT',
            (node_kind.STMT, stmt_kind.READ):'STMT_READ',
            (node_kind.STMT, stmt_kind.WRITE):'STMT_WRITE',
            (node_kind.EXP, exp_kind.IDENTIFIER):'EXP_IDENTIFIER',
            (node_kind.EXP, exp_kind.NUMBER):'EXP_NUMBER',
            (node_kind.EXP, exp_kind.OPERATION):'EXP_OPERATION',
        }
        self._ast_to_xml = {}
        self._xml_root = None

        self._bfs_visit(ast)

    def _bfs_visit(self, ast):
        nodes = deque()
        nodes.append((None, ast))

        while nodes:
            parent, node = nodes.popleft()
            self._build_xml_node(parent, node)

            for child in node.children():
                 # child like else_part in if_stmt can be missing(None)
                if not child:
                    continue
                nodes.append((node, child))

                # each stmt can have one sibling
                sibling = child.sibling()
                while sibling:
                    nodes.append((node, sibling))
                    sibling = sibling.sibling()


    def _build_xml_node(self, ast_parent, ast_node):
        key = (ast_node.first_kind(), ast_node.second_kind())
        tag = self._node_processor_entries[key]

        xml_node = None
        if ast_parent:
            xml_parent = self._ast_to_xml[ast_parent]
            xml_node = ElementTree.SubElement(xml_parent, tag)
        else:
            xml_node = ElementTree.Element(tag)
            self._xml_root = xml_node

        self._ast_to_xml[ast_node] = xml_node

        keys = ast_node.__dict__.keys()
        for k in keys:
            if not k.startswith(ATTR_PREFIX):
                continue
            attr_name = k[ATTR_PREFIX_LEN:]
            attr_value = getattr(ast_node, k)
            xml_node.set(attr_name, '{}'.format(attr_value))

    def print(self):
        print(ElementTree.tostring(self._xml_root))
        