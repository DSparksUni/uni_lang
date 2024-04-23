from .util.pos import pos

class FileIterator:
    def __init__(self, file_path: str):
        with open(file_path, "r") as input_file:
            self.src = [i.strip() + " " for i in input_file.readlines()]

        self.c = self.src[0][0]
        self.done = False

        self.pos = pos()

    def advance(self):
        self.pos.col += 1

        if self.pos.col >= len(self.src[self.pos.line]):
            self.pos.line += 1
            self.pos.col = 0

            if self.pos.line >= len(self.src):
                self.done = True

        if not self.done:
            self.c = self.src[self.pos.line][self.pos.col]
            self.pos.serial += 1
        else:
            self.c = '~'

    def skip_whitespace(self):
        while self.c.isspace() or self.c == '\n':
            self.advance()

    def collect_till(self, delim: str) -> tuple[str, int]:
        string = ""
        length = 0

        while self.c != delim and not self.done:
            string += self.c
            length += 1
            self.advance()

        return string, length
    
    def collect(self) -> tuple[str, int]:
        string = ""
        length = 0

        while not self.c.isspace() and not self.c == '\n' and not self.done:
            string += self.c
            length += 1
            self.advance()

        return string, length

    def __iter__(self):
        return self
    
    def __next__(self):
        if self.done:
            raise StopIteration
        else:
            value = self.c
            self.advance()
            return value
