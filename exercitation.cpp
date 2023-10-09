#include<fstream>
#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <sstream>
#include <vector>
#include <limits>
#include <queue>
using namespace std;
#define OK 1
#define ERROR 0

#ifndef _TIMER_H_
#define _TIMER_H_

#include <windows.h>

class Timer {
private:
	__int64 freq, tStart, tStop;
public:
	Timer() {
		// Get the frequency of the hi-res timer
		QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	} //end-TimerClass

	void Start() {
		// Use hi-res timer
		QueryPerformanceCounter((LARGE_INTEGER*)&tStart);
	} //end-Start

	void Stop() {
		// Perform operations that require timing
		QueryPerformanceCounter((LARGE_INTEGER*)&tStop);
	} //end-Stop

	// Returns time in milliseconds
	double ElapsedTime() {
		// Calculate time difference in milliseconds
		return ((double)(tStop - tStart) / (double)freq) * 1e3;
	} //end-Elapsed
};

#endif

//��һ���ṹ�ͱ���������dijkstra�����оͳ�Ѳ��ʱ�����Ҫ�õĶ���
//����ֻ��һ���֣�ԭ��ͼ
typedef struct InfoPreserver {
	string country;
	string city;
	string cityj;
	string cityw;

	string transport;
	string time;
	string cost;
	string courseinfo;
}PreserverNode;


typedef struct CITIES {
	string country;
	string city;
	string Longitude;//����
	string Latitude;//ά��
	struct CITIES* next = NULL;
	struct CITIES* former = NULL;
}CITIESInfo, * CitiesInfoHead;//������ṹ�ͱ���������ģʽ�����Ҵ�CITIES.csv�ļ��ж������ĳ�����Ϣ

typedef struct ROUTES {
	string routecity1;
	string routecity2;
	string transport;
	string time;//��ʱ����ΪȨֵ
	string cost;
	string courseinfo;
	struct ROUTES* next = NULL;
	struct ROUTES* former = NULL;
}ROUTESInfo, * RoutesInfoHead;//������ṹ�ͱ���������ģʽ�����Ҵ�ROUTES.csv�ļ��ж�������·����Ϣ


//�ҵ������ļ���ÿһ�еĸ����Զ���

void CITIES_FindTheCommasAndCutIntoPieces(string sentence, CITIESInfo& presentCity) {
	string str1 = sentence;//�滻��ɨ���string
	vector<string> vec_str;
	string substr;
	size_t pos = 0;
	string delimiter = ",";

	// ���ַ����ָ�Ϊ���ַ������洢��vector������
	while ((pos = str1.find(delimiter)) != string::npos) {
		substr = str1.substr(0, pos);
		vec_str.push_back(substr);
		str1.erase(0, pos + delimiter.length());
	}
	vec_str.push_back(str1);

	presentCity.country = vec_str[0];
	presentCity.city = vec_str[1];
	presentCity.Longitude = vec_str[2];
	presentCity.Latitude = vec_str[3];

	vec_str.clear();
}

//1��CSV��ʽ�����ļ��Ķ�д
//�������������
void CITIES_ReadCSV(char* CSVFileAddress, CitiesInfoHead& CityHead, CitiesInfoHead& CityTail, int& howmanycities) {
	ifstream CSVReading;
	CITIESInfo* tempcitynode, * present;
	CityHead->next = CityTail;
	CityTail->former = CityHead;
	CityHead->former = NULL;
	CityTail->next = NULL;
	present = CityHead;
	int justfirsttime = 0;
	CSVReading.open(CSVFileAddress);//��CSV
	string EveryLine;
	vector<double> timer1collector;
	while (getline(CSVReading, EveryLine)) {
		howmanycities++;
		tempcitynode = new CITIESInfo;

		Timer t1;
		t1.Start();
		CITIES_FindTheCommasAndCutIntoPieces(EveryLine, *tempcitynode);
		t1.Stop();
		double buffer = t1.ElapsedTime();
		timer1collector.push_back(buffer);
		

		if (justfirsttime == 0)
		{
			CityHead->next = tempcitynode;
			tempcitynode->former = CityHead;
		}
		else {
			present->next = tempcitynode;
			tempcitynode->former = present;
		}
		tempcitynode->next = CityTail;
		CityTail->former = tempcitynode;
		present = tempcitynode;
		justfirsttime++;
	}
	double averaget1=0;
	for (auto calcu : timer1collector) {
		averaget1 += calcu;
	}
	averaget1 /= timer1collector.size();
	double longestt1=0,shortestt1=0;
	longestt1= *max_element(timer1collector.begin(), timer1collector.end());
	shortestt1= *std::min_element(timer1collector.begin(), timer1collector.end());
	printf("             ����CITIES_FindTheCommasAndCutIntoPieces�����ʱ��Time elapsed is : %lf (ms). \n ", longestt1);
	printf("            ����CITIES_FindTheCommasAndCutIntoPieces�������ʱ��Time elapsed is : %lf (ms). \n ", shortestt1);
	printf("            ����CITIES_FindTheCommasAndCutIntoPieces��ƽ����ʱ��Time elapsed is : %lf (ms). \n ", averaget1);
	CSVReading.close();//�ر�CSV
}

