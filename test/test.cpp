// test.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<fstream>
#include <string>
#include<set>
#include<list>
#include<algorithm>
#include<map>
#include<vector>
#include"Json.h"
#include <iterator>
#include <ctime>

using namespace std;
int main()
{
	ifstream read1;
	read1.open("input_data.txt", ios::binary);
	Json::Value jv;
	Json::Reader jr;
	if (!jr.parse(read1, jv))
	{
		cout << "failed";
		return -1;
	}
	vector<vector<string>> studentsTime;
	vector<vector<string>> studentsApp;
	vector<vector<string>> studentsID;
	vector<vector<string>> studentsTags;

	vector<vector<string>> deptNo;
	vector<vector<string>> deptTags;
	vector<vector<string>> deptEvent;
	vector<vector<int>> deptMumLimit;;

//read data
	map<string, int> dict_no2index;
	for(int i = 0;jv["students"][i].asBool();i++)
	{
		//ID
		vector<string> tID;
		tID.push_back(jv["students"][i]["student_no"].asString());
		
		studentsID.push_back(tID);
		//cout << i << endl;
		//time
		vector<string> tTime;
		for(int j = 0;jv["students"][i]["free_time"][j].asBool();j++)
		{
			//string ss = "\"" + jv["students"][i]["free_time"][j].asString() + "\"";
			tTime.push_back(jv["students"][i]["free_time"][j].asString());
		}
		studentsTime.push_back(tTime);

		//tags
		vector<string> tTags;
		for(int j = 0;jv["students"][i]["tags"][j].asBool();j++)
		{
			tTags.push_back(jv["students"][i]["tags"][j].asString());
		}
		studentsTags.push_back(tTags);

		//apps
		vector<string> tApp;
		for (int j = 0; jv["students"][i]["applications_department"][j].asBool(); j++)
		{
			tApp.push_back(jv["students"][i]["applications_department"][j].asString());
		}
		studentsApp.push_back(tApp);
	}
	/*for (int i = 0; i < studentsTime.size(); i++)
	{
		for (int j = 0; j < studentsTime[0].size(); j++)
			cout << studentsTime[i][j] << " ";
		cout << endl;
	}
	for (int j = 0; j < studentsTime[1].size(); j++)
		cout << studentsTime[1][j] << " ";
	for (int j = 0; j < studentsID[1].size(); j++)
		cout << studentsID[1][j] << " ";
	for (int j = 0; j < studentsApp[1].size(); j++)
		cout << studentsApp[1][j] << " ";
	for (int j = 0; j < studentsTags[1].size(); j++)
		cout << studentsTags[1][j] << " ";
    return 0;*/
	//for (int j = 0; j < studentsTime[0].size(); j++)
		//cout << studentsTime[0][j] << " ";
	for (int i = 0; jv["departments"][i].asBool(); i++)
	{
		//ID
		vector<string> tID;
		tID.push_back(jv["departments"][i]["department_no"].asString());
		//cout << jv["departments"][i]["department_no"].asString()<<endl;
		dict_no2index.insert(pair<string, int>(jv["departments"][i]["department_no"].asString(), i));

		//cout << dict_no2index[jv["departments"][i]["department_no"].asString()] << "   "<< i<< endl;
		deptNo.push_back(tID);
		//time
		vector<string> tTime;
		for (int j = 0; jv["departments"][i]["event_schedules"][j].asBool(); j++)
		{
			//string ss = "\"" + jv["departments"][i]["event_schedules"][j].asString() + "\"";
			tTime.push_back(jv["departments"][i]["event_schedules"][j].asString());
		}
		deptEvent.push_back(tTime);

		//tags
		vector<string> tTags;
		for (int j = 0; jv["departments"][i]["tags"][j].asBool(); j++)
		{
			tTags.push_back(jv["departments"][i]["tags"][j].asString());
		}
		deptTags.push_back(tTags);

		//limit
		vector<int> tLimit;
		tLimit.push_back(jv["departments"][i]["member_limit"].asInt());
		deptMumLimit.push_back(tLimit);
	}
	/*for (int j = 0; j < deptNo[1].size(); j++)
		cout << deptNo[1][j] << " ";
	for (int j = 0; j < deptEvent[1].size(); j++)
		cout << deptEvent[1][j] << " ";
	for (int j = 0; j < deptMumLimit[1].size(); j++)
		cout << deptMumLimit[1][j] << " ";
	for (int j = 0; j < deptTags[1].size(); j++)
		cout << deptTags[1][j] << " ";*/
	//for (int j = 0; j < deptEvent[1].size(); j++)
		//cout << deptEvent[1][j] << " ";
	//make dict
	vector<vector<string>> unlucky_stu;
	vector<vector<string>> addmited;
	vector<vector<string>> unlucky_dept;
	
	map<string, set<int>> dict_tags;
	
	for(int i = 0;i < deptTags.size();i++)
	{
		for(int j = 0;j < deptTags[i].size();j++)
		{
			if(dict_tags.count(deptTags[i][j]) == 0)
			{
				set<int> IDs;
				IDs.insert(i);
				dict_tags.insert(pair<string, set<int>>(deptTags[i][j],IDs));
			}
			else
			{
				dict_tags[deptTags[i][j]].insert(i);
			}
		}
	}
	/*set<int> ss = dict_tags["film"];
	set<int>::iterator it;
	for(it = ss.begin();it != ss.end();it++)
	{
		printf("%d", *it);
		cout << endl;
	}*/
	map<string, set<int>> dict_time;  //建造时间段->部门下标的字典
	for(int i = 0;i < deptEvent.size();i++)  //遍历部门
	{
		for(int j = 0;j < deptEvent[i].size();j++)  //遍历部门的时间段
		{
			if(dict_time.count(deptEvent[i][j]) == 0)  //如果时间段没有在这个字典中
			{
				set<int> frees;  //添加字典项
				frees.insert(i);
				dict_time.insert(pair<string, set<int>>(deptEvent[i][j],frees));
			}
			else
			{
				dict_time[deptEvent[i][j]].insert(i);  //如果时间段已存在，就添加到对应的集合中
			}
		}
	}
	/*map<string, set<int>>::iterator it;
	for(it = dict_time.begin();it != dict_time.end();it++)
	{
		cout << "key:" << it->first;
		cout << "value:" << *(it->second.begin()) << endl;
	}*/
	set<int> ss = dict_time["Wed.16:00~18:00"];
	set<int>::iterator it;
	for (it = ss.begin(); it != ss.end(); it++)
	{
		printf("%d", *it);
		cout << endl;
	}

	/*map<string, int>::iterator it;
	for(it = dict_no2index.begin();it !=dict_no2index.end();it++)
	{
		cout << it->first << " " << it->second << endl;
	}*/


	//match
	

	
	for (int i = 0; i < studentsID.size(); i++)
	{
		set<int> apps;
		set<int> apps2;
		for (int j = 0; j < studentsApp[i].size(); j++)
		{
			//cout << studentsApp[i][j] << " ";
			//cout << dict_no2index[studentsApp[i][j]] << " " << endl;
			apps.insert(dict_no2index[studentsApp[i][j]]);
		}
		for (int j = 0; j < studentsTags[i].size(); j++)
		{
			set<int>::iterator it;
			set<int> tags = dict_tags[studentsTags[i][j]];
			cout << "tags ";
			for (it = tags.begin(); it != tags.end(); it++)
			{
				cout << *it << " ";
			}
			cout << endl;
			cout << "apps ";
			for (it = apps.begin(); it != apps.end(); it++)
			{
				cout << *it << " ";
			}
			cout << endl;
			set<int>::iterator it1;
			set<int>::iterator it2;

			for (it1 = apps.begin(); it1 != apps.end(); it1++)
			{
				bool remove = true;;
				for (it2 = tags.begin(); it2 != tags.end(); it2++)
				{
					if (*it1 == *it2)
					{
						remove = false;
						break;
					}
				}
				if (!remove)
				{
					apps2.insert(*it1);
				}
			}
			cout << "apps2 ";
			//set<int>::iterator it;
			for (it = apps2.begin(); it != apps2.end(); it++)
			{
				cout << *it << " ";
			}
			cout << endl;
			set<int> apps3;
			for (int k = 0; k < studentsTime[i].size(); k++) {
				set<int>::iterator it1;
				set<int>::iterator it2;
				cout << "\"" + studentsTime[i][k] + "\"";
				/*if (dict_time.count(studentsTime[i][k]) == 0)
					continue;*/
					/*map<string, set<int>>::iterator x;
					x = dict_time.find(studentsTime[i][k]);
					cout << studentsTime[i][k] << endl;
					if(x != dict_time.end())
					{
						cout << "find!";
						cout << x->first << " " << *(x->second.begin()) << endl;
					}*/

				set<int> event = dict_time[studentsTime[i][k]];
				cout << "event:";
				it = event.begin();
				cout << *it << endl;;
				if (k == 2)break;
				/*for (it = event.begin(); it != event.end(); it++)
				{
					cout << *it << " ";
				}
				cout << endl;*/
				for (it1 = apps2.begin(); it1 != apps2.end(); it1++)
				{
					bool remove = true;
					for (it2 = event.begin(); it2 != event.end(); it2++)
					{
						if (*it1 == *it2)
						{
							remove = false;
							break;
						}
					}
					if (!remove)
					{
						apps3.insert(*it1);
					}
				}
			}
			set<int>::iterator it;
			cout << "apps3";
			for (it = apps3.begin(); it != apps3.end(); it++)
			{
				cout << *it;
				cout << studentsID[i][0] << " matchs " << deptNo[*it][0] << endl;
			}


		}
	}
}

