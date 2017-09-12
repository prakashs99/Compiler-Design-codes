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
	}
    }
}


void findFirst(char NT) {
    if(visited[NT]==1) {
	return;
    }
    for(int i=0;i<rules[NT].size();i++) {
	string rhs = rules[NT][i];
	int j;
	int flag;
	for(j=0;j<rhs.length();j++) {
	    findFirst(rhs[j]);
	    set<char> fs = frst[rhs[j]];
	    flag = 0;
	    for(set<char>::iterator it=fs.begin(); it!=fs.end();it++) {
		char temp = *it;
		if(temp != '~') {
		    frst[NT].insert(temp);
		}
		if(temp=='~') {
		    flag = 1;
		}
	    }
	    if(flag==0) break;
	}
	if(flag) {
	    frst[NT].insert('~');
	}
    }
    visited[NT] = 1;
}

void findFollow() {
    flw[strt].insert('$');
    while(1) {
	int ff = 0;
	for(int i=0;i<nr;i++) {
	    for(int j=0;j<RHS[i].length();j++) {
		if(RHS[i][j]>='A' && RHS[i][j]<='Z') {
		    string rhs = "";
		    if(j+1<RHS[i].length()) {
			rhs = RHS[i].substr(j+1);
		    }
		    int flag=1;
		    set<char> fw; fw.clear();
		    for(int k=0;k<rhs.length();k++) {
			findFirst(rhs[k]);
			set<char> fs = frst[rhs[k]];
			flag = 0;
			for(set<char>::iterator it=fs.begin(); it!=fs.end();it++) {
			    char temp = *it;
			    if(temp != '~') {
				fw.insert(temp);
			    }
			    if(temp=='~') {
				flag = 1;
			    }
			}
			if(flag==0) {
			    break;
			}
		    }
		    if(flag) {
			if(LHS[i][0] != RHS[i][j]) {
			    for(set<char>::iterator it=flw[LHS[i][0]].begin(); it!=flw[LHS[i][0]].end();it++) {
				fw.insert(*it);
			    }
			}
		    }
		    int temp = flw[RHS[i][j]].size();
		    for(set<char>::iterator it=fw.begin(); it!=fw.end(); it++) {
			flw[RHS[i][j]].insert(*it);
		    }
		    if(temp != flw[RHS[i][j]].size()) {
			ff = 1;
		    }
		}
	    }
	}
	if(ff==0) break;
    }
}

map<pair<char,char>, string> table;

void constructTable() {
    for(int i=0;i<nr;i++) {
	string rhs = RHS[i];
	int j;
	int flag;
	set<char> into; into.clear();
	for(j=0;j<rhs.length();j++) {
	    findFirst(rhs[j]);
	    set<char> fs = frst[rhs[j]];
	    flag = 0;
	    for(set<char>::iterator it=fs.begin(); it!=fs.end();it++) {
		char temp = *it;
		if(temp != '~') {
		    into.insert(temp);
		}
		if(temp=='~') {
		    flag = 1;
		}
	    }
	    if(flag==0) break;
	}
	if(flag) {
	    for(set<char>::iterator it=flw[LHS[i][0]].begin(); it!=flw[LHS[i][0]].end(); it++) {
		into.insert(*it);
	    }
	}
	for(set<char>::iterator it=into.begin(); it!=into.end(); it++) {
	    if(table.find(mp(LHS[i][0],(*it))) == table.end()) {
		table[mp(LHS[i][0],(*it))] = rhs;
	    }
	    else {
		cout<<LHS[i]<<" -> "<<RHS[i]<<endl;
		cout<<"CONFLICT\n";
	    }
	}
    }
}

bool parse(string s) {
    s += "$";
    stack<char> st; st.push('$'); st.push(strt);
    int i=0;
    while(1) {
	while(st.size() && st.top() == '~') {
	    st.pop();
	}
	if(st.empty() || i==s.length()) {
	    if(st.empty() && i==s.length()) return 1;
	    return 0;
	}
	char x = st.top(); st.pop();
	char y = s[i];
	if(table.find(mp(x,y))==table.end()) {
	    return 0;
	}
	string repl = table[mp(x,y)];
	for(int j=repl.size()-1;j>=0;j--) {
	    st.push(repl[j]);
	}
	while(st.size() && st.top()=='~') {
	    st.pop();
	}
	if(st.empty() || i==s.length()) {
	    if(st.empty() && i==s.length()) return 1;
	    return 0;
	}
	while(st.size() && i<s.length() && st.top()==s[i]) {
	    i++;
	    st.pop();
	}
	if(i==s.length() || st.empty()) {
	    if(i==s.length() && st.empty()) return 1;
	    return 0;
	}
    }
    return 1;
}

void reset() {
    LHS.clear(); RHS.clear();
    for(map<char,vector<string> >::iterator it=rules.begin(); it!=rules.end(); it++) {
	vector<string> r = it->se;
	char NT = it->fr;
	string nt = "";
	nt += NT;
	for(int i=0;i<r.size();i++) {
	    LHS.pb(nt); RHS.pb(r[i]);
	}
    }
    ter.clear(); nter.clear();
    nr = LHS.size();
    memset(mark,0,sizeof(mark));
    for(int i=0;i<nr;i++) {
	nter.insert(LHS[i][0]);
	string rhs = RHS[i];
	mark[LHS[i][0]-'A'] = 1;
	for(int j=0;j<rhs.length();j++) {
	    if(rhs[j]>='A' && rhs[j]<='Z') {
		nter.insert(rhs[j]);
		mark[rhs[j]-'A'] = 1;
	    }
	    else {
		ter.insert(rhs[j]);
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
    reset();
    leftFactoring();
    reset();
    cout<<endl<<endl;
    for(int i=0;i<LHS.size();i++) {
	cout<<LHS[i]<<" -> "<<RHS[i]<<endl;
    }
    cout<<endl<<endl;
    for(set<char>::iterator it=ter.begin(); it!=ter.end(); it++) {
	frst[*it].insert(*it);
	visited[*it] = 1;
    }
    for(set<char>::iterator it=nter.begin(); it!=nter.end(); it++) {
	char NT = *it;
	findFirst(NT);
    }
    findFollow();
    cout<<"\n\nFIRST\n\n";
    for(set<char>::iterator it=nter.begin(); it!=nter.end(); it++) {
	set<char> fst = frst[*it];
	cout<<(*it)<<" - ";
	for(set<char>::iterator it2 = fst.begin(); it2!=fst.end(); it2++) {
	    cout<<(*it2)<<" ";
	}
	cout<<endl;
    }
    cout<<"\n\nFOLLOW\n\n";
    for(set<char>::iterator it=nter.begin(); it!=nter.end(); it++) {
	set<char> fw = flw[*it];
	cout<<(*it)<<" - ";
	for(set<char>::iterator it2 = fw.begin(); it2!=fw.end(); it2++) {
	    cout<<(*it2)<<" ";
	}
	cout<<endl;
    }
    cout<<endl;
    constructTable();
    cout<<endl<<endl;
    for(map<pair<char,char>,string>::iterator it=table.begin(); it!=table.end(); it++) {
	cout<<"( "<<(it->fr).fr<<","<<(it->fr).se<<" )  "<<it->se<<endl;
    }
    cout<<endl<<endl;
    cout<<"Enter number of test cases : ";
    int tc; cin>>tc;
    while(tc--) {
	cout<<"Enter input string : ";
	string s; cin>>s;
	bool ac = parse(s);
	if(ac) {
	    cout<<"Accepted\n";
	}
	else {
	    cout<<"Rejected\n";
	}
    }
}