//�ҵ�·���ļ���ÿһ�еĸ����Զ��ţ�ע�����·����Ϣ������д������ţ�����Ҫ�ܿ���һ�ζ��Ŵ���Ȼ���ֶ�ɾ�������������������
	void ROUTES_FindTheCommasAndCutIntoPieces(string sentence, ROUTESInfo& presentRoute) {
		string str1 = sentence;//�滻��ɨ���string
		vector<string> vec_str;
		string substr;
		size_t pos = 0;
		string delimiter = ",";
		int onlyfind5commas = 0;
		// ���ַ����ָ�Ϊ���ַ������洢��vector������
		while ((pos = str1.find(delimiter)) != string::npos && onlyfind5commas <= 4) {
			substr = str1.substr(0, pos);
			vec_str.push_back(substr);
			onlyfind5commas++;
			str1.erase(0, pos + delimiter.length());
		}
		//route�ļ���·����Ϣ������6��','������Ҫ�ֶ�ɾ��
		str1.erase(str1.end() - 6, str1.end());
		//ɾ����ϣ���������
		vec_str.push_back(str1);

		presentRoute.routecity1 = vec_str[0];
		presentRoute.routecity2 = vec_str[1];
		presentRoute.transport = vec_str[2];
		presentRoute.time = vec_str[3];
		presentRoute.cost = vec_str[4];
		presentRoute.courseinfo = vec_str[5];

		vec_str.clear();
	}

