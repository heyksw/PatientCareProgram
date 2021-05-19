#include <iostream>
#include <set>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

// Patient �� ����ü�� ȯ�� data ����
struct Patient {
	// ȯ�ڹ�ȣ, �̸�, ��ȭ��ȣ, �ּ� ��ǥ x y, ����, �����
	int patientNum;	// key
	string name;
	string phoneNumber;
	int addressX;
	int addressY;
	string disease;
	int expense;

};

// Red Black Tree�� ���� ������
enum Color
{
	RED,
	BLACK
};

// Red Balck Tree�� node
struct Node {
	Patient patient;
	Node* left = nullptr;
	Node* right = nullptr;
	Node* parent = nullptr;
	Color color = BLACK;
};

// Red Black Tree
class RedBlackTree{
private:
	Node* root;	
	Node* leaf;

public:
	RedBlackTree() {
		leaf = new Node;
		leaf->color = BLACK;
		leaf->left = nullptr;
		leaf->right = nullptr;
		leaf->parent = nullptr;
		root = leaf;
	}

	Node* getRoot() {
		return this->root;
	}

	// Tree�� patient�� �ִ��� - ������ ������ �� ��ȯ, ������ NULL ��ȯ
	Node* isPatient(Patient patient) {	// patient�� patientNum�� key�� Ž��
		int key = patient.patientNum;
		Node* temp = root;
		Node* parent = NULL;

		while (temp != NULL && temp->patient.patientNum != key) {
			parent = temp;
			temp = (key < parent->patient.patientNum) ? parent->left : parent->right;
		}
		return temp;
	}

	// Tree�� ȯ�� ���� ���� 
	void Insert(Patient patient) {
		Node* insertPosition = this->root;
		Node* insertParentPosition = nullptr;
		Node* temp = new Node();
		temp->patient = patient;
		temp->color = RED;
		temp->parent = nullptr;
		temp->left = leaf;
		temp->right = leaf;

		// ���� ����
		while (insertPosition != leaf)
		{
			insertParentPosition = insertPosition;
			if (insertPosition->patient.patientNum < patient.patientNum)
				insertPosition = insertPosition->right;
			else
				insertPosition = insertPosition->left;
		}

		temp->parent = insertParentPosition;

		if (insertParentPosition == nullptr)
			root = temp;
		else if (temp->patient.patientNum > insertParentPosition->patient.patientNum)
			insertParentPosition->right = temp;
		else
			insertParentPosition->left = temp;

		//temp�� root���
		if (temp->parent == nullptr)
		{
			temp->color = BLACK;
			return;
		}
		// temp�� �θ��尡 root����� Fix Up �ʿ���� RED�� ���δ�.
		if (temp->parent->parent == nullptr)
		{
			return;
		}
		InsertFixUp(temp);

		return;

	}

	// Reconstructing or Recoloring	// O(log n) time
	void InsertFixUp(Node* temp) {
		// Root�� �ƴϰ� �θ� Red���
		while (temp != root && temp->parent->color == RED)
		{
			Node* grandparent = temp->parent->parent;
			Node* uncle = (temp->parent == grandparent->left) ? grandparent->right : grandparent->left;
			bool side = (temp->parent == grandparent->left) ? true : false; //if p[temp]�� p[p[temp]]�� ���� �ڽ��̸� 1 / �������̸� 0

			/*case 1*/
			if (uncle && uncle->color == RED)
			{
				temp->parent->color = BLACK;
				uncle->color = BLACK;
				grandparent->color = RED;
				temp = grandparent;
			}

			/*case 2
				side == true ) p[temp]�� p[p[temp]]�� ���� �ڽ� �� ����̴�.
				side == false ) p[z]�� p[p[temp]]�� ������ �ڽ� �� ����̴�. */
			else
			{
				/*case 2-1*/
				if (temp == (side ? temp->parent->right : temp->parent->left))
				{
					temp = temp->parent;
					side ? RotateLeft(temp) : RotateRight(temp);
				}
				/*case 2-2*/
				temp->parent->color = BLACK;
				grandparent->color = RED;
				side ? RotateRight(grandparent) : RotateLeft(grandparent);
			}
		}
		root->color = BLACK;


	}

