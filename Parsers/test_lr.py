from Grammar import Grammar, Rule
from EarleyParser import EarleyParser
from LR import LR


def test_lr_1():
    gr = Grammar()
    gr.add_rule(Rule('S', 'SS'))
    gr.add_rule(Rule('S', 'aSb'))
    gr.add_rule(Rule('S', 't'))
    lr = LR(gr, 'atbatbatb')
    assert lr.check() == 'YES'


def test_lr_2():
    gr = Grammar()
    gr.add_rule(Rule('S', 'aSbS'))
    gr.add_rule(Rule('S', '1'))
    lr = LR(gr, 'ababba')
    assert lr.check() == 'NO'


def test_earley_3():
    gr = Grammar()
    gr.add_rule(Rule('S', 'S+A'))
    gr.add_rule(Rule('S', '1'))
    gr.add_rule(Rule('A', 'aSa'))
    lr = LR(gr, '+aa+a+aaa+aa')
    assert lr.check() == 'YES'
