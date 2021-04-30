import os
import signal
import sys


def tokenize_string_literal(string, i, quote_char):
    string_literal = ""
    while i < len(string):
        char = string[i]
        if char == quote_char:
            return string_literal, (i + 1)  # move past quote char
        else:
            string_literal += char
            i += 1
    
    return string_literal, i 


def tokenize(string):
    tokens = []
    cur_token = ""

    i = 0
    while i < len(string):
        char = string[i]
        if char in ["'", "\""]:
            # take care of case where quote butted up against quote
            # don't really know what correct behavior is
            tokens.append(cur_token)
            cur_token = ""

            token, i = tokenize_string_literal(string, i + 1, quote_char=char)
            tokens.append(token)
        elif char in [" ", "\n", "\t"]:
            tokens.append(cur_token)
            cur_token = ""
            i += 1
        else:
            cur_token += char
            i += 1

    tokens.append(cur_token)

    return [t for t in tokens if t]


def help_(args):
    print("Sorry partner, don't have much help to give")


def exit_(args):
    # hacky but it works lol
    raise EOFError


BUILT_INS = {
    "help": help_,
    "exit": exit_,
}


def fork_and_exec(command, args):
    # child_id is 0 for child
    child_id = os.fork()
    if child_id > 0:
        _, exit_code = os.wait()
        return exit_code
    else:
        try:
            os.execv(f"/usr/bin/{command}", [f"/usr/bin/{command}", *args])
        except FileNotFoundError:
            print(f"{command}: command not found")
            sys.exit(0)


def execute_statement(statement):
    if not statement:
        return 0

    command = statement[0]
    args = statement[1:]

    if command in BUILT_INS:
        BUILT_INS[command](args)
    else:
        return fork_and_exec(command, args)


def execute(line):
    tokens = tokenize(line)
    # print(tokens)
    execute_statement(tokens)


def main():
    while True:
        try:
            line = input(":) ")
            execute(line)
        except EOFError:
            print("\nSee ya later partner ")
            return 0


if __name__ == "__main__":
    main()
