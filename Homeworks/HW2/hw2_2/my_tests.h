#ifndef HW2_1_MY_TESTS_H
#define HW2_1_MY_TESTS_H

TEST_CASE("example incorporate")
{
    Hierarchy loz(loz_new);
    CHECK(loz.longest_chain() == 3);
    CHECK(loz.num_employees() == 4);
    CHECK(loz.num_subordinates(TheBoss) == 1);
    CHECK(loz.manager("MishoPetrov") == TheBoss);
    loz.hire("Mitko", TheBoss);
    loz.hire("MishoPetrov", "Mitko");
    loz.incorporate();

    REQUIRE(loz.print() ==
            "Uspeshnia-Mitko\nMitko-MishoPetrov\nMishoPetrov-Misho\nMisho-Slav\n");
}


#endif //HW2_1_MY_TESTS_H
