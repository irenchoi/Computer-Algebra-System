#include <bits/stdc++.h>
using namespace std;
struct variable {
    string val;
    int coeff;
    int deg;
};
struct node {
    variable val;
    node* left;
    node* right;
};
bool isint(string s) {
    bool x = true;
    for (int i = 0; i<s.length(); i++) {
        x &= (s[i] >= '0' and s[i] <= '9');
    }
    return x;
}
bool operation (string s) {
    return s == "+" || s == "-" || s == "*" || s == "/" || s == "^";
}
bool operand(string s) {
    return s == "x" or (!operation(s) and s != "(" and s != ")");
}
map<string,int> precmap = {{"+",1},{"-",1},{"*",2},{"/",2},{"^",3}};
int prec(string s) {
    if (precmap.count(s)) {
        return precmap[s];
    }
    return -1;    
}
ostream& operator << (ostream& o, variable& a)
{
    o << "(" << a.val << " " << a.coeff << " "<<a.deg<<")";
    return o;
}
variable strtovar(string s) {
    bool x  = true;
    for (int i = 0; i<s.length(); i++) {
        x &= (s[i] >= '0' and s[i] <= '9');
    }
    variable v = {s,1,1};
    if (x) v = {s,stoi(s),0};
    if (operation(s)) v = {s,-1,-1};
    return v;
}

bool leftassoc(string s) {
    return s == "+" || s == "-" || s == "*" || s == "/";
}
vector<string> shunt(vector<string> v) {
    vector<string> post;
    stack<string> s;
    for (auto x: v) {
       if (operand(x)) post.push_back(x); // 1
       else if (x == "(") s.push(x);
       else if (x == ")") {
        while (!s.empty() and s.top() != "(") {
            post.push_back(s.top());
            s.pop();
        }
        if (!s.empty()) s.pop();
       }
       else {
        while (!s.empty() and prec(x) <= prec(s.top())) {
            post.push_back(s.top());
            s.pop();
            }
            s.push(x); // stack: +
       }
    }
    while (!s.empty()) {
        post.push_back(s.top());
        s.pop();
    }
    return post;
}
node* ast(vector<string> post) {
    stack<node*> s;
    for (auto x: post) {
        if (operand(x)) {
            node* nod = new node;
            variable var = strtovar(x);
            nod->val = var;
            s.push(nod);
        }
        if (operation(x)) {
            node* a = s.top();
            s.pop();
            node* b = s.top();
            s.pop();
            node* n = new node;
            node* nod = new node;
            variable var = strtovar(x);
            n->val = var;
            n->left = b;
            n->right = a; 
            s.push(n);
        }
    }
    return s.top();
}
string ctostring(char c) {
    string s(1,c);
    return s;
}
 int intparser(int acc, string s, int &ix) {
     if (ix >= s.length()) return acc;
     char c = s[ix];
     if (c >= '0' && c <= '9') return intparser(acc*10 + c - '0', s, ++ix);
     else return acc;
 }
node *add(node *l, node *r) {
    if (isint((l->val).val) and stoi((l->val).val) == 0) {
        return r;
    }
    if (isint((r->val).val) and stoi((r->val).val) == 0) {
        return l;
    }
    if (isint((r->val).val) and isint((l->val).val)) {
        node *n = new node;
        variable var = {to_string((r->val).coeff+(l->val).coeff), (r->val).coeff+(l->val).coeff, 0};
        n->val = var;
        return n;
    }
    if ((r->val).val == "x" and (l->val).val == "x" and (r->val).deg == (l->val).deg) {
        variable var = {"x",(r->val).coeff+(l->val).coeff,(r->val).deg};
        node *n = new node;
        n->val = var;
        return n;
    }
    variable var = {"+",-1,-1};
    node *n = new node;
    n->val = var;
    n->left = l;
    n->right = r;
    return n;
}
node *subhelp(node *r) {
    if ((r->val).val == "x") {
        (r->val).coeff = -1*(r->val).coeff;
        return r;
    }
    else if ((r->val).val == "+" or (r->val).val == "-") {
        r->left = subhelp(r->left);
        r->right = subhelp(r->right);
        return r;
    }
    else if ((r->val).val == "*" or (r->val).val == "/") {
        r->left = subhelp(r->left);
        return r;
    }
    
    (r->val).coeff = -1*(r->val).coeff;
    return r;
}
node *sub(node *l, node *r) {
    variable var = {"+",-1,-1};
    node *n = new node;
    n->val = var;
    n->left = l;
    r = subhelp(r);
    n->right = r;
    return n;
}
node *mult(node *l, node *r) {
    if (isint((l->val).val) and stoi((l->val).val) == 1) {
        return r;
    }
    if (isint((r->val).val) and stoi((r->val).val) == 1) {
        return l;
    }
    if (isint((r->val).val) and isint((l->val).val)) {
        variable var = {to_string(stoi((l->val).val)*stoi((r->val).val)),stoi((l->val).val)*stoi((r->val).val),0};
    }

}
node *div(node *l, node *r) {
    
}
node *expt(node *l, node *r) {
    if (isint((r->val).val) and stoi((r->val).val) == 0) {
        variable var = {"1",1,0};
        node *n = new node;
        n->val = var;
        return n;
    }
    if (isint((r->val).val) and stoi((r->val).val) == 1) {
        return l;
    }
    if (isint((l->val).val) and isint((r->val).val)) {
        int val = pow(stoi((l->val).val),stoi((r->val).val));
        variable var = {to_string(val),val,0};
        node *n = new node;
        n->val = var;
        return n;
    }
    if ((l->val).deg > 0 and isint((r->val).val)) {
        node *n = new node;
        
    }
}
node *simplify(node *root) {
    if ((root->val).val == "x") {
        return root;
    }
    if (operation((root->val).val)) {
        string op = (root->val).val;
        if (op == "+") return add(simplify(root->left), simplify(root->right));
        if (op == "-") {
            cout<<"calling subhelp\n";
            return sub(simplify(root->left), simplify(root->right));
        }
        if (op == "*") return mult(simplify(root->left), simplify(root->right));
        if (op == "/") return div(simplify(root->left), simplify(root->right));
        if (op == "^") return expt(simplify(root->left), simplify(root->right));
    }
    return root;
}
bool comparevar(variable a, variable b) {
    return a.val == b.val and a.coeff == b.coeff and a.deg == b.deg;
}
bool comparetree(node *a, node *b) {
    if (a == NULL and b == NULL) {
        return true;
    }
    if (a != NULL and b != NULL)
    return comparevar(a->val,b->val) and comparetree(a->left,b->left) and comparetree(a->right,b->right);
    return false;
}
void solve(string s) {
    vector<string> exp;
    for (int i = 0; i<s.length(); i++) {
        if (isdigit(s[i])) {
            int val = intparser(0,s,i);
           exp.push_back(to_string(val));
           i--;
        }
        else {
            exp.push_back(ctostring(s[i]));
        }
    }
    vector<string> post = shunt(exp);
    for (auto x: post) {
        cout<<x<<endl;
    }
    node *root = ast(post);
    cout<<(root->val)<<" "<<root->left->val<<" "<<root->right->val<<endl;
    node *root2 = simplify(root);
    while (!comparetree(root,root2)) {
        root = root2;
        root2 = simplify(root2);
    }
    cout<<(root->val)<<endl;
}
int main() {
    string s;
    cin>>s;
    solve(s);
}