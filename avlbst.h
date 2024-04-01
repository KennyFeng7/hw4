#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotate(AVLNode<Key,Value>* grand, AVLNode<Key,Value>* parent);
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);

    // Fix-up methods
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* target);
    void removeFix(AVLNode<Key, Value>* node, int8_t diff);



};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */


 
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO change root
    Key ky = new_item.first;
    Value val = new_item.second;
    AVLNode<Key, Value>* target = new AVLNode<Key, Value>(ky, val, nullptr);
    if(BinarySearchTree<Key,Value>::empty()){
        BinarySearchTree<Key, Value>::root_ = target;
        return;
    }
    AVLNode<Key, Value>* curr = dynamic_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_);
    AVLNode<Key, Value>* parent;
    while(curr != nullptr){
        parent = curr;
        if(ky < curr->getKey())
            curr = curr->getLeft();
        else if(ky > curr->getKey())
            curr = curr->getRight();
        else{
            curr->setValue(val);
            delete target;
            return;
        }
    }
    target->setParent(parent);

    if(ky < parent->getKey()){
        parent->setLeft(target);
    }
    else{
        parent->setRight(target);
    }
    target->setBalance(0);

    if(parent->getBalance() != 0){
      parent->setBalance(0);
      return;
    }
    else{
        if (target == parent->getLeft()) {
            parent->setBalance(-1);
        } 
        else {
            parent->setBalance(1);
        }
        insertFix(parent, target);
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* target) {
    if (parent == nullptr){
        return;
    }
    AVLNode<Key, Value>* grand = parent->getParent();
    if (grand == nullptr){
        return;
    }
    int8_t diff;
    if (parent == grand->getLeft()) {
        diff = -1;
    } 
    else {
        diff = 1;
    }

    grand->setBalance(grand->getBalance() + diff);
    if (grand->getBalance() == 0) {
        return;
    } 
    else if (grand->getBalance() == 1 || grand->getBalance() == -1) {
        insertFix(grand, parent);
    } 
    else {
        if (diff < 0) {
            parent = grand->getLeft();
        } 
        else {
            parent = grand->getRight();
        }
        rotate(grand, parent);
    }
}



template<class Key, class Value>
void AVLTree<Key, Value>::rotate(AVLNode<Key,Value>* grand, AVLNode<Key,Value>* parent){
    if(parent == grand->getLeft()){
        if(parent->getBalance() == -1){
            rotateRight(grand);
            grand->setBalance(0);
            parent->setBalance(0);
        }
        else if(parent->getBalance() == 0){
            rotateRight(grand);
            grand->setBalance(-1);
            parent->setBalance(1);
        }
        else{
            AVLNode<Key,Value>* node = parent->getRight();
            int8_t i = node->getBalance();
            rotateLeft(parent);
            rotateRight(grand);
            node->setBalance(0);
            grand->setBalance(0);
            parent->setBalance(0);
            if(i == -1){
              grand->setBalance(1);
            }
            else if(i == 1){
              parent->setBalance(-1);
            }
        }
    } 
    else{
        if(parent->getBalance() == 1){
            rotateLeft(grand);
            grand->setBalance(0);
            parent->setBalance(0);
        }
        else if(parent->getBalance() == 0){
            rotateLeft(grand);
            grand->setBalance(1);
            parent->setBalance(-1);
        }
        else{
            AVLNode<Key,Value>* node = parent->getLeft();
            int8_t i = node->getBalance();
            rotateRight(parent);
            rotateLeft(grand);
            node->setBalance(0);
            grand->setBalance(0);
            parent->setBalance(0);
            if(i == -1){
              parent->setBalance(1);
            }
            else if(i == 1){
              grand->setBalance(-1);
            }
        }
    }
}




template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* grand){
    if (!grand){
      return;
    }

    AVLNode<Key, Value>* parent = grand->getRight();
    if (!parent){
      return;
    }
    grand->setRight(parent->getLeft());
    if (parent->getLeft() != nullptr) {
        parent->getLeft()->setParent(grand);
    }

    parent->setLeft(grand);
    parent->setParent(grand->getParent());
    grand->setParent(parent);

    if (parent->getParent() != nullptr) {
        if (parent->getParent()->getLeft() == grand) {
            parent->getParent()->setLeft(parent);
        } 
        else {
            parent->getParent()->setRight(parent);
        }
    } 
    else {
        this->root_ = parent;
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* grand){
    if (!grand){
      return;
    }

    AVLNode<Key, Value>* parent = grand->getLeft();
    if (!parent){
      return;
    }
    grand->setLeft(parent->getRight());
    if (parent->getRight()) {
        parent->getRight()->setParent(grand);
    }

    parent->setRight(grand);
    parent->setParent(grand->getParent());
    grand->setParent(parent);

    if (parent->getParent() != nullptr) {
        if (parent->getParent()->getLeft() == grand) {
            parent->getParent()->setLeft(parent);
        } 
        else {
            parent->getParent()->setRight(parent);
        }
    } 
    else {
        this->root_ = parent;
    }
    
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* target = dynamic_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
    if(target == nullptr){
      return;
    }
    AVLNode<Key, Value>* parent = target->getParent();
    int8_t diff = 0;
    if (target->getLeft() != nullptr && target->getRight() != nullptr) {
        AVLNode<Key, Value>* pred = dynamic_cast<AVLNode<Key, Value>*>(this->predecessor(target));
        nodeSwap(target,pred);
        parent = target->getParent();
        if(target == parent->getLeft()){
            parent->setLeft(target->getLeft());
            diff = 1;
        }
        else{
            parent->setRight(target->getLeft());
            diff = -1;
        }

        if(target->getLeft() != nullptr){
          target->getLeft()->setParent(parent);
        }
    }
    else if(target->getLeft() == nullptr){
        if(parent == nullptr){
          BinarySearchTree<Key, Value>::root_ = target->getRight();
        }
        else if(target == parent->getLeft()){
            parent->setLeft(target->getRight());
            diff = 1;
        }
        else{
            parent->setRight(target->getRight());
            diff = -1;
        }

        if(target->getRight() != nullptr){
          target->getRight()->setParent(parent);
        }
    }
    else{
        if(parent == nullptr){
          BinarySearchTree<Key, Value>::root_ = target->getLeft();
        }
        else if(target == parent->getLeft()){
            parent->setLeft(target->getLeft());
            diff = 1;
        }
        else{
            parent->setRight(target->getLeft());
            diff = -1;
        }

        if(target->getLeft() != nullptr){
          target->getLeft()->setParent(parent);
        }
    }
    delete target;
    removeFix(parent, diff);
}



template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* grand, int8_t diff){
    if(grand == nullptr){
      return;
    }
    AVLNode<Key, Value>* great = grand->getParent();
    
    int8_t grand_diff;
    if (grand->getParent() == nullptr) {
        grand_diff = 0;
    }
    else {
        if (grand == grand->getParent()->getLeft()) {
            grand_diff = 1;
        } 
        else {
            grand_diff = -1;
        }
    }

    grand->setBalance(grand->getBalance() + diff);
    if(grand->getBalance() > 1 || grand->getBalance() < -1){
      AVLNode<Key, Value>* parent;
      if (diff < 0) {
        parent = grand->getLeft();
      } 
      else {
        parent = grand->getRight();
      }
      int8_t balance_parent = parent->getBalance();
      rotate(grand, parent);
      if(balance_parent != 0){
        removeFix(great, grand_diff);
      }
    }
    else if(grand->getBalance() == 1 || grand->getBalance() == -1){
      return;
    }
    else{
      removeFix(great, grand_diff);
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
