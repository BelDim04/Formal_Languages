from Grammar import Grammar


class EarleyParser:
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
                and self.output == other.output \
                and self.baseInd == other.baseInd \
                and self.pointPos == other.pointPos

    grammar = Grammar
    situationSets = list
    word = str

    def __init__(self, grammar, word):
        self.grammar = grammar
        self.word = word
        self.situationSets = []
        for i in range(len(word) + 1):
            self.situationSets.append(set())
        self.situationSets[0].add(self.Situation('S\'', 'S', 0, 0))

    def scan(self, set_id: int, symbol):
        for situation in self.situationSets[set_id]:
            if situation.pointPos < len(situation.output) and situation.output[situation.pointPos] == symbol:
                self.situationSets[set_id + 1].add(
                    self.Situation(situation.input, situation.output, situation.pointPos + 1, situation.baseInd))

    def predict(self, set_id: int):
        situations_to_insert = []
        for situation in self.situationSets[set_id]:
            if situation.pointPos < len(situation.output):
                rule_input = situation.output[situation.pointPos]
                for rule in self.grammar.rules:
                    if rule.input == rule_input:
                        situations_to_insert.append(self.Situation(rule_input, rule.output, 0, set_id))

        for sit in situations_to_insert:
            self.situationSets[set_id].add(sit)

    def complete(self, set_id: int):
        situations_to_insert = []
        for situation in self.situationSets[set_id]:
            if situation.pointPos == len(situation.output):
                for other in self.situationSets[situation.baseInd]:
                    if other.pointPos < len(other.output) \
                            and other.output[other.pointPos: other.pointPos + len(situation.input)] == situation.input:
                        sit = self.Situation(other.input, other.output, other.pointPos + 1, other.baseInd)
                        situations_to_insert.append(sit)

        for sit in situations_to_insert:
            self.situationSets[set_id].add(sit)

    def check(self):
        for i in range(len(self.word) + 1):
            if i > 0:
                self.scan(i - 1, self.word[i - 1])
            prev_len = len(self.situationSets[i])
            self.predict(i)
            self.complete(i)
            new_len = len(self.situationSets[i])
            while new_len != prev_len:
                prev_len = new_len
                self.predict(i)
                self.complete(i)
                new_len = len(self.situationSets[i])

        for situation in self.situationSets[len(self.word)]:
            if situation.input == 'S\'' and situation.output == 'S' and situation.baseInd == 0 and situation.pointPos == 1:
                return 'YES'

        return 'NO'
