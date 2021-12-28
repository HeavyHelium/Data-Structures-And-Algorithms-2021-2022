#ifndef HW2_1_MY_TESTS_H
#define HW2_1_MY_TESTS_H
#include <stdexcept>

/*
int throws()
{
    throw std::runtime_error("joker");
    return -1;
}*/

TEST_CASE("more on incorporate", "[Hierarchy]")
{
    SECTION("example")
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
                "Uspeshnia-Mitko\n"
                "Mitko-MishoPetrov\n"
                "MishoPetrov-Misho\n"
                "Misho-Slav\n");
    }

}

TEST_CASE("more on join", "[Hierarchy]")
{
    SECTION("Joining of hierarchies with employee in both hierarchies, "
            "the higher one is taken")
    {
        std::string lower_data = "Uspeshnia-A\n"
                                 "Uspeshnia-B\n"
                                 "Uspeshnia-C\n"
                                 "B-M\n"
                                 "M-s\n";
        std::string higher_data = "Uspeshnia-A\n"
                                  "Uspeshnia-C\n"
                                  "Uspeshnia-P\n"
                                  "A-M\n"
                                  "C-s\n";
        Hierarchy lower(lower_data);
        Hierarchy higher(higher_data);
        Hierarchy merged(lower.join(higher));
        REQUIRE(merged.print() == "Uspeshnia-A\n"
                                  "Uspeshnia-B\n"
                                  "Uspeshnia-C\n"
                                  "Uspeshnia-P\n"
                                  "A-M\n"
                                  "C-s\n");
    }
    SECTION("Joining of hierarchies with employee in both hierarchies, "
            "same level, but different managers")
    {
        Hierarchy h1("Uspeshnia-F\n"
                     "Uspeshnia-B\n"
                     "Uspeshnia-C\n"
                     "F-D\n"
                     "C-E\n"
                     "C-Q\n");

        Hierarchy h2("Uspeshnia-M\n"
                     "Uspeshnia-N\n"
                     "Uspeshnia-A\n"
                     "A-D\n"
                     "A-P\n");
        // D should be a child of A, not a child of F, since A < F lexicographically
        Hierarchy h3(h1.join(h2));
        REQUIRE(h3.print() == "Uspeshnia-A\n"
                              "Uspeshnia-B\n"
                              "Uspeshnia-C\n"
                              "Uspeshnia-F\n"
                              "Uspeshnia-M\n"
                              "Uspeshnia-N\n"
                              "A-D\n"
                              "A-P\n"
                              "C-E\n"
                              "C-Q\n");
    }

    SECTION("When joining is impossible, "
            "the resultant hierarchy must be completely empty")
    {
       std::string h1_data =  "Uspeshnia-B\n"
                              "Uspeshnia-C\n"
                              "Uspeshnia-F\n"
                              "C-E\n"
                              "C-Q\n"
                              "F-D\n";

       std::string h2_data = "Uspeshnia-B\n"
                             "Uspeshnia-P\n"
                             "Uspeshnia-Q\n"
                             "Q-C\n"
                             "Q-D\n";

       Hierarchy h1(h1_data);
       Hierarchy h2(h2_data);
       Hierarchy h3 = h1.join(h2);
       REQUIRE(h3.num_employees() == 0);
    }

    SECTION("example from discord")
    {
        std::string h1_data = "Uspeshnia-B\n"
                              "Uspeshnia-C\n"
                              "C-D\n"
                              "C-F\n"
                              "D-Q";
        std::string h2_data = "Uspeshnia-B\n"
                              "Uspeshnia-C\n"
                              "Uspeshnia-D\n"
                              "B-F\n"
                              "F-Q\n";
        Hierarchy h1(h1_data);
        Hierarchy h2(h2_data);
        Hierarchy h3 = h1.join(h2);
        REQUIRE(h3.print() == "Uspeshnia-B\n"
                              "Uspeshnia-C\n"
                              "Uspeshnia-D\n"
                              "B-F\n"
                              "D-Q\n");
    }

}



#endif //HW2_1_MY_TESTS_H
