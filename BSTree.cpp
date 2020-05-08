/* Code by Stephen Dong */

#include "BSTree.h"

BSTree::BSTree() : root(0) {}   // done
BSTree::~BSTree() {

}   // edit

void BSTree::insert(const string& newString) {
    Node* currNode = root;

    if (root == nullptr) {  // empty list
        Node* newNode = new Node(newString);
        root = newNode;
        // newNode = nullptr;
    }
    else {  // list has at least one node
        Node* parentNode = currNode;
        while (currNode != nullptr) {
            if (newString == currNode->GetData()) {     // string already exists
                currNode->ChangeCount(1);
                return;
            }
            else if (newString < currNode->GetData()) {     // string is less than current string
                parentNode = currNode;
                currNode = currNode->GetLeft();
            }
            else {      // string is greater than current string
                parentNode = currNode;
                currNode = currNode->GetRight();
            }
        }

        Node* newNode = new Node(newString);
        if (newString < parentNode->GetData()) {    // string is less than current string
            parentNode->SetLeft(newNode);
        }
        else {      // string is greater than current string
            parentNode->SetRight(newNode);
        }
    }
}   // done
void BSTree::remove(const string& key) {    // increment deletion works, leaf deletion not work, middle of tree deletion seg fault when reading
    if (root == nullptr) {return;}  // empty

    Node* currNode = root;
    Node* prevNode = nullptr;
    while (key != currNode->GetData()) {    // exit if string is equal to current
        if (key < currNode->GetData()) {    // string is less than current
            prevNode = currNode;
            currNode = currNode->GetLeft();
        }
        else {  // string is more than current
            prevNode = currNode;
            currNode = currNode->GetRight();
        }

        if (currNode == nullptr) {  // key does not exist
//            cout << "Not Found" << endl;
            return;
        }
    }

    if (currNode->GetCount() > 1) { // multiple instances of string
        currNode->ChangeCount(-1);
    }
    else {  // only one instance of string
        Node* sucNode = nullptr;
        Node* sucParentNode = currNode;    // parent to sucNode
        Node* tempLeft = currNode->GetLeft();
        Node* tempRight = currNode->GetRight();
        if (currNode->GetLeft() != nullptr) {   // left exists
            // left one, right until leaf (greatest of the least)
            sucNode = currNode->GetLeft();

            while (sucNode->GetRight() != nullptr) {
                sucParentNode = sucNode;
                sucNode = sucNode->GetRight();
            }

            currNode->SetLeft(sucNode->GetLeft());
            currNode->SetRight(sucNode->GetRight());
            sucNode->SetLeft(tempLeft);
            sucNode->SetRight(tempRight);
            if (currNode == root) { // change root pointer if currNode was the root
                root = sucNode;
            }
            else {
                if (currNode->GetData() < prevNode->GetData()) {    // deleting node was on the left side
                    prevNode->SetLeft(sucNode);
                }
                else if (currNode->GetData() > prevNode->GetData()) {   // deleting node was on the right
                    prevNode->SetRight(sucNode);
                }
            }

            if (sucParentNode != currNode) {    // not equal b/c if it is, currNode is already swapped,
                sucParentNode->SetRight(currNode->GetLeft());   // connets previous node's right pointer to the parts after currNode
            }
            else {
                sucNode->SetLeft(currNode->GetLeft());  // connects sucNode's left pointer to the left pointer of currNode to keep consistency (no right since the successor was only one node away)
            }

            delete currNode;
        }
        else if (currNode->GetRight() != nullptr) { // right exists
            // right one, left until leaf (least of the greatest)
            sucNode = currNode->GetRight();

            while (sucNode->GetLeft() != nullptr) {
                sucParentNode = sucNode;
                sucNode = sucNode->GetLeft();
            }

            currNode->SetRight(sucNode->GetRight());
            currNode->SetLeft(sucNode->GetLeft());
            sucNode->SetRight(tempRight);
            sucNode->SetLeft(tempLeft);
            if (currNode == root) { // change root pointer if currNode was the root
                root = sucNode;
            }
            else {
                if (currNode->GetData() < prevNode->GetData()) {    // deleting node was on the left side
                    prevNode->SetLeft(sucNode);
                }
                else if (currNode->GetData() > prevNode->GetData()) {   // deleting node was on the right
                    prevNode->SetRight(sucNode);
                }
            }

            if (sucParentNode != currNode) {    // not equal b/c if it is, currNode is already swapped,
                sucParentNode->SetLeft(currNode->GetRight());   // connets previous node's right pointer to the parts after currNode
            }
            else {
                sucNode->SetRight(currNode->GetRight());  // connects sucNode's left pointer to the left pointer of currNode to keep consistency (no right since the successor was only one node away)
            }

            delete currNode;
        }
        else {  // leaf node
            if (currNode != root) { // not last item
                if (currNode->GetData() < prevNode->GetData()) {    // deleting node was on the left side
                prevNode->SetLeft(nullptr);
                }
                else if (currNode->GetData() > prevNode->GetData()) {   // deleting node was on the right
                    prevNode->SetRight(nullptr);
                }
            }
            else {
                root = nullptr;
            }

            delete currNode;
        }

//        if (currNode->GetRight() == nullptr && currNode->GetLeft() == nullptr){  // leaf node
//
//        }
//        else if (sucParentNode != currNode) {
//            sucParentNode->SetLeft(currNode->GetLeft());
//            sucParentNode->SetRight(currNode->GetRight());
//        }
//
//        delete currNode;
    }
}   // done

