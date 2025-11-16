#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>

/**
 * A templated class for a Node in a search tree.
 * The getters for parent/left/right are virtual so
 * that they can be overridden for future kinds of
 * search trees, such as Red Black trees, Splay trees,
 * and AVL trees.
 */
template <typename Key, typename Value>
class Node
{
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value &value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};

/*
  -----------------------------------------
  Begin implementations for the Node class.
  -----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent) :
    item_(key, value),
    parent_(parent),
    left_(NULL),
    right_(NULL)
{

}

/**
* Destructor, which does not need to do anything since the pointers inside of a node
* are only used as references to existing nodes. The nodes pointed to by parent/left/right
* are freed by the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node()
{

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const
{
    return item_;
}

/**
* A non-const getter for the item.
*/
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem()
{
    return item_;
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
    return item_.first;
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
    return item_.second;
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
    return item_.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
    return parent_;
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
    return left_;
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
    return right_;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
    parent_ = parent;
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
    left_ = left;
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
    right_ = right;
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree
{
public:
    BinarySearchTree(); //TODO
    virtual ~BinarySearchTree(); //TODO
    virtual void insert(const std::pair<const Key, Value>& keyValuePair); //TODO
    virtual void remove(const Key& key); //TODO
    void clear(); //TODO
    bool isBalanced() const; //TODO
    void print() const;
    bool empty() const;

    template<typename PPKey, typename PPValue>
    friend void prettyPrintBST(BinarySearchTree<PPKey, PPValue> & tree);
public:
    /**
    * An internal iterator class for traversing the contents of the BST.
    */
    class iterator  // TODO
    {
    public:
        iterator();

        std::pair<const Key,Value>& operator*() const;
        std::pair<const Key,Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key,Value>* ptr);
        Node<Key, Value> *current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;
    Value& operator[](const Key& key);
    Value const & operator[](const Key& key) const;

protected:
    // Mandatory helper functions
    Node<Key, Value>* internalFind(const Key& k) const; // TODO
    Node<Key, Value> *getSmallestNode() const;  // TODO
    static Node<Key, Value>* predecessor(Node<Key, Value>* current); // TODO
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.

    // Provided helper functions
    virtual void printRoot (Node<Key, Value> *r) const;
    virtual void nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2) ;

    // Add helper functions here
    void clearHelper(Node<Key, Value>* node);
    int getHeight(Node<Key, Value>* node) const;
    bool checkBalance(Node<Key, Value>* node) const;
    Node<Key, Value>* insertHelper(Node<Key, Value>* node, const std::pair<const Key, Value>& keyValuePair, Node<Key, Value>* parent);
    Node<Key, Value>* internalFindHelper(Node<Key, Value>* node, const Key& key) const;
    Node<Key, Value>* getSmallestHelper(Node<Key, Value>* node) const;
    static Node<Key, Value>* getRightmostHelper(Node<Key, Value>* node);
    static Node<Key, Value>* findPredecessorAncestorHelper(Node<Key, Value>* current, Node<Key, Value>* parent);


protected:
    Node<Key, Value>* root_;
    // You should not need other data members
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value> *ptr)
{
    current_ = ptr;
}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator() 
{
    current_ = NULL;
}

/**
* Provides access to the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> &
BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return current_->getItem();
}

/**
* Provides access to the address of the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> *
BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(current_->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator==(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return current_ == rhs.current_;
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator!=(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return current_ != rhs.current_;
}


/**
* Advances the iterator's location using an in-order sequencing
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator&
BinarySearchTree<Key, Value>::iterator::operator++()
{
    if (current_ == NULL) {
        return *this; // already at end, stay at end
    }
    
    // Case 1: current has right child
    if (current_->getRight() != NULL) {
        // go right, then left as far as possible
        current_ = current_->getRight();
        while (current_->getLeft() != NULL) {
            current_ = current_->getLeft();
        }
    }
    // Case 2: no right child
    else {
        Node<Key, Value>* parent = current_->getParent();
        // go up until we find a parent where we came from the left
        while (parent != NULL && current_ == parent->getRight()) {
            current_ = parent;
            parent = parent->getParent();
        }
        current_ = parent; // will be NULL if we've gone past the largest element
    }
    
    return *this;
}


/*
-------------------------------------------------------------
End implementations for the BinarySearchTree::iterator class.
-------------------------------------------------------------
*/

