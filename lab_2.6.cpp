#include "lab_2.6.h"
using std::cin;
using std::cout;
using std::endl;
using std::vector;

size_t powerOfTwo(size_t& value)
{
	size_t tmp = 1;
	while (tmp < value)
		tmp *= 2;
	return tmp;
}

void change(vector<int>::iterator x, vector<int>::iterator y, bool reverse)
{
	int temp1 = *x > *y ? *x : *y, temp2 = *x > *y ? *y : *x;
	if (reverse)
	{
		*x = temp1;
		*y = temp2;
	}
	else
	{
		*x = temp2;
		*y = temp1;
	}
}

void sort(vector<int>::iterator begin, vector<int>::iterator end, bool reverse = false)
{
	size_t len = end - begin;
	for (size_t i = 0; i < len / 2; ++i)
	{
		change(begin + i, begin + i + len/2, reverse);
	}
	if (len != 2)
	{
		sort(begin, begin + len/2, reverse);
		sort(begin + len/2, end, reverse);
	}
}

void bitonicSort(vector<int>& vect, int t_num)
{
	std::thread** threads = new std::thread* [t_num];
	for (int i = 0; i < t_num; ++i) threads[i] = nullptr;
	int curThread = 0;
	for (size_t i = 2; i <= vect.size(); i *= 2)
	{
		for (size_t j = 0; j < vect.size() / i; ++j)
		{
			size_t dif1 = i * j, dif2 = i * (j + 1);
			if (j % 2)
			{
				if (i >= pow(2, 20) && t_num > 1)
				{
					if (threads[curThread] != nullptr)
					{
						threads[curThread]->join();
						delete threads[curThread];
					}
					threads[curThread] = new std::thread(sort, vect.begin() + dif1,
						vect.begin() + dif2, true);
					++curThread;
					if (curThread == t_num) curThread = 0;
				}
				else sort(vect.begin() + dif1, vect.begin() + dif2, true);
			}
			else
			{
				if (i >= pow(2, 20) && t_num > 1)
				{
					if (threads[curThread] != nullptr)
					{
						threads[curThread]->join();
						delete threads[curThread];
					}
					threads[curThread] = new std::thread(sort, vect.begin() + dif1, 
						vect.begin() + dif2, false);
					++curThread;
					if (curThread == t_num) curThread = 0;
				}
				else sort(vect.begin() + dif1, vect.begin() + dif2);
			}
		}
		for (int j = 0; j < t_num; ++j)
		{
			if (threads[j] != nullptr)
			{
				threads[j]->join();
				delete threads[j];
				threads[j] = nullptr;
			}
		}
	}
}

int main()
{
	cout << "Input number of symbols and number of threads: ";
	size_t sym, thr;
	cin >> sym >> thr;
	vector<int> a(sym);
	std::srand(std::time(0));
	for (int i = 0; i < sym; ++i)
		a[i] = rand();
	if (sym <= 100)
	{
		for (auto i : a) cout << i << ' ';
		cout << endl;
	}
	size_t nearPow = powerOfTwo(sym);
	size_t thr1 = powerOfTwo(thr);
	if(thr1 != thr) thr = thr1 / 2;
	if (nearPow != sym)
	{
		a.resize(nearPow);
		for (size_t i = sym; i < nearPow; ++i)
		{
			a[i] = 0;
		}
	}
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	bitonicSort(a, thr);
	end = std::chrono::system_clock::now();
	cout << "Time of work: " << std::chrono::duration_cast<std::chrono::seconds>
		(end - start).count() << endl;
	if (nearPow != sym)
	{
		auto pos = a.begin();
		for (auto i : a)
		{
			if (i == 0) break;
			++pos;
		}
		a.erase(pos, pos + nearPow - sym);
	}
	if (sym <= 100)
	{
		for (auto i : a) cout << i << ' ';
		cout << endl;
	}
	return 0;
}
