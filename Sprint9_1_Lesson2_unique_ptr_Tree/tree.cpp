#include <cassert>
#include <iostream>
#include <memory>

template <typename T>
struct TreeNode;

template <typename T>
using TreeNodePtr = std::unique_ptr<TreeNode<T>>;

template <typename T>
struct TreeNode {
    // Используйте TreeNodePtr<T> вместо сырых указателей
    // Примите умные указатели по rvalue-ссылке
    TreeNode(T val, TreeNodePtr<T>&& left, TreeNodePtr<T>&& right)
        : value(std::move(val))
        , left_(std::move(left)) 
        , right_(std::move(right)) {
    }

    T value;
    TreeNodePtr<T> left_;//TreeNode* left;  Замените TreeNode* на TreeNodePtr<T>
    TreeNodePtr<T> right_;//TreeNode* right; Замените TreeNode* на TreeNodePtr<T>
    TreeNode* parent = nullptr;
};

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node) noexcept {
    // реализуйте функцию
    if (node->left_.get()) {
        if (node->left_.get()->value > node->value || !CheckTreeProperty(node->left_.get())) {
            return false;
        }
    }
    if (node->right_.get()) {
        if (node->right_.get()->value < node->value || !CheckTreeProperty(node->right_.get())) {
            return false;
        }
    }
    return true;
}

template <typename T>
TreeNode<T>* begin(TreeNode<T>* node) noexcept {
    while (node->left_.get()) {
        node = node->left_.get();
    }
    return node;
}

template <typename T>
TreeNode<T>* next(TreeNode<T>* node) noexcept {
    if (node->right_ != nullptr) {
        node = node->right_.get();
        while (node->left_ != nullptr) {
            node = node->left_.get();
        }
    }
    else {
        if (node->parent == nullptr) return nullptr;
        while (node->parent != nullptr) {
            if (node->parent->right_.get() == node) {
                node = node->parent;
                if (node->parent == nullptr) return nullptr;
            }
            return node->parent;
        }
    }
    return node;
}

// функция создаёт новый узел с заданным значением и потомками
TreeNodePtr<int> N(int val, TreeNodePtr<int>&& left = {}, TreeNodePtr<int>&& right = {}) {
    // Реализуйте функцию
    auto node = std::make_unique<TreeNode<int>>(std::move(val), std::move(left), std::move(right));

    if (node->left_) {
        auto l = node.get()->left_.get();
        l->parent = node.get();
    }
    if (node->right_) {
        auto r = node.get()->right_.get();
        r->parent = node.get();
    }
    return node;
}

int main() {
    using namespace std;
    using T = TreeNode<int>;
    auto root1 = N(6, N(4, N(3), N(5)), N(7));
    assert(CheckTreeProperty(root1.get()));

    T* iter = begin(root1.get());
    while (iter) {
        cout << iter->value << " "s;
        iter = next(iter);
    }
    cout << endl;

    auto root2 = N(6, N(4, N(3), N(5)), N(7, N(8)));
    assert(!CheckTreeProperty(root2.get()));

    // Функция DeleteTree не нужна. Узлы дерева будут рекурсивно удалены
    // благодаря деструкторам unique_ptr
}