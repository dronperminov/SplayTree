#pragma once

#include <iostream>

template <typename T>
class SplayTree {
    struct Node {
    	T value; // значение элемента

    	Node *left; // указатель на левое поддерево
    	Node *right; // указатель на правое поддерево
    };

    Node *root; // корень дерева

    Node* RotateLeft(Node *node); // левый поворот
    Node* RotateRight(Node *node); // правый поворот

    Node* Splay(Node *node, const T& value); // splay операция

	Node* Insert(Node *node, const T& value); // вставка в дерево
    Node* Remove(Node *node, const T& value); // удаление из дерева

    void Copy(Node *node, Node* &newNode); // копирование дерева
    void Clear(Node* &node); // очистка дерева (освобождение памяти)

    int GetSize(Node *node) const; // количество элементов в дереве
    int GetHeight(Node *node) const; // высота дерева

    void Print(std::ostream &os, Node *node, int level) const; // вывод дерева в виде повёрнутого дерева в поток

public:
    SplayTree(); // конструктор по умолчанию
    SplayTree(const SplayTree& tree); // конструктор копирования

    SplayTree& operator=(const SplayTree& tree); // оператор присваивания

    void Insert(const T& value); // вставка элемента в дерево
    bool Remove(const T& value); // удаление элемента из дерева
    void Clear(); // удаление всех элементов из дерева

    bool Find(const T& value); // поиск элемента в дереве

    int GetSize() const; // количество элементов в дереве
    int GetHeight() const; // высота дерева

    ~SplayTree(); // деструктор (освобождение памяти)

    template <typename T1>
    friend std::ostream& operator<<(std::ostream &os, const SplayTree<T1>& tree); // оператор вывода в поток
};

// левый поворот
template <typename T>
typename SplayTree<T>::Node* SplayTree<T>::RotateLeft(Node *node) {
	Node *right = node->right;
	node->right = right->left;
	right->left = node;

	return right;
}

// правый поворот
template <typename T>
typename SplayTree<T>::Node* SplayTree<T>::RotateRight(Node *node) {
	Node *left = node->left;
	node->left = left->right;
	left->right = node;

	return left;
}

// splay операция
template <typename T>
typename SplayTree<T>::Node* SplayTree<T>::Splay(Node *node, const T& value) {
	if (node == nullptr || node->value == value)
		return node;

	if (value < node->value) {
		if (node->left == nullptr)
			return node;
		
		if (value < node->left->value) {
			node->left->left = Splay(node->left->left, value);

			node = RotateRight(node);
		}
		else if (value > node->left->value) {
			node->left->right = Splay(node->left->right, value);

			if (node->left->right)
				node->left = RotateLeft(node->left);
		}

		return (node->left == nullptr)? node : RotateRight(node);
	}
	else {
		if (node->right == nullptr)
			return node;

		if (value < node->right->value) {
			node->right->left = Splay(node->right->left, value);

			if (node->right->left)
				node->right = RotateRight(node->right);
		}
		else if (value > node->right->value) {
			node->right->right = Splay(node->right->right, value);
			
			node = RotateLeft(node);
		}

		return (node->right == nullptr) ? node : RotateLeft(node);
	}
}

// вставка в дерево
template <typename T>
typename SplayTree<T>::Node* SplayTree<T>::Insert(Node *node, const T& value) {
	if (node == nullptr) {
		node = new Node; // создаём новый элемент

		node->value = value; // сохраняем значение
		node->left = nullptr;
		node->right = nullptr;

		return node; // возвращаем его
	}

	node = Splay(node, value);

	Node *newNode = new Node; // создаём новый элемент

	newNode->value = value; // сохраняем значение

	// если значение меньше значения узла
	if (value < node->value) {
		newNode->right = node;
		newNode->left = node->left;
		node->left = nullptr;
	}
	else {
		newNode->left = node;
		newNode->right = node->right;
		node->right = nullptr;
	}

	return newNode; // возвращаем новый узел
}

