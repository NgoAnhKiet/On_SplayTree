#include <iostream>
using namespace std;

struct Node
{
	int info;
	Node* left;
	Node* right;
};
typedef Node* NodePtr;

NodePtr makeNode(int value)
{
	NodePtr newNode = new Node();
	newNode->info = value;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

//quay TRÁI, KHÁC VỚI AVL_TREE là KHÔNG CÓ CẬP NHẬT ĐỘ CAO
void rotateLeft(NodePtr& root)
{
	if (root == NULL || root->right == NULL) return;
	NodePtr pointer = root->right;
	root->right = pointer->left;
	pointer->left = root;
	root = pointer;
}

//quay PHẢI, KHÁC VỚI AVL_TREE là KHÔNG CÓ CẬP NHẬT ĐỘ CAO
void rotateRight(NodePtr& root)
{
	if (root == NULL || root->left == NULL) return;
	NodePtr pointer = root->left;
	root->left = pointer->right;
	pointer->right = root;
	root = pointer;
}

//LOE NÚT
bool splayNode(NodePtr& root, int value)
{
	if (root == NULL) return true;
	if (value == root->info) return true;
	if (value < root->info)
	{
		NodePtr pointer = root->left;
		if (pointer == NULL) return true;
		if (value == pointer->info) //Zig
		{
			rotateRight(root);
			return true;
		}
		else if (value < pointer->info) 
		{
			NodePtr temp = pointer->left;
			if (temp == NULL) return true;
			if (value == temp->info) //Zig-Zig
			{
				rotateRight(root);
				rotateRight(root);
				return false;
			}
			else
				return splayNode(root->left, value);
		}
		else							
		{
			NodePtr temp = pointer->right;
			if (temp == NULL) return true;
			if (value == temp->info) //Zig-Zag
			{
				rotateLeft(root->left);
				rotateRight(root);
				return false;
			}
			else
				return splayNode(root->left, value);
		}
	}
	else
	{
		NodePtr pointer = root->right;
		if (pointer == NULL) return true;
		if (value == pointer->info) //Zig
		{
			rotateLeft(root);
			return true;
		}
		else if (value < pointer->info) 
		{
			NodePtr temp = pointer->left;
			if (temp == NULL) return true;
			if (value == temp->info) //Zig-Zag
			{
				rotateRight(root->right);
				rotateLeft(root);
				return false;
			}
			else
				return splayNode(root->right, value);
		}
		else							
		{
			NodePtr temp = pointer->right;
			if (temp == NULL) return true;
			if (value == temp->info) //Zig-Zig
			{
				rotateLeft(root);
				rotateLeft(root);
				return false;
			}
			else
				return splayNode(root->right, value);
		}
	}
}

//LOE CÂY
void splayTree(NodePtr& root, int value)
{
	while (splayNode(root, value) == false);
}

//Tìm
bool search(NodePtr& root, int key)
{
	if (root == NULL) return false;
	NodePtr pointer = root;
	while (pointer)
	{
		if (key == pointer->info)
		{
			splayTree(root, key);
			return true;
		}
		else if (key < pointer->info)
			pointer = pointer->left;
		else
			pointer = pointer->right;
	}

	//Không tìm thấy
	NodePtr cha = root;
	NodePtr con = NULL;
	while (cha)
	{
		if (key < cha->info)
		{
			con = cha->left;
			if (con == NULL)
			{
				splayTree(root, cha->info);
				return false;
			}
		}
		else if (key > cha->info)
		{
			con = cha->right;
			if (con == NULL)
			{
				splayTree(root, cha->info);
				return false;
			}
		}
		cha = con;
	}
}

//Chèn
void insert(NodePtr& root, int value)
{
	if (root == NULL) 
	{
		root = makeNode(value);
		return;
	}

	NodePtr pointer = root;
	while (true)
	{
		if (value == pointer->info)
			return;
		else if (value < pointer->info) 
		{
			if (pointer->left == NULL) 
			{
				pointer->left = makeNode(value);
				break;
			}
			else
				pointer = pointer->left;
		}
		else {
			if (pointer->right == NULL) 
			{
				pointer->right = makeNode(value);
				break;
			}
			else
				pointer = pointer->right;
		}
	}
	splayTree(root, value);
}

//Xóa 
void deleteNode(NodePtr& root, int key) 
{
	if (root == NULL) return;

	//Splay nút cần xóa lên gốc cây
	splayTree(root, key);

	//Kiểm tra xem nút cần xóa có tồn tại trong cây không
	if (root->info != key) return;

	NodePtr maxRightOfLeftSubTree = root->left;
	if (maxRightOfLeftSubTree != NULL) {
		while (maxRightOfLeftSubTree->right != NULL)
			maxRightOfLeftSubTree = maxRightOfLeftSubTree->right;

		splayTree(root->left, maxRightOfLeftSubTree->info);

		NodePtr temp = root;
		root = root->left;
		root->right = temp->right;
		delete temp;
	}
	else 
	{
		NodePtr temp = root;
		root = root->right;
		delete temp;
	}
}

void preOrder(NodePtr root) 
{
	if (root) 
	{
		cout << root->info << " ";
		preOrder(root->left);
		preOrder(root->right);
	}
}

int main() {
	NodePtr root = NULL;
	const int MAX = 10;
	int array[] = { 24,17,11,53,4,13,15,8,14,27 };
	for (int i = 0; i < MAX; i++) {
		insert(root, array[i]);
	}
	cout << "\nPreOrder: ";
	preOrder(root);

	int number;
	cout << "\nEnter number your want to search: ";
	cin >> number;
	if (search(root, number))
		cout << "\nFOUND";
	else
		cout << "\nNOT found";
	cout << "\nPreOrder after search: ";
	preOrder(root);

	cout << "\nEnter number your want to delete: ";
	cin >> number;
	deleteNode(root, number);
	cout << "\nPreOrder after delete: ";
	preOrder(root);

	return 0;
}