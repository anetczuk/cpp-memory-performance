//
//
//

#ifndef SRC_LINKEDLIST_H_
#define SRC_LINKEDLIST_H_

#include <vector>
#include <algorithm>


typedef uint64_t ListPayload[248];


struct CustomItem {
	ListPayload data;					// occupy memory
    CustomItem* next = nullptr;
};

static const std::size_t CUSTOM_ITEM_SIZE = sizeof(CustomItem);


struct CustomLinkedList {
	typedef CustomItem value_type;

    std::vector<value_type> content;
    value_type* head;

    CustomLinkedList(const std::size_t count = 0): content(count), head(nullptr) {
    }

    virtual ~CustomLinkedList() {
    }

    const value_type* data() const {
    	return head;
    }

    void setInOrder() {
        head = nullptr;
        const std::size_t count = content.size();
        if (count < 1)
            return ;
        head = &content[0];
        for(std::size_t i=1; i<count; ++i) {
            content[i-1].next = &(content[i]);
        }
        content[count-1].next = nullptr;
    }

    void randomize() {
        head = nullptr;
        const std::size_t count = content.size();
        if (count < 1)
            return ;
        std::vector<value_type*> list( count );
        for(std::size_t i=0; i<count; ++i) {
            list[i] = &content[i];
        }
        head = list[0];
        random_shuffle(list.begin()+1, list.end());
        for(std::size_t i=1; i<count; ++i) {
            list[i-1]->next = list[i];
        }
        list[count-1]->next = nullptr;
    }

    std::size_t size() const {
        return content.size();
    }

    std::size_t memSize() const;
};


static const std::size_t CUSTOM_LIST_SIZE = sizeof(CustomLinkedList);


std::size_t CustomLinkedList::memSize() const {
    return CUSTOM_LIST_SIZE + content.size() * CUSTOM_ITEM_SIZE;
}


#endif /* SRC_LINKEDLIST_H_ */
