#include <iostream>
#include <stack>
#include <queue>
#include <cctype>

int main() {
	std::string expr ;
	std::queue<char> q;
	std::queue<char> p;
	std::stack<char> s;
	std::cin >> expr;

	for(char c:expr) q.push(c);
	int pr=-1;
	while(!q.empty()) {
		char c = q.front();
		q.pop();
		p.push(c);
		if(c=='*'&&pr==0) {
			std::queue<char> t;
			while(!q.empty() && q.front()!='+') p.push(q.front()), q.pop();
			while(!p.empty() && p.front()!='+') s.push(p.front()), p.pop();
			if(!p.empty()) s.push(p.front()), p.pop();
			while(!s.empty()) p.push(s.top()), s.pop();
			while(!q.empty()) t.push(q.front()), q.pop();
			while(!t.empty()) p.push(t.front()), t.pop();
			while(!p.empty()) q.push(p.front()), p.pop();
		}
		if(c=='+') pr = pr==1 ? 2 : pr==2 ? 0 : 0;
		if(c=='*') pr = 1;
	}
	while(!p.empty()) std::cout<<p.front(), p.pop();
	std::cout<<std::endl;
	return 0;
}