// удаление из дерева
template <typename T>
typename SplayTree<T>::Node* SplayTree<T>::Remove(Node *node, const T& value) {
	if (node == nullptr) 
		return nullptr; 

	node = Splay(node, value); 

	// если нет значения, то выходим, возвращая узел
	if (value != node->value) 
		return node; 

	Node *tmp;

	// если нет левого потомка, то делаем правого потомка корнем
	if (node->left == nullptr) { 
		tmp = node; 
		node = node->right; 
	}
	else {
		tmp = node; 

		node = Splay(node->left, value); 
		node->right = tmp->right; 
	} 

	delete tmp; // удаляем элемент

	return node; 
}

// копирование дерева
template <typename T>
void SplayTree<T>::Copy(Node *node, Node* &newNode) {
	if (node == nullptr)
		return;

	newNode = new Node;

	newNode->value = node->value;
	newNode->left = nullptr;
	newNode->right = nullptr;

	Copy(node->left, newNode->left); // копируем левое поддерево
	Copy(node->right, newNode->right); // копируем правое поддерево
}

// удаление элементов из дерева и освобождение памяти
template <typename T>
void SplayTree<T>::Clear(Node* &node) {
	if (node == nullptr)
		return;

	Clear(node->left); // удаляем левое поддерево
	Clear(node->right); // удаляем правое поддерево

	delete node; // удаляем элемент
	node = nullptr; // обнуляем значение элемента
}

// количество элементов в дереве
template <typename T>
int SplayTree<T>::GetSize(Node *node) const {
	if (node == nullptr)
		return 0; // лист не считается за элемент

	int leftSize = GetSize(node->left); // находим число элементов в левом поддереве
	int rightSize = GetSize(node->right); // находим число элементов в правом поддереве

	return 1 + leftSize + rightSize; // итоговое число элементов - сумма числа элементов в поддеревьях + 1
}

// высота дерева
template <typename T>
int SplayTree<T>::GetHeight(Node *node) const {
	if (node == nullptr)
		return 0; // высота листа равна 0

	int leftHeight = GetHeight(node->left); // получаем высоту левого поддеревп
	int rightHeight = GetHeight(node->right); //  получаем высоту правого поддерева

	return 1 + std::max(leftHeight, rightHeight); // высота узла на 1 больше, чем максимальная из высот поддеревьев
}

// вывод дерева в виде повёрнутого дерева в поток
template <typename T>
void SplayTree<T>::Print(std::ostream &os, Node *node, int level) const {
	if (node == nullptr)
		return;

	Print(os, node->right, level + 1);

	for (int i = 0; i < level; i++)
		os << "    ";

	os << node->value << std::endl;

	Print(os, node->left, level + 1);
}

// конструктор по умолчанию
template <typename T>
SplayTree<T>::SplayTree() {
	root = nullptr; // изначально дерева нет
}

// конструктор копирования
template <typename T>
SplayTree<T>::SplayTree(const SplayTree& tree) {
	Copy(tree.root, root);
}

// оператор присваивания
template <typename T>
SplayTree<T>& SplayTree<T>::operator=(const SplayTree& tree) {
    if (this == &tree)
        return *this;

    Clear(root); // очищаем текущее дерево
    Copy(tree.root, root); // копируем второе дерево

    return *this;
}

template <typename T>
void SplayTree<T>::Insert(const T& value) {
	root = Insert(root, value);
}

template <typename T>
bool SplayTree<T>::Remove(const T& value) {
	if (!Find(value))
		return false;

	root = Remove(root, value);

	return true;
}

// удаление всех элементов из дерева
template <typename T>
void SplayTree<T>::Clear() {
	Clear(root); // вызываем рекурсивную функцию очистки от дерева
}

// поиск элемента в дереве
template <typename T>
bool SplayTree<T>::Find(const T& value) {
	root = Splay(root, value);

	if (root == nullptr)
		return false;

	return root->value == value;
}

// количество элементов в дереве
template <typename T>
int SplayTree<T>::GetSize() const {
	return GetSize(root); // вызываем рекурсивную версию получения размера
}

// высота дерева
template <typename T>
int SplayTree<T>::GetHeight() const {
	return GetHeight(root); // вызываем рекурсивную версию получения высоты
}

// деструктор (освобождения памяти)
template <typename T>
SplayTree<T>::~SplayTree() {
	Clear(root);
}

// оператор вывода в поток
template <typename T>
std::ostream& operator<<(std::ostream &os, const SplayTree<T>& tree) {
    tree.Print(os, tree.root, 0);

    return os;
}