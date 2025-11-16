#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

// Helper function is needed. The helper would return height if subtree has equal paths, -1 otherwise
int checkEqualPaths(Node* root);

bool equalPaths(Node * root)
{
    //if helper function returns -1, paths are NOT equal
    return checkEqualPaths(root) != -1;
}

int checkEqualPaths(Node* root)
{
    // Base case 1: empty tree - return -1 to indicate "no tree exists here"
    if (root == nullptr) {
        return -1;
    }
    
    // Base case 2: leaf node - has height 0 and valid equal paths (trivially)
    if (root->left == nullptr && root->right == nullptr) {
        return 0;
    }
    
    // case 3 (recursive case) get heights from left and right subtrees
    int leftHeight = checkEqualPaths(root->left);
    int rightHeight = checkEqualPaths(root->right);
    
    // We check if either subtree has invalid paths (not from being empty)
    // We need to differentiate between "empty" (-1) and "invalid due to unequal paths"
    bool leftExists = (root->left != nullptr);
    bool rightExists = (root->right != nullptr);
    
    // If a subtree exists but returned -1, it has unequal paths... so invalid!
    if (leftExists && leftHeight == -1) return -1;
    if (rightExists && rightHeight == -1) return -1;
    
    // Now, part two, handle the cases different cases of the subtrees
    if (!leftExists && !rightExists) {
        // Both children are null. this is a leaf
        return 0;
    }
    else if (!leftExists) {
        // Only right subtree exists
        return 1 + rightHeight;
    }
    else if (!rightExists) {
        // Only left subtree exists  
        return 1 + leftHeight;
    }
    else {
        // Both subtrees exist so they must have equal heights
        if (leftHeight != rightHeight) {
            return -1; // Unequal paths
        }
        return 1 + leftHeight; // (same as 1 + rightHeight since they're equal)
    }
}

