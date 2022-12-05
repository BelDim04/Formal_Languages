class Rule:
    input = str
    output = str

    def __init__(self, str_in: str, str_out: str):
        str_in = str_in.replace('1', '')
        str_out = str_out.replace('1', '')
        self.input = str_in
        self.output = str_out


class Grammar:
    rules = list

    def __init__(self):
        self.rules = []

    def add_rule(self, rule):
        self.rules.append(rule)


class Situation:
    input = str
    output = str
    pointPos = int
    baseInd = int

    def __init__(self, str_in, str_out, point, ind):
        self.input = str_in
        self.output = str_out
        self.pointPos = point
        self.baseInd = ind

    def __hash__(self):
        status = self.input + self.output + str(self.pointPos) + str(self.baseInd)
        return status.__hash__()

    def __eq__(self, other):
        return self.input == other.input \
            and self.output == other.output\
            and self.baseInd == other.baseInd\
            and self.pointPos == other.pointPos
