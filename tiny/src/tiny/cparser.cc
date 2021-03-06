#include "tiny/cparser.h"

#include <exception>

#include "tiny/cast.h"
#include "tiny/castnoderoot.h"
#include "tiny/castnodestmt.h"
#include "tiny/castnodestmtif.h"
#include "tiny/castnodestmtrepeat.h"
#include "tiny/castnodestmtassign.h"
#include "tiny/castnodestmtread.h"
#include "tiny/castnodestmtwrite.h"
#include "tiny/castnodeexpop.h"
#include "tiny/castnodeexpid.h"
#include "tiny/castnodeexpnum.h"
#include "tiny/clexer.h"
#include "tiny/etokenkind.h"
#include "tiny/iastnode.h"
#include "tiny/stokenvalue.h"

using std::exception;
using std::unique_ptr;

namespace tiny {

struct CParser::SImpl {
  static void match(CLexer& lexer, ETokenKind token) { 
    if (lexer.token() != token) 
      throw exception("CParser::SImpl::match failed.");

    lexer.next();
  }

  // ->stmt_sequence
  static IASTNode* parseProgram(CAST* ast, CLexer& lexer) {
    lexer.next();
    return parseStmtSeq(ast, lexer);
  }

  // ->statement {';' statement}
  static CASTNodeStmt* parseStmtSeq(CAST* ast, CLexer& lexer) {
    CASTNodeStmt* first_stmt = parseStmt(ast, lexer);
    CASTNodeStmt* stmt = first_stmt;

    while (ETokenKind::kSemicolon == lexer.token()) {
      match(lexer, ETokenKind::kSemicolon);
      CASTNodeStmt* next = parseStmt(ast, lexer);
      stmt->setSibling(next);
      stmt = next;
    }

    return first_stmt;
  }

  // ->if_stmt | repeat_stmt | assign_stmt | read_stmt | write_stmt
  static CASTNodeStmt* parseStmt(CAST* ast, CLexer& lexer) {
    ETokenKind token = lexer.token();
    switch (token) {
      case ETokenKind::kIf:
        return parseStmtIf(ast, lexer);
      break;

      case ETokenKind::kRepeat:
        return parseStmtRepeat(ast, lexer);
      break;

      case ETokenKind::kIdentifier:
        return parseStmtAssign(ast, lexer);
      break;

      case ETokenKind::kRead:
        return parseStmtRead(ast, lexer);
      break;

      case ETokenKind::kWrite:
        return parseStmtWrite(ast, lexer);
      break;

      default:
        throw exception("Unknown stmt!");
    }
  }

  // ->IF exp THEN stat_sequence [ELSE stmt-sequence] END
  static CASTNodeStmtIf* parseStmtIf(CAST* ast, CLexer& lexer) {
    CASTNodeStmtIf* stmt = ast->createStmtIf();

    match(lexer, ETokenKind::kIf);
    stmt->setIfPart(parseExp(ast, lexer));
    match(lexer, ETokenKind::kThen);
    stmt->setThenPart(parseStmtSeq(ast, lexer));    
    
    if (ETokenKind::kElse==lexer.token()) {
      lexer.next();
      stmt->setElsePart(parseStmtSeq(ast, lexer));
      match(lexer, ETokenKind::kEnd);
    }
    
    return stmt;
  }

  // ->REPEAT stat_sequence UNTIL exp
  static CASTNodeStmtRepeat* parseStmtRepeat(CAST* ast, CLexer& lexer) {
    CASTNodeStmtRepeat* stmt = ast->createStmtRepeat();

    match(lexer, ETokenKind::kRepeat);
    stmt->setRepeatPart(parseStmtSeq(ast, lexer));
    match(lexer, ETokenKind::kUntil);
    stmt->setUntilPart(parseExp(ast, lexer));
    
    return stmt;
  }

  // ->IDENTIFIER ':=' exp
  static CASTNodeStmtAssign* parseStmtAssign(CAST* ast, CLexer& lexer) {
    CASTNodeStmtAssign* stmt = ast->createStmtAssign();

    stmt->setLeftPart(parseExpID(ast, lexer));
    match(lexer, ETokenKind::kAssign);
    stmt->setRightPart(parseExp(ast, lexer));

    return stmt;
  }

