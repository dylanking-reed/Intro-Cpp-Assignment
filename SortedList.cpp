#include "SortedList.hpp"

#include <cassert>

ListNode::ListNode(string_view _val) {
  this->prev = NULL;
  this->next = NULL;
  this->val = _val;
}

ListNode::ListNode(const ListNode &other) {
  this->prev = NULL;
  this->next = NULL;
  this->val = other.val;
}
void ListNode::attach_after(ListNode* node) {
  ListNode* old_next = node->next;
  node->next = this;
  this->prev = node;
  this->next = old_next;
}
void ListNode::attach_before(ListNode* node) {
  ListNode* old_prev = node->prev;
  node->prev = this;
  this->prev = old_prev;
  this->next = node;
}
void ListNode::remove_after() {
  ListNode* new_next = this->next->next;
  delete this->next;
  this->next = new_next;
}
void ListNode::remove_before() {
  ListNode* new_prev = this->prev->prev;
  delete this->prev;
  this->prev = new_prev;
}


SortedList::SortedList(SortingMethod method) {
  this->first = NULL;
  this->length = 0;
  this->sorting_method = method;
}

SortedList::SortedList(const SortedList &other) {
  this->first = new ListNode(*(other.first));
  ListNode* curr_other = other.first;
  ListNode* curr_this = this->first;
  ListNode* prev_this = NULL;
  while (curr_this != NULL && curr_other != NULL) {
    curr_this->next = new ListNode(*(curr_other->next)); 
    curr_this->prev = prev_this; 
    prev_this = curr_this;
    curr_this = curr_this->next;
    curr_other = curr_other->next;
  }
}

SortedList::~SortedList() {
  ListNode* curr = this->first;
  ListNode* next = this->first->next;
  while (next != NULL) {
    next = curr->next;
    delete curr;
    curr = next;
  } 
}

void SortedList::insert(const string_view item) {
  ListNode* curr = this->first;
  while (curr != NULL) {
    if (this->sorting_method == SortingMethod::Lexicographical) {
      if (curr->val > item) {
        (new ListNode(item))->attach_before(curr);
	this->length++;
	return;
      }
    } else if (this->sorting_method == SortingMethod::Length) {
      if (curr->val.length() > item.length()) {
        (new ListNode(item))->attach_before(curr);
	this->length++;
	return;
      }
    }
    curr = curr->next;
  } 
  if (curr->prev != NULL) {
    (new ListNode(item))->attach_after(curr->prev);
    this->length++;
  } else {
    assert(this->first == NULL);
    this->first = new ListNode(item);
    this->length++;
  }
}

void SortedList::remove_value(const string_view item) {
  if (this->first->val == item) {
    delete this->first;
    this->first = NULL;
    this->length--;
  }
}

void SortedList::empty() {
  ListNode* curr = this->first;
  ListNode* next = this->first->next;
  while (next != NULL) {
    next = curr->next;
    delete curr;
    curr = next;
  }
  this->first = NULL; 
  this->length = 0;
}

SortingMethod SortedList::get_sorting_method() const {
  return this->sorting_method;
}

size_t SortedList::get_length() const {
  return this->length;
}

vector<string_view> *SortedList::into_vec() const {
  auto out = new vector<string_view>();
  auto curr = this->first;
  while (curr != NULL) {
    out->push_back(curr->val);
    curr = curr->next;
  }
  return out;
}

string_view SortedList::at(size_t i) const {
  auto curr = this->first;
  size_t a = 0;
  while (curr != NULL) {
    if (a == i) return curr->val;
    a++;
    curr = curr->next;
  }
  return string_view();
}

void SortedList::sort_by_length() {
  auto vec = this->into_vec();
  this->empty();
  this->sorting_method = SortingMethod::Length;
  for (auto elem : *vec) {
    this->insert(elem);
  }
}

void SortedList::sort_by_lexicographical() {
  auto vec = this->into_vec();
  this->empty();
  this->sorting_method = SortingMethod::Lexicographical;
  for (auto elem : *vec) {
    this->insert(elem);
  }
}

