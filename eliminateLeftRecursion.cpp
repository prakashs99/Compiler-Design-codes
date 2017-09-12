// Input : Grammar.
// Output : First, Follow sets. Parse Table. Accept/Reject verdict of parser.
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

int nr;
set<char> nter;
set<char> ter;
map<char,vector<string> > rules;
vector<string> LHS;
vector<string> RHS;
map<char,set<char> > frst, flw;
map<char,int> visited;
char strt;
vector<char> nonTerminals;
int mark[26];

void removeLeftRec(char NT, vector<string> r) {
    vector<string> alpha, beta;
    vector<string> ret;
    ret.clear();
    int flag = 0;
    for(int i=0;i<r.size();i++) {
	if(r[i][0]==NT) {
	    flag = 1;
	    if(r[i].length()==1) {
	    }
	    else {
		alpha.pb(r[i].substr(1));
	    }
	}
	else {
	    beta.pb(r[i]);
	}
    }
    if(flag==0) {
	return;
    }
    char c;
    for(int i=0;i<26;i++) {
	if(mark[i]==0) {
	    mark[i] = 1;
	    c = (char)('A'+i);
	    break;
	}
    }
    if(beta.empty()) {
	rules[NT].clear();
	string temp = "";
	temp += c;
	rules[NT].pb(temp);
	if(alpha.empty()) {
	}
	else {
	    for(int i=0;i<alpha.size();i++) {
		rules[c].pb(alpha[i]+(char)(c));
	    }
	}
	rules[c].pb("~");
    }
    else {
	rules[NT].clear();
	string temp = "";
	temp += c;
	for(int i=0;i<beta.size();i++) {
	    if(beta[i]!="~") {
		rules[NT].pb(beta[i]+(char)(c));
	    }
	    else {
		string temp = ""; temp += c;
		rules[NT].pb(temp);
	    }
	}
	if(alpha.empty()) {
	}   
	else {
	    for(int i=0;i<alpha.size();i++) {
		rules[c].pb(alpha[i]+(char)(c));
	    }
	}   
	rules[c].pb("~");
    }
}

int main() {

    cout<<"For epsilon, use the symbol ~"<<endl;
    cout<<"Enter the number of production rules : ";
    cin>>nr;
    string input;
    for(int i=0;i<nr;i++) {
	cout<<"LHS : "; cin>>input; LHS.pb(input);
	nter.insert(LHS[i][0]);
	cout<<"RHS : "; cin>>input; RHS.pb(input);
	rules[LHS[i][0]].pb(RHS[i]);
	for(int j=0;j<RHS[i].size();j++) {
	    if(!(RHS[i][j]>='A' && RHS[i][j]<='Z')) {
		ter.insert(RHS[i][j]);
	    }
	    else if(RHS[i][j]>='A' && RHS[i][j]<='Z') {
		nter.insert(RHS[i][j]);
	    }
	    else {
		ter.insert(RHS[i][j]);
	    }
	}
    }
    cout<<"Enter start symbol : ";
    cin>>strt;
    for(set<char>::iterator it=nter.begin(); it!=nter.end(); it++) {
	nonTerminals.pb(*it);
	mark[(*it)-'A'] = 1;
    }
    for(int i=0;i<nonTerminals.size();i++) {
	for(int j=0;j<i;j++) {
	    vector<string> r1 = rules[nonTerminals[i]];
	    vector<string> r2; r2.clear();
	    for(int k=0;k<r1.size();k++) {
		if(r1[k][0] == nonTerminals[j]) {
		    vector<string> r3 = rules[nonTerminals[j]];
		    for(int w=0;w<r3.size();w++) {
			r2.pb(r3[w]+r1[k].substr(1));
		    }
		}
		else {
		    r2.pb(r1[k]);
		}
	    }
	    rules[nonTerminals[i]] = r2;
	}
	removeLeftRec(nonTerminals[i],rules[nonTerminals[i]]);
    }
    for(map<char,vector<string> >::iterator it=rules.begin(); it!=rules.end(); it++) {
	vector<string> r = it->se;
	char NT = it->fr;
	for(int i=0;i<r.size();i++) {
	    cout<<NT<<" -> "<<r[i]<<endl<<flush;
	}
    }
}
