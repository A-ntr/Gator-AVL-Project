//
// Created by An on 9/18/2026.
//

#ifndef PROJECT1_AVLTREE_H
#define PROJECT1_AVLTREE_H
#include <string>
#include <vector>


class AVLTree {
    public:
    AVLTree();
    ~AVLTree();

    void insert(std::string node, int data);
    void remove(std::string node, int data);
    void display();
    void execute(std::string command);
    std::vector<int> getPreOrderIDs();
    private:
    AVLTree* left;
    AVLTree* right;
    AVLTree* root;

};


#endif //PROJECT1_AVLTREE_H