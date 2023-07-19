from Grammar import Grammar, Rule
from EarleyParser import EarleyParser


def test_grammar():
    gr = Grammar()
    gr.add_rule(Rule('S', 'S+T'))
    gr.add_rule(Rule('T', 'aTb'))
    gr.add_rule(Rule('T', '1'))
    gr.add_rule(Rule('S', '1'))
    assert len(gr.rules) == 4
    assert gr.rules[2].output == ''
    assert gr.rules[3].output == ''
