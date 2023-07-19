class Rule:
    input = str
    output = str

    def __init__(self, str_in: str, str_out: str):
        str_in = str_in.replace('1', '')
        str_out = str_out.replace('1', '')
        self.input = str_in
        self.output = str_out

    def __eq__(self, other):
        return self.input == other.input \
            and self.output == other.output

    def __hash__(self):
        return hash((self.input, self.output))


def is_terminal(c: str):
    return not c.isalpha() or (len(c) == 1 and c.lower() == c)


class Grammar:
    rules = list

    def __init__(self):
        self.rules = []

    def add_rule(self, rule):
        self.rules.append(rule)
