//
//
//

#include <iostream>

#include "CustomLinkedList.h"
#include "testutils.h"



int main() {
    int failures = 0;

    {
        CustomLinkedList list(1);
        list.randomize();
        if (list.head->next != nullptr) {
            ++failures;
            std::cout << __LINE__ << ": test failed" << std::endl;
        }
    }
    {
        CustomLinkedList list(2);
        list.randomize();
        if (list.head->next == nullptr) {
            ++failures;
            std::cout << __LINE__ << ": test failed" << std::endl;
        }
    }
    {
        // test list null item
        CustomLinkedList list(1000);
        list.randomize();
        std::size_t items = 0;
        CustomItem* item = list.head;
        while(item != nullptr) {
            item = item->next;
            ++items;
        }
        if (items != list.size()) {
            ++failures;
            std::cout << __LINE__ << ": test failed" << std::endl;
        }
    }
    {
        // test list randomized
        CustomLinkedList list(1000);
        list.randomize();
        std::size_t same = 0;
        CustomItem* item = list.head;
        for(std::size_t i=0; i<list.size(); ++i) {
            if ( item == &(list.content[i]) ) ++same;
            item = item->next;
        }
        if (same == list.size()) {
            ++failures;
            std::cout << __LINE__ << ": test failed" << std::endl;
        }
    }

    if (failures == 0) {
        std::cout << "OK" << std::endl;
    }
    return std::min( failures, 127 );
}