//1��CSV��ʽ�����ļ��Ķ�д
//������·������
void ROUTES_ReadCSV(char* CSVFileAddress, RoutesInfoHead& RouteHead, RoutesInfoHead& RouteTail, int& howmanyroutes) {
	ifstream CSVReading;
	ROUTESInfo* temproutenode, * present;
	RouteHead->next = RouteTail;
	RouteTail->former = RouteHead;
	RouteHead->former = NULL;
	RouteTail->next = NULL;
	present = RouteHead;
	int justfirsttime = 0;
	CSVReading.open(CSVFileAddress);//��CSV
	string EveryLine;
	vector<double> timer1collector;
	while (getline(CSVReading, EveryLine)) {
		howmanyroutes++;
		temproutenode = new ROUTESInfo;

		Timer t4;
		t4.Start();
		ROUTES_FindTheCommasAndCutIntoPieces(EveryLine, *temproutenode);
		t4.Stop();
		double buffer = t4.ElapsedTime();
		timer1collector.push_back(buffer);
		
		
		if (justfirsttime == 0)
		{
			RouteHead->next = temproutenode;
			temproutenode->former = RouteHead;
		}
		else {
			present->next = temproutenode;
			temproutenode->former = present;
		}
		temproutenode->next = RouteTail;
		RouteTail->former = temproutenode;
		present = temproutenode;
		justfirsttime++;
	}
	double averaget1 = 0;
	for (auto calcu : timer1collector) {
		averaget1 += calcu;
	}
	averaget1 /= timer1collector.size();
	double longestt1 = 0, shortestt1 = 0;
	longestt1 = *max_element(timer1collector.begin(), timer1collector.end());
	shortestt1 = *std::min_element(timer1collector.begin(), timer1collector.end());
	printf("             ����ROUTES_FindTheCommasAndCutIntoPieces�����ʱ��Time elapsed is : %lf (ms). \n ", longestt1);
	printf("            ����ROUTES_FindTheCommasAndCutIntoPieces�������ʱ��Time elapsed is : %lf (ms). \n ", shortestt1);
	printf("            ����ROUTES_FindTheCommasAndCutIntoPieces��ƽ����ʱ��Time elapsed is : %lf (ms). \n ", averaget1);
	CSVReading.close();//�ر�CSV
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///*�ڽӾ����ʾ�������ṹ�������������Լ���ʼ��*/
#define MaxInt 32767
#define ERROR -1

typedef struct {
	string country;
	string city;
	string Longitude;
	string Latitude;
} VerTexType;

typedef struct {
	string transport;
	string weight;
	string notthatimportant;
	string courseinfo;
} ArcType;

typedef int Status;

typedef struct ArcNode {
	int adjvex; // �ڽӶ����±�
	ArcType info; // �ߵ���Ϣ
	string otherside;
	struct ArcNode* next; // ָ����һ���ߵ�ָ��
} ArcNode;

typedef struct VexNode {
	VerTexType data; // ������Ϣ
	ArcNode* first; // ָ���һ�������ö���ıߵ�ָ��
} VexNode;

typedef struct {
	vector<VexNode> vertices; // ��������
	int vexnum, arcnum; // ��ǰ�������ͱ���
} ALGraph;

int LocateVex(ALGraph G, string vname) {
	for (int findit = 0; findit < G.vexnum; findit++) {
		if (G.vertices[findit].data.city == vname) {
			return findit;
		}
	}
	return ERROR;
}



Status CreateUDN(ALGraph& G, CitiesInfoHead CityHead, int howmanycities, RoutesInfoHead RouteHead, int howmanyroutes, int yourpreference,int whetherdirection) {
	G.vexnum = howmanycities;
	G.arcnum = howmanyroutes;

	// ��ʼ������
	CitiesInfoHead replacenow = CityHead;
	int inputflag_vex = 0;

	while (replacenow = replacenow->next) {
		VexNode vexNode;
		vexNode.data.city = replacenow->city;
		vexNode.data.country = replacenow->country;
		vexNode.data.Longitude = replacenow->Longitude;
		vexNode.data.Latitude = replacenow->Latitude;
		vexNode.first = nullptr;
		G.vertices.push_back(vexNode);
		inputflag_vex++;
	}

	// ��ʼ���ڽӱ�
	RoutesInfoHead replacenow2 = RouteHead;
	int inputarcflag_value = 0;
	int guard = 1;
	vector<double>L1collector;
	vector<double>L2collector;
	switch (whetherdirection) {
	case 1://����ͼ
		while (replacenow2 = replacenow2->next) {
			if (guard > G.arcnum)
				break;
			string v1 = replacenow2->routecity1;
			string v2 = replacenow2->routecity2;

			Timer t5;
			t5.Start();
			int i = LocateVex(G, v1);
			t5.Stop();
			double bowl1 = t5.ElapsedTime();
			L1collector.push_back(bowl1);
			
			
			Timer t6;
			t6.Start();
			int j = LocateVex(G, v2);
			t6.Stop();
			double bowl2 = t6.ElapsedTime();
			L2collector.push_back(bowl2);

			ArcNode* arcNode2 = new ArcNode;
			arcNode2->adjvex = i;
			arcNode2->info.weight = MaxInt;
			arcNode2->otherside = v1;
			arcNode2->next = G.vertices[j].first;
			G.vertices[j].first = arcNode2;

			ArcNode* arcNode1 = new ArcNode;
			arcNode1->adjvex = j;
			arcNode1->info.weight = MaxInt;
			arcNode1->otherside = v2;
			arcNode1->next = G.vertices[i].first;
			G.vertices[i].first = arcNode1;



			switch (yourpreference) {
			case 1: // ʱ������
				arcNode1->info.weight = replacenow2->time;////////////////////////////////////////////////////////����֮������Ͳ�����stod��
				arcNode1->info.notthatimportant = replacenow2->cost;
				arcNode2->info.weight = replacenow2->time;
				arcNode2->info.notthatimportant = replacenow2->cost;
				break;
			case 2: // ��������
				arcNode1->info.weight = replacenow2->cost;
				arcNode1->info.notthatimportant = replacenow2->time;
				arcNode2->info.weight = replacenow2->cost;
				arcNode2->info.notthatimportant = replacenow2->time;
				break;
			}//�����ԣ�������������,�����ֶ�����������ı߸���ͬ����ֵ

			arcNode1->info.transport = replacenow2->transport;
			arcNode1->info.courseinfo = replacenow2->courseinfo;
			arcNode2->info.transport = replacenow2->transport;
			arcNode2->info.courseinfo = replacenow2->courseinfo;

			//ͬ���ģ�����������ı߸���ͬ����ֵ



			inputarcflag_value++;
			guard++;//���guard����Ϊ���ϵģ���Ҫ��Ϊ�˽��replacenow2 = replacenow2->next��ָ��һ���յģ�û���õ�CityTail
		}
		break;
	case 2://����ͼ
		while (replacenow2 = replacenow2->next) {
			if (guard > G.arcnum)
				break;
			string v1 = replacenow2->routecity1;
			string v2 = replacenow2->routecity2;

			Timer t5;
			t5.Start();
			int i = LocateVex(G, v1);
			t5.Stop();
			double bowl1 = t5.ElapsedTime();
			L1collector.push_back(bowl1);


			Timer t6;
			t6.Start();
			int j = LocateVex(G, v2);
			t6.Stop();
			double bowl2 = t6.ElapsedTime();
			L2collector.push_back(bowl2);

			ArcNode* arcNode1 = new ArcNode;
			arcNode1->adjvex = j;//��otherside��һ������
			arcNode1->info.weight = MaxInt;
			arcNode1->otherside = v2;
			arcNode1->next = G.vertices[i].first;
			G.vertices[i].first = arcNode1;



			switch (yourpreference) {
			case 1: // ʱ������
				arcNode1->info.weight = replacenow2->time;////////////////////////////////////////////////////////����֮������Ͳ�����stod��
				arcNode1->info.notthatimportant = replacenow2->cost;
				
				break;
			case 2: // ��������
				arcNode1->info.weight = replacenow2->cost;
				arcNode1->info.notthatimportant = replacenow2->time;
				
				break;
			}//�����ԣ�������������,�����ֶ�����������ı߸���ͬ����ֵ

			arcNode1->info.transport = replacenow2->transport;
			arcNode1->info.courseinfo = replacenow2->courseinfo;
			

			//ͬ���ģ�����������ı߸���ͬ����ֵ



			inputarcflag_value++;
			guard++;//���guard����Ϊ���ϵģ���Ҫ��Ϊ�˽��replacenow2 = replacenow2->next��ָ��һ���յģ�û���õ�CityTail
		}
		break;
	}

	double averaget1 = 0;
	for (auto calcu : L1collector) {
		averaget1 += calcu;
	}
	averaget1 /= L1collector.size();
	double longestt1 = 0, shortestt1 = 0;
	longestt1 = *max_element(L1collector.begin(), L1collector.end());
	shortestt1 = *std::min_element(L1collector.begin(), L1collector.end());
	printf("             ����LocateVex(G, v1)�����ʱ��Time elapsed is : %lf (ms). \n ", longestt1);
	printf("            ����LocateVex(G, v1)�������ʱ��Time elapsed is : %lf (ms). \n ", shortestt1);
	printf("            ����LocateVex(G, v1)��ƽ����ʱ��Time elapsed is : %lf (ms). \n ", averaget1);
	cout << endl << endl;

	double averaget2 = 0;
	for (auto calcu : L2collector) {
		averaget2 += calcu;
	}
	averaget2 /= L2collector.size();
	double longestt2 = 0, shortestt2 = 0;
	longestt2 = *max_element(L2collector.begin(), L2collector.end());
	shortestt2 = *std::min_element(L2collector.begin(), L2collector.end());
	printf("             ����LocateVex(G, v2)�����ʱ��Time elapsed is : %lf (ms). \n ", longestt2);
	printf("            ����LocateVex(G, v2)�������ʱ��Time elapsed is : %lf (ms). \n ", shortestt2);
	printf("            ����LocateVex(G, v2)��ƽ����ʱ��Time elapsed is : %lf (ms). \n ", averaget2);
	cout << endl << endl;



	return OK;
}


void addBackslash(string& str) {
	size_t pos = 0;
	while ((pos = str.find('\'', pos)) != std::string::npos) {
		str.insert(pos, "\\");
		pos += 2;
	}
}


int Dijkstra(const ALGraph& graph, const string& startCity, const string& endCity, int preference, vector<PreserverNode>& kitchen,string &s1, string& s2, string& s3) {
	int vertexCount = graph.vexnum;
	int arcsCount = graph.arcnum;
	vector<double> dist(vertexCount, MaxInt); // �洢��ʼ�㵽���������̾���
	vector<bool> visited(vertexCount, false); // ��¼�����Ƿ��ѱ�����
	vector<int> path(vertexCount, -1); // �洢���·����ǰ������
	PreserverNode plates;

	int startIndex = -1; // ��ʼ���е�����
	int endIndex = -1; // �������е�����
	/*------------------------------------------------------------�����Ǿ������顢�жϷ��������·��������ĳ�ʼ��--------------------------------------------------------------------------*/

	// �ҵ���ʼ���кͽ������е�����
	for (int i = 0; i < vertexCount; ++i) {
		if (graph.vertices[i].data.city == startCity)
			startIndex = i;
		else if (graph.vertices[i].data.city == endCity)
			endIndex = i;

		if (startIndex != -1 && endIndex != -1)
			break;
	}

	if (startIndex == -1 || endIndex == -1) {
		return ERROR;
	}

	dist[startIndex] = 0; // ��ʼ�㵽����ľ���Ϊ0

	///////////////////////////////////////////////////////////////////
	if (startIndex<0 || startIndex>vertexCount - 1) {
		cout << "            findyou!" << "  visited[startIndex] = true;//����Ѿ����Ķ�   " << startIndex << endl<<endl;
	}
	///////////////////////////////////////////////////////////////////
	visited[startIndex] = true;//����Ѿ����Ķ�


	/*------------------------------------------------------------������Ŀ����еļ�����λ--------------------------------------------------------------------------*/


	// ��������ʼ�����ڶ������̾���
	ArcNode* arcNode = graph.vertices[startIndex].first;
	while (arcNode != nullptr) {
		int neighborIndex1 = arcNode->adjvex;
		string weight1 = arcNode->info.weight;
		if(dist[neighborIndex1]> stod(weight1))//�����Ҷ�����Ľ������
		dist[neighborIndex1] = stod(weight1);//ע�⣬Ϊʲô������·����ͷ�Ҿͻ�ʧȥ��ȷ�жϵ���������Ϊ������ڸ��ǣ�����ڴ��ڸ��ǵ������ȡ��Ȩֵ�����·����
		path[neighborIndex1] = startIndex;
		arcNode = arcNode->next;
	}


	/*------------------------------------------------------------�����ǽ����ڽӱ���е�dist�������������path�Ļ��������۾���˵����ľ���һ��ģ���ڽӾ����Dijkstra--------------------------------------------------------------------------*/
	//������������ѭ����ÿ�������ʼ�㵽ĳ����������·��

	for (int i = 1; i < vertexCount; ++i) {
		double minDist = MaxInt;
		int minIndex = -1; // ���ڱ���ҵ��ĵ���±�

		// ѡ��δ���ʵĶ����о�����С�Ķ���
		for (int j = 0; j < vertexCount; ++j) {
			///////////////////////////////////////////////////////////////////
			if (j<0 || j>vertexCount - 1) {
				cout << "            findyou!" << "  if (!visited[j] && dist[j] < minDist) {					minDist = dist[j];				minIndex = j; //���������j��			}   " << j << endl<<endl;
			}
			///////////////////////////////////////////////////////////////////
			if (!visited[j] && dist[j] < minDist) {
				minDist = dist[j];
				minIndex = j; //���������j��
			}
		}

		///////////////////////////////////////////////////////////////////
		if (minIndex<0 || minIndex>vertexCount - 1) {
			//cout << "            findyou!" << "  visited[minIndex] = true; // ��Ǹö���Ϊ�ѷ���   " << minIndex << endl<<endl;
			break;
		}
		///////////////////////////////////////////////////////////////////
		visited[minIndex] = true; // ��Ǹö���Ϊ�ѷ���


		// �����뵱ǰ�������ڶ������̾���
		ArcNode* arcNodebowl = graph.vertices[minIndex].first;
		while (arcNodebowl != nullptr) {
			int neighborIndex2 = arcNodebowl->adjvex;
			string weight2 = arcNodebowl->info.weight;
	/*		cout << minIndex<<endl;*/
			// ���ͨ����ǰ������Ի�ø��̵ľ��룬�������̾����·��
				///////////////////////////////////////////////////////////////////
			if (neighborIndex2<0 || neighborIndex2>vertexCount - 1) {
				cout << "            findyou!" << "  if (!visited[neighborIndex2] && dist[minIndex] + weight2 < dist[neighborIndex2])   " << neighborIndex2 <<endl<< endl;
			}
			///////////////////////////////////////////////////////////////////
			if (!visited[neighborIndex2] && dist[minIndex] + stod(weight2) < dist[neighborIndex2]) {/////////////////////////////////////////////////////////////////////////////////��stodʹ�õĵ���д�뱨��
				dist[neighborIndex2] = dist[minIndex] + stod(weight2);
				path[neighborIndex2] = minIndex;
				/*if (neighborIndex2 == 57 || neighborIndex2 == 189 || neighborIndex2 == 142 || neighborIndex2 == 56 || neighborIndex2 == 86) {
					cout << neighborIndex2<<"   "<< arcNodebowl->info.weight << "   " << arcNodebowl->info.notthatimportant << "   " << endl;
				}*///����cout��Ϊ�����һ��ͼ�����ӹ�ϵ�����һ�½ڵ�����ôŪ�����ģ���ǰ�ڵ�����Ϸ����Ǹ���minIndex��
			}

			arcNodebowl = arcNodebowl->next;//��Ϊ���ﲻ���ں�֮ǰ��ͼ��ʱ�����������һ����head��һ����tail����ѰҪһ��guard����������tail����ȥ��
			//�����ڽӱ�û�˾���û�ˣ�û�п�tail�����һ������next����NULL
		}

		if (minIndex == -1)
			break; // ���ж��㶼�ѷ��ʣ��㷨����
	}


	//������㷨��

	if (dist[endIndex] == MaxInt) {//�����㵽�յ�ľ���û�з����κθı�
		//����Ƿ���ǰ���Ǹ���Ϊ-1�غϣ���������ʱ��������ͼ����Ϊ-1��������û·
		cout << "            û���κ�һ����ͨ�������·����������������ԭ��" << endl << endl;
		return ERROR;
	}//��˵��û���������Ե����·

	// �������·��
	vector<int> shortestPath;
	int currIndex = endIndex;
	while (currIndex != startIndex) {
		shortestPath.push_back(currIndex);
		/////////////////////////////////////////////////
		if (currIndex<0 || currIndex>graph.vexnum - 1) {
			cout << "gatcha!!!!!!!!!!";
		}
		////////////////////////////////////////////////////
		currIndex = path[currIndex];
	}
	shortestPath.push_back(startIndex);

	// ��ת·��˳��
	reverse(shortestPath.begin(), shortestPath.end());

	// ������·��
	switch (preference)
	{
	case 1:
		cout << "            �� " << startCity << " �� " << endCity << " ��ʱ��̵�·��:" << endl << endl;
		s1 = "            �� " + startCity + " �� " + endCity + " �����·��:";
		cout << "            ";
		break;
	case 2:
		cout << "            �� " << startCity << " �� " << endCity << " �������ٵ�·��:" << endl << endl;
		s1 = "            �� " + startCity + " �� " + endCity + " �����·��:";
		cout << "            ";
		break;
	default:
		break;
	}
	

	vector<string> collector;
	for (int i = 0; i < shortestPath.size(); ++i) {

		int index = shortestPath[i];
		cout << graph.vertices[index].data.city;
		collector.push_back(graph.vertices[index].data.city);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		plates.city = graph.vertices[index].data.city;//���ǵ�ǰ��㱾��
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (i != shortestPath.size() - 1) {//���²�������ִ�������һ��i
			//���������ҵ���ǰ�ڵ�����һ�����֮���arc
			// ���ҽ�����ֵ�������ǵ�kitchen
			///////////////////////////////////////////////////////////////////////////////////////////////////////////// 
			ArcNode* pinarc = graph.vertices[index].first;
			while (pinarc) {
				//if (pinarc->otherside == graph.vertices[shortestPath[i + 1]].data.city && stoi(pinarc->info.weight) == (int)(dist[shortestPath[i + 1]] - dist[index])) {
				//	break;
				//}
				if (pinarc->otherside == graph.vertices[shortestPath[i + 1]].data.city && (stoi(pinarc->info.weight) -(int)(dist[shortestPath[i + 1]] - dist[index]))<=1) {
					break;
				}
				pinarc = pinarc->next;
			}//�ҵ�����һ����������arc
			///////////////////////////////////////////////////////////////////////////////////////////////////
			plates.transport = pinarc->info.transport;
			plates.courseinfo = pinarc->info.courseinfo;
			switch (preference)
			{
			case 1:
				plates.time = pinarc->info.weight;
				plates.cost = pinarc->info.notthatimportant;
				break;
			case 2:
				plates.cost = pinarc->info.weight;
				plates.time = pinarc->info.notthatimportant;
				break;
			default:
				break;
			}
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////
			plates.cityj = graph.vertices[index].data.Longitude;
			plates.cityw = graph.vertices[index].data.Latitude;
			plates.country = graph.vertices[index].data.country;

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
		else {
			plates.cityj = graph.vertices[index].data.Longitude;
			plates.cityw = graph.vertices[index].data.Latitude;
			plates.country = graph.vertices[index].data.country;
		}
		kitchen.push_back(plates);

		if (i != shortestPath.size() - 1) {
			cout << " -> ";
			collector.push_back(" -> ");
		}
	}

	
	cout << endl << endl;

	for (int i = 0; i < collector.size(); i++) {
		s2 += collector[i];
	}


	// �����̾���
	switch (preference) {
	case 1:
		cout << "            ���ʱ��: " << dist[endIndex] << "Сʱ" << endl << endl;
		s3 = "            ���ʱ��: " + to_string(dist[endIndex]) + "Сʱ";
		break;
	case 2:
		cout << "            ��������: US$ " << dist[endIndex] << endl << endl;
		s3 = "            ��������: US$ " + to_string(dist[endIndex]);
		break;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*���htm�ļ�*/          //��ʧ�ܣ�һ�����д�����������
//���ƣ�����һ��ѭ��
void CreathtmMap(char* htmFileAddress, vector<PreserverNode> kitchen, string s1, string s2, string s3) {


	int i = 0, j = 0;
	int size = kitchen.size();

	ofstream drawmaphtm(htmFileAddress);


	char head[2000];
	sprintf_s(head, 2000, "<!DOCTYPE html><html><head><style type='text/css'>body, html{width: 100%%;height: 100%%;margin:0;font-family:'΢���ź�';}#allmap{height:100%%;width:100%%;}#r-result{width:100%%;}<style type=\"text/css\">#print - message{position: fixed;top: 50%%;left: 50%%;transform: translate(-50%%, -50%%);font - size: 18px;text - align: center;z - index: 999;}</style></style><script type='text/javascript' src='http://api.map.baidu.com/api?v=2.0&ak=nSxiPohfziUaCuONe4ViUP2N'></script><title>Shortest path from %s to %s</title></head><body><div id=\"print-message\">%s<br>%s<br>%s</div><div id='allmap'></div></div></body></html><script type='text/javascript'>var map = new BMap.Map('allmap');var point = new BMap.Point(0, 0);map.centerAndZoom(point, 2);map.enableScrollWheelZoom(true);", kitchen[0].city.c_str(), kitchen[size - 1].city.c_str(),s1.c_str(),s2.c_str(),s3.c_str());
	drawmaphtm << head;

	for (; i < size; i++) {
		char dotinfo[400];
		sprintf_s(dotinfo, 400, "var marker%d = new BMap.Marker(new BMap.Point(%s, %s)); map.addOverlay(marker%d);\nvar infoWindow%d = new BMap.InfoWindow(\"<p style = 'font-size:14px;'>country: %s<br/>city: %s</p>\"); marker%d.addEventListener(\"click\", function(){this.openInfoWindow(infoWindow%d);}); ", i, kitchen[i].cityw.c_str(), kitchen[i].cityj.c_str(), i, i, kitchen[i].country.c_str(), kitchen[i].city.c_str(), i, i);
		drawmaphtm << '\n' << dotinfo;
	}

	
	for (; j < size - 1; j++) {
		char linecontent[5000];
		addBackslash(kitchen[j].courseinfo);
		sprintf_s(linecontent, 5000, "var contentString%d = '%s, %s --> %s, %s (%s - %s hours - $%s - %s)';\nvar path%d = new BMap.Polyline([new BMap.Point(%s, %s), new BMap.Point(%s,%s)], { strokeColor:'#18a45b', strokeWeight : 8, strokeOpacity : 0.8 }); map.addOverlay(path%d); path%d.addEventListener(\"click\", function(){alert(contentString%d);}); ", j, kitchen[j].city.c_str(), kitchen[j].country.c_str(), kitchen[j + 1].city.c_str(), kitchen[j + 1].country.c_str(), kitchen[j].transport.c_str(), kitchen[j].time.c_str(), kitchen[j].cost.c_str(), kitchen[j].courseinfo.c_str(), j, kitchen[j].cityw.c_str(), kitchen[j].cityj.c_str(), kitchen[j + 1].cityw.c_str(), kitchen[j + 1].cityj.c_str(), j, j, j);
		drawmaphtm << '\n' << linecontent;
	}
	drawmaphtm << "</script>";//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!���ԶϺ�·
	drawmaphtm.close();
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// ������ȱ����㷨
void BFSTraverse(const ALGraph& graph) {
	vector<bool> visited(graph.vexnum, false); // ��Ƕ����Ƿ��ѷ���
	queue<int> queue; // �洢�����ʵĶ���

	for (int i = 0; i < graph.vexnum; i++) {
		if (!visited[i]) {
			visited[i] = true;
			if (i != 0)
				cout << " -> " << graph.vertices[i].data.city; // ���������Ϣ���ӵڶ������㿪ʼ��Ӽ�ͷ���ţ�
			else
				cout << graph.vertices[i].data.city; // ���������Ϣ����һ������������Ӽ�ͷ���ţ�
			queue.push(i);

			while (!queue.empty()) {
				int vertex = queue.front();
				queue.pop();

				ArcNode* arcNode = graph.vertices[vertex].first;
				while (arcNode != nullptr) {
					int adjVertex = arcNode->adjvex;
					if (!visited[adjVertex]) {
						visited[adjVertex] = true;
						cout << " -> " << graph.vertices[adjVertex].data.city; // ���������Ϣ����Ӽ�ͷ���ţ�
						queue.push(adjVertex);
					}
					arcNode = arcNode->next;
				}
			}
		}
	}
}




int main() {

	int howmanycities = 0, howmanyroutes = 0;//���ź�����

	/*------------------------------------------------------------�������ļ�����--------------------------------------------------------------------------*/
	char CSVCities[] = "C:\\Users\\Nighfearti\\Desktop\\cities.csv";
	CitiesInfoHead CityHead, CityTail;
	CityHead = new CITIESInfo;
	CityTail = new CITIESInfo;

	Timer t2;
	t2.Start();
	CITIES_ReadCSV(CSVCities, CityHead, CityTail, howmanycities);
	t2.Stop();
	printf("            ����CITIES_ReadCSV����ʱ����Time elapsed is : %lf (ms). \n ", t2.ElapsedTime());
	cout << endl << endl;
	
	
	CitiesInfoHead PrintChecker1 = CityHead;
	//while ((PrintChecker1 = PrintChecker1->next) && PrintChecker1 != CityTail) {
	//	cout << PrintChecker1->country << "   " << PrintChecker1->city << "   " << PrintChecker1->Longitude << "   " << PrintChecker1->Latitude << endl;
	//}
	//���ɹ���ע�͵�

	/*------------------------------------------------------------��·���ļ�����--------------------------------------------------------------------------*/
	char CSVRoutes[] = "C:\\Users\\Nighfearti\\Desktop\\routes.csv";
	RoutesInfoHead RouteHead, RouteTail;
	RouteHead = new ROUTESInfo;
	RouteTail = new ROUTESInfo;

	Timer t3;
	t3.Start();
	ROUTES_ReadCSV(CSVRoutes, RouteHead, RouteTail, howmanyroutes);
	t3.Stop();
	printf("            ����ROUTES_ReadCSV����ʱ����Time elapsed is : %lf (ms). \n ", t3.ElapsedTime());
	cout << endl<<endl;
	
	
	RoutesInfoHead PrintChecker2 = RouteHead;
	//while ((PrintChecker2 = PrintChecker2->next) && PrintChecker2 != RouteTail) {
	//	cout <<what<<"   " << PrintChecker2->routecity1 << "   " << PrintChecker2->routecity2 << "   " << PrintChecker2->transport << "   " << PrintChecker2->time << "   " << PrintChecker2->cost << "   " << PrintChecker2->courseinfo << endl << endl;
	//}
	//���ɹ���ע�͵�

	cout << endl << endl << "            " << howmanycities << "�����нڵ�    " << howmanyroutes << "��·����Ϣ   " << endl << endl;

	/*------------------------------------------------------------��ʼ����ͼ������--------------------------------------------------------------------------*/
	int yourpreference,whetherdirection;
	ALGraph G;
	cout << endl << "            ����ͼ��������ͼ��" << endl << endl << "            ����ͼ  :  ������1" << endl << endl << "            ����ͼ  :  ������2" << endl  << endl;
	redirect:cin >> whetherdirection;
	if (whetherdirection != 1 && whetherdirection != 2) {
		cout << "            ���������������������룡" << endl;
		std::cin.clear();
		std::cin.ignore(10000, '\n');  // �滻Ϊ�ʵ�������ֵ//����������������ַ��ĺ�������//��һ��������ʾҪ���Ե��ַ��������ڶ���������ʾҪ���Ե���ֹ�ַ�//cin������С��1000�����õ���
		goto redirect;
	}
	cout << endl << "            ��ѡ��ʱ��������Ȼ��ǻ����������ȣ�" << endl << endl << "            �����Ҫʱ�����      ������1" << endl << endl << "            ��Ҫ��������          ������2" << endl  << endl;
	reinput:cin >> yourpreference;
	if (yourpreference != 1 && yourpreference != 2) {
		cout << "            ���������������������룡"<<endl;
		std::cin.clear();
		std::cin.ignore(10000, '\n');  // �滻Ϊ�ʵ�������ֵ//����������������ַ��ĺ�������//��һ��������ʾҪ���Ե��ַ��������ڶ���������ʾҪ���Ե���ֹ�ַ�//cin������С��1000�����õ���
		goto reinput;
	}
	

	Timer t7;
	t7.Start();
	CreateUDN(G, CityHead, howmanycities, RouteHead, howmanyroutes, yourpreference,whetherdirection);
	t7.Stop();
	printf("             ����CreateUDN����ʱ����Time elapsed is : %lf (ms). \n ", t7.ElapsedTime());
	cout << endl<<endl;
	
	/*------------------------------------------------------------��ͼ��ϣ���ʼ����--------------------------------------------------------------------------*/

	// ���ù�����ȱ����㷨
	BFSTraverse(G);

	/*------------------------------------------------------------������ϣ���ʼDjkstra�㷨--------------------------------------------------------------------------*/
	/*Djkstra�㷨*/
	again : string startCity;
	cout << "            ���������" << endl;
	cin >> ws; // ������뻺�����еĻ��з�
	getline(cin, startCity);//��������������Ϊ��֮ǰ����������У�cin ������һ�����з������뻺�����У��� getline ���������з�ʱ����Ϊ�����������˻��������롣�������������
	string endCity;
	cout << "            �������յ�" << endl;
	cin >> ws; // ������뻺�����еĻ��з�
	getline(cin, endCity);

	vector<PreserverNode> kitchen;
	//����Ϊʲô��Ҫ��������鹹������
	//�����dijkstra�㷨��ֻ�����ڵ�������ô�ҽ����������������һ��LocateVex�������ҽڵ�indexȻ���ٽ���index��ԭ����
	//�����������㲻��
	//1.�ܹ�һ������õ�����ΪʲôҪ�������أ���Ȼ��������ʱ�临�Ӷȣ���������ʱ��
	//2.������д��LocateVex������������Ҫ����ͼʱΪ���ҽڵ㲢�ҽ�������ϵ��������������һ������Ҿܾ���

	string str1, str2, str3;

	
 
	Timer t;
	t.Start();
	int a = Dijkstra(G, startCity, endCity, yourpreference, kitchen, str1, str2, str3);
	t.Stop();
	printf("            ����Dijkstra��Time elapsed is : %lf (ms). \n ", t.ElapsedTime());
	if (a == -1) {
		cout << "            û���κ�һ����ͨ�������·����������������ͨ��" << endl << endl;
		goto again;
	}
	else {

		int size = kitchen.size();
		//cout << size << endl;
	/*	for (int i = 0; i < size; i++) {
			cout << kitchen[i].city << endl;
			cout << kitchen[i].county << endl;
			cout << kitchen[i].cityj << endl;
			cout << kitchen[i].cityw << endl;
			cout << kitchen[i].cost << endl;
			cout << kitchen[i].time << endl;
			cout << kitchen[i].transport << endl;
			cout << kitchen[i].courseinfo << endl << endl << endl;
		}*/

		

		/*------------------------------------------------------------��Ү����ʼ�����--------------------------------------------------------------------------*/
		/*CreathtmMap();*/
		char mapfilename[] = "C:\\Users\\Nighfearti\\Desktop\\newmap.htm";
		CreathtmMap(mapfilename, kitchen, str1, str2, str3);
	}





	/*------------------------------------------------------------�������htm�ļ�--------------------------------------------------------------------------*/

	//�ƺ�ɾ������Ŀռ�
	CitiesInfoHead Deletehelper1, Deletehelper2;
	Deletehelper1 = CityHead;
	Deletehelper2 = CityHead->next;
	while (Deletehelper2 != CityTail && Deletehelper2 && Deletehelper1) {
		delete Deletehelper1;
		Deletehelper2 = Deletehelper2->next;
		Deletehelper1 = Deletehelper2->former;
	}
	delete Deletehelper2;

	RoutesInfoHead Deletehelper3, Deletehelper4;
	Deletehelper3 = RouteHead;
	Deletehelper4 = RouteHead->next;
	while (Deletehelper4 != RouteTail && Deletehelper4 && Deletehelper3) {
		delete Deletehelper3;
		Deletehelper4 = Deletehelper4->next;
		Deletehelper3 = Deletehelper4->former;
	}
	delete Deletehelper4;



	return 666;

}