#include <iostream>
#include <set>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

// Patient 형 구조체로 환자 data 관리
struct Patient {
	// 환자번호, 이름, 전화번호, 주소 좌표 x y, 병명, 진료비
	int patientNum;	// key
	string name;
	string phoneNumber;
	int addressX;
	int addressY;
	string disease;
	int expense;

};

// Red Black Tree를 위한 열거형
enum Color
{
	RED,
	BLACK
};

// Red Balck Tree의 node
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

	// Tree에 patient가 있는지 - 있으면 포인터 값 반환, 없으면 NULL 반환
	Node* isPatient(Patient patient) {	// patient의 patientNum을 key로 탐색
		int key = patient.patientNum;
		Node* temp = root;
		Node* parent = NULL;

		while (temp != NULL && temp->patient.patientNum != key) {
			parent = temp;
			temp = (key < parent->patient.patientNum) ? parent->left : parent->right;
		}
		return temp;
	}

	// Tree에 환자 정보 삽입 
	void Insert(Patient patient) {
		Node* insertPosition = this->root;
		Node* insertParentPosition = nullptr;
		Node* temp = new Node();
		temp->patient = patient;
		temp->color = RED;
		temp->parent = nullptr;
		temp->left = leaf;
		temp->right = leaf;

		// 삽입 과정
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

		//temp가 root라면
		if (temp->parent == nullptr)
		{
			temp->color = BLACK;
			return;
		}
		// temp의 부모노드가 root노드라면 Fix Up 필요없이 RED로 붙인다.
		if (temp->parent->parent == nullptr)
		{
			return;
		}
		InsertFixUp(temp);

		return;

	}

	// Reconstructing or Recoloring	// O(log n) time
	void InsertFixUp(Node* temp) {
		// Root가 아니고 부모가 Red라면
		while (temp != root && temp->parent->color == RED)
		{
			Node* grandparent = temp->parent->parent;
			Node* uncle = (temp->parent == grandparent->left) ? grandparent->right : grandparent->left;
			bool side = (temp->parent == grandparent->left) ? true : false; //if p[temp]가 p[p[temp]]의 왼쪽 자식이면 1 / 오른쪽이면 0

			/*case 1*/
			if (uncle && uncle->color == RED)
			{
				temp->parent->color = BLACK;
				uncle->color = BLACK;
				grandparent->color = RED;
				temp = grandparent;
			}

			/*case 2
				side == true ) p[temp]는 p[p[temp]]의 왼쪽 자식 인 경우이다.
				side == false ) p[z]는 p[p[temp]]의 오른쪽 자식 인 경우이다. */
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

	// x 중심 왼쪽 회전
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

	// x 중심 오른쪽 회전
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

	// 트리 전위 순회하며 유행병 체크	// O(n) time
	void Preorder(Node* target, int& n, string epidemic)
	{
		if (target == leaf)
			return;
		// std::cout << target->patient.patientNum << " ";
		
		if (target->patient.disease == epidemic) n++;

		Preorder(target->left, n, epidemic);
		Preorder(target->right, n, epidemic);
	}

	// depth 출력	// O(log n) time
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

// 문자열 파싱 함수
vector<string> parsing(string str) {
	int previous = 0;
	int current = 0;
	vector<string> x;
	x.clear();

	current = str.find(' ');
	//find는 찾을게 없으면 npos를 리턴함
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
	
	// 신규가입 (최초진료)
	void newPatient(Patient p) {
		int D, S;
		if (PatientTree.isPatient(p) != NULL) {	// 환자가 이미 등록 되어있으면 거절 
			S = 0;	
			D = PatientTree.depth(p);
		}
		else {	// 환자가 등록 되어있지 않았으면 신규 가입
			S = 1;
			PatientTree.Insert(p);
			D = PatientTree.depth(p);
		}

		//cout << "환자 정보가 저장된 노드의 깊이 : " << D << endl;
		//cout << "신규 가입 처리 : " << S << endl;
		cout << "정보가 저장된 노드의 깊이 : " << D << " 처리 여부 : " << S << endl;
		cout << "신규가입이 완료되었습니다." << endl;
	}

	// 환자검색
	void searchPatient(int key) {
		Patient p = { key, "0", "0", 0, 0, "0", 0 };	// key로 탐색하기 때문에 나머지 정보는 더미 값 삽입.
		Node* tempPatient = PatientTree.isPatient(p);
		if (tempPatient != NULL) {	// 환자가 이미 등록 되어있으면 정보 출력
			cout << "등록된 환자 정보: ";
			cout << PatientTree.depth(tempPatient->patient) << " " << tempPatient->patient.name << " " << tempPatient->patient.phoneNumber << " " <<
				tempPatient->patient.addressX << " " << tempPatient->patient.addressY << endl;	

		}
		else {	// 환자가 등록 되어있지 않았으면 Not found
			cout << "Not found" << endl;
		}
	}

	// 추가진료
	void additionalCare(int key, string d, int c) {	// 환자번호 key, 병명 d, 진료비 c
		Patient p = { key, "0", "0", 0, 0, "0", 0 };	// key로 탐색하기 때문에 나머지 정보는 더미 값 삽입.
		Node* tempPatient = PatientTree.isPatient(p);
		if (tempPatient != NULL) {	// 환자가 이미 등록 되어있으면 진료기록 추가 후, 노드의 깊이 출력
			cout << "추가진료 처리 완료, 자료가 저장된 노드의 깊이 : ";
			cout << PatientTree.depth(tempPatient->patient) << endl;
			tempPatient->patient.disease = d;
			tempPatient->patient.expense = c;
			// cout << "추가진료 처리가 완료되었습니다." << endl;
		}
		else {	// 환자가 등록 되어있지 않았으면 Not found
			cout << "Not found" << endl;
		}
	}

	// 유행병 조사
	void epidemic(string di) {
		int T = 0;	// 유행병 앓는 환자 수
		PatientTree.Preorder(PatientTree.getRoot(), T, di);
		cout << endl;
		cout << di<<"를 앓는 환자 수 : " << T <<"명"<< endl;
	}

	// 모든걸 담은 메인 프로그램
	void program() {
		cout << " ====== Patient Care Program ======   (Kim Sang Woo)" << endl << endl;
		cout << "몇 개의 질의를 입력하시겠습니까 ?" << endl;
		cout << " -- 질의 종류 -- " << endl;
		cout << "I: 신규 가입,  F: 환자 검색,  A: 추가 진료,  E: 유행병 조사" << endl;
		int n;
		cin >> n;

		cin.ignore();

		for (int i = 0; i < n; i++) {
			string cmd;
			cout << endl;
			cout << " -- " << i+1 << "번째 질의를 입력하세요 :" << endl;
			getline(cin, cmd);
			vector<string> cmdVector = parsing(cmd);


			// < 신규 가입 I >
			/* 새로운 환자 정보를 입력받아 레드블랙트리에 노드를 삽입하고, 그 노드의 깊이를 출력한다.
				만약 동일한 환자번호가 이미 존재한다면, 그 노드의 깊이를 출력하고 신규 가입은 거절된다.*/
			if (cmdVector[0] == "I")
			{
				// cout << "I 신규가입" << endl;				
				Patient p = {	// stoi : string to int
					stoi(cmdVector[1]), cmdVector[2],	// 환자번호, 이름
					cmdVector[3], stoi(cmdVector[4]),	// 연락처, 주소 x
					stoi(cmdVector[5]), cmdVector[6],
					stoi(cmdVector[7])
				};
				newPatient(p);

			}
			// < 환자 검색 F >
			/*설명: 해당 환자를 탐색하여 존재하면 환자의 정보를 출력한다. 만약 존재하지 않으면,
				“Not found”*/
			else if ((cmdVector[0] == "F")) {
				// cout << "F 환자 검색" << endl;
				int key = stoi(cmdVector[1]);
				searchPatient(key);

			}
			// < 추가 진료 A >
			/*설명: 해당 환자를 탐색하여 입력받은 진료 내용을 진료기록에 추가하고, 환자에 대한 노드의
				깊이를 출력한다. 만약 환자가 존재하지 않는 경우, “Not found”를 출력한다.*/
			else if ((cmdVector[0] == "A")) {
				// cout << "A 추가 진료" << endl;
				int key = stoi(cmdVector[1]);
				string d = cmdVector[2];
				int c = stoi(cmdVector[3]);
				additionalCare(key, d, c);

			}
			// 유행병 조사
			/*설명: 트리에 저장된 모든 환자들에 대해, 마지막으로 진단받은 병명이 입력으로 주어진 병명과
				동일하면 집계하고, 집계된 수를 출력하여 유행성의 정도를 파악한다. 편의상, 골절 같은 전염성이
				없는 병명도 유행병으로 간주한다. */
			else if ((cmdVector[0] == "E")) {
				// cout << "E 유행병 조사" << endl;
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