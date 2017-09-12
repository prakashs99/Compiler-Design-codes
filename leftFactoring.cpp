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
char strt;
vector<char> nonTerminals;
int mark[26];

void leftFactoring() {
    int flag = 1;
    while(flag) {
	flag = 0;
	for(int i=0;i<nonTerminals.size();i++) {
	    vector<string> r = rules[nonTerminals[i]];
	    int w = 0;
	    string alpha = "";
	    for(int j=0;j<r.size();j++) {
		for(int k=j+1;k<r.size();k++) {
		    int l = 0;
		    for(int m=0;m<r[j].size() && m<r[k].size() && r[j][m]==r[k][m];m++) {
			l++;
		    }
		    if(l>w) {
			w = l;
			alpha = r[j].substr(0,w);
		    }
		}
	    }
	    if(w>0) {
		flag = 1;
		vector<string> r2; r2.clear();
		vector<string> beta; beta.clear();
		for(int j=0;j<r.size();j++) {
		    if(r[j].substr(0,w) == alpha) {
			if(w==r[j].length()) {
			    beta.pb("~");
			}
			else {
			    beta.pb(r[j].substr(w));
			}
		    }
		    else {
			r2.pb(r[j]);
		    }
		}
		char c;
		for(int j=0;j<26;j++) {
		    if(mark[j]==0) {
			mark[j] = 1;
			c = (char)('A'+j);
			break;
		    }
		}
		nonTerminals.pb(c);
		string nt = "";
		nt += c;
		r2.pb(alpha+nt);
		rules[nonTerminals[i]] = r2;
		rules[c] = beta;
	    }
	    for(map<char,vector<string> >::iterator it=rules.begin(); it!=rules.end(); it++) {
		char NT = it->fr;
		vector<string> r = it->se;
		for(int j=0;j<r.size();j++) {
		    //		cout<<NT<<" -> "<<r[j]<<endl;
		}
	    }
	}
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
	mark[*it-'A'] = 1;
    }
    leftFactoring();
    for(map<char,vector<string> >::iterator it=rules.begin(); it!=rules.end(); it++) {
	char NT = it->fr;
	vector<string> r = it->se;
	for(int j=0;j<r.size();j++) {
	    cout<<NT<<" -> "<<r[j]<<endl;
	}
    }
}
