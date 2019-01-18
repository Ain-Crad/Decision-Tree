//#include <bits/stdc++.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>

using namespace std;

const int maxn = 100; //max number of sample
const int maxm = 10; //max number of attribute
int Atr[maxm];
int n, m;

struct Node{
	Node(int Attr = 0, int Cate = 0, bool leaf = 0):Attribute(Attr), Category(Cate), IsLeaf(leaf){}
	int Attribute;
	int Category;
	bool IsLeaf;
	vector<Node*> child;
};

struct TrainData{
	vector<vector<int> > X;
	vector<int> Y;
}; 
struct TestData{
	vector<vector<int> > X;
	vector<int> Y;
};

void InputData(TrainData& data);
Node* BuildTree(TrainData data, vector<int> vis);
int MostValue(TrainData data);
int BestAttribute(TrainData data, vector<int>vis);
double CalGain(TrainData data, int Attr);
double CalEntropy(TrainData data);
void GetTestData(TestData& Tdata);
int Judge(Node* root, TestData Tdata, int state);
void Prune(Node* root, Node* u, TestData Tdata);
void PrintTreeStruct(Node* root);

void InputData(TrainData& data){
	cin >> n >> m;
	for(int i = 0; i < m; i++){
		cin >> Atr[i];
	}
	vector<int> tmp;
	int Attr, Cate;
	for(int i = 0; i < n; i++){
		tmp.clear();
		for(int i = 0; i < m; i++){
			cin >> Attr;
			tmp.push_back(Attr);
		}
		data.X.push_back(tmp);
		cin >> Cate;
		data.Y.push_back(Cate);
	}

}

int MostValue(TrainData data){
	int count[maxn + 1];
	memset(count, 0, sizeof(count));
	int len = data.Y.size();
	for(int i = 0; i < len; i++){
		count[data.Y[i]]++;
	}

	if(count[0] == count[1]) return 1;
	else return max_element(count, count + maxn) - count;
}

double CalEntropy(TrainData data){
	int count[maxn];
	memset(count, 0, sizeof(count));
	for(int i = 0; i < (int)data.Y.size(); i++){
		count[data.Y[i]]++;
	}
	double p[maxn];
	memset(p, 0, sizeof(p));
	int len = data.Y.size();
	for(int i = 0; i < maxn; i++){
		if(count[i] == 0) break;
		p[i] = 1.0 * count[i] / len;
	}
	double Ent = 0;
	for(int i = 0; i < maxn; i++){
		if(p[i] == 0) break;
		Ent += -1 * p[i] * log2(p[i]);
	}
	return Ent;
}

double CalGain(TrainData data, int Attr){
	int AtrNum = Atr[Attr];
	int len = data.Y.size();
	TrainData d[AtrNum];
	for(int i = 0; i < (int)data.X.size(); i++){
		int id = data.X[i][Attr];
		d[id].X.push_back(data.X[i]);
		d[id].Y.push_back(data.Y[i]);
	}
	double EntD = CalEntropy(data);
	double Ent[maxm];
	for(int i = 0;i < AtrNum; i++){
		Ent[i] = CalEntropy(d[i]);
		int l = d[i].Y.size(); 
		EntD -= 1.0 * l * Ent[i] / len;
	}
	return EntD;
}

int BestAttribute(TrainData data, vector<int>vis){
	bool vist[maxm];
	memset(vist, 0, sizeof(vist));
	for(int i = 0; i < (int)vis.size(); i++){
		vist[vis[i]] = 1;
	}

	double Gain[maxm + 1];
	memset(Gain, 0, sizeof(Gain));
	for(int i = 0; i < m; i++){
		if(!vist[i]){
			Gain[i] = CalGain(data, i);
		}
	}
	return max_element(Gain, Gain + maxm) - Gain;
}

