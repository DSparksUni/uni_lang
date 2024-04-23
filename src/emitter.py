from typing import Generator
from dataclasses import dataclass

from .types import Op, OpType, Intrinsics, StackTypes, TYPE_STRS, assert_type, assert_types

__name_iota = 0
def name_iota() -> str:
    global __name_iota

    name = f"U{__name_iota}"
    __name_iota += 1
    return name


@dataclass
class Emitter:
    head: str = ""
    bss: str = ""
    data: str = ""
    main: str = ""

    def __init__(self):
        self.emit_head("\tglobal main")
        self.emit_main("\tsection .text")
        self.emit_main("main: ")

    def emit_head(self, code: str, end: str = "\n") -> None:
        self.head += code
        self.head += end

    def emit_main(self, code: str, end: str = "\n") -> None:
        self.main += code
        self.main += end

    def emit_data(self, code: str, end: str = "\n") -> None: 
        self.data += code
        self.data += end

    def emit_bss(self, code: str, end: str = "\n") -> None:
        self.bss += code
        self.bss += end

    def collect(self) -> str:
        self.emit_main("\tret")

        return self.head + self.main + "\n" + self.data + "\n" + self.bss


def emit_ops(out_name: str, ops: Generator[Op, None, None]) -> None:
    emit = Emitter()

    string_literals: dict[str, str] = {}
    type_stack = []
    puti_format_flag = False
    putb_use_flag = False
    puts_use_flag = False

    for op in ops:
        if op.type == OpType.OP_PUSHINT:
            emit.emit_main(f"\tpush {op.operand}")
            type_stack.append(StackTypes.T_INT)
        elif op.type == OpType.OP_PUSHFLOAT:
            raise NotImplementedError(
                "[ERROR] Floating point numbers are not implemented"
            )
        elif op.type == OpType.OP_PUSHSTR:
            if op.operand not in string_literals.keys():
                literal = name_iota()
                emit.emit_data(literal + ": ")
                emit.emit_data(f"\tdb\t'{op.operand}', 0")
                string_literals[op.operand] = literal

            emit.emit_main(f"\tmov rcx, {string_literals[op.operand]}")
            emit.emit_main("\tpush rcx")
            type_stack.append(StackTypes.T_STRING)
        elif op.type == OpType.OP_PUSHWORD:
            raise NotImplementedError(
                "[ERROR] Parsing word literals is not implemented"
            )
        elif op.type == OpType.OP_INTRINSIC:
            if op.operand == Intrinsics.INTRIN_ADD:
                if len(type_stack) < 2: raise ValueError(
                    "[ERROR] Attempted to pop from empty stack"
                )

                st1 = type_stack.pop() 
                st2 = type_stack.pop()
                assert_types(StackTypes.T_INT, st1, st2)

                emit.emit_main("\tpop rcx")
                emit.emit_main("\tpop rdx")
                emit.emit_main("\tadd rcx, rdx")
                emit.emit_main("\tpush rcx")
                type_stack.append(StackTypes.T_INT)
            elif op.operand == Intrinsics.INTRIN_SUB:
                if len(type_stack) < 2: raise ValueError(
                    "[ERROR] Attempted to pop from empty stack"
                )

                st1 = type_stack.pop()
                st2 = type_stack.pop()
                assert_types(StackTypes.T_INT, st1, st2)

                emit.emit_main("\tpop rcx")
                emit.emit_main("\tpop rdx")
                emit.emit_main("\tsub rcx, rdx")
                emit.emit_main("\tpush rcx")
                type_stack.append(StackTypes.T_INT)
            elif op.operand == Intrinsics.INTRIN_MULT:
                if len(type_stack) < 2: raise ValueError(
                    "[ERROR] Attempted to pop from empty stack"
                )

                st1 = type_stack.pop()
                st2 = type_stack.pop()
                assert_types(StackTypes.T_INT, st1, st2)

                emit.emit_main("\tpop rcx")
                emit.emit_main("\tpop rdx")
                emit.emit_main("\timul rcx, rdx")
                emit.emit_main("\tpush rcx")
                type_stack.append(StackTypes.T_INT)
            elif op.operand == Intrinsics.INTRIN_DIV:
                if len(type_stack) < 2: raise ValueError(
                    "[ERROR] Attempted to pop from empty stack"
                )

                st1 = type_stack.pop()
                st2 = type_stack.pop()
                assert_types(StackType.T_INT, st1, st2)

                emit.emit_main("\tpop rcx")
                emit.emit_main("\tpop rdx")
                emit.emit_main("\tdivsd rcx, rdx")
                emit.emit_main("\tpush rcx")
                type_stack.append(StackTypes.T_INT)
            elif op.operand == Intrinsics.INTRIN_EQ:
                if len(type_stack) < 2: raise ValueError(
                    "[ERROR] Attempted to pop from empty stack"
                )

                st1 = type_stack.pop()
                st2 = type_stack.pop()
                assert_types(StackTypes.T_INT, st1, st2)

                emit.emit_main("\tpop rcx")
                emit.emit_main("\tpop rdx")
                emit.emit_main("\tcmp rcx, rdx")
                emit.emit_main("\tsete al")
                emit.emit_main("\tpush rax")
                type_stack.append(StackTypes.T_INT)
            elif op.operand == Intrinsics.INTRIN_PUTS:
                st1 = type_stack.pop()
                assert_type(st1, StackTypes.T_STRING)

                if not puts_use_flag:
                    emit.emit_head("\textern puts")
                    puts_use_flag = True
    
                emit.emit_main("\tpop rcx")
                emit.emit_main("\tsub rsp, 40")
                emit.emit_main("\tcall puts")
                emit.emit_main("\tadd rsp, 40")
                emit.emit_main("")
            elif op.operand == Intrinsics.INTRIN_PUTI:
                st1 = type_stack.pop()
                assert_type(st1, StackTypes.T_INT)

                if not puti_format_flag:
                    emit.emit_head("\textern printf")
                    emit.emit_data("PUTI_FMT: ")
                    emit.emit_data("\tdb\t'%d', 10, 0")
                    puti_format_flag = True

                emit.emit_main("\tmov rcx, PUTI_FMT")
                emit.emit_main("\tpop rdx")
                emit.emit_main("\tsub rsp, 48")
                emit.emit_main("\tcall printf")
                emit.emit_main("\tadd rsp, 48")
                emit.emit_main("")
            elif op.operand == Intrinsics.INTRIN_PUTB:
                st1 = type_stack.pop()
                assert_type(st1, StackTypes.T_INT)

                if not putb_use_flag:
                    if not puts_use_flag:
                        emit.emit_head("\textern puts")
                    emit.emit_data("UFALSE: ")
                    emit.emit_data("\tdb\t'false', 0")
                    emit.emit_data("\tUTRUE: ")
                    emit.emit_data("\tdb\t'true', 0")
                    putb_use_flag = True

                emit.emit_main("\tpop rdx")
                emit.emit_main("\tcmp rdx, 0")
                false_jmp = name_iota()
                putb_jmp = name_iota()
                emit.emit_main(f"\tje {false_jmp}")
                emit.emit_main("\tmov rcx, UTRUE")
                emit.emit_main(f"\tjmp {putb_jmp}")
                emit.emit_main(f"{false_jmp}: ")
                emit.emit_main("\tmov rcx, UFALSE")
                emit.emit_main(f"{putb_jmp}: ")
                emit.emit_main("\tsub rsp, 40")
                emit.emit_main("\tcall puts")
                emit.emit_main("\tadd rsp, 40")

    out_file_content = emit.collect()
    with open(out_name, "w") as out_file:
        out_file.write(out_file_content)
