#include "AVLTree.h"

AVLTree::AVLTree() : root(0) {} // done

Node* AVLTree::findUnbalancedNode(Node* currNode) {
    while (currNode != nullptr) {
        if (balanceFactor(currNode) == -2 || balanceFactor(currNode) == 2) {
            return currNode;
        }
        currNode = currNode->parent;
    }

    return nullptr;
}   // done
void AVLTree::rotate(Node* currNode) {
    int currBalance = balanceFactor(currNode);
    Node* parentNode = currNode->parent;
    char childSide;

    if (parentNode == nullptr) {childSide = 't';}   // currNode was a root (top)
    else if (parentNode->left == currNode) {childSide = 'l';}    // currNode was a left node
    else if (parentNode->right == currNode) {childSide = 'r';}  // currNode was a right node


    if (currBalance == 2) { // left side higher
        if (balanceFactor(currNode->left) == -1) {  // right side of left side is unbalanced (zig zag)
            rotateLeft(currNode->left, 'l');
        }
        rotateRight(currNode, childSide);
    }
    else if (currBalance == -2) {   // right side higher
        if (balanceFactor(currNode->right) == 1) {  // left side of right side is unbalanced (zig zag)
            rotateRight(currNode->right, 'r');
        }
        rotateLeft(currNode, childSide);
    }
}   // done
void AVLTree::rotateLeft(Node* axisNode, const char cSide) {
    Node* parentNode = axisNode->parent;

    axisNode->right->parent = parentNode;
    axisNode->parent = axisNode->right;

    axisNode->right = axisNode->parent->left;
    axisNode->parent->left = axisNode;
    if (cSide == 't') {
        root = axisNode->parent;
    }
    else if (cSide == 'l') {
        parentNode->left = axisNode->parent;
    }
    else if (cSide == 'r') {
        parentNode->right = axisNode->parent;
    }

    updateHeights(axisNode);
}   // done
void AVLTree::rotateRight(Node* axisNode, const char cSide) {
    Node* parentNode = axisNode->parent;

    axisNode->left->parent = parentNode;
    axisNode->parent = axisNode->left;

    axisNode->left = axisNode->parent->right;
    axisNode->parent->right = axisNode;
    if (cSide == 't') {
        root = axisNode->parent;
    }
    else if (cSide == 'l') {
        parentNode->left = axisNode->parent;
    }
    else if (cSide == 'r') {
        parentNode->right = axisNode->parent;
    }

    updateHeights(axisNode);
}   // done
void AVLTree::printBalanceFactors(Node* currNode) {
    if (currNode == nullptr) {  // branch is done
        return;
    }
    printBalanceFactors(currNode->left);
    cout << currNode->data << "(" << balanceFactor(currNode) << "), ";
    printBalanceFactors(currNode->right);
}   // done
void AVLTree::updateHeights(Node* currNode) {
    if (currNode == nullptr) {
        return;
    }
    int leftHeight = 0;
    int rightHeight = 0;

    if (currNode->left != nullptr) {
        leftHeight = currNode->left->height + 1;
    }
    if (currNode->right != nullptr) {
        rightHeight = currNode->right->height + 1;
    }

    if (leftHeight >= rightHeight) {
        currNode->height = leftHeight;
    }
    else {
        currNode->height = rightHeight;
    }
    updateHeights(currNode->parent);
}
void AVLTree::visualizeTree(ofstream& outFS, Node* n) {
    if(n){
        if(n->left){
            visualizeTree(outFS,n->left);
            outFS<<n->data <<" -> " <<n->left->data<<";"<<endl;
        }

        if(n->right){
            visualizeTree(outFS,n->right);
            outFS<<n->data <<" -> " <<n->right->data<<";"<<endl;
        }
    }
}   // given, done

void AVLTree::insert(const string& newStr) {
    Node* newNode = new Node(newStr);
    if (root == nullptr) {  // empty list
        root = newNode;
        return;
    }

    Node* currNode = root;
    Node* parentNode = nullptr;
    while (currNode != nullptr) {
        if (currNode->data == newStr) {return;} // duplicate
        else if (newStr < currNode->data) { // less than node
            parentNode = currNode;
            currNode = currNode->left;
        }
        else {  // greater than node
            parentNode = currNode;
            currNode = currNode->right;
        }
    }

    if (newStr < parentNode->data) {
        parentNode->left = newNode;
        newNode->parent = parentNode;
    }
    else if (newStr > parentNode->data) {
        parentNode->right = newNode;
        newNode->parent = parentNode;
    }

    updateHeights(parentNode);

    Node* unbalancedNode = findUnbalancedNode(newNode);
    while (unbalancedNode != nullptr) {
        rotate(unbalancedNode);
        updateHeights(parentNode);
        unbalancedNode = findUnbalancedNode(newNode);
    }
}   // done
int AVLTree::balanceFactor(Node* currNode) {
    if (root == nullptr) {  // empty
        return 0;   // -1 - -1 = 0
    }
    int leftHeight = 0;
    int rightHeight = 0;

    if (currNode->left != nullptr) {leftHeight = currNode->left->height + 1;}
    if (currNode->right != nullptr) {rightHeight = currNode->right->height + 1;}
    int balanceFactor = leftHeight - rightHeight;

    return balanceFactor;
}   // done
void AVLTree::printBalanceFactors() {
    if (root == nullptr) {
        cout << "Empty List" << endl;
    }
    else {
        printBalanceFactors(root);
        cout << endl;
    }

    visualizeTree("graph");
}   // done
void AVLTree::visualizeTree(const string& outputFilename) {
    ofstream outFS(outputFilename.c_str());
    if(!outFS.is_open()){
        cout<<"Error"<<endl;
        return;
    }
    outFS<<"digraph G {"<<endl;
    visualizeTree(outFS,root);
    outFS<<"}";
    outFS.close();
    string jpgFilename = outputFilename.substr(0,outputFilename.size()-4)+".jpg";
    string command = "dot -Tjpg " + outputFilename + " -o " + jpgFilename;
    system(command.c_str());
}   // given, done
