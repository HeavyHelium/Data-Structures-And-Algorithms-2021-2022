#ifndef HW2_1_MY_TESTS_H
#define HW2_1_MY_TESTS_H
#include <stdexcept>
#include <utility>

TEST_CASE("copy semantics", "[Hierarchy]")
{
    Hierarchy h1(lozenec);
    Hierarchy h2(h1);
    REQUIRE(h1.num_employees() == h2.num_employees());
    REQUIRE(h2.manager("Gosho") == "Uspeshnia");
    REQUIRE(h2.manager("Misho") == "Uspeshnia");
    REQUIRE(h2.manager("Pesho") == "Gosho");
    REQUIRE(h2.manager("Alex") == "Pesho");
    REQUIRE(h2.manager("Boris") == "Dancho");
    REQUIRE(h2.manager("Kamen") == "Dancho");
    REQUIRE(h2.manager("Slavi") == "Uspeshnia");
    REQUIRE(h2.manager("Slav1") == "Slavi");
    REQUIRE(h2.manager("Slav2") == "Slavi");
    REQUIRE(h2.manager("Mecho") == "Slav1");
    REQUIRE(h2.manager("Q12Adl") == "Mecho");
}

TEST_CASE("move semantics", "[Hierarchy]")
{
    Hierarchy h1(lozenec);
    std::size_t temp = h1.num_employees();
    Hierarchy h2(std::move(h1));
    REQUIRE(h1.num_employees() == 0);
    REQUIRE(h2.num_employees() == temp);
}

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

TEST_CASE("more on oerloaded", "[Hierarchy]")
{
    std::string data = "Uspeshnia - Gosho\n"
                       "Uspeshnia - Misho\n"
                       "Uspeshnia - Slavi\n"
                       "Gosho - Dancho\n"
                       "Gosho - Pesho\n"
                       "Slavi - Slav1\n"
                       "Slavi - Slav2\n"
                       "Dancho - Boris\n"
                       "Dancho - Kamen\n"
                       "Pesho - Alex\n"
                       "Pesho - Elizabeth\n"
                       "Pesho - Hercule\n"
                       "Pesho - Sherlock\n"
                       "Slav1 - Mecho\n"
                       "Elizabeth - Andrew\n"
                       "Elizabeth - Harry\n"
                       "Elizabeth - Kate\n"
                       "Elizabeth - William\n"
                       "Elizabeth - Megan\n"
                       "Hercule - Hastings\n"
                       "Sherlock - George\n"
                       "Sherlock - Watson\n"
                       "Sherlock - MsHudson\n"
                       "Mecho - Q12Adl\n"
                       "Andrew - Fergie\n"
                       "Megan - Archie\n"
                       "Megan - Lilibet\n"
                       "Archie - Cat1\n"
                       "Archie - Cat2\n"
                       "Archie - Cat3\n"
                       "Archie - Doggo1\n"
                       "Archie - Doggo2\n"
                       "Lilibet - Cat4\n"
                       "Lilibet - Cat5\n"
                       "Lilibet - Cat6\n"
                       "Lilibet - Doggo3\n"
                       "Lilibet - Doggo4\n";
    Hierarchy h1(data);
    REQUIRE(h1.num_overloaded() == 3);
    REQUIRE(h1.num_overloaded(15) == 4);
    REQUIRE(h1.num_overloaded(10) == 5);
    REQUIRE(h1.num_overloaded(4) == 7);
    REQUIRE(h1.num_overloaded(3) == 8);
    REQUIRE(h1.num_overloaded(2) == 9);
    REQUIRE(h1.num_overloaded(1) == 11);
    REQUIRE(h1.num_overloaded(0) == 14);
}


#endif //HW2_1_MY_TESTS_H
