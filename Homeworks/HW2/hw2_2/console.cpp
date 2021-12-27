#include <iostream>
#include "input_handler.h"

#ifdef IDIOT_TESTING
const string TheBoss = "Uspeshnia";

const string lozenec =
        "Uspeshnia - Gosho \n"
        "Uspeshnia - Misho \n"
        "Gosho     - Pesho \n"
        "Gosho     - Dancho\n"
        "Pesho     - Alex  \n"
        "Dancho    - Boris \n"
        "Dancho    - Kamen \n"
        "Uspeshnia - Slavi \n"
        "Slavi     - Slav1 \n"
        "Slavi     - Slav2 \n"
        "Slav1     - Mecho \n"
        "Mecho     - Q12Adl\n";

const string large =
        "Uspeshnia - 1\n"
        "1-2\n1-3\n1-4\n1-5\n1-6\n1-7\n"
        "2-21\n2-22\n2-23\n2-24\n2-25\n2-26\n2-27\n2-28\n"
        "3-31\n3-32\n3-33\n3-34\n3-35\n3-36\n3-37\n3-38\n3-39\n";

const string loz_new =
        " Uspeshnia   -  MishoPetrov \n"
        " MishoPetrov -  Misho       \n"
        " MishoPetrov -  Slav        \n";


void create(const string& str)
{
    Hierarchy h(str);
}
#endif

int main()try
{
#ifdef IDIOT_TESTING
    Hierarchy l_new(loz_new);
    Hierarchy loz(lozenec);
    Hierarchy h(large);

    Hierarchy l2 = loz.join(loz);
    assert(l2.print() == loz.print());
    assert(h.print() == h.join(h).print());

    Hierarchy joined = l_new.join(loz);
    assert(joined.print() ==
            "Uspeshnia-Gosho\nUspeshnia-Misho\nUspeshnia-MishoPetrov\nUspeshnia-Slavi\nGosho-Dancho\nGosho-Pesho"
            "\nMishoPetrov-Slav\nSlavi-Slav1\nSlavi-Slav2\nDancho-Boris\nDancho-Kamen\nPesho-Alex\nSlav1-Mecho\nMecho-Q12Adl\n");
    assert(joined.num_employees() == 15);
    assert(joined.longest_chain() == 5);
#endif

#ifndef IDIOT_TESTING
    input_handler ih;
    ih.getInput();
#endif
    return 0;
}
catch(const std::exception& e)
{ std::cerr << "error: " << e.what(); }