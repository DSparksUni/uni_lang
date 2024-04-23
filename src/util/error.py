from sys import stderr
from typing import Literal

def eprint(
    *values: object, sep: str | None = ' ', end: str | None = "\n",
    flush: Literal[False] = False
):
    print("[ERROR]", *values, sep=sep, end=end, file=stderr, flush=flush)
