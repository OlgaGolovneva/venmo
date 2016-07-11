#include <iostream>
//file operations
#include <fstream> 
//strings
#include <sstream> 
#include <ctime>
//precision
#include <iomanip>
#include <math.h>
//heaps and hashtables
#include <unordered_map>
#include <queue>
using namespace std;
#include<vector>

//this class dynamically updates/computes median
class Medianer
{
	vector<int> count;
	int n;
	int median;
	int smallerEls;

	int findprev(int index)
	{
		int i = index-1;
		while (count[i]==0)
			i--;
		return i;
	}

	int findnext(int index)
	{
		int i = index+1;
		while (count[i]==0)
			i++;
		return i;
	}

	void balance()
	{
		if (smallerEls>=(n+1)/2)
		{
			median=findprev(median);
			smallerEls-=count[median];
		}
		else if (smallerEls+count[median]<(n+1)/2)
		{
			smallerEls+=count[median];
			median=findnext(median);
		}
	}

	void increment(int index)
	{
		n++;
		if (index>=count.size())
			count.resize(count.size()*2,0);
		count[index]++;
		if (index < median)
			smallerEls++;
		balance();
	}

	void decrement(int index)
	{
		n--;
		count[index]--;
		if (index < median)
			smallerEls--;
		balance();
	}

	public:
	Medianer()
	{
		n=0;
		median=0;
		smallerEls=0;
		count=vector<int>(128,0);
	}

	double getMedian()
	{
		
		if (smallerEls+count[median]>=(n+2)/2)
			return median;
		return ((double)median + findnext(median))/2;
	}

	void incrementElement(int val)
	{

		if (val>0)
			decrement(val);
		increment(val+1);
	}

	void decrementElement(int val)
	{

		if (val>1)
		  increment(val-1);
		decrement(val);
	}
};

long long parsetime(string &s)
{
	stringstream ss(s);
	tm timeout;
	string tmp;
	char c;
	ss >> tmp >> c >> timeout.tm_year >> c >> timeout.tm_mon >>c>> timeout.tm_mday >>c>> timeout.tm_hour >>c>> timeout.tm_min >>c>>timeout.tm_sec;
	setenv("TZ", "UTC", 1);
	time_t curtime = mktime(&timeout);
	tm epoch_strt;
	epoch_strt.tm_sec = 0;
	epoch_strt.tm_min = 0;
	epoch_strt.tm_hour = 0;
	epoch_strt.tm_mday = 1;
	epoch_strt.tm_mon = 0;
	epoch_strt.tm_year = 70;
	epoch_strt.tm_isdst = -1;
	time_t basetime = mktime(&epoch_strt);
	return difftime(curtime, basetime);
}

//input parsers
string parse1name(string &s)
{
	stringstream ss(s);
	string tmp,out;
	char c;

	ss>>tmp>>tmp>>tmp>>c>>out;

	size_t pos=out.find(",");

	out=out.erase(pos-1,2);

	return out;
}

string parse2name(string &s)
{
	stringstream ss(s);
	string tmp,out;
	char c;

	ss>>tmp>>tmp>>tmp>>tmp>>tmp>>c>>out;

	size_t pos=out.find("}");

	out=out.erase(pos-1,2);

	return out;
}

string combine(string name1, string name2)
{
	if (name1<=name2)
		return name1+"@"+name2;
	return combine(name2,name1);
}

string getFirstName(string pair)
{
	return pair.substr(0, pair.find("@"));
}

string getSecondName(string pair)
{
	return pair.substr(pair.find("@")+1);
}

//minHeap comparator
typedef pair<long long, string> Key;
struct comparator 
{
 	bool operator()(Key i, Key j) 
 	{
 		return i.first > j.first;
 	}
};



int main (int argc, char *argv[])
{
	string line,name1,name2;

	long long curtime;
	long long maxtime=0;
	long long mintime=0;

	unordered_map<string,int> name2deg;
	unordered_map<string, long long> pair2time;

	priority_queue<Key, vector<Key>, comparator> time2pair;
	Medianer medianer;

	long long realtime=0;

	ifstream input ("venmo_input/venmo-trans.txt");
	ofstream output ("venmo_output/output.txt");

  	if (input.is_open() && output.is_open())
  	{
   		while (getline (input,line))
    		{
    			//parse time as a number
    			curtime=parsetime(line);
    			realtime=max(realtime,curtime);
    			//parse names
    			name1=parse1name(line);
    			name2=parse2name(line);
    			if (name1=="" || name2=="")
    				continue;

    			string names=combine(name1,name2);
    			if (pair2time.find(names)==pair2time.end()) //new pair
    			{
    				pair2time[names]=curtime;
    				//update degree of both vertices
    				medianer.incrementElement(name2deg[name1]);
    				name2deg[name1]++;
    				medianer.incrementElement(name2deg[name2]);
    				name2deg[name2]++;
    				time2pair.push(make_pair(curtime, names));
    			}
    			else //update time for an existing pair
    			{
    				if (curtime>pair2time[names])
    				{
    					pair2time[names]= curtime;
    					time2pair.push(make_pair(curtime, names));
					}
				}

			//remove old pairs		
    			while (!time2pair.empty() && time2pair.top().first<=realtime-60) 
    			{
    				if (pair2time[time2pair.top().second]==time2pair.top().first)
    				{
    					name1=getFirstName(time2pair.top().second);
    					name2=getSecondName(time2pair.top().second);
    					medianer.decrementElement(name2deg[name1]);
    					name2deg[name1]--;
    					medianer.decrementElement(name2deg[name2]);
    					name2deg[name2]--;
    					pair2time.erase(time2pair.top().second);
    				}
 				time2pair.pop();
 			}

 			output<< fixed << setprecision(2)<<floor(100 * medianer.getMedian()) / 100<< "\n";
 			//cout<< fixed << setprecision(2)<<floor(100 * medianer.getMedian()) / 100<< "\n";
    		}
    		input.close();
  		output.close();
  	}
	else cout << "Unable to open venmo-trans.txt file"; 

	return 0;
}