	// x �߽� ���� ȸ��
	void RotateLeft(Node* x)
	{
		Node* y;

		y = x->right;
		x->right = y->left;
		if (y->left != leaf)
		{
			y->left->parent = x;
		}
		y->parent = x->parent;

		if (!x->parent)
		{
			root = y;
		}
		else if (x == x->parent->left)
		{
			x->parent->left = y;
		}
		else
		{
			x->parent->right = y;
		}
		x->parent = y;
		y->left = x;
	}

	// x �߽� ������ ȸ��
	void RotateRight(Node* y)
	{
		Node* x;

		x = y->left;
		y->left = x->right;
		if (x->right != leaf)
		{
			x->right->parent = y;
		}
		x->parent = y->parent;

		if (!y->parent)
		{
			root = x;
		}
		else if (y == y->parent->left)
		{
			y->parent->left = x;
		}
		else
		{
			y->parent->right = x;
		}
		y->parent = x;
		x->right = y;
	}

	// Ʈ�� ���� ��ȸ�ϸ� ���ິ üũ	// O(n) time
	void Preorder(Node* target, int& n, string epidemic)
	{
		if (target == leaf)
			return;
		// std::cout << target->patient.patientNum << " ";
		
		if (target->patient.disease == epidemic) n++;

		Preorder(target->left, n, epidemic);
		Preorder(target->right, n, epidemic);
	}

	// depth ���	// O(log n) time
	int depth(Patient p) {
		int d = 0;
		Node* temp = isPatient(p);

		while (temp->parent != NULL) {
			temp = temp->parent;
			d++;
		}
		
		return d;
	}


};

// ���ڿ� �Ľ� �Լ�
vector<string> parsing(string str) {
	int previous = 0;
	int current = 0;
	vector<string> x;
	x.clear();

	current = str.find(' ');
	//find�� ã���� ������ npos�� ������
	while (current != string::npos) {
		string substring = str.substr(previous, current - previous);
		x.push_back(substring);
		previous = current + 1;
		current = str.find(' ', previous);
	}
	x.push_back(str.substr(previous, current - previous));

	/*
	for (int i = 0; i < x.size(); i++) {
		cout << x[i] << " ";
	}
	cout << endl;
	*/
	return x;
}

class PatientCare {
private:
	RedBlackTree PatientTree;


public:
	
	// �ű԰��� (��������)
	void newPatient(Patient p) {
		int D, S;
		if (PatientTree.isPatient(p) != NULL) {	// ȯ�ڰ� �̹� ��� �Ǿ������� ���� 
			S = 0;	
			D = PatientTree.depth(p);
		}
		else {	// ȯ�ڰ� ��� �Ǿ����� �ʾ����� �ű� ����
			S = 1;
			PatientTree.Insert(p);
			D = PatientTree.depth(p);
		}

		//cout << "ȯ�� ������ ����� ����� ���� : " << D << endl;
		//cout << "�ű� ���� ó�� : " << S << endl;
		cout << "������ ����� ����� ���� : " << D << " ó�� ���� : " << S << endl;
		cout << "�ű԰����� �Ϸ�Ǿ����ϴ�." << endl;
	}

	// ȯ�ڰ˻�
	void searchPatient(int key) {
		Patient p = { key, "0", "0", 0, 0, "0", 0 };	// key�� Ž���ϱ� ������ ������ ������ ���� �� ����.
		Node* tempPatient = PatientTree.isPatient(p);
		if (tempPatient != NULL) {	// ȯ�ڰ� �̹� ��� �Ǿ������� ���� ���
			cout << "��ϵ� ȯ�� ����: ";
			cout << PatientTree.depth(tempPatient->patient) << " " << tempPatient->patient.name << " " << tempPatient->patient.phoneNumber << " " <<
				tempPatient->patient.addressX << " " << tempPatient->patient.addressY << endl;	

		}
		else {	// ȯ�ڰ� ��� �Ǿ����� �ʾ����� Not found
			cout << "Not found" << endl;
		}
	}

