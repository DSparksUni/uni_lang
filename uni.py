from sys import argv

from src.compiler import parse_cmd

def main():
    try:
        parse_cmd(argv[1:])
    except Exception as e:
        print(e)
        exit(-1)


if __name__ == '__main__':
    main()