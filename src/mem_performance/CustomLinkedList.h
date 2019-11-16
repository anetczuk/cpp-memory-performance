/// MIT License
///
/// Copyright (c) 2019 Arkadiusz Netczuk <dev.arnet@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

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
