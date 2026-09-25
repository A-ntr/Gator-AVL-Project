//
// Created by An on 9/18/2026.
//

#ifndef PROJECT1_AVLTREE_H
#define PROJECT1_AVLTREE_H
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <queue>
#include <regex>
#include <unordered_set>
#include <iomanip>
#include <stack>
#include <queue>
using namespace std;

struct Node {
    std::string name, ID;
    Node* left;
    Node* right;
    int height;

    Node() : name(""), ID(""), left(nullptr), right(nullptr), height(0) {}
    Node(std::string name, std::string ID) : name(name), ID(ID), left(nullptr), right(nullptr), height(1) {}
};
class AVLTree {
    Node* root;
    int height(Node* node) {
        if (!node)
            return 0;
        return 1 + max(height(node->left), height(node->right));
    }
    Node* rotateLeft(Node* node) {
        Node* rightChild = node->right;
        node->right = rightChild->left;
        rightChild->left = node;
        return rightChild;
    }
    Node* rotateRight(Node* node) {
        Node* leftChild = node->left;
        node->left = leftChild->right;
        leftChild->right = node;
        return leftChild;
    }
    Node* rotateLR (Node* node) {
        node->left = rotateLeft(node->left);
        node = rotateRight(node);
        return node;
    }
    Node* rotateRL (Node* node) {
        node->right = rotateRight(node->right);
        node = rotateLeft(node);
        return node;
    }
    void inorder(Node* node) {
        if (!node) return;
        inorder(node->left);
        cout << node->name << ":" << node->ID << endl;
        inorder(node->right);
    }
    void preorder(Node* node) {
        if (!node) return;
        cout << node->name << ":" << node->ID << endl;
        preorder(node->left);
        preorder(node->right);
    }
    void postorder(Node* node) {
        if (!node) return;
        postorder(node->left);
        postorder(node->right);
        cout << node->name << ":" << node->ID << endl;
    }
public:
    AVLTree() : root(nullptr) {}
    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
        node = nullptr;
    }
    ~AVLTree() {
        // if (!root) return;
        // stack<Node*> node_stack;
        // if (root) node_stack.push(root);
        // while (!node_stack.empty()) {
        //     if (node_stack.top()->right)
        //         node_stack.push(node_stack.top()->right);
        //     if (node_stack.top()->left)
        //         node_stack.push(node_stack.top()->left);
        //     if (!node_stack.top()->left && !node_stack.top()->right) {
        //         delete node_stack.top();
        //         node_stack.pop();
        //     }
        // }
        clear(root);
    }
    Node* insert(Node* node, string name, string id) {
        if (!node) return new Node(name, id);
        // Traversing left subtree
        if (node->ID > id)
            node->left = insert(node->left, name, id);
        // Traversing right subtree
        else if (node->ID < id)
            node->right = insert(node->right, name, id);
        else
            return node;
        // update height
        node->height = height(node);

        return node;
    }
    void remove(std::string node, int data);
    void display(string traversal) {
        if (!root) {
            cout << "Tree is empty.\n";
            return;
        }
        queue<Node*> q;
        q.push(root);
        if (traversal == "inorder") {
            inorder(root);
            cout << "\n";
        }
        else if (traversal == "preorder") {
            preorder(root);
            cout << "\n";
        }
        else if (traversal == "postorder") {
            postorder(root);
            cout << "\n";
        }
        if (traversal == "display") {
            while (!q.empty()) {
                for (int i = 0; i < q.size(); ++i) {
                    Node* curr = q.front();
                    cout << q.front()->name << ": " << q.front()->ID << " h=" << q.front()->height << "\n";
                    q.pop();
                    if (curr->left)
                        q.push(curr->left);
                    if (curr->right)
                        q.push(curr->right);
                }
            }
        }
    }
    void execute(std::string& command){

        // convert to lower string
        transform(command.begin(), command.end(), command.begin(), [](unsigned char c) {
            return tolower(c);
        });
        cout << "command: " << command << endl;

        // static unordered_set<string> validCommands = {
        //     "insert",
        //     "remove",
        //     "search",
        //     "printinorder",
        //     "printpreorder",
        //     "printpostorder",
        //     "printlevelcount",
        //     "removeinorder"
        // };
        //
        // bool isValidCommand = false;
        // if (validCommands.find(command) != validCommands.end())
        //     isValidCommand = true;



        // Processing command
        stringstream ss(command);
        string word;
        vector<string> commandLine;
        while (ss >> quoted(word)) {
            commandLine.push_back(word);
        }
        // Command line indexing 0 = command
        // 1, 2 = additional arguments
        // 1 = name
        // 2 = UFID
        // Executing
        if (commandLine[0] == "insert") { // insert command
            cout << "insert command\n";
            if (commandLine.size() == 3) {
                cout << commandLine[0] << ", " <<commandLine[1] << ", " << commandLine[2] << endl;
                regex ufIDRegex = regex("[0-9]{8}");
                bool isUFID = regex_match(commandLine[2], ufIDRegex);
                regex nameRegex = regex("[A-Za-z\\s]+");
                bool isName = regex_match(commandLine[1], nameRegex);
                if (isUFID && isName) {
                    root = insert(root, commandLine[1], commandLine[2]);
                    cout << "successful\n";
                }
                else cout << "unsuccessful\n";
            }
            else cout << "unsuccessful\n";
        }
        else if (commandLine[0] == "printinorder") {
            display("inorder");
        }
        else if (commandLine[0] == "printpreorder") {
            display("preorder");
        }
        else if (commandLine[0] == "printpostorder") {
            display("postorder");
        }
        else if (commandLine[0] == "display") {
            display("display");
        }
    }
    // std::vector<int> getPreOrderIDs() {
    //     return {};
    // }

};


#endif //PROJECT1_AVLTREE_H