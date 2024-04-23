from collections import namedtuple
from enum import Enum, auto
from typing import Generator

from .file import FileIterator
from .types import *
from .emitter import emit_ops
from .util.error import eprint

def get_ops(input_file: str, args: list[str]) -> Generator[Op, None, None]:
    def get_token(file_iter: FileIterator) -> Token:
        token_raw: tuple[str, int]
        if file_iter.c == "\"":
            file_iter.advance()
            __token_raw = file_iter.collect_till("\"")
            token_raw = ("\"" + __token_raw[0] + "\"", __token_raw[1])
        else:
            token_raw = file_iter.collect()

        token_type: TokenType
        if token_raw[0].isdigit():
            token_type = TokenType.TOKEN_INT
        elif token_raw[0].replace('.', '', 1).isdigit():
            token_type = TokenType.TOKEN_FLOAT
        elif token_raw[0].startswith("\"") and token_raw[0].endswith("\""):
            token_type = TokenType.TOKEN_STR
        else:
            token_type = TokenType.TOKEN_WORD

        if token_type == TokenType.TOKEN_STR:
            return Token(
                token_type, token_raw[0].replace("\"", ""), token_raw[1]
            )
        else:
            return Token(token_type, token_raw[0], token_raw[1])
        
    def parse_ast_piece(token: Token) -> Op:
        if token.type == TokenType.TOKEN_INT:
            return Op(OpType.OP_PUSHINT, int(token.value), 0)
        elif token.type == TokenType.TOKEN_FLOAT:
            return Op(OpType.OP_PUSHFLOAT, float(token.value), 0)
        elif token.type == TokenType.TOKEN_STR:
            return Op(OpType.OP_PUSHSTR, token.value, token.len)
        elif token.type == TokenType.TOKEN_WORD:
            if token.value in intrinsic_map.keys():
                return Op(OpType.OP_INTRINSIC, intrinsic_map[token.value], 0)
            else:
                return Op(OpType.OP_PUSHWORD, token.value, 0)
        else:
            assert False, "Unreachable"


    try:
        file_iter = FileIterator(input_file)
    except FileNotFoundError:
        eprint(f"Failed to open file '{input_file}'")
        raise FileNotFoundError()

    while not file_iter.done:
        file_iter.skip_whitespace()

        token = get_token(file_iter)
        op = parse_ast_piece(token)

        file_iter.advance()

        yield op

def compile(input_file: str, args: list[str]):
    emit_ops(input_file.replace(".uni", ".asm"), get_ops(input_file, args))

def run(input_file: str, args: list[str]):
    emit_ops(get_ops(input_file, args))

def help(args: list[str]):
    eprint("Printing help is not implemented...\n")

def parse_cmd(args: list[str]):    
    match args:
        case []:
            eprint("Command must be supplied...\n")
            raise ValueError("Command must be supplied")
        case ["comp"] | ["run"]:
            eprint("File must be supplied...\n")
            raise ValueError("File must be supplied")

        case ["comp", file, *args]:
            compile(file, args)
        case ["run", file, *args]:
            run(file, args)

        case ["help", *args]:
            help(args)
