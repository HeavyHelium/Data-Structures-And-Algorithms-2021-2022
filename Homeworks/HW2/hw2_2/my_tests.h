#ifndef HW2_1_MY_TESTS_H
#define HW2_1_MY_TESTS_H
#include <stdexcept>
#include <utility>

std::string large_hierarchy = "Uspeshnia - Gosho\n"
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

TEST_CASE("num_all_subordinates", "[Hierarchy]")
{
    Hierarchy h(large_hierarchy);
    REQUIRE(h.num_employees() == 38);
    REQUIRE(h.num_all_subordinates("Uspeshnia") == 37);
    REQUIRE(h.num_all_subordinates("Gosho") == 30);
    REQUIRE(h.num_all_subordinates("Slavi") == 4);
    REQUIRE(h.num_all_subordinates("Pesho") == 26);
    REQUIRE(h.num_all_subordinates("Lilibet") == 5);
    REQUIRE(h.num_all_subordinates("Cat3") == 0);
    REQUIRE(h.num_all_subordinates("Non_existent") == -1);
}

TEST_CASE("more on manager", "[Hierarchy]")
{
    Hierarchy h(large_hierarchy);
    SECTION("When there is no such person in the hierarchy, an exception is thrown")
    {
        REQUIRE_THROWS(h.manager("Non_existent"));
    }
    SECTION("Uspeshnia has no manager")
    {
        REQUIRE(h.manager("Uspeshnia") == "");
    }
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
    SECTION("big hierarchy")
    {
        Hierarchy b(large_hierarchy);
        REQUIRE(b.longest_chain() == 7);
        b.incorporate();
        std::string res = "Uspeshnia-Gosho\n"
                          "Gosho-Misho\n"
                          "Gosho-Pesho\n"
                          "Gosho-Slavi\n"
                          "Pesho-Dancho\n"
                          "Pesho-Elizabeth\n"
                          "Slavi-Slav1\n"
                          "Dancho-Boris\n"
                          "Elizabeth-Alex\n"
                          "Elizabeth-Hercule\n"
                          "Elizabeth-Megan\n"
                          "Elizabeth-Sherlock\n"
                          "Slav1-Mecho\n"
                          "Slav1-Slav2\n"
                          "Boris-Kamen\n"
                          "Hercule-Hastings\n"
                          "Megan-Andrew\n"
                          "Megan-Archie\n"
                          "Megan-Harry\n"
                          "Megan-Kate\n"
                          "Megan-William\n"
                          "Sherlock-George\n"
                          "Mecho-Q12Adl\n"
                          "Andrew-Fergie\n"
                          "Archie-Cat1\n"
                          "Archie-Lilibet\n"
                          "George-MsHudson\n"
                          "George-Watson\n"
                          "Cat1-Cat2\n"
                          "Cat1-Cat3\n"
                          "Cat1-Doggo1\n"
                          "Cat1-Doggo2\n"
                          "Lilibet-Cat4\n"
                          "Cat4-Cat5\n"
                          "Cat4-Cat6\n"
                          "Cat4-Doggo3\n"
                          "Cat4-Doggo4\n";
        REQUIRE(b.longest_chain() == 9);
        REQUIRE(b.print() == res);
    }

}

