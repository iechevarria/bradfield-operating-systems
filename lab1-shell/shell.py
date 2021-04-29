import sys


def help():
    print("Sorry partner, don't have much help to give")


def execute(line):
    tokens = line.split()
    print(tokens)
    if tokens[0] == "help":
        help()

def main():
    while True:
        try:
            line = input(":) ")
        except EOFError:
            print("\nSee ya later partner ")
            return 0
    
        execute(line)

if __name__ == "__main__":
    main()
