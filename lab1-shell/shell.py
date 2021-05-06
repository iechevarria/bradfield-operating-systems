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


def peek(string, index):
    if index >= len(string):
        return ""
    return string[index]


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
        elif char in ["|", "&"]:
            # handle when stuff is butted up against operators
            # pretty sure this is correct behavior?
            tokens.append(cur_token)
            cur_token = ""
 
            if peek(string, i + 1) == char:
                tokens.append(2 * char)
                i += 2
            else:
                tokens.append(char)
                i += 1
        elif char == ";":
            tokens.append(cur_token)
            cur_token = ""

            tokens.append(";")
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


def fork_and_exec(state, command, args):
    # child_id is 0 for child process
    child_id = os.fork()
    if child_id > 0:
        state["fg_child_id"] = child_id
        _, exit_code = os.wait()
        state["fg_child_id"] = None
        return exit_code
    else:
        try:
            os.execv(f"/usr/bin/{command}", [f"/usr/bin/{command}", *args])
        except FileNotFoundError:
            print(f"{command}: command not found")
            sys.exit(0)


def execute_statement(state, statement):
    if not statement:
        return 0

    command = statement[0]
    args = statement[1:]

    if command in BUILT_INS:
        BUILT_INS[command](args)
    else:
        return fork_and_exec(state, command, args)


def execute(state, line):
    tokens = tokenize(line)
    print(tokens)
    execute_statement(state, tokens)


def main():
    state = {"fg_child_id": None}

    def signal_handler(sig, frame):
        if state["fg_child_id"]:
            os.kill(state["fg_child_id"], signal.SIGINT)

    signal.signal(signal.SIGINT, signal_handler)

    while True:
        try:
            line = input(":) ")
            execute(state, line)
        except EOFError:
            print("\nSee ya later partner ")
            return 0


if __name__ == "__main__":
    main()
