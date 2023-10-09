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

//整一个结构型变量用于在dijkstra过程中就出巡到时候输出要用的东西
//但是只存一部分，原因看图
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
	string Longitude;//经度
	string Latitude;//维度
	struct CITIES* next = NULL;
	struct CITIES* former = NULL;
}CITIESInfo, * CitiesInfoHead;//用这个结构型变量的数组模式储存我从CITIES.csv文件中读出来的城市信息

typedef struct ROUTES {
	string routecity1;
	string routecity2;
	string transport;
	string time;//将时间作为权值
	string cost;
	string courseinfo;
	struct ROUTES* next = NULL;
	struct ROUTES* former = NULL;
}ROUTESInfo, * RoutesInfoHead;//用这个结构型变量的数组模式储存我从ROUTES.csv文件中读出来的路径信息


//找到城市文件中每一行的隔开性逗号

void CITIES_FindTheCommasAndCutIntoPieces(string sentence, CITIESInfo& presentCity) {
	string str1 = sentence;//替换成扫描的string
	vector<string> vec_str;
	string substr;
	size_t pos = 0;
	string delimiter = ",";

	// 将字符串分隔为子字符串，存储到vector容器中
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

//1）CSV格式数据文件的读写
//并存入城市数组
void CITIES_ReadCSV(char* CSVFileAddress, CitiesInfoHead& CityHead, CitiesInfoHead& CityTail, int& howmanycities) {
	ifstream CSVReading;
	CITIESInfo* tempcitynode, * present;
	CityHead->next = CityTail;
	CityTail->former = CityHead;
	CityHead->former = NULL;
	CityTail->next = NULL;
	present = CityHead;
	int justfirsttime = 0;
	CSVReading.open(CSVFileAddress);//打开CSV
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
	printf("             函数CITIES_FindTheCommasAndCutIntoPieces：最长用时：Time elapsed is : %lf (ms). \n ", longestt1);
	printf("            函数CITIES_FindTheCommasAndCutIntoPieces：最短用时：Time elapsed is : %lf (ms). \n ", shortestt1);
	printf("            函数CITIES_FindTheCommasAndCutIntoPieces：平均用时：Time elapsed is : %lf (ms). \n ", averaget1);
	CSVReading.close();//关闭CSV
}

//找到路径文件中每一行的隔开性逗号，注意的是路径信息里面会有大量逗号，所以要避开这一次逗号处理，然后手动删除最后多出来的留个逗号
	void ROUTES_FindTheCommasAndCutIntoPieces(string sentence, ROUTESInfo& presentRoute) {
		string str1 = sentence;//替换成扫描的string
		vector<string> vec_str;
		string substr;
		size_t pos = 0;
		string delimiter = ",";
		int onlyfind5commas = 0;
		// 将字符串分隔为子字符串，存储到vector容器中
		while ((pos = str1.find(delimiter)) != string::npos && onlyfind5commas <= 4) {
			substr = str1.substr(0, pos);
			vec_str.push_back(substr);
			onlyfind5commas++;
			str1.erase(0, pos + delimiter.length());
		}
		//route文件中路径信息最后会多跟6个','，我需要手动删除
		str1.erase(str1.end() - 6, str1.end());
		//删除完毕，存入数组
		vec_str.push_back(str1);

		presentRoute.routecity1 = vec_str[0];
		presentRoute.routecity2 = vec_str[1];
		presentRoute.transport = vec_str[2];
		presentRoute.time = vec_str[3];
		presentRoute.cost = vec_str[4];
		presentRoute.courseinfo = vec_str[5];

		vec_str.clear();
	}

//1）CSV格式数据文件的读写
//并存入路径数组
void ROUTES_ReadCSV(char* CSVFileAddress, RoutesInfoHead& RouteHead, RoutesInfoHead& RouteTail, int& howmanyroutes) {
	ifstream CSVReading;
	ROUTESInfo* temproutenode, * present;
	RouteHead->next = RouteTail;
	RouteTail->former = RouteHead;
	RouteHead->former = NULL;
	RouteTail->next = NULL;
	present = RouteHead;
	int justfirsttime = 0;
	CSVReading.open(CSVFileAddress);//打开CSV
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
	printf("             函数ROUTES_FindTheCommasAndCutIntoPieces：最长用时：Time elapsed is : %lf (ms). \n ", longestt1);
	printf("            函数ROUTES_FindTheCommasAndCutIntoPieces：最短用时：Time elapsed is : %lf (ms). \n ", shortestt1);
	printf("            函数ROUTES_FindTheCommasAndCutIntoPieces：平均用时：Time elapsed is : %lf (ms). \n ", averaget1);
	CSVReading.close();//关闭CSV
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///*邻接矩阵表示法声明结构、构造无向网以及初始化*/
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
	int adjvex; // 邻接顶点下标
	ArcType info; // 边的信息
	string otherside;
	struct ArcNode* next; // 指向下一条边的指针
} ArcNode;

typedef struct VexNode {
	VerTexType data; // 顶点信息
	ArcNode* first; // 指向第一条依附该顶点的边的指针
} VexNode;

typedef struct {
	vector<VexNode> vertices; // 顶点数组
	int vexnum, arcnum; // 当前顶点数和边数
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

	// 初始化顶点
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

	// 初始化邻接表
	RoutesInfoHead replacenow2 = RouteHead;
	int inputarcflag_value = 0;
	int guard = 1;
	vector<double>L1collector;
	vector<double>L2collector;
	switch (whetherdirection) {
	case 1://无向图
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
			case 1: // 时间优先
				arcNode1->info.weight = replacenow2->time;////////////////////////////////////////////////////////改了之后这里就不能用stod了
				arcNode1->info.notthatimportant = replacenow2->cost;
				arcNode2->info.weight = replacenow2->time;
				arcNode2->info.notthatimportant = replacenow2->cost;
				break;
			case 2: // 费用优先
				arcNode1->info.weight = replacenow2->cost;
				arcNode1->info.notthatimportant = replacenow2->time;
				arcNode2->info.weight = replacenow2->cost;
				arcNode2->info.notthatimportant = replacenow2->time;
				break;
			}//很明显，这里是无向网,所以手动将两条方向的边附上同样的值

			arcNode1->info.transport = replacenow2->transport;
			arcNode1->info.courseinfo = replacenow2->courseinfo;
			arcNode2->info.transport = replacenow2->transport;
			arcNode2->info.courseinfo = replacenow2->courseinfo;

			//同样的，将两条方向的边附上同样的值



			inputarcflag_value++;
			guard++;//这个guard是人为加上的，主要是为了解决replacenow2 = replacenow2->next会指向一个空的，没有用的CityTail
		}
		break;
	case 2://有向图
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
			arcNode1->adjvex = j;//和otherside是一个东西
			arcNode1->info.weight = MaxInt;
			arcNode1->otherside = v2;
			arcNode1->next = G.vertices[i].first;
			G.vertices[i].first = arcNode1;



			switch (yourpreference) {
			case 1: // 时间优先
				arcNode1->info.weight = replacenow2->time;////////////////////////////////////////////////////////改了之后这里就不能用stod了
				arcNode1->info.notthatimportant = replacenow2->cost;
				
				break;
			case 2: // 费用优先
				arcNode1->info.weight = replacenow2->cost;
				arcNode1->info.notthatimportant = replacenow2->time;
				
				break;
			}//很明显，这里是无向网,所以手动将两条方向的边附上同样的值

			arcNode1->info.transport = replacenow2->transport;
			arcNode1->info.courseinfo = replacenow2->courseinfo;
			

			//同样的，将两条方向的边附上同样的值



			inputarcflag_value++;
			guard++;//这个guard是人为加上的，主要是为了解决replacenow2 = replacenow2->next会指向一个空的，没有用的CityTail
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
	printf("             函数LocateVex(G, v1)：最长用时：Time elapsed is : %lf (ms). \n ", longestt1);
	printf("            函数LocateVex(G, v1)：最短用时：Time elapsed is : %lf (ms). \n ", shortestt1);
	printf("            函数LocateVex(G, v1)：平均用时：Time elapsed is : %lf (ms). \n ", averaget1);
	cout << endl << endl;

	double averaget2 = 0;
	for (auto calcu : L2collector) {
		averaget2 += calcu;
	}
	averaget2 /= L2collector.size();
	double longestt2 = 0, shortestt2 = 0;
	longestt2 = *max_element(L2collector.begin(), L2collector.end());
	shortestt2 = *std::min_element(L2collector.begin(), L2collector.end());
	printf("             函数LocateVex(G, v2)：最长用时：Time elapsed is : %lf (ms). \n ", longestt2);
	printf("            函数LocateVex(G, v2)：最短用时：Time elapsed is : %lf (ms). \n ", shortestt2);
	printf("            函数LocateVex(G, v2)：平均用时：Time elapsed is : %lf (ms). \n ", averaget2);
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
	vector<double> dist(vertexCount, MaxInt); // 存储起始点到各顶点的最短距离
	vector<bool> visited(vertexCount, false); // 记录顶点是否已被访问
	vector<int> path(vertexCount, -1); // 存储最短路径的前驱顶点
	PreserverNode plates;

	int startIndex = -1; // 起始城市的索引
	int endIndex = -1; // 结束城市的索引
	/*------------------------------------------------------------以上是距离数组、判断访问数组和路径点数组的初始化--------------------------------------------------------------------------*/

	// 找到起始城市和结束城市的索引
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

	dist[startIndex] = 0; // 起始点到自身的距离为0

	///////////////////////////////////////////////////////////////////
	if (startIndex<0 || startIndex>vertexCount - 1) {
		cout << "            findyou!" << "  visited[startIndex] = true;//起点已经被阅读   " << startIndex << endl<<endl;
	}
	///////////////////////////////////////////////////////////////////
	visited[startIndex] = true;//起点已经被阅读


	/*------------------------------------------------------------以上是目标城市的检索定位--------------------------------------------------------------------------*/


	// 更新与起始点相邻顶点的最短距离
	ArcNode* arcNode = graph.vertices[startIndex].first;
	while (arcNode != nullptr) {
		int neighborIndex1 = arcNode->adjvex;
		string weight1 = arcNode->info.weight;
		if(dist[neighborIndex1]> stod(weight1))//这是我对下面的解决方案
		dist[neighborIndex1] = stod(weight1);//注意，为什么遇到多路径开头我就会失去正确判断的能力？因为这里存在覆盖，如何在存在覆盖的情况下取消权值过大的路径？
		path[neighborIndex1] = startIndex;
		arcNode = arcNode->next;
	}


	/*------------------------------------------------------------以上是借助邻接表进行的dist数组基本处理与path的基本处理，咱就是说主打的就是一个模仿邻接矩阵的Dijkstra--------------------------------------------------------------------------*/
	//接下来进入主循环，每次求得起始点到某个顶点的最短路径

	for (int i = 1; i < vertexCount; ++i) {
		double minDist = MaxInt;
		int minIndex = -1; // 用于标记找到的点的下标

		// 选择未访问的顶点中距离最小的顶点
		for (int j = 0; j < vertexCount; ++j) {
			///////////////////////////////////////////////////////////////////
			if (j<0 || j>vertexCount - 1) {
				cout << "            findyou!" << "  if (!visited[j] && dist[j] < minDist) {					minDist = dist[j];				minIndex = j; //距离最近的j点			}   " << j << endl<<endl;
			}
			///////////////////////////////////////////////////////////////////
			if (!visited[j] && dist[j] < minDist) {
				minDist = dist[j];
				minIndex = j; //距离最近的j点
			}
		}

		///////////////////////////////////////////////////////////////////
		if (minIndex<0 || minIndex>vertexCount - 1) {
			//cout << "            findyou!" << "  visited[minIndex] = true; // 标记该顶点为已访问   " << minIndex << endl<<endl;
			break;
		}
		///////////////////////////////////////////////////////////////////
		visited[minIndex] = true; // 标记该顶点为已访问


		// 更新与当前顶点相邻顶点的最短距离
		ArcNode* arcNodebowl = graph.vertices[minIndex].first;
		while (arcNodebowl != nullptr) {
			int neighborIndex2 = arcNodebowl->adjvex;
			string weight2 = arcNodebowl->info.weight;
	/*		cout << minIndex<<endl;*/
			// 如果通过当前顶点可以获得更短的距离，则更新最短距离和路径
				///////////////////////////////////////////////////////////////////
			if (neighborIndex2<0 || neighborIndex2>vertexCount - 1) {
				cout << "            findyou!" << "  if (!visited[neighborIndex2] && dist[minIndex] + weight2 < dist[neighborIndex2])   " << neighborIndex2 <<endl<< endl;
			}
			///////////////////////////////////////////////////////////////////
			if (!visited[neighborIndex2] && dist[minIndex] + stod(weight2) < dist[neighborIndex2]) {/////////////////////////////////////////////////////////////////////////////////将stod使用的调整写入报告
				dist[neighborIndex2] = dist[minIndex] + stod(weight2);
				path[neighborIndex2] = minIndex;
				/*if (neighborIndex2 == 57 || neighborIndex2 == 189 || neighborIndex2 == 142 || neighborIndex2 == 56 || neighborIndex2 == 86) {
					cout << neighborIndex2<<"   "<< arcNodebowl->info.weight << "   " << arcNodebowl->info.notthatimportant << "   " << endl;
				}*///这里cout是为了输出一下图的连接关系，检查一下节点是怎么弄出来的，当前节点就是上方的那个（minIndex）
			}

			arcNodebowl = arcNodebowl->next;//因为这里不存在和之前建图的时候城市链表有一个空head和一个空tail所以寻要一个guard免得浏览到空tail里面去了
			//这里邻接表没了就是没了，没有空tail，最后一个结点的next就是NULL
		}

		if (minIndex == -1)
			break; // 所有顶点都已访问，算法结束
	}


	//在这个算法中

	if (dist[endIndex] == MaxInt) {//如果起点到终点的距离没有发生任何改变
		//这个是否与前面那个都为-1重合？并不，有时候是有向图，不为-1，但就是没路
		cout << "            没有任何一条联通这两点的路径！！！！（有向原因）" << endl << endl;
		return ERROR;
	}//就说明没有这条可以到达的路

	// 构建最短路径
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

	// 反转路径顺序
	reverse(shortestPath.begin(), shortestPath.end());

	// 输出最短路径
	switch (preference)
	{
	case 1:
		cout << "            从 " << startCity << " 到 " << endCity << " 用时最短的路径:" << endl << endl;
		s1 = "            从 " + startCity + " 到 " + endCity + " 的最短路径:";
		cout << "            ";
		break;
	case 2:
		cout << "            从 " << startCity << " 到 " << endCity << " 花费最少的路径:" << endl << endl;
		s1 = "            从 " + startCity + " 到 " + endCity + " 的最短路径:";
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
		plates.city = graph.vertices[index].data.city;//这是当前结点本尊
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (i != shortestPath.size() - 1) {//以下操作不能执行于最后一个i
			//下面是想找到当前节点与下一个结点之间的arc
			// 并且将他的值赋给我们的kitchen
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
			}//找到与下一个点相连的arc
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


	// 输出最短距离
	switch (preference) {
	case 1:
		cout << "            最短时间: " << dist[endIndex] << "小时" << endl << endl;
		s3 = "            最短时间: " + to_string(dist[endIndex]) + "小时";
		break;
	case 2:
		cout << "            最少消费: US$ " << dist[endIndex] << endl << endl;
		s3 = "            最少消费: US$ " + to_string(dist[endIndex]);
		break;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*输出htm文件*/          //真失败，一下午就写出来这个玩意
//完善：设置一个循环
void CreathtmMap(char* htmFileAddress, vector<PreserverNode> kitchen, string s1, string s2, string s3) {


	int i = 0, j = 0;
	int size = kitchen.size();

	ofstream drawmaphtm(htmFileAddress);


	char head[2000];
	sprintf_s(head, 2000, "<!DOCTYPE html><html><head><style type='text/css'>body, html{width: 100%%;height: 100%%;margin:0;font-family:'微软雅黑';}#allmap{height:100%%;width:100%%;}#r-result{width:100%%;}<style type=\"text/css\">#print - message{position: fixed;top: 50%%;left: 50%%;transform: translate(-50%%, -50%%);font - size: 18px;text - align: center;z - index: 999;}</style></style><script type='text/javascript' src='http://api.map.baidu.com/api?v=2.0&ak=nSxiPohfziUaCuONe4ViUP2N'></script><title>Shortest path from %s to %s</title></head><body><div id=\"print-message\">%s<br>%s<br>%s</div><div id='allmap'></div></div></body></html><script type='text/javascript'>var map = new BMap.Map('allmap');var point = new BMap.Point(0, 0);map.centerAndZoom(point, 2);map.enableScrollWheelZoom(true);", kitchen[0].city.c_str(), kitchen[size - 1].city.c_str(),s1.c_str(),s2.c_str(),s3.c_str());
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
	drawmaphtm << "</script>";//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!别自断后路
	drawmaphtm.close();
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// 广度优先遍历算法
void BFSTraverse(const ALGraph& graph) {
	vector<bool> visited(graph.vexnum, false); // 标记顶点是否已访问
	queue<int> queue; // 存储待访问的顶点

	for (int i = 0; i < graph.vexnum; i++) {
		if (!visited[i]) {
			visited[i] = true;
			if (i != 0)
				cout << " -> " << graph.vertices[i].data.city; // 输出城市信息（从第二个顶点开始添加箭头符号）
			else
				cout << graph.vertices[i].data.city; // 输出城市信息（第一个顶点无需添加箭头符号）
			queue.push(i);

			while (!queue.empty()) {
				int vertex = queue.front();
				queue.pop();

				ArcNode* arcNode = graph.vertices[vertex].first;
				while (arcNode != nullptr) {
					int adjVertex = arcNode->adjvex;
					if (!visited[adjVertex]) {
						visited[adjVertex] = true;
						cout << " -> " << graph.vertices[adjVertex].data.city; // 输出城市信息（添加箭头符号）
						queue.push(adjVertex);
					}
					arcNode = arcNode->next;
				}
			}
		}
	}
}




int main() {

	int howmanycities = 0, howmanyroutes = 0;//留着后面用

	/*------------------------------------------------------------读城市文件！！--------------------------------------------------------------------------*/
	char CSVCities[] = "C:\\Users\\Nighfearti\\Desktop\\cities.csv";
	CitiesInfoHead CityHead, CityTail;
	CityHead = new CITIESInfo;
	CityTail = new CITIESInfo;

	Timer t2;
	t2.Start();
	CITIES_ReadCSV(CSVCities, CityHead, CityTail, howmanycities);
	t2.Stop();
	printf("            函数CITIES_ReadCSV：总时长：Time elapsed is : %lf (ms). \n ", t2.ElapsedTime());
	cout << endl << endl;
	
	
	CitiesInfoHead PrintChecker1 = CityHead;
	//while ((PrintChecker1 = PrintChecker1->next) && PrintChecker1 != CityTail) {
	//	cout << PrintChecker1->country << "   " << PrintChecker1->city << "   " << PrintChecker1->Longitude << "   " << PrintChecker1->Latitude << endl;
	//}
	//检测成功，注释掉

	/*------------------------------------------------------------读路径文件！！--------------------------------------------------------------------------*/
	char CSVRoutes[] = "C:\\Users\\Nighfearti\\Desktop\\routes.csv";
	RoutesInfoHead RouteHead, RouteTail;
	RouteHead = new ROUTESInfo;
	RouteTail = new ROUTESInfo;

	Timer t3;
	t3.Start();
	ROUTES_ReadCSV(CSVRoutes, RouteHead, RouteTail, howmanyroutes);
	t3.Stop();
	printf("            函数ROUTES_ReadCSV：总时长：Time elapsed is : %lf (ms). \n ", t3.ElapsedTime());
	cout << endl<<endl;
	
	
	RoutesInfoHead PrintChecker2 = RouteHead;
	//while ((PrintChecker2 = PrintChecker2->next) && PrintChecker2 != RouteTail) {
	//	cout <<what<<"   " << PrintChecker2->routecity1 << "   " << PrintChecker2->routecity2 << "   " << PrintChecker2->transport << "   " << PrintChecker2->time << "   " << PrintChecker2->cost << "   " << PrintChecker2->courseinfo << endl << endl;
	//}
	//检测成功，注释掉

	cout << endl << endl << "            " << howmanycities << "个城市节点    " << howmanyroutes << "条路径信息   " << endl << endl;

	/*------------------------------------------------------------开始创建图辣！！--------------------------------------------------------------------------*/
	int yourpreference,whetherdirection;
	ALGraph G;
	cout << endl << "            有向图还是无向图？" << endl << endl << "            无向图  :  请输入1" << endl << endl << "            有向图  :  请输入2" << endl  << endl;
	redirect:cin >> whetherdirection;
	if (whetherdirection != 1 && whetherdirection != 2) {
		cout << "            您的输入有误，请重新输入！" << endl;
		std::cin.clear();
		std::cin.ignore(10000, '\n');  // 替换为适当的整数值//用于清除输入流中字符的函数调用//第一个参数表示要忽略的字符数量，第二个参数表示要忽略的终止字符//cin缓冲区小于1000，不用担心
		goto redirect;
	}
	cout << endl << "            你选择时间最短优先还是花费最少优先？" << endl << endl << "            如果想要时间最短      请输入1" << endl << endl << "            想要花费最少          请输入2" << endl  << endl;
	reinput:cin >> yourpreference;
	if (yourpreference != 1 && yourpreference != 2) {
		cout << "            您的输入有误，请重新输入！"<<endl;
		std::cin.clear();
		std::cin.ignore(10000, '\n');  // 替换为适当的整数值//用于清除输入流中字符的函数调用//第一个参数表示要忽略的字符数量，第二个参数表示要忽略的终止字符//cin缓冲区小于1000，不用担心
		goto reinput;
	}
	

	Timer t7;
	t7.Start();
	CreateUDN(G, CityHead, howmanycities, RouteHead, howmanyroutes, yourpreference,whetherdirection);
	t7.Stop();
	printf("             函数CreateUDN：总时长：Time elapsed is : %lf (ms). \n ", t7.ElapsedTime());
	cout << endl<<endl;
	
	/*------------------------------------------------------------建图完毕！开始遍历--------------------------------------------------------------------------*/

	// 调用广度优先遍历算法
	BFSTraverse(G);

	/*------------------------------------------------------------遍历完毕！开始Djkstra算法--------------------------------------------------------------------------*/
	/*Djkstra算法*/
	again : string startCity;
	cout << "            请输入起点" << endl;
	cin >> ws; // 清除输入缓冲区中的换行符
	getline(cin, startCity);//被跳过：这是因为在之前的输入操作中，cin 留下了一个换行符在输入缓冲区中，而 getline 在遇到换行符时会认为输入结束，因此会跳过输入。解决方案如上行
	string endCity;
	cout << "            请输入终点" << endl;
	cin >> ws; // 清除输入缓冲区中的换行符
	getline(cin, endCity);

	vector<PreserverNode> kitchen;
	//关于为什么我要将这个数组构建出来
	//如果在dijkstra算法中只保留节点名，那么我将在主函数中再添加一个LocateVex函数来找节点index然后再借助index找原数据
	//但是这有两点不好
	//1.能够一遍就做好的事情为什么要花两步呢？虽然不会增加时间复杂度，但会增加时间
	//2.我现在写了LocateVex，那我岂不是又要将建图时为边找节点并且将它们联系起来的事情再做一遍嘛？！我拒绝！

	string str1, str2, str3;

	
 
	Timer t;
	t.Start();
	int a = Dijkstra(G, startCity, endCity, yourpreference, kitchen, str1, str2, str3);
	t.Stop();
	printf("            函数Dijkstra：Time elapsed is : %lf (ms). \n ", t.ElapsedTime());
	if (a == -1) {
		cout << "            没有任何一条联通这两点的路径！！！！（不连通）" << endl << endl;
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

		

		/*------------------------------------------------------------好耶！开始输出！--------------------------------------------------------------------------*/
		/*CreathtmMap();*/
		char mapfilename[] = "C:\\Users\\Nighfearti\\Desktop\\newmap.htm";
		CreathtmMap(mapfilename, kitchen, str1, str2, str3);
	}





	/*------------------------------------------------------------输出到了htm文件--------------------------------------------------------------------------*/

	//善后：删除申请的空间
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