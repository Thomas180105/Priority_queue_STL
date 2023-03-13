#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.h"

namespace sjtu
{

/**
 * a container like std::priority_queue which is a heap internal.
 */
    template<typename T, class Compare = std::less<T>>
    class priority_queue {
    public:
        struct Node{
            T element;
            Node *left;
            Node *right;
            int npl;
            int cnt;

            Node() : left(nullptr), right(nullptr), npl(-1), cnt(1) {}
            explicit Node(T e, Node *l = nullptr, Node *r = nullptr, int n = 0, int c = 1) : element(e), left(l), right(r), npl(n),  cnt(c){}
        };
        Node *root;
        Compare helper;
        /**
         * TODO constructors
         */
        priority_queue() : root(nullptr) {}

        Node *copy(Node *rhs)
        {
            if (rhs == nullptr) return nullptr;
            return new Node(rhs->element, copy(rhs->left), copy(rhs->right), rhs->npl, rhs->cnt);
        }
        priority_queue(const priority_queue &other)
        {
            root = copy(other.root);
        }
        /**
         * TODO destructor
         */
        void clear(Node *t)
        {
            if (t == nullptr) return;
            clear(t->left);
            clear(t->right);
            delete t;
            t = nullptr;//attention!!!
        }
        ~priority_queue()
        {
            clear(root);
        }

        /**
         * TODO Assignment operator
         */
        priority_queue &operator=(const priority_queue &other)
        {
            if (this == &other) return *this;
            clear(root);
            root = copy(other.root);
            return *this;
        }
        /**
         * get the top of the queue.
         * @return a reference of the top element.
         * throw container_is_empty if empty() returns true;
         */
        const T & top() const
        {
            if (empty()) throw container_is_empty();
            return root->element;
        }
        /**
         * TODO
         * push new element to the priority queue.
         */
        void push(const T &e)
        {
            Node *tmp = new Node(e);
            try
            {
                root = merge(tmp, root);
            }
            catch(runtime_error)
            {
                delete tmp;
                throw runtime_error();
            }
        }
        /**
         * TODO
         * delete the top element.
         * throw container_is_empty if empty() returns true;
         */
        void pop()
        {
            if (empty()) throw container_is_empty();
            Node *tmp = root;
            root = merge(root->left, root->right);
            delete tmp;
        }
        /**
         * return the number of the elements.
         */
        size_t size() const
        {
            if (root == nullptr) return 0;
            return root->cnt;
        }
        /**
         * check if the container has at least an element.
         * @return true if it is empty, false if it has at least an element.
         */
        bool empty() const
        {
            return root == nullptr;
        }
        /**
         * merge two priority_queues with at least O(logn) complexity.
         * clear the other priority_queue.
         */
        void swapSon(Node *t)
        {
            Node *tmp = t->left;
            t->left = t->right;
            t->right = tmp;
        }
        Node *merge(Node *t1, Node *t2)
        {
            if (t1 == nullptr) return t2;//递归出口
            if (t2 == nullptr) return t1;
//            if (t1->element < t2->element) return merge1(t1, t2);
            if (helper(t1->element, t2->element)) return merge1(t2, t1);
            return merge1(t1, t2);
        }
        //merge t2 to the right of t1
        Node *merge1(Node *t1, Node *t2)
        {
            t1->cnt += t2->cnt;//proved true
            if (t1->left == nullptr) t1->left = t2;
            else
            {
                t1->right = merge(t1->right, t2);
                if (t1->left->npl < t1->right->npl) swapSon(t1);
                t1->npl = t1->right->npl + 1;
            }
            return t1;
        }
        void merge(priority_queue &other)
        {
            if (this == &other) return;
            root = merge(root, other.root);
            other.root = nullptr;
        }
    };
}

#endif
