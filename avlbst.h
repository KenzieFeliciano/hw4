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

    // Helper functions for AVL operations
    // I made these because I was repeating the same logic over and over
    void bubbleUp(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void fixTree(AVLNode<Key, Value>* node, int8_t diff);
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    
    // Recursive helper for insert to replace the loop
    AVLNode<Key, Value>* insertHelper(AVLNode<Key, Value>* curr, const std::pair<const Key, Value>& new_item, AVLNode<Key, Value>*& newNode);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // handle empty tree first, thats the easy case
    if (this->root_ == NULL) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        return;
    }
    
    // Use recursive helper to find insertion point and insert
    AVLNode<Key, Value>* newNode = nullptr;
    AVLNode<Key, Value>* parent = insertHelper(static_cast<AVLNode<Key, Value>*>(this->root_), new_item, newNode);
    
    // If newNode is null, the key already existed and value was updated
    if (newNode == nullptr) {
        return;
    }
    
    // now handle the AVL balancing part. this was tricky to get right
    if (newNode == parent->getLeft()) {
        // adding to left side
        if (parent->getBalance() == 1) {
            // was right heavy, now balanced
            parent->setBalance(0);
        } else if (parent->getBalance() == 0) {
            // was balanced, now left heavy
            parent->setBalance(-1);
            bubbleUp(parent, newNode);
        } else {
            // was left heavy, now would be very left heavy which is bad
            // Need to rotate around parent immediately
            if (newNode->getKey() < parent->getKey()) {
                // Left Left case: newNode is left child of parent
                rotateRight(parent);
                parent->setBalance(0);
                newNode->setBalance(0);
            } else {
                // Left Right case: newNode is right child of parent's left child
                // This shouldn't happen since newNode is direct child of parent
                // But handle it just in case I messed something up
                parent->setBalance(-2);
                bubbleUp(parent, newNode);
            }
        }
    } else {
        // adding to right side  
        if (parent->getBalance() == -1) {
            // was left heavy, now balanced
            parent->setBalance(0);
        } else if (parent->getBalance() == 0) {
            // was balanced, now right heavy
            parent->setBalance(1);
            bubbleUp(parent, newNode);
        } else {
            // was right heavy, now would be very right heavy which violates AVL
            // Need to rotate around parent immediately
            if (newNode->getKey() > parent->getKey()) {
                // Right Right case: newNode is right child of parent
                rotateLeft(parent);
                parent->setBalance(0);
                newNode->setBalance(0);
            } else {
                // Right Left case: newNode is left child of parent's right child
                // This shouldn't happen since newNode is direct child of parent
                // But handle it just in case there's a bug somewhere
                parent->setBalance(2);
                bubbleUp(parent, newNode);
            }
        }
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    AVLNode<Key, Value>* toDelete = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    
    if (toDelete == NULL) {
        return; // not there, nothing to do
    }
    
    // if it has 2 kids, swap with predecessor like regular BST
    if (toDelete->getLeft() != NULL && toDelete->getRight() != NULL) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(toDelete));
        nodeSwap(toDelete, pred);
    }
    
    // now toDelete has at most 1 child
    AVLNode<Key, Value>* parent = toDelete->getParent();
    AVLNode<Key, Value>* child = NULL;
    int8_t diff = 0;
    
    // figure out which child to use and calculate diff BEFORE deleting
    if (toDelete->getLeft() != NULL) {
        child = toDelete->getLeft();
    } else if (toDelete->getRight() != NULL) {
        child = toDelete->getRight();
    }
    
    // Calculate diff before deleting toDelete
    if (parent != NULL) {
        if (toDelete == parent->getLeft()) {
            diff = 1; // left side got shorter
        } else {
            diff = -1; // right side got shorter
        }
    }
    
    // hook child to parent
    if (child != NULL) {
        child->setParent(parent);
    }
    
    if (parent == NULL) {
        // deleting root
        this->root_ = child;
    } else {
        if (toDelete == parent->getLeft()) {
            parent->setLeft(child);
        } else {
            parent->setRight(child);
        }
    }
    
    delete toDelete;
    
    // rebalance the tree starting from parent using the standard AVL approach
    // I spent way too much time debugging this part
    if (parent != NULL) {
        fixTree(parent, diff);
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

// this is where all the AVL rotation stuff happens
template<class Key, class Value>
void AVLTree<Key, Value>::bubbleUp(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node)
{
    if (parent == NULL || parent->getParent() == NULL) {
        return; // cant go up anymore
    }
    
    AVLNode<Key, Value>* g = parent->getParent(); // grandparent
    
    if (parent == g->getLeft()) {
        // parent is left child, so g gets more left heavy
        g->updateBalance(-1);
        
        if (g->getBalance() == 0) {
            return; // balanced now
        } else if (g->getBalance() == -1) {
            bubbleUp(g, parent); // keep going up
        } else {
            // g->getBalance() == -2, need to rotate!
            AVLNode<Key, Value>* leftChild = static_cast<AVLNode<Key, Value>*>(g->getLeft());
            if (leftChild->getBalance() <= 0) {
                // left left case (balance is -1 or 0)
                rotateRight(g);
                if (leftChild->getBalance() == 0) {
                    // This case can happen in deletions, not insertions
                    leftChild->setBalance(1);
                    g->setBalance(-1);
                } else {
                    leftChild->setBalance(0);
                    g->setBalance(0);
                }
            } else {
                // left right case (leftChild balance is 1)
                AVLNode<Key, Value>* grandchild = static_cast<AVLNode<Key, Value>*>(leftChild->getRight());
                rotateLeft(leftChild);
                rotateRight(g);
                
                if (grandchild->getBalance() == 1) {
                    leftChild->setBalance(-1);
                    g->setBalance(0);
                } else if (grandchild->getBalance() == -1) {
                    leftChild->setBalance(0);
                    g->setBalance(1);
                } else {
                    leftChild->setBalance(0);
                    g->setBalance(0);
                }
                grandchild->setBalance(0);
            }
        }        } else {
            // parent is right child, so g gets more right heavy
            g->updateBalance(1);
            
            if (g->getBalance() == 0) {
                return; // balanced now
            } else if (g->getBalance() == 1) {
                bubbleUp(g, parent); // keep going up
            } else {
                // g->getBalance() == 2, need to rotate the other way!
                AVLNode<Key, Value>* rightChild = static_cast<AVLNode<Key, Value>*>(g->getRight());
                if (rightChild->getBalance() >= 0) {
                    // right right case (balance is 1 or 0)
                    rotateLeft(g);
                    if (rightChild->getBalance() == 0) {
                        // This case can happen in deletions, not insertions  
                        rightChild->setBalance(-1);
                        g->setBalance(1);
                    } else {
                        rightChild->setBalance(0);
                        g->setBalance(0);
                    }
                } else {
                    // right left case (rightChild balance is -1)
                    AVLNode<Key, Value>* grandchild = static_cast<AVLNode<Key, Value>*>(rightChild->getLeft());
                    rotateRight(rightChild);
                    rotateLeft(g);
                    
                    if (grandchild->getBalance() == -1) {
                        rightChild->setBalance(1);
                        g->setBalance(0);
                    } else if (grandchild->getBalance() == 1) {
                        rightChild->setBalance(0);
                        g->setBalance(-1);
                    } else {
                        rightChild->setBalance(0);
                        g->setBalance(0);
                    }
                    grandchild->setBalance(0);
                }
            }
        }
}

//handles removal rebalancing
template<class Key, class Value>
void AVLTree<Key, Value>::fixTree(AVLNode<Key, Value>* node, int8_t diff)
{
    if (node == NULL) {
        return;
    }
    
    AVLNode<Key, Value>* parent = node->getParent();
    int8_t nextDiff = 0;
    
    // figure out what diff to use for parent
    if (parent != NULL) {
        if (node == parent->getLeft()) {
            nextDiff = 1;
        } else {
            nextDiff = -1;
        }
    }
    
    node->updateBalance(diff);
    
    if (node->getBalance() == 0) {
        fixTree(parent, nextDiff); // height decreased, keep going up
    } else if (abs(node->getBalance()) == 1) {
        return; // height didn't change, stop here
    } else {
        // node is unbalanced (balance is 2 or -2), need to rotate
        AVLNode<Key, Value>* child;
        
        if (diff == -1) {
            // right side got shorter, left side is now taller
            child = node->getLeft();
            
            // If child is NULL but balance is +/- 2, there's a balance factor bug
            // Force a correction and continue
            if (child == NULL) {
                node->setBalance(0);
                if (parent != NULL) {
                    fixTree(parent, nextDiff);
                }
                return;
            }
            
            if (child->getBalance() <= 0) {
                // left left case
                rotateRight(node);
                if (child->getBalance() == 0) {
                    // special case for removal
                    node->setBalance(-1);
                    child->setBalance(1);
                } else {
                    node->setBalance(0);
                    child->setBalance(0);
                    fixTree(parent, nextDiff);
                }
            } else {
                // left right case
                AVLNode<Key, Value>* grandchild = child->getRight();
                if (grandchild == NULL) {
                    return; // shouldn't happen in balanced tree, but safety check
                }
                rotateLeft(child);
                rotateRight(node);
                
                if (grandchild->getBalance() == -1) {
                    node->setBalance(1);
                    child->setBalance(0);
                } else if (grandchild->getBalance() == 1) {
                    node->setBalance(0);
                    child->setBalance(-1);
                } else {
                    node->setBalance(0);
                    child->setBalance(0);
                }
                grandchild->setBalance(0);
                fixTree(parent, nextDiff);
            }
        } else {
            // left side got shorter, right side is now taller  
            child = node->getRight();
            
            // If child is NULL but balance is +/- 2, there's a balance factor bug
            // Force a correction and continue
            if (child == NULL) {
                node->setBalance(0);
                if (parent != NULL) {
                    fixTree(parent, nextDiff);
                }
                return;
            }
            
            if (child->getBalance() >= 0) {
                // right-right case
                rotateLeft(node);
                if (child->getBalance() == 0) {
                    // special case for removal
                    node->setBalance(1);
                    child->setBalance(-1);
                } else {
                    node->setBalance(0);
                    child->setBalance(0);
                    fixTree(parent, nextDiff);
                }
            } else {
                // right-left case
                AVLNode<Key, Value>* grandchild = child->getLeft();
                if (grandchild == NULL) {
                    return; // shouldn't happen in balanced tree, but safety check
                }
                rotateRight(child);
                rotateLeft(node);
                
                if (grandchild->getBalance() == -1) {
                    node->setBalance(1);
                    child->setBalance(0);
                } else if (grandchild->getBalance() == 1) {
                    node->setBalance(0);
                    child->setBalance(-1);
                } else {
                    node->setBalance(0);
                    child->setBalance(0);
                }
                grandchild->setBalance(0);
                fixTree(parent, nextDiff);
            }
        }
    }
}

// rotate left. node goes down, right child goes up
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* rightKid = node->getRight();
    node->setRight(rightKid->getLeft());
    
    if (rightKid->getLeft() != NULL) {
        rightKid->getLeft()->setParent(node);
    }
    
    rightKid->setParent(node->getParent());
    
    if (node->getParent() == NULL) {
        this->root_ = rightKid;
    } else if (node == node->getParent()->getLeft()) {
        node->getParent()->setLeft(rightKid);
    } else {
        node->getParent()->setRight(rightKid);
    }
    
    rightKid->setLeft(node);
    node->setParent(rightKid);
}

// rotate right. node goes down, left child goes up
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* leftKid = node->getLeft();
    node->setLeft(leftKid->getRight());
    
    if (leftKid->getRight() != NULL) {
        leftKid->getRight()->setParent(node);
    }
    
    leftKid->setParent(node->getParent());
    
    if (node->getParent() == NULL) {
        this->root_ = leftKid;
    } else if (node == node->getParent()->getLeft()) {
        node->getParent()->setLeft(leftKid);
    } else {
        node->getParent()->setRight(leftKid);
    }
    
    leftKid->setRight(node);
    node->setParent(leftKid);
}

// Recursive helper for insert - replaces the while loop
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::insertHelper(AVLNode<Key, Value>* curr, const std::pair<const Key, Value>& new_item, AVLNode<Key, Value>*& newNode)
{
    if (curr == nullptr) {
        // This shouldn't happen since we check for empty tree before calling this
        return nullptr;
    }
    
    if (new_item.first == curr->getKey()) {
        // key already exists, just update the value and we're done
        curr->setValue(new_item.second);
        newNode = nullptr; // signal that no new node was created
        return curr;
    }
    
    if (new_item.first < curr->getKey()) {
        if (curr->getLeft() == nullptr) {
            // Found insertion point on left
            newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, curr);
            curr->setLeft(newNode);
            return curr; // return parent of new node
        } else {
            // Keep searching left
            return insertHelper(curr->getLeft(), new_item, newNode);
        }
    } else {
        if (curr->getRight() == nullptr) {
            // Found insertion point on right
            newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, curr);
            curr->setRight(newNode);
            return curr; // return parent of new node
        } else {
            // Keep searching right
            return insertHelper(curr->getRight(), new_item, newNode);
        }
    }
}

#endif
