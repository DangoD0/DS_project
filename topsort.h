#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
const int N = 50;
class course
{
private:
	string name;
	double score;
	int max_depth;
	int min_depth;
	int index;
public:
	//int Indegree;
	int start_class;
	int end_class;
	int day;
	vector<course*> preCourse;
	course()
	{
		name = "none";
		score = 0;
		start_class = 0;
		end_class = 0;
		max_depth = 0;
		min_depth = 0;
		day = 0;
		//	Indegree = 0;
	}
	~course()
	{

	}
	course(string name, double score, int start_class, int end_class, int index, int day)
	{
		this->name = name;
		this->score = score;
		this->start_class = start_class;
		this->end_class = end_class;
		this->max_depth = 0;
		this->day = day;
		//	Indegree = 0;
		this->index = index;
	}
	bool operator ==(const course& other) const
	{
		return this->name == other.name && this->index == other.index;
	}
	void print()
	{
		cout << "name:" << name << " ";
		printf("score:%.1lf index:%d", score, index);
		puts("");
	}
	string get_name() const
	{
		return name;
	}
	void set_max_depth(int depth)
	{
		max_depth = depth;
	}
	void set_min_depth(int depth)
	{
		min_depth = depth;
	}
	int get_max_depth()
	{
		return max_depth;
	}
	int get_min_depth()
	{
		return min_depth;
	}
	float get_score()const
	{
		return score;
	}
	int get_index() const
	{
		return index;
	}
};
struct MyHash {
	size_t operator()(course* s) const {
		std::string str = s->get_name();
		return std::hash<std::string>()(str);
	}
};
class layer
{
private:
	double limit = 0;
	friend class graph;
public:
	vector<course*> a;
	bool occupy[8][13] = { false };
	double credit = 0;
	bool compareCredit()
	{
		return credit >= limit;
	}
	void setLimit(double limit)
	{
		this->limit = limit;
	}
	double restCredit()
	{
		return limit - credit;
	}
};
class graph
{
private:
	course* node;
	int Node_index = 0, Edge_index = 0;
	int node_num;
	unordered_map<string, course*> search;
	unordered_map<course*, int, MyHash> h;
	unordered_map<course*, int, MyHash> Indegree;
	unordered_map<course*, int, MyHash> Todegree;
	unordered_map<int, course*> e;
	unordered_map<int, int> ne;
public:
	layer* l;
	graph(int num)
	{
		node = new course[num];
		node_num = num;
		l = new layer[8];
	}
	~graph()
	{
		delete[] node;
		delete[] l;
	}
	void addCourse(const course c)
	{
		node[Node_index] = c;
		search[c.get_name()] = &node[Node_index];
		h[&node[Node_index]] = -1;
		Node_index++;
	}
	course getCourse(int index)
	{
		return node[index];
	}
	course* getCourse_Pointer(int index)
	{
		return &node[index];
	}
	void setEdge(int index1, int index2)
	{
		Indegree[&node[index2]]++;
		Todegree[&node[index1]]++;
		e[Edge_index] = &node[index2];
		ne[Edge_index] = h[&node[index1]];
		node[index2].preCourse.push_back(&node[index1]);
		for (auto p : node[index1].preCourse)
		{
			if (count(node[index2].preCourse.begin(), node[index2].preCourse.end(), p) == 0)
				node[index2].preCourse.push_back(p);
		}
		h[&node[index1]] = Edge_index++;
	}
	bool sortForSchedule()
	{
		queue<course*> p, q;
		for (int i = 0; i < Node_index; i++)
			if (Indegree[&node[i]] == 0)
			{
				int flag = true;
				for (int j = node[i].start_class; j <= node[i].end_class; j++)
					if (l[0].occupy[node[i].day][j]) {
						flag = false;
						break;
					}
				if (!flag) break;
				p.push(&node[i]);
				for (int j = node[i].start_class; j <= node[i].end_class; j++)
					l[0].occupy[node[i].day][j] = true;
				l[0].a.push_back(&node[i]);
			}
		for (int k = 1; k < 8; k++)
		{
			while (p.size())
			{
				auto t = p.front();
				p.pop();
				for (int i = h[t]; i != -1; i = ne[i])
				{
					Indegree[e[i]]--;
					if (Indegree[e[i]] == 0)
					{
						int flag = true;
						for (int j = e[i]->start_class; j <= e[i]->end_class; j++)
							if (l[0].occupy[e[i]->day][j]) {
								flag = false;
								break;
							}
						if (!flag) break;
						for (int j = e[i]->start_class; j <= e[i]->end_class; j++)
							l[k].occupy[e[i]->day][j] = true;
						q.push(e[i]);
						l[k].a.push_back(e[i]);
					}
				}
			}
			if (q.size()) p.swap(q);
			else break;
		}
		return true;
	}
	bool sortForList()
	{
		queue<course*> p, q;
		for (int i = 0; i < Node_index; i++)
			if (Indegree[&node[i]] == 0)
			{
				p.push(&node[i]);
				l[0].a.push_back(&node[i]);
				node[i].set_min_depth(0);
			}
		for (int k = 1; k < 8; k++)
		{
			while (p.size())
			{
				auto t = p.front();
				p.pop();
				for (int i = h[t]; i != -1; i = ne[i])
				{
					Indegree[e[i]]--;
					if (Indegree[e[i]] == 0)
					{
						q.push(e[i]);
						l[k].a.push_back(e[i]);
						e[i]->set_min_depth(k);
					}
				}
			}
			if (q.size()) p.swap(q);
			else break;
		}
		for (int i = 0; i < Node_index; i++)
		{
			get_max_depth(i);
		}
		return true;
	}
	int get_max_depth(int i)
	{
		int max_depth = 0;
		if (h[&node[i]] != -1)
		{
			for (int j = h[&node[i]]; j != -1; j = ne[j])
			{
				max_depth = max(max_depth, get_max_depth(e[j]->get_index()));
			}
			node[i].set_max_depth(max_depth + 1);
		}
		return node[i].get_max_depth();
	}
	vector<course*> forReach(int index)
	{
		vector<course*> target;
		if (h[&node[index]] != -1)
		{
			for (int i = h[&node[index]]; i != -1; i = ne[i])
			{
				target.push_back(e[i]);
			}
		}
		return target;
	}
	layer sortToMiddle(layer l)
	{
		layer tmp,result;
		tmp = l;
		int n = tmp.a.size();
		sort(tmp.a.begin(), tmp.a.end(), [=](course* a, course* b) {return Todegree[a] < Todegree[b]; });
		for (int i = 0; i < n - (n + 1) % 2; i += 2) 
			result.a.push_back(tmp.a[i]);
		for (int i = n - 1 - n % 2; i >= 1; i -= 2) 
			result.a.push_back(tmp.a[i]);
		return result;
	}
};

//int main()
//{
//	ifstream ifs;
//	graph G(20);
//	ifs.open("course.txt", ios::in);
//	if (!ifs.is_open())
//	{
//		cout << "error opening course.txt!" << endl;
//		return 0;
//	}
//	string buf;
//	while (getline(ifs, buf))
//	{
//		stringstream word(buf);
//		course tmp1, tmp2;
//		string name;
//		double score;
//		int day;
//		int start_class;
//		int end_class;
//		int index;
//		word >> name, word >> score, word >> day >> start_class, word >> end_class, word >> index;
//		tmp1 = course(name, score, start_class, end_class, index, day);
//		G.addCourse(tmp1);
//		G.getCourse(index).print();
//	}
//	ifs.close();
//	ifs.open("edge.txt", ios::in);
//	if (!ifs.is_open())
//	{
//		cout << "error opening edge.txt!" << endl;
//		return 0;
//	}
//	while (getline(ifs, buf))
//	{
//		stringstream word(buf);
//		int index1, index2;
//		word >> index1 >> index2;
//		G.setEdge(index1, index2);
//	}
//	ifs.close();
//	G.sortForSchedule();
//	for (int i = 0; i < 8; i++)
//	{
//		cout << "semester " << i << ":";
//		for (auto p : G.l[i].getA())
//		{
//			cout << p.get_name() << " ";
//		}
//		puts("");
//	}
//	cout << G.get_max_depth(0);
//}