/*
-----------------------------------------------------
Begin implementations for the BinarySearchTree class.
-----------------------------------------------------
*/

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() 
{
    root_ = NULL;
}

/**
* Destructor - called when BST object is destroyed
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
    clear();
}

/**
 * Returns true if tree is empty
*/
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const
{
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
    printRoot(root_);
    std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const
{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const
{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key & k) const
{
    Node<Key, Value> *curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

/**
 * @precondition The key exists in the map
 * Returns the value associated with the key
 */
template<class Key, class Value>
Value& BinarySearchTree<Key, Value>::operator[](const Key& key)
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}
template<class Key, class Value>
Value const & BinarySearchTree<Key, Value>::operator[](const Key& key) const
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}

/**
* An insert method to insert into a Binary Search Tree.
* The tree will not remain balanced when inserting.
* Recall: If key is already in the tree, you should 
* overwrite the current value with the updated value.
*/
//note, pair is used with .first and .second
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value> &keyValuePair)
{
    // Use recursive helper
    root_ = insertHelper(root_, keyValuePair, NULL);
}


/**
* A remove method to remove a specific key from a Binary Search Tree.
* Recall: The writeup specifies that if a node has 2 children you
* should swap with the predecessor and then remove.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
    Node<Key, Value>* toDelete = internalFind(key);
    
    if (toDelete == NULL) {
        return; // key not found, nothing to do
    }
    
    // Three cases to handle: 0 children, 1 child, or 2 children
    
    // Case 1: Node has 2 children - this is the tricky one
    // We need to swap with predecessor first to simplify the removal
    if (toDelete->getLeft() != NULL && toDelete->getRight() != NULL) {
        Node<Key, Value>* pred = predecessor(toDelete);
        nodeSwap(toDelete, pred);
        // after swap, toDelete is guaranteed to have at most 1 child
    }
    
    // Case 2 & 3: Node has at most 1 child (much easier)
    Node<Key, Value>* child = NULL;
    
    // figure out which child exists (if any)
    if (toDelete->getLeft() != NULL) {
        child = toDelete->getLeft();
    } else if (toDelete->getRight() != NULL) {
        child = toDelete->getRight();
    }
    // if both are null, child stays null (leaf node)
    
    // connect the child to toDelete's parent
    if (child != NULL) {
        child->setParent(toDelete->getParent());
    }
    
    // update the parent to point to child instead of toDelete
    if (toDelete->getParent() == NULL) {
        // special case: removing root
        root_ = child;
    } else if (toDelete == toDelete->getParent()->getLeft()) {
        toDelete->getParent()->setLeft(child);
    } else {
        toDelete->getParent()->setRight(child);
    }
    
    delete toDelete;
}



template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current)
{
    if (current == NULL) {
        return NULL;
    }
    
    // Case 1: if has left child, go left then all the way right using helper
    if (current->getLeft() != NULL) {
        current = current->getLeft();
        // Find rightmost node in left subtree using helper
        return getRightmostHelper(current);
    }
    
    // Case 2: no left child - go up until we find where we came from right side using helper
    Node<Key, Value>* parent = current->getParent();
    return findPredecessorAncestorHelper(current, parent);
}


/**
* A method to remove all contents of the tree and
* reset the values in the tree for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
    clearHelper(root_);
    root_ = NULL;
}

// Helper function to recursively delete nodes
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clearHelper(Node<Key, Value>* node)
{
    if (node == NULL) {
        return;
    }
    
    // delete left and right subtrees first
    clearHelper(node->getLeft());
    clearHelper(node->getRight());
    
    // then delete current node
    delete node;
}


/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getSmallestNode() const
{
    return getSmallestHelper(root_);
}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
    return internalFindHelper(root_, key);
}

/**
 * Return true iff the BST is balanced.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
    return checkBalance(root_);
}

// Helper function to check if tree is balanced
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::checkBalance(Node<Key, Value>* node) const
{
    if (node == NULL) {
        return true; // empty tree is balanced
    }
    
    // check if left and right subtrees are balanced
    if (!checkBalance(node->getLeft()) || !checkBalance(node->getRight())) {
        return false;
    }
    
    // check if height difference is at most 1
    int leftH = getHeight(node->getLeft());
    int rightH = getHeight(node->getRight());
    
    return abs(leftH - rightH) <= 1;
}

// Helper function to calculate height of a subtree
template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::getHeight(Node<Key, Value>* node) const
{
    if (node == NULL) {
        return 0;
    }
    
    int leftH = getHeight(node->getLeft());
    int rightH = getHeight(node->getRight());
    
    return 1 + std::max(leftH, rightH);
}



template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2)
{
    if((n1 == n2) || (n1 == NULL) || (n2 == NULL) ) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if(n1p != NULL && (n1 == n1p->getLeft())) n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if(n2p != NULL && (n2 == n2p->getLeft())) n2isLeft = true;


    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if( (n1r != NULL && n1r == n2) ) {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if( n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    }
    else if( n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    }
    else if( n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);

    }


    if(n1p != NULL && n1p != n2) {
        if(n1isLeft) n1p->setLeft(n2);
        else n1p->setRight(n2);
    }
    if(n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if(n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if(n2p != NULL && n2p != n1) {
        if(n2isLeft) n2p->setLeft(n1);
        else n2p->setRight(n1);
    }
    if(n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if(n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }


    if(this->root_ == n1) {
        this->root_ = n2;
    }
    else if(this->root_ == n2) {
        this->root_ = n1;
    }

}

/**
 * Lastly, we are providing you with a print function,
   BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->root_) // or any other node pointer

   It will print up to 5 levels of the tree rooted at the passed node,
   in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

// Recursive helper for getSmallestNode
template<class Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::getSmallestHelper(Node<Key, Value>* node) const
{
    // Base case: empty tree or no left child
    if (node == NULL || node->getLeft() == NULL) {
        return node;
    }
    
    // Recurse left
    return getSmallestHelper(node->getLeft());
}

// Recursive helper for BST insert
template<class Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::insertHelper(Node<Key, Value>* node, const std::pair<const Key, Value>& keyValuePair, Node<Key, Value>* parent)
{
    // Base case: found the spot to insert
    if (node == NULL) {
        return new Node<Key, Value>(keyValuePair.first, keyValuePair.second, parent);
    }
    
    // Key already exists, just update value
    if (keyValuePair.first == node->getKey()) {
        node->setValue(keyValuePair.second);
        return node;
    }
    
    // Update parent pointer
    node->setParent(parent);
    
    // Recurse left or right
    if (keyValuePair.first < node->getKey()) {
        Node<Key, Value>* newLeft = insertHelper(node->getLeft(), keyValuePair, node);
        node->setLeft(newLeft);
    } else {
        Node<Key, Value>* newRight = insertHelper(node->getRight(), keyValuePair, node);
        node->setRight(newRight);
    }
    
    return node;
}

// Recursive helper for BST internalFind
template<class Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFindHelper(Node<Key, Value>* node, const Key& key) const
{
    // Base case: node is null or we found the key
    if (node == NULL || key == node->getKey()) {
        return node;
    }
    
    // Recurse left or right
    if (key < node->getKey()) {
        return internalFindHelper(node->getLeft(), key);
    } else {
        return internalFindHelper(node->getRight(), key);
    }
}

// Static helper function to find rightmost node in a subtree (for predecessor)
template<class Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::getRightmostHelper(Node<Key, Value>* node)
{
    if (node == NULL || node->getRight() == NULL) {
        return node;
    }
    return getRightmostHelper(node->getRight());
}

// Static helper function to find predecessor ancestor (for predecessor)
template<class Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::findPredecessorAncestorHelper(Node<Key, Value>* current, Node<Key, Value>* parent)
{
    while (parent != NULL && current == parent->getLeft()) {
        current = parent;
        parent = parent->getParent();
    }
    return parent; // will be NULL if current was already the smallest
}

/*
---------------------------------------------------
End implementations for the BinarySearchTree class.
---------------------------------------------------
*/

#endif
