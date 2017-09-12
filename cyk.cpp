// Code to implement the CYK Algorithm
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
#define SZ 101

string LHS[SZ], RHS[SZ];
int nr;
char strt;
set<char> table[SZ][SZ];

bool isChomsky() {
    for(int i=0;i<nr;i++) {
	if(LHS[i].length()==1) {
	    if(LHS[i][0]>='A' && LHS[i][0]<='Z') {
	    }
	    else {
		return 0;
	    }
	}
	else {
	    return 0;
	}
    }
    for(int i=0;i<nr;i++) {
	if(RHS[i].length()==1) {
	    if(RHS[i][0]>='A' && RHS[i][0]<='Z') {
		return 0;
	    }   
	}   
	else {
	    if(RHS[i].length()==2) {
		if((RHS[i][0]>='A' && RHS[i][0]<='Z') && (RHS[i][1]>='A' && RHS[i][1]<='Z')) {
		}
		else {
		    return 0;
		}
	    }
	    else {
		return 0;
	    }
	}
    }
    return 1;
}

bool isAccepted(string s) {
    for(int i=0;i<s.length();i++) {
	for(int j=0;j<nr;j++) {
	    if(RHS[j].length()==1 && RHS[j][0]==s[i]) {
		table[0][i].insert(LHS[j][0]);
	    }
	}
    }
    for(int i=1;i<s.length();i++) {
	for(int j=0;j<s.length()-i;j++) {
	    int a,b,x,y;
	    a=0; b=j;
	    y=j+1; x=i-1;
	    for(int k=0;k<i;k++) {
		for(set<char>::iterator it1=table[a][b].begin(); it1 != table[a][b].end(); it1++) {
		    char c1 = (*it1);
		    for(set<char>::iterator it2=table[x][y].begin(); it2 != table[x][y].end(); it2++) {
			char c2 = (*it2);
			string s = "";
			s += c1;
			s += c2;
			for(int p=0;p<nr;p++) {
			    if(RHS[p]==s) {
				table[i][j].insert(LHS[p][0]);
			    }
			}
		    }
		}
		a++;
		y++; x--;
	    }
	}
    }
    int L = s.length();
    for(int i=0;i<L;i++) {
	for(int j=0;j<L-i;j++) {
	    for(set<char>::iterator it=table[i][j].begin(); it!=table[i][j].end();it++) {
		cout<<(*it);
	    }
	    cout<<"\t";
	}
	cout<<endl;
    }
    for(set<char>::iterator it=table[L-1][0].begin(); it!=table[L-1][0].end(); it++) {
	char c = (*it);
	if(c==strt) {
	    return 1;
	}
    }
    return 0;
}

int main() {
    cout<<"Enter number of productions : "; cin>>nr;
    for(int i=0;i<nr;i++) {
	cout<<"Enter LHS : "; cin>>LHS[i];
	cout<<"Enter RHS : "; cin>>RHS[i];
    }
    cout<<"Enter start symbol : "; cin>>strt;
    if(!isChomsky()) {
	cout<<"\n\nGrammar is not in Chomsky Normal Form.\n\n";
	return 0;
    }
    int t;
    cout<<"Enter number of test cases : "; cin>>t;
    while(t--) {
	string input; cout<<"Enter string : "; cin>>input;
	for(int i=0;i<input.length();i++) {
	    for(int j=0;j<input.length();j++) {
		table[i][j].clear();
	    }
	}
	if(isAccepted(input)) {
	    cout<<"\n\nAccepted\n\n";
	}
	else {
	    cout<<"\n\nRejected\n\n";
	}
    }
}
