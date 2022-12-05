from Grammar import Grammar, Rule
from EarleyParser import EarleyParser
if __name__ == '__main__':
    print('Введите кол-во правил в грамматике\n')
    n = int(input())

    print('Введите правила грамматики в виде S -> aTbT\n')
    grammar = Grammar()
    for i in range(0, n):
        s = input()
        parts = s.split(' -> ')
        grammar.add_rule(Rule(parts[0], parts[1]))

    print('Введите распознаваемое слово\n')
    word = input()

    earley = EarleyParser(grammar, word)

    print('Verdict: ', earley.check())