bool BSTree::search(const string &key) const {
    if (root == nullptr) {return false;}  // empty

    Node* currNode = root;
    while (key != currNode->GetData()) {    // exit if string is equal to current
        if (key < currNode->GetData()) {    // string is less than current
            currNode = currNode->GetLeft();
        }
        else {  // string is more than current
            currNode = currNode->GetRight();
        }

        if (currNode == nullptr) {  // key does not exist
            return false;
        }
    }

    return true;
}   // done
string BSTree::largest() const {
    if (root == nullptr) {  // empty
        return "";
    }

    Node* currNode = root;
    while (currNode->GetRight() != nullptr) {
        currNode = currNode->GetRight();
    }

    return currNode->GetData();
}   // done
string BSTree::smallest() const {
    if (root == nullptr) {
        return "";
    }

    Node* currNode = root;
    while (currNode->GetLeft() != nullptr) {
        currNode = currNode->GetLeft();
    }

    return currNode->GetData();
}   // done
int BSTree::height(const string& findString) {
    if (root == nullptr) {  // empty
        return -1;
    }

    Node* currNode = root;
    int height = 0;
    while (currNode != nullptr) {
        if (findString == currNode->GetData()) {      // string has been found
            break;
        }
        else if (findString < currNode->GetData()) {     // string is less than current string
            currNode = currNode->GetLeft();
        }
        else {      // string is greater than current string
            currNode = currNode->GetRight();
        }
    }
    if (currNode == nullptr) {return -1;}   // reached bottom before finding string
//    while (currNode != nullptr) {
    height = nodeHeight(currNode);
//    }

    return height;
}   // done
void BSTree::preOrder() const {
    Node* currNode = root;
    preOrder(currNode);
}   // done
void BSTree::postOrder() const {
    Node* currNode = root;
    postOrder(currNode);
}   // done
void BSTree::inOrder() const {
    Node* currNode = root;
    inOrder(currNode);
}   // done

void BSTree::preOrder(Node* currNode) const {
    if (currNode == nullptr) {return;}

    cout << currNode->GetData() << "(" << currNode->GetCount() << "), ";
//    if (currNode->GetData() != this->largest()) {cout << " ";}
    preOrder(currNode->GetLeft());
    preOrder(currNode->GetRight());
}   // done
void BSTree::postOrder(Node* currNode) const {
    if (currNode == nullptr) {return;}

    postOrder(currNode->GetLeft());
    postOrder(currNode->GetRight());
    cout << currNode->GetData() << "(" << currNode->GetCount() << "), ";
//    if (currNode != root) {cout << " ";}
}   // done
void BSTree::inOrder(Node* currNode) const {
    if (currNode == nullptr) {return;}

    inOrder(currNode->GetLeft());
    cout << currNode->GetData() << "(" << currNode->GetCount() << "), ";
//    if (currNode->GetData() != this->largest()) {cout << " ";}
    inOrder(currNode->GetRight());
}   // done
int BSTree::nodeHeight(Node* currNode) {
    if (currNode == nullptr) {  // leaf node
        return -1;
    }

    int thisHeight = 0;
    int leftHeight = 1 + nodeHeight(currNode->GetLeft());
    int rightHeight = 1 + nodeHeight(currNode->GetRight());

    if (leftHeight >= rightHeight) {
        thisHeight += leftHeight;
    }
    else {
        thisHeight += rightHeight;
    }

    return thisHeight;
}