TEST_CASE("more on modernize", "[Hierarchy]")
{
    SECTION("large hierarchy")
    {
        Hierarchy h(large_hierarchy);
        REQUIRE(h.num_employees() == 38);
        std::string res = "Uspeshnia-Dancho\n"
                          "Uspeshnia-Misho\n"
                          "Uspeshnia-Pesho\n"
                          "Uspeshnia-Slav1\n"
                          "Uspeshnia-Slav2\n"
                          "Dancho-Boris\n"
                          "Dancho-Kamen\n"
                          "Pesho-Alex\n"
                          "Pesho-Andrew\n"
                          "Pesho-George\n"
                          "Pesho-Harry\n"
                          "Pesho-Hastings\n"
                          "Pesho-Kate\n"
                          "Pesho-Megan\n"
                          "Pesho-MsHudson\n"
                          "Pesho-Watson\n"
                          "Pesho-William\n"
                          "Slav1-Q12Adl\n"
                          "Andrew-Fergie\n"
                          "Megan-Cat1\n"
                          "Megan-Cat2\n"
                          "Megan-Cat3\n"
                          "Megan-Cat4\n"
                          "Megan-Cat5\n"
                          "Megan-Cat6\n"
                          "Megan-Doggo1\n"
                          "Megan-Doggo2\n"
                          "Megan-Doggo3\n"
                          "Megan-Doggo4\n";
        h.modernize();
        REQUIRE(h.num_employees() == 30);
        REQUIRE(h.print() == res);
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

    SECTION("Large hierarchy joined with smaller")
    {
        Hierarchy h1(large_hierarchy);
        Hierarchy h2(lozenec);
        std::string res = "Uspeshnia-Gosho\n"
                          "Uspeshnia-Misho\n"
                          "Uspeshnia-Slavi\n"
                          "Gosho-Dancho\n"
                          "Gosho-Pesho\n"
                          "Slavi-Slav1\n"
                          "Slavi-Slav2\n"
                          "Dancho-Boris\n"
                          "Dancho-Kamen\n"
                          "Pesho-Alex\n"
                          "Pesho-Elizabeth\n"
                          "Pesho-Hercule\n"
                          "Pesho-Sherlock\n"
                          "Slav1-Mecho\n"
                          "Elizabeth-Andrew\n"
                          "Elizabeth-Harry\n"
                          "Elizabeth-Kate\n"
                          "Elizabeth-Megan\n"
                          "Elizabeth-William\n"
                          "Hercule-Hastings\n"
                          "Sherlock-George\n"
                          "Sherlock-MsHudson\n"
                          "Sherlock-Watson\n"
                          "Mecho-Q12Adl\n"
                          "Andrew-Fergie\n"
                          "Megan-Archie\n"
                          "Megan-Lilibet\n"
                          "Archie-Cat1\n"
                          "Archie-Cat2\n"
                          "Archie-Cat3\n"
                          "Archie-Doggo1\n"
                          "Archie-Doggo2\n"
                          "Lilibet-Cat4\n"
                          "Lilibet-Cat5\n"
                          "Lilibet-Cat6\n"
                          "Lilibet-Doggo3\n"
                          "Lilibet-Doggo4\n";
        Hierarchy merged = h2.join(h1);
        REQUIRE(merged.print() == res);
    }

    SECTION("When joining is impossible, "
            "the resultant hierarchy must be completely empty")
    {
        INFO("An error message must be showed, but not as an exception");
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

TEST_CASE("more on overloaded", "[Hierarchy]")
{
    SECTION("On an empty hierarchy must return 0")
    {
        Hierarchy h("");
        REQUIRE(h.num_overloaded() == 0);
    }
    SECTION("On a large hierarchy")
    {
        Hierarchy h1(large_hierarchy);
        REQUIRE(h1.num_overloaded() == 3);
        REQUIRE(h1.num_overloaded(15) == 4);
        REQUIRE(h1.num_overloaded(10) == 5);
        REQUIRE(h1.num_overloaded(4) == 7);
        REQUIRE(h1.num_overloaded(3) == 8);
        REQUIRE(h1.num_overloaded(2) == 9);
        REQUIRE(h1.num_overloaded(1) == 11);
        REQUIRE(h1.num_overloaded(0) == 14);

    }

}

TEST_CASE("empty hierarchy operations", "[Hierarchy]")
{
    SECTION("Hiring people in an empty hierarchy")
    {
        Hierarchy h1("");
        REQUIRE(h1.num_employees() == 0);
        REQUIRE(!h1.find("Uspeshnia"));
        INFO("The boss should be created when hiring, even if the hierarchy is empty");
        h1.hire("Slartibarfast","Uspeshnia");
        REQUIRE(h1.num_employees() == 2);
    }
    SECTION("Firing people in an empty hierarchy must return false")
    {
        Hierarchy h("");
        REQUIRE(!h.fire("Gosho"));
        REQUIRE(!h.fire("Uspeahnia"));
        REQUIRE_THROWS(h.manager("Gosho"));
    }
    SECTION("Joining two empty hierarchies must be an empty hierarchy")
    {
        Hierarchy h("");
        Hierarchy h1("");
        Hierarchy merged = h1.join(h);
        REQUIRE(merged.num_employees() == 0);
        REQUIRE(!merged.find("Uspeshnia"));
    }
    SECTION("Modernizing an empty hierarchy must have no effect")
    {
        Hierarchy h("");
        h.modernize();
        REQUIRE(h.num_employees() == 0);
        REQUIRE(!h.find("Uspeshnia"));
    }
    SECTION("Incorporating an empty hierarchy must have no effect")
    {
        Hierarchy h("");
        h.incorporate();
        REQUIRE(h.num_employees() == 0);
        REQUIRE(!h.find("Uspeshnia"));
    }
}

TEST_CASE("only one subordinate in hierarchy", "[Hierarchy]")
{
    Hierarchy h("Uspeshnia-Neo\n");
    REQUIRE(h.num_subordinates("Uspeshnia") == 1);
    REQUIRE(h.num_employees() == 2);
    REQUIRE(h.manager("Neo") == "Uspeshnia");
    SECTION("Firing and hiring the boss must return false")
    {
        REQUIRE(!h.fire("Uspeshnia"));
        REQUIRE(!h.hire("Uspeshnia", "Uspeshnia"));
        REQUIRE(!h.hire("Uspeshnia", "Gosho"));
    }
    SECTION("Modernization must have no effect")
    {
        REQUIRE(h.num_employees() == 2);
        REQUIRE(h.num_subordinates("Uspeshnia") == 1);
        REQUIRE(h.manager("Neo") == "Uspeshnia");
    }
    SECTION("Incorporation must have no effect")
    {
        REQUIRE(h.num_employees() == 2);
        REQUIRE(h.num_subordinates("Uspeshnia") == 1);
        REQUIRE(h.manager("Neo") == "Uspeshnia");
    }
}


#endif //HW2_1_MY_TESTS_H
