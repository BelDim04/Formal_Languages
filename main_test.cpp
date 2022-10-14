//
// Created by dmitriy on 14.10.22.
//
#include "gtest/gtest.h"
#include "src/Task_3_Solver.h"

class General : public ::testing::Test {

};

TEST_F(General, first){
    EXPECT_EQ(Task_3_Solver::task3_with_automat("ab+c.aba.*.bac.+.+*", 'a', 2), true);
    EXPECT_EQ(Task_3_Solver::task3_with_stack("ab+c.aba.*.bac.+.+*", 'a', 2), true);
}

TEST_F(General, second){
    EXPECT_EQ(Task_3_Solver::task3_with_automat("acb..bab.c.*.ab.ba.+.+*a.", 'b', 3), false);
    EXPECT_EQ(Task_3_Solver::task3_with_stack("acb..bab.c.*.ab.ba.+.+*a.", 'b', 3), false);
}

TEST_F(General, third){
    EXPECT_EQ(Task_3_Solver::task3_with_automat("abc+*.*", 'a', 3), true);
    EXPECT_EQ(Task_3_Solver::task3_with_stack("abc+*.*", 'a', 3), true);
}

TEST_F(General, fourth){
    EXPECT_EQ(Task_3_Solver::task3_with_automat("a**bc+*.*", 'b', 3), true);
    EXPECT_EQ(Task_3_Solver::task3_with_stack("a**bc+*.*", 'b', 3), true);
}

TEST_F(General, pdka_test){
    auto pdka = NFA(NFA_from_regex_builder::build_NFA("a1.*")).to_PDKA();
    pdka.minimize();
    EXPECT_EQ(pdka.size(), 2);
}

TEST_F(General, exception_bad_char){
    EXPECT_ANY_THROW(NFA_from_regex_builder::build_NFA("at+"));
    EXPECT_ANY_THROW(Task_3_Solver::task3_with_stack("at+", 'a', 1));
}

TEST_F(General, exception_task3_bad_char){
    EXPECT_ANY_THROW(Task_3_Solver::task3_with_automat("acb..bab.c.*.ab.ba.+.+*a.", 'i', 3));
    EXPECT_ANY_THROW(Task_3_Solver::task3_with_stack("acb..bab.c.*.ab.ba.+.+*a.", 'i', 3));
}

TEST_F(General, exception_bad_reg_1){
    EXPECT_ANY_THROW(Task_3_Solver::task3_with_automat("acb.", 'c', 1));
    EXPECT_ANY_THROW(Task_3_Solver::task3_with_stack("acb.", 'c', 1));
}

TEST_F(General, exception_bad_reg_2){
    EXPECT_ANY_THROW(Task_3_Solver::task3_with_automat("ac.bab.c.*.ab.ba.+.++*a.", 'a', 1));
    EXPECT_ANY_THROW(Task_3_Solver::task3_with_stack("ac.bab.c.*.ab.ba.+.++*a.", 'a', 1));
}

TEST_F(General, exception_bad_reg_3){
    EXPECT_ANY_THROW(Task_3_Solver::task3_with_automat("*ac.bab.c.*.ab.ba.+.++*a.", 'a', 1));
    EXPECT_ANY_THROW(Task_3_Solver::task3_with_stack("*ac.bab.c.*.ab.ba.+.++*a.", 'a', 1));
}

int main(int argc, char* argv[]){
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}