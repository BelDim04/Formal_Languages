from copy import deepcopy as copy

from Grammar import Rule, Grammar, is_terminal

REAL_START = '\''
END_SYMBOL = '$'


class LR:
    class Configuration:
        def __init__(self, rule: Rule, next_symbol: str, point_position: int):
            self.rule = rule
            self.next_symbol = next_symbol
            self.point_position = point_position

        def __eq__(self, other):
            return ((self.rule == other.rule) and
                    (self.next_symbol == other.next_symbol) and
                    (self.point_position == other.point_position))

        def __hash__(self):
            return hash((self.rule, self.next_symbol, self.point_position))

    class Node:
        def __init__(self):
            self.children = {}
            self.confs = set()

        def __eq__(self, other) -> bool:
            return self.confs == other.confs

        def __hash__(self) -> int:
            return hash(tuple(self.confs))

    class Shift:
        def __init__(self, to: int):
            self.to = to

    class Reduce:
        def __init__(self, rule: Rule):
            self.rule = rule

    def __init__(self, grammar: Grammar, word: str):
        self.grammar = None
        self.nodes = None
        self.nodes_set = None
        self.table = None
        self.word = word
        self._init_grammar(grammar)

    def check(self):
        self.word += END_SYMBOL
        stack = [0]
        i = 0
        while i < len(self.word):
            c = self.word[i]
            top = stack[-1]
            if c not in self.table[top]:
                return 'NO'
            if isinstance(self.table[top][c], self.Reduce):
                if self.table[top][c].rule == Rule(REAL_START, 'S'):
                    if i == (len(self.word) - 1):
                        return 'YES'
                    return 'NO'
                if (len(self.table[top][c].rule.output) * 2) >= len(stack):
                    return 'NO'
                next_stack_elem = self.table[top][c].rule.input
                rule_len = len(self.table[top][c].rule.output)
                stack = stack[:len(stack) - (rule_len * 2)]
                top = stack[-1]
                stack.append(next_stack_elem)
                stack.append(self.table[top][next_stack_elem].to)

            elif isinstance(self.table[top][c], self.Shift):
                stack.append(c)
                stack.append(self.table[top][c].to)
                i += 1
        return 'NO'

    def _init_grammar(self, grammar: Grammar):
        self.grammar = grammar
        self.nodes = [self.Node()]
        self.nodes[0].confs.add(self.Configuration(Rule(REAL_START, 'S'),
                                                   END_SYMBOL, 0))
        self.nodes[0] = self._get_closure(self.nodes[0])
        self.nodes_set = {self.nodes[0]}
        i = 0
        while i < len(self.nodes):
            used = set()
            for conf in self.nodes[i].confs:
                if ((len(conf.rule.output) > conf.point_position) and
                        (conf.rule.output[conf.point_position] not in used)):
                    self._goto(i, conf.rule.output[conf.point_position])
                    used.add(conf.rule.output[conf.point_position])
            i += 1

        self.table = [{} for _ in range(len(self.nodes))]
        self._fill_table(0, set())

    def _get_closure(self, node):
        has_changed = True
        while has_changed:
            new_node = copy(node)
            has_changed = False
            for conf in node.confs:
                for rule in self.grammar.rules:
                    if ((len(conf.rule.output) > conf.point_position) and
                            (conf.rule.output[conf.point_position] == rule.input)):
                        for next_symbol in self._first(conf.rule.output[conf.point_position + 1:] +
                                                       conf.next_symbol, set()):
                            if self.Configuration(rule, next_symbol, 0) not in new_node.confs:
                                new_node.confs.add(self.Configuration(rule, next_symbol, 0))
                                has_changed = True
            node = new_node

        return node

    def _goto(self, i: int, char: str):
        new_node = self.Node()
        for conf in self.nodes[i].confs:
            if ((len(conf.rule.output) > conf.point_position) and
                    (conf.rule.output[conf.point_position] == char)):
                new_node.confs.add(self.Configuration(conf.rule,
                                                      conf.next_symbol,
                                                      conf.point_position + 1))
        new_node = self._get_closure(new_node)
        if new_node not in self.nodes_set:
            self.nodes.append(new_node)
            self.nodes_set.add(new_node)
        self.nodes[i].children[char] = self.nodes.index(new_node)

    def _fill_table(self, i: int, used: set[int]):
        if i in used:
            return

        for c in self.nodes[i].children:
            self.table[i][c] = self.Shift(self.nodes[i].children[c])

        for conf in self.nodes[i].confs:
            if len(conf.rule.output) == conf.point_position:
                self.table[i][conf.next_symbol] = self.Reduce(conf.rule)
        used.add(i)
        for c in self.nodes[i].children:
            self._fill_table(self.nodes[i].children[c], used)

    def _first(self, w: str, cur_open: set[str]):
        if w in cur_open:
            return set()
        cur_open.add(w)
        if len(w) == 0:
            return set()
        res = [w[0]]
        result_set = {w[0]}
        if is_terminal(w):
            return result_set
        has_changed = True
        while has_changed:
            has_changed = False
            u_index = 0
            while u_index < len(res):
                alpha = res[u_index]
                if is_terminal(alpha):
                    break
                has_changed = has_changed or self._add_non_terminal_first(alpha, res, result_set)
                u_index += 1

        if '' in result_set:
            result_set.remove('')
            result_set.update(self._first(w[1:], cur_open))
        return result_set

    def _add_non_terminal_first(self, c: str, res: list[str], result_set: set[str]):
        changed = False
        for rule in self.grammar.rules:
            if rule.input != c:
                continue
            if c in result_set:
                changed = True
                result_set.discard(c)
            if ((c != rule.output[:1]) and
                    (rule.output[:1] not in result_set)):
                changed = True
                result_set.add(rule.output[:1])
                res.append(rule.output[:1])
        return changed
