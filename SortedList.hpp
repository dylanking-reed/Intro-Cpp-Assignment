#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <string_view>
#include <vector>
using std::string_view;
using std::vector;

class ListNode {
  public:
    ListNode *prev;  
    ListNode *next;
    string_view val;
    ListNode(string_view _val);
    ListNode(const ListNode &other);
    void attach_after(ListNode* node);
    void attach_before(ListNode* node);
    void remove_after();
    void remove_before();
};


typedef enum SortingMethod_ {
  Lexicographical, Length
} SortingMethod;

class SortedList {
  public:
    SortedList(SortingMethod method = SortingMethod::Lexicographical);
    SortedList(const SortedList &other);
    ~SortedList();
    void insert(const string_view item);
    void remove_value(const string_view item);
    void empty();
    SortingMethod get_sorting_method() const;
    size_t get_length() const;
    vector<string_view> *into_vec() const;
    string_view at(size_t i) const;
    void sort_by_length();
    void sort_by_lexicographical();
  private:
    ListNode *first; 
    size_t length;
    SortingMethod sorting_method;
};

#endif