	// �߰�����
	void additionalCare(int key, string d, int c) {	// ȯ�ڹ�ȣ key, ���� d, ����� c
		Patient p = { key, "0", "0", 0, 0, "0", 0 };	// key�� Ž���ϱ� ������ ������ ������ ���� �� ����.
		Node* tempPatient = PatientTree.isPatient(p);
		if (tempPatient != NULL) {	// ȯ�ڰ� �̹� ��� �Ǿ������� ������ �߰� ��, ����� ���� ���
			cout << "�߰����� ó�� �Ϸ�, �ڷᰡ ����� ����� ���� : ";
			cout << PatientTree.depth(tempPatient->patient) << endl;
			tempPatient->patient.disease = d;
			tempPatient->patient.expense = c;
			// cout << "�߰����� ó���� �Ϸ�Ǿ����ϴ�." << endl;
		}
		else {	// ȯ�ڰ� ��� �Ǿ����� �ʾ����� Not found
			cout << "Not found" << endl;
		}
	}

	// ���ິ ����
	void epidemic(string di) {
		int T = 0;	// ���ິ �δ� ȯ�� ��
		PatientTree.Preorder(PatientTree.getRoot(), T, di);
		cout << endl;
		cout << di<<"�� �δ� ȯ�� �� : " << T <<"��"<< endl;
	}

	// ���� ���� ���� ���α׷�
	void program() {
		cout << " ====== Patient Care Program ======   (Kim Sang Woo)" << endl << endl;
		cout << "�� ���� ���Ǹ� �Է��Ͻðڽ��ϱ� ?" << endl;
		cout << " -- ���� ���� -- " << endl;
		cout << "I: �ű� ����,  F: ȯ�� �˻�,  A: �߰� ����,  E: ���ິ ����" << endl;
		int n;
		cin >> n;

		cin.ignore();

		for (int i = 0; i < n; i++) {
			string cmd;
			cout << endl;
			cout << " -- " << i+1 << "��° ���Ǹ� �Է��ϼ��� :" << endl;
			getline(cin, cmd);
			vector<string> cmdVector = parsing(cmd);


			// < �ű� ���� I >
			/* ���ο� ȯ�� ������ �Է¹޾� �����Ʈ���� ��带 �����ϰ�, �� ����� ���̸� ����Ѵ�.
				���� ������ ȯ�ڹ�ȣ�� �̹� �����Ѵٸ�, �� ����� ���̸� ����ϰ� �ű� ������ �����ȴ�.*/
			if (cmdVector[0] == "I")
			{
				// cout << "I �ű԰���" << endl;				
				Patient p = {	// stoi : string to int
					stoi(cmdVector[1]), cmdVector[2],	// ȯ�ڹ�ȣ, �̸�
					cmdVector[3], stoi(cmdVector[4]),	// ����ó, �ּ� x
					stoi(cmdVector[5]), cmdVector[6],
					stoi(cmdVector[7])
				};
				newPatient(p);

			}
			// < ȯ�� �˻� F >
			/*����: �ش� ȯ�ڸ� Ž���Ͽ� �����ϸ� ȯ���� ������ ����Ѵ�. ���� �������� ������,
				��Not found��*/
			else if ((cmdVector[0] == "F")) {
				// cout << "F ȯ�� �˻�" << endl;
				int key = stoi(cmdVector[1]);
				searchPatient(key);

			}
			// < �߰� ���� A >
			/*����: �ش� ȯ�ڸ� Ž���Ͽ� �Է¹��� ���� ������ �����Ͽ� �߰��ϰ�, ȯ�ڿ� ���� �����
				���̸� ����Ѵ�. ���� ȯ�ڰ� �������� �ʴ� ���, ��Not found���� ����Ѵ�.*/
			else if ((cmdVector[0] == "A")) {
				// cout << "A �߰� ����" << endl;
				int key = stoi(cmdVector[1]);
				string d = cmdVector[2];
				int c = stoi(cmdVector[3]);
				additionalCare(key, d, c);

			}
			// ���ິ ����
			/*����: Ʈ���� ����� ��� ȯ�ڵ鿡 ����, ���������� ���ܹ��� ������ �Է����� �־��� �����
				�����ϸ� �����ϰ�, ����� ���� ����Ͽ� ���༺�� ������ �ľ��Ѵ�. ���ǻ�, ���� ���� ��������
				���� ���� ���ິ���� �����Ѵ�. */
			else if ((cmdVector[0] == "E")) {
				// cout << "E ���ິ ����" << endl;
				string di = cmdVector[1];
				epidemic(di);
			}
		}

	
	}

};



int main() {

	PatientCare PC;
	PC.program();

	return 0;
}