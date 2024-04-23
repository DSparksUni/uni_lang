from collections import namedtuple
from enum import Enum, auto

Token = namedtuple("Token", ["type", "value", "len"])
class TokenType(Enum):
    TOKEN_INT = auto()
    TOKEN_FLOAT = auto()
    TOKEN_STR = auto()
    TOKEN_WORD = auto()
def print_token(token: Token):
    TOKEN_TYPE_STRS = {
        TokenType.TOKEN_INT: "int",
        TokenType.TOKEN_FLOAT: "float",
        TokenType.TOKEN_STR: "string",
        TokenType.TOKEN_WORD: "word"
    }
    print(f"Token({TOKEN_TYPE_STRS[token.type]}, '{token.value}', length={token.len})")


Op = namedtuple("Op", ["type", "operand", "len"])
class OpType(Enum):
    OP_PUSHINT =   auto()
    OP_PUSHFLOAT = auto()
    OP_PUSHSTR =   auto()
    OP_PUSHWORD =  auto()
    OP_INTRINSIC = auto()
class Intrinsics(Enum):
    INTRIN_ADD      = auto()
    INTRIN_SUB      = auto()
    INTRIN_MULT     = auto()
    INTRIN_DIV      = auto()
    INTRIN_EQ       = auto()
    INTRIN_IF       = auto()
    INTRIN_BEGSCOPE = auto()
    INTRIN_ENDSCOPE = auto()
    INTRIN_PUTI     = auto()
    INTRIN_PUTS     = auto()
    INTRIN_PUTB     = auto()
intrinsic_map = {
    "+"   : Intrinsics.INTRIN_ADD,
    "-"   : Intrinsics.INTRIN_SUB,
    "*"   : Intrinsics.INTRIN_MULT,
    "/"   : Intrinsics.INTRIN_DIV,
    "="   : Intrinsics.INTRIN_EQ,
    "if"  : Intrinsics.INTRIN_IF,
    "{"   : Intrinsics.INTRIN_BEGSCOPE,
    "}"   : Intrinsics.INTRIN_ENDSCOPE,
    "puti": Intrinsics.INTRIN_PUTI,
    "puts": Intrinsics.INTRIN_PUTS,
    "putb": Intrinsics.INTRIN_PUTB
}
def print_op(op: Op):
    OP_TYPE_STRS = {
        OpType.OP_PUSHINT  : "push_int",
        OpType.OP_PUSHFLOAT: "push_float",
        OpType.OP_PUSHSTR  : "push_string",
        OpType.OP_PUSHWORD : "push_word",
        OpType.OP_INTRINSIC: "intrinsic"
    }
    INTRINSIC_STRS = {
        Intrinsics.INTRIN_ADD      : "add",
        Intrinsics.INTRIN_SUB      : "subtract",
        Intrinsics.INTRIN_MULT     : "multiply",
        Intrinsics.INTRIN_DIV      : "divide",
        Intrinsics.INTRIN_EQ       : "equals",
        Intrinsics.INTRIN_IF       : "if",
        Intrinsics.INTRIN_BEGSCOPE : "begin_scope",
        Intrinsics.INTRIN_ENDSCOPE : "end_scope",
        Intrinsics.INTRIN_PUTI     : "puti",
        Intrinsics.INTRIN_PUTS     : "puts",
        Intrinsics.INTRIN_PUTB     : "putb"
    }

    if op.type == OpType.OP_INTRINSIC:
        print(f"Op({OP_TYPE_STRS[op.type]}, {INTRINSIC_STRS[op.operand]})")
    elif op.type == OpType.OP_PUSHSTR:
        print(f"Op({OP_TYPE_STRS[op.type]}, \"{op.operand}\", length={op.len})")
    else:
        print(f"Op({OP_TYPE_STRS[op.type]}, {op.operand})")


class StackTypes(Enum):
    T_INT    = auto()
    T_FLOAT  = auto()
    T_STRING = auto()
    T_NULL   = auto()
TYPE_STRS = {
    StackTypes.T_INT: "int",
    StackTypes.T_FLOAT: "float",
    StackTypes.T_STRING: "string",
    StackTypes.T_NULL: "null"
}
def assert_type(x: StackTypes, t: StackTypes) -> None:
    if x != t: raise TypeError(f"[ERROR] invalid type '{x}', expected '{t}'")
def assert_types(t: StackTypes, *args) -> None:
    for val in args: assert_type(val, t)