  // ->READ identifier
  static CASTNodeStmtRead* parseStmtRead(CAST* ast, CLexer& lexer) {
    CASTNodeStmtRead* node = ast->createStmtRead();
    match(lexer, ETokenKind::kRead);
    
    CASTNodeExpID* id = parseExpID(ast, lexer);
    node->setIdentifier(id->name());

    return node;
  }

  // ->WRITE exp
  static CASTNodeStmtWrite* parseStmtWrite(CAST* ast, CLexer& lexer) {
    CASTNodeStmtWrite* node = ast->createStmtWrite();
    match(lexer, ETokenKind::kWrite);
    
    CASTNodeExpID* id = parseExpID(ast, lexer);
    node->setIdentifier(id->name());

    return node;
  }

  // ->simple_exp [COMPARISION_OP simple_exp]
  static CASTNodeExp* parseExp(CAST* ast, CLexer& lexer) {
    CASTNodeExp* exp = parseSimpleExp(ast, lexer);

    const ETokenKind token = lexer.token();
    const bool is_cmp_op =
        (ETokenKind::kLessThan == token) || (ETokenKind::kEqual == token);
    
    if (!is_cmp_op) return exp;

    lexer.next();
    CASTNodeExpOp* op = ast->createExpOp();
    op->setOpToken(token);
    op->setLeftPart(exp);
    op->setRightPart(parseSimpleExp(ast, lexer));
    
    return op;
  }

  // ->term {ADD_OP term}
  static CASTNodeExp* parseSimpleExp(CAST* ast, CLexer& lexer) {
    CASTNodeExp* term = parseTerm(ast, lexer);
    
    const ETokenKind token = lexer.token();
    const bool is_add_op =
        (ETokenKind::kPlus == token) || (ETokenKind::kMinus == token);

    if (!is_add_op) return term;

    lexer.next();
    CASTNodeExpOp* op = ast->createExpOp();
    op->setOpToken(token);
    op->setLeftPart(term);
    op->setRightPart(parseTerm(ast, lexer));
    
    return op;
  }

  // ->factor {MUL_OP factor}
  static CASTNodeExp* parseTerm(CAST* ast, CLexer& lexer) {
    CASTNodeExp* factor = parseFactor(ast, lexer);
    
    const ETokenKind token = lexer.token();
    const bool is_mul_op =
        (ETokenKind::kMultiply == token) || (ETokenKind::kDivide == token);

    if (!is_mul_op) return factor;

    lexer.next();
    CASTNodeExpOp* op = ast->createExpOp();
    op->setOpToken(token);
    op->setLeftPart(factor);
    op->setRightPart(parseFactor(ast, lexer));
    
    return op;
  }

  // ->'(' exp ')' | NUMBER | IDENTIFIER
  static CASTNodeExp* parseFactor(CAST* ast, CLexer& lexer) {
    ETokenKind token = lexer.token();
    
    switch (token) {
      case ETokenKind::kLeftBrace: {
        match(lexer, ETokenKind::kLeftBrace);
        CASTNodeExp* exp = parseExp(ast, lexer);
        match(lexer, ETokenKind::kRightBrace);

        return exp;
      }
      break;

      case ETokenKind::kNumber:
        return parseExpNum(ast, lexer);
      break;

      case ETokenKind::kIdentifier:
        return parseExpID(ast, lexer);
      break;

      default:
        throw exception("Unknown factor format!");
    }
  }

  static CASTNodeExpID* parseExpID(CAST* ast, CLexer& lexer) {
    CASTNodeExpID* id = ast->createExpID();
    id->setName(lexer.value().lexeme());
    match(lexer, ETokenKind::kIdentifier);

    return id;
  }

  static CASTNodeExpNum* parseExpNum(CAST* ast, CLexer& lexer) {
    CASTNodeExpNum* num = ast->createExpNum();
    num->setValue(lexer.value().lexeme());
    match(lexer, ETokenKind::kNumber);

    return num;
  }
}; // struct CParser::SImpl

std::unique_ptr<CAST> CParser::parse(CLexer& lexer) { 
  unique_ptr<CAST> ast(new CAST{});
  IASTNode* entry = SImpl::parseProgram(ast.get(), lexer);
  ast->setEntry(entry);

  return ast;
}

} // namespace tiny