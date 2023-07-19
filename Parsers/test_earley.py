from Grammar import Grammar, Rule
from EarleyParser import EarleyParser


def test_earley_1():
    gr = Grammar()
    gr.add_rule(Rule('S', 'SS'))
    gr.add_rule(Rule('S', 'aSb'))
    gr.add_rule(Rule('S', '1'))
    ep = EarleyParser(gr, 'ababab')
    assert ep.check() == 'YES'


def test_earley_2():
    gr = Grammar()
    gr.add_rule(Rule('S', 'aSbS'))
    gr.add_rule(Rule('S', '1'))
    ep = EarleyParser(gr, 'ababba')
    assert ep.check() == 'NO'


def test_earley_3():
    gr = Grammar()
    gr.add_rule(Rule('S', 'S+A'))
    gr.add_rule(Rule('S', '1'))
    gr.add_rule(Rule('A', 'aSa'))
    ep = EarleyParser(gr, '+aa+a+aaa+aa')
    assert ep.check() == 'YES'
