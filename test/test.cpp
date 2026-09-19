// Name: An Tran
// UFID: 78249429

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

// Stream redirection helper to test functions that print directly to std::cout
struct CoutRedirect {
    std::stringstream buffer;
    std::streambuf* old_buf;

    CoutRedirect() : old_buf(std::cout.rdbuf(buffer.rdbuf())) {}
    ~CoutRedirect() { std::cout.rdbuf(old_buf); }

    std::string str() const { return buffer.str(); }
    void clear() {
        buffer.str("");
        buffer.clear();
    }
};


#ifndef AVL_TREE_STUB_DEFINED
#define AVL_TREE_STUB_DEFINED
class AVLTree {
public:
    void executeCommand(const std::string& command) {
        // Stub: fails by default so tests run and fail cleanly
        std::cout << "unsuccessful\n";
    }

    bool insert(const std::string& name, int id) {
        return false;
    }

    bool remove(int id) {
        return false;
    }

    std::vector<int> getInOrder() const {
        return {};
    }

    std::vector<int> getPreOrder() const {
        return {};
    }
};
#endif

// TEST 1: At least five command executions that print "unsuccessful"
TEST_CASE("AVL Tree: Unsuccessful Command Executions", "[unsuccessful]") {
    AVLTree tree;
    CoutRedirect redirect;

    // Case 1: Invalid Name containing digits / non-alphabetic characters
    tree.executeCommand("insert \"A11y\" 45679999");
    REQUIRE(redirect.str() == "unsuccessful\n");
    redirect.clear();

    // Case 2: Invalid ID with length not equal to 8 digits
    tree.executeCommand("insert \"ValidName\" 1234");
    REQUIRE(redirect.str() == "unsuccessful\n");
    redirect.clear();

    // Case 3: Duplicate ID insertion
    tree.executeCommand("insert \"FirstStudent\" 10000001");
    redirect.clear();
    tree.executeCommand("insert \"SecondStudent\" 10000001");
    REQUIRE(redirect.str() == "unsuccessful\n");
    redirect.clear();

    // Case 4: Removing an ID that does not exist in the tree
    tree.executeCommand("remove 99999999");
    REQUIRE(redirect.str() == "unsuccessful\n");
    redirect.clear();

    // Case 5: Searching for an ID that does not exist in the tree
    tree.executeCommand("search 88888888");
    REQUIRE(redirect.str() == "unsuccessful\n");
    redirect.clear();
}

// TEST 2: Test insert command and all four rotation cases
TEST_CASE("AVL Tree: Insertions and All Four Rotation Cases", "[rotations]") {
    SECTION("Left-Left (LL) Imbalance -> Single Right Rotation") {
        AVLTree tree;
        // Inserting strictly descending values triggers LL imbalance at node 30
        tree.insert("NodeC", 30);
        tree.insert("NodeB", 20);
        tree.insert("NodeA", 10);

        // Expected Pre-Order after Right Rotation on 30: 20 (root), 10, 30
        std::vector<int> expectedPreOrder = {20, 10, 30};
        std::vector<int> expectedInOrder  = {10, 20, 30};

        REQUIRE(tree.getPreOrder() == expectedPreOrder);
        REQUIRE(tree.getInOrder() == expectedInOrder);
    }

    SECTION("Right-Right (RR) Imbalance -> Single Left Rotation") {
        AVLTree tree;
        // Inserting strictly ascending values triggers RR imbalance at node 10
        tree.insert("NodeA", 10);
        tree.insert("NodeB", 20);
        tree.insert("NodeC", 30);

        // Expected Pre-Order after Left Rotation on 10: 20 (root), 10, 30
        std::vector<int> expectedPreOrder = {20, 10, 30};
        std::vector<int> expectedInOrder  = {10, 20, 30};

        REQUIRE(tree.getPreOrder() == expectedPreOrder);
        REQUIRE(tree.getInOrder() == expectedInOrder);
    }

    SECTION("Left-Right (LR) Imbalance -> Left-Right Double Rotation") {
        AVLTree tree;
        // 30 -> 10 -> 20 creates LR imbalance at node 30
        tree.insert("NodeC", 30);
        tree.insert("NodeA", 10);
        tree.insert("NodeB", 20);

        // Expected Pre-Order after Left Rotate on 10, Right Rotate on 30: 20, 10, 30
        std::vector<int> expectedPreOrder = {20, 10, 30};
        std::vector<int> expectedInOrder  = {10, 20, 30};

        REQUIRE(tree.getPreOrder() == expectedPreOrder);
        REQUIRE(tree.getInOrder() == expectedInOrder);
    }

    SECTION("Right-Left (RL) Imbalance -> Right-Left Double Rotation") {
        AVLTree tree;
        // 10 -> 30 -> 20 creates RL imbalance at node 10
        tree.insert("NodeA", 10);
        tree.insert("NodeC", 30);
        tree.insert("NodeB", 20);

        // Expected Pre-Order after Right Rotate on 30, Left Rotate on 10: 20, 10, 30
        std::vector<int> expectedPreOrder = {20, 10, 30};
        std::vector<int> expectedInOrder  = {10, 20, 30};

        REQUIRE(tree.getPreOrder() == expectedPreOrder);
        REQUIRE(tree.getInOrder() == expectedInOrder);
    }
}

// TEST 3: Insert 100 nodes, remove 10 random nodes, check in-order
TEST_CASE("AVL Tree: 100 Insertions and 10 Random Deletions", "[stress]") {
    AVLTree tree;
    const int totalInsertions = 100;
    const int totalRemovals = 10;

    std::vector<int> ids;
    ids.reserve(totalInsertions);

    // Generate 100 unique 8-digit IDs
    for (int i = 0; i < totalInsertions; ++i) {
        ids.push_back(10000000 + i);
    }

    // Shuffle insertion order to exercise balancing across diverse branches
    std::mt19937 rng(42);
    std::vector<int> insertOrder = ids;
    std::shuffle(insertOrder.begin(), insertOrder.end(), rng);

    // 1. Insert 100 nodes and verify each insertion
    for (int id : insertOrder) {
        tree.insert("Student", id);
    }

    // Verify all 100 nodes are present via sorted in-order traversal
    std::vector<int> expectedInOrder = ids;
    std::sort(expectedInOrder.begin(), expectedInOrder.end());
    REQUIRE(tree.getInOrder().size() == totalInsertions);
    REQUIRE(tree.getInOrder() == expectedInOrder);

    // 2. Select 10 random distinct elements to delete
    std::vector<int> removalCandidates = ids;
    std::shuffle(removalCandidates.begin(), removalCandidates.end(), rng);
    std::vector<int> toDelete(removalCandidates.begin(), removalCandidates.begin() + totalRemovals);

    // Perform deletions on the AVL tree and update the expected tracker
    for (int id : toDelete) {
        tree.remove(id);
        expectedInOrder.erase(std::remove(expectedInOrder.begin(), expectedInOrder.end(), id), expectedInOrder.end());
    }

    // 3. Verify remaining 90 nodes in order
    REQUIRE(tree.getInOrder().size() == (totalInsertions - totalRemovals));
    REQUIRE(tree.getInOrder() == expectedInOrder);
}