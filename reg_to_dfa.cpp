// Code to convert Regular expression to DFA.
// author : Sahil Prakash

#include<bits/stdc++.h>

using namespace std;

#define si(x) scanf("%d",&x)
#define sll(x) scanf("%lld",&x)
#define pri(x) printf("%d",x)
#define pll(x) printf("%lld",x)
#define sstr(s) scanf("%s",s)
#define nl printf("\n")
#define ll long long int
#define pii pair<int,int>
#define pLL pair<ll,ll>
#define vi vector<int>
#define pb push_back
#define mp make_pair
#define fr first
#define se second
#define mod 1000000007
#define SZ 105

string reg;

int getPrecedence(char c) {
    switch(c) {
	case '(': return 1;
	case '|': return 2;
	case '.': return 3; // explicit concatenation operator
	case '?': return 4;
	case '*': return 4;
	case '+': return 4;
	case '^': return 5;
	default : return 6;
    }
}

string formatRegEx(string regex) {
    string res = "";
    char allOperators[] = {'|', '?', '+', '*', '^'};
    char binaryOperators[] = {'^', '|'};
    for (int i = 0; i < regex.length(); i++) {
	char c1 = regex[i];
	if (i + 1 < regex.length()) {
	    char c2 = regex[i + 1];
	    res += c1;
	    if (c1!='(' && c2!=')') {
		int flag = 1;
		for(int j=0;j<5;j++) {
		    if(c2==allOperators[j]) {
			flag = 0;
			break;
		    }
		}
		if(flag==0) {
		    continue;
		}
		else {
		    if(c1!='^' && c1!='|') {
			res += '.';
		    }
		}
	    }
	}
    }
    res += regex[regex.length() - 1];
    return res;
}

string infixToPostfix(string regex) {
    string postfix = "";
    stack<char> st;
    string formattedRegEx = formatRegEx(regex);
    for (int i=0;i<formattedRegEx.length();i++) {
	char c = formattedRegEx[i];
	switch (c) {
	    case '(':
		st.push(c);
		break;
	    case ')':
		while (st.top()!='(') {
		    postfix += st.top(); st.pop();
		}
		st.pop();
		break;
	    default:
		while (st.size() > 0) {
		    char peekedChar = st.top();
		    int peekedCharPrecedence = getPrecedence(peekedChar);
		    int currentCharPrecedence = getPrecedence(c);
		    if (peekedCharPrecedence >= currentCharPrecedence) {
			postfix += st.top(); st.pop();
		    } else {
			break;
		    }
		}
		st.push(c);
		break;
	}
    }
    while (st.size() > 0) {
	postfix += st.top(); st.pop();
    }
    return postfix;
}

struct node {
    char c;
    bool nullable;
    set<int> firstpos, lastpos;
    node *left, *right;

    node() {
	nullable = false;
	firstpos.clear(); lastpos.clear();
	left = right = NULL;
    }
};

node *root;
map<int,set<int> > followpos;
map<int,char> hash;
map<set<int>,int> stateHash;
map<pair<int,char>, int> dfa;
map<int,int> isFinal;
int sharp;

void buildTree(string s) {
    s += "#.";
    stack<node*> st;
    int w = 0;
    for(int i=0;i<s.length();i++) {
	if(s[i]=='.' || s[i]=='|') {
	    node *t1,*t2;
	    assert(st.size()>=2);
	    t2 = st.top(); st.pop();
	    t1 = st.top(); st.pop();
	    node *temp = new node();
	    temp->c = s[i];
	    temp->left = t1;
	    temp->right = t2;
	    if(s[i]=='.') {
		// Get nullable
		if(t1->nullable && t2->nullable) {
		    temp->nullable = true;
		}
		// Get firstpos
		for(set<int>::iterator it=(t1->firstpos).begin();it!=(t1->firstpos).end(); it++) {
		    (temp->firstpos).insert(*it);
		}
		if(t1->nullable) {
		    for(set<int>::iterator it=(t2->firstpos).begin();it!=(t2->firstpos).end(); it++) {
			(temp->firstpos).insert(*it);
		    }  
		}
		// Get lastpos
		for(set<int>::iterator it=(t2->lastpos).begin();it!=(t2->lastpos).end(); it++) {
		    (temp->lastpos).insert(*it);
		}
		if(t2->nullable) {
		    for(set<int>::iterator it=(t1->lastpos).begin();it!=(t1->lastpos).end(); it++) {
			(temp->lastpos).insert(*it);
		    }
		}
		// Get followpos
		vector<int> v1;
		for(set<int>::iterator it=(t1->lastpos).begin(); it!= (t1->lastpos).end(); it++) {
		    v1.pb(*it);
		}
		vector<int> v2;
		for(set<int>::iterator it=(t2->firstpos).begin(); it!=(t2->firstpos).end(); it++) {
		    v2.pb(*it);
		}
		for(int j=0;j<v1.size();j++) {
		    for(int k=0;k<v2.size();k++) {
			followpos[v1[j]].insert(v2[k]);
		    }
		}
	    }
	    else if(s[i]=='|') {
		// Get nullable
		if(t1->nullable || t2->nullable) {
		    temp->nullable = true;
		}
		// Get firstpos
		for(set<int>::iterator it=(t1->firstpos).begin();it!=(t1->firstpos).end(); it++) {
		    (temp->firstpos).insert(*it);
		}
		for(set<int>::iterator it=(t2->firstpos).begin();it!=(t2->firstpos).end(); it++) {
		    (temp->firstpos).insert(*it);
		}
		// Get lastpos
		for(set<int>::iterator it=(t2->lastpos).begin();it!=(t2->lastpos).end(); it++) {
		    (temp->lastpos).insert(*it);
		}
		for(set<int>::iterator it=(t1->lastpos).begin();it!=(t1->lastpos).end(); it++) {
		    (temp->lastpos).insert(*it);
		}
	    }
	    st.push(temp);
	}
	else if(s[i]=='*') {
	    assert(st.size()>=1);
	    node *t = st.top(); st.pop();
	    node *temp = new node();
	    temp->c = s[i];
	    temp->nullable = true;
	    temp->left = t;
	    temp->right = NULL;
	    // Get firstpos
	    for(set<int>::iterator it=(t->firstpos).begin();it!=(t->firstpos).end(); it++) {
		(temp->firstpos).insert(*it);
	    }
	    // Get lastpos
	    for(set<int>::iterator it=(t->lastpos).begin();it!=(t->lastpos).end(); it++) {
		(temp->lastpos).insert(*it);
	    }
	    // Get followpos
	    vector<int> v1;
	    for(set<int>::iterator it=(t->lastpos).begin(); it!= (t->lastpos).end(); it++) {
		v1.pb(*it);
	    }
	    vector<int> v2;
	    for(set<int>::iterator it=(t->firstpos).begin(); it!=(t->firstpos).end(); it++) {
		v2.pb(*it);
	    }
	    for(int j=0;j<v1.size();j++) {
		for(int k=0;k<v2.size();k++) {
		    followpos[v1[j]].insert(v2[k]);
		}
	    }
	    st.push(temp);
	}
	else { // Leaf Node
	    w++;
	    hash[w] = s[i];
	    node *temp = new node();
	    temp->c = s[i];
	    temp->left = temp->right = NULL;
	    temp->nullable = false;
	    (temp->firstpos).insert(w);
	    (temp->lastpos).insert(w);
	    st.push(temp);
	    sharp = w;
	}
    }
    if(st.size()==1) {
	root = st.top(); st.pop();
    }
    else {
	cout<<"Fail\n";
    }
}

void dfs(node *t) {
    if(t==NULL) return;
    dfs(t->left);
    cout<<(t->c)<<" -- \n";
    cout<<(t->nullable)<<endl;
    cout<<"firstpos - ";
    for(set<int>::iterator it=(t->firstpos).begin(); it!=(t->firstpos).end(); it++) {
	cout<<(*it)<<" ";
    }
    cout<<endl;
    cout<<"lastpos - ";
    for(set<int>::iterator it=(t->lastpos).begin(); it!=(t->lastpos).end(); it++) {
	cout<<(*it)<<" ";
    }
    cout<<endl;
    dfs(t->right);
}

void buildDFA() {
    int sc = 0;
    set<int> startState = root->firstpos;
    stateHash[startState] = sc++;
    if(startState.find(sharp) != startState.end()) {
	isFinal[0] = 1;
    }
    else {
	isFinal[0] = 0;
    }
    stack< set<int> > states;
    states.push(startState);
    while(states.size()) {
	set<int> tp = states.top(); states.pop();
	int cur = stateHash[tp];
	map<char,set<int> > trans;
	for(set<int>::iterator it=tp.begin(); it!=tp.end(); it++) {
	    char al = hash[(*it)];
	    set<int> follow = followpos[*it];
	    for(set<int>::iterator it2=follow.begin(); it2!=follow.end(); it2++) {
		trans[al].insert(*it2);
	    }
	}
	for(map<char,set<int> >::iterator it=trans.begin(); it!=trans.end(); it++) {
	    set<int> nxt = it->se;
	    char what = it->fr;
	    if(stateHash.find(nxt) == stateHash.end()) {
		stateHash[nxt] = sc;
		states.push(nxt);
		dfa[mp(cur,what)] = sc;
		if(nxt.find(sharp) != nxt.end()) {
		    isFinal[sc] = 1;
		}
		else {
		    isFinal[sc] = 0;
		}
		sc++;
	    }
	    else {
		dfa[mp(cur,what)] = stateHash[nxt];
	    }
	}
    }
    cout<<"Done\n";
}

int main() {
    cout<<"Enter Regex : ";
    cin>>reg;
    string postfix = infixToPostfix(reg);
    buildTree(postfix);
    buildDFA();
    for(map<pair<int,char>,int>::iterator it=dfa.begin(); it!=dfa.end(); it++) {
	pair<int,char> ic = it->fr;
	int nxt = it->se;
	cout<<"( "<<ic.fr<<","<<ic.se<<" ) -> "<<nxt<<endl;
    }
    cout<<"Enter number of testcases : ";
    int tc; cin>>tc;
    while(tc--) {
	cout<<"Enter string : ";
	string x; cin>>x;
	int cur = 0;
	int rej=0;
	for(int i=0;i<x.length();i++) {
	    if(dfa.find(mp(cur,x[i]))==dfa.end()) {
		cout<<"Rejected\n";
		rej = 1;
		break;
	    }
	    else {
		cur = dfa[mp(cur,x[i])];
	    }
	}
	if(rej==0 && isFinal[cur]) {
	    cout<<"Accepted\n";
	}
	else if(rej==0) {
	    cout<<"Rejected\n";
	}
    }
}
