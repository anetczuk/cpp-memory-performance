//
//
//

#include <gtest/gtest.h>

#include "CustomLinkedList.h"


TEST(CustomLinkedList, listMemSize) {
    CustomLinkedList listA;
    listA.randomize();
    EXPECT_EQ(20, listA.memSize());

    CustomLinkedList listB(1);
    listB.randomize();
    EXPECT_EQ(2012, listB.memSize());
}

TEST(CustomLinkedList, properHead) {
    CustomLinkedList listA;
    listA.randomize();
    EXPECT_EQ(nullptr, listA.head);

    CustomLinkedList listB(1);
    listB.randomize();
    EXPECT_NE(nullptr, listB.head);
}

TEST(CustomLinkedList, properNext001) {
    CustomLinkedList listA(1);
    listA.randomize();
    EXPECT_EQ(nullptr, listA.head->next);

    CustomLinkedList listB(2);
    listB.randomize();
    EXPECT_NE(nullptr, listB.head->next);
}

TEST(CustomLinkedList, properNext002) {
    CustomLinkedList list(1000);
    list.randomize();
    std::size_t items = 0;
    CustomItem* item = list.head;
    while(item != nullptr) {
        item = item->next;
        ++items;
    }
    EXPECT_EQ(list.size(), items);
}

TEST(CustomLinkedList, properNext003) {
    CustomLinkedList list(1000);
    list.randomize();
    std::size_t same = 0;
    CustomItem* item = list.head;
    for(std::size_t i=0; i<list.size(); ++i) {
        if ( item == &(list.content[i]) ) ++same;
        item = item->next;
    }
    EXPECT_NE(list.size(), same);
}