Node* BuildTree(TrainData data, vector<int> vis){
	Node* u = new Node();

	bool flag = false;
	for(int i = 1; i < (int)data.Y.size(); i++){
		if(data.Y[i] != data.Y[i - 1]){
			flag = true;
			break;
		}
	}

	if(!flag) return new Node(0, data.Y[0], 1);

	if((int)vis.size() == m) return new Node(0, MostValue(data), 1);

	int Attr = BestAttribute(data, vis);
	vis.push_back(Attr);
	u->Attribute = Attr;
	u->Category = MostValue(data);

	int AtrNum = Atr[Attr];
	TrainData d[AtrNum];
	for(int i = 0; i < (int)data.X.size(); i++){
		int id = data.X[i][Attr];
		d[id].X.push_back(data.X[i]);
		d[id].Y.push_back(data.Y[i]);
	}
	for(int i = 0; i < AtrNum; i++){
		if(d[i].Y.empty()) u->child.push_back(new Node(0, MostValue(data), 1));
		else u->child.push_back(BuildTree(d[i], vis));
	}
	return u;
}

int Judge(Node* root, TestData Tdata, int state){
	int d[maxm];
	int TrueAns[maxn];
	int res[maxn];

	for(int i = 0; i < n; i++){
		Node* u = root;
		for(int j = 0; j < m; j++){
			d[j] = Tdata.X[i][j];
		}
		TrueAns[i] = Tdata.Y[i];
		while(1){
			if(u->IsLeaf == 1){
				res[i] = u->Category;
				if(state == 1 && u->Category == 0) cout << "Bad watermelon" << endl;
				else if(state == 1 && u->Category == 1) cout << "Good watermelon" << endl;
				break;
			}
			else{
				u = u->child[d[u->Attribute]];
			}
		}
	}
	int cnt = 0;
	for(int i = 0; i < n; i++){
		if(TrueAns[i] == res[i]) cnt++;
	}
	if(state == 1) cout << "Correct rate: " << cnt << "/" << n << endl;

	return cnt;
}

void GetTestData(TestData& Tdata){
	cin >> n >> m;
	for(int i = 0; i < n; i++){
		vector<int> vec;
		for(int j = 0; j < m; j++){
			int x;
			cin >> x;
			vec.push_back(x);
		}
		Tdata.X.push_back(vec);
		int y;
		cin >> y;
		Tdata.Y.push_back(y);
	}
}

void PrintTreeStruct(Node* root){
	Node* u = root;
	if(u->IsLeaf == 1){
		cout << "Category: " << u->Category << endl;
		return;
	}
	else{
		cout << "Attribute: " << u->Attribute << endl;
		for(int i = 0; i < (int)u->child.size(); i++){
			PrintTreeStruct(u->child[i]);
		}
	}
}

void Prune(Node* root, Node* u, TestData Tdata){
	if(u->IsLeaf == 1) return;
	for(int i = 0; i < (int)u->child.size(); i++){
		Prune(root, u->child[i], Tdata);
	}
	bool flag = false;
	for(int i = 0; i < (int)u->child.size(); i++){
		if(u->child[i]->IsLeaf == 0){
			flag = true;
			break;
		}
	}
	if(!flag){
		int PreNum = Judge(root, Tdata, 0);
		u->IsLeaf = 1;
		int PostNum = Judge(root, Tdata, 0);
		if(PostNum <= PreNum) u->IsLeaf = 0;
	}
}

int main(){
	//Train
	freopen("./TrainData.txt", "r", stdin);
	TrainData data;
	InputData(data);
	Node* root;	
	vector<int> vis;
	cout << "Building Desicion Tree..." << endl << endl;
	root = BuildTree(data, vis);
	//PrintTreeStruct(root);

	//Test
	freopen("./TestData.txt", "r", stdin);
	TestData Tdata;
	GetTestData(Tdata);
	cout << "Start to Test..." << endl << endl;
	Judge(root, Tdata, 1);
	cout << endl;
	cout << "Pruning The Desicion Tree..." << endl << endl;
	Prune(root, root, Tdata);
	cout << "Pruning Done." << endl << endl;
	cout << "Test again..." << endl << endl;
	Judge(root, Tdata, 1);
	return 0;
}