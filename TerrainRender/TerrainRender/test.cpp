#include<iostream>
#include<string>
#include<vector>
#include <algorithm>
using namespace std;

void print(string str)
{
	cout << str << endl;
}
vector<string> pattern_seg(const string str, const string pattern)
{
	string::size_type pos;
	vector<string> vec;
	string buf(str);
	buf += pattern;
	int size = buf.size();
	for (int i = 0; i < size; i++)
	{
		pos = buf.find(pattern, i);
		if (pos < size)
		{
			string s = buf.substr(i, pos - i);
			if(s != "")
				vec.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return vec;
}
int main()
{
	string pattern(" ");
	string buf(" aa bb cc dd ee ");
	vector<string> strvec(pattern_seg(buf, pattern));
	for_each(strvec.begin(), strvec.end(), print);
	return 0;
}

//
//int main()
//{
//	string str = "test+";
//	str += 't';
//	str += "t";
//	str += "\n";
//	cout << str;
//	
//	string num = "0.88888";
//	double number = 0.0;
//	number = stod(num);
//	cout << number << endl;
//
//	double n = stod(num.substr(0, 5));
//	cout << n << endl;
//
//	vector<string> t;
//
//	string empty = num.substr(0, 0);
//	t.push_back(num.substr(0, 0));
//	t.push_back(num);
//	if(t[0]=="") 
//		cout << "+" << t[0] << "+" << t[1] <<endl;
//	return 0;
//}