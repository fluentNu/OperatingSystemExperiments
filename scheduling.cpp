#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm>
using namespace std;
struct PCB {
	string name;		//进程名
	float ArriveTime;	//到达时间
	float ServerTime;	//服务时间
	float FinishTime;	//完成时间
	float RoundTime;	//周转时间
	float WeightTime;	//带权周转时间
};
PCB arr[100];			//全局变量，存放进程
void Menu();
void Input(int& n);
void Output(int n, float t1, float t2);
void Show(int i);
void FCFS(int n, float& t1, float& t2);		//先来先服务算法
void SJF(int n, float& t1, float& t2);		//短作业优先算法
void Priority(int n, float& t1, float& t2);	//优先级调度算法
bool cmp1(PCB a, PCB b) {
	return a.ArriveTime < b.ArriveTime;
}
bool cmp2(PCB a, PCB b) {
	if (a.ArriveTime != b.ArriveTime)
		return a.ArriveTime < b.ArriveTime;
	else
		return a.ServerTime < b.ServerTime;
}
int main()
{
	float roundtime = 0;	//总周转时间
	float weightime = 0;	//总带权周转时间
	int n;
	int select;
	while (1)
	{
		Menu();
		scanf("%d", &select);
		if (select == 1) {
			printf("\n\n=================先来先服务算法================\n\n");
			Input(n);
			sort(arr, arr + n, cmp1);			//按到达时间排序
			FCFS(n, roundtime, weightime);		//先来先服务
			Output(n, roundtime, weightime);
		}
		else if (select == 2) {
			printf("\n\n=================短作业优先算法================\n\n");
			Input(n);
			sort(arr, arr + n, cmp2);			//按服务时间排序
			SJF(n, roundtime, weightime);		//短作业优先
			Output(n, roundtime, weightime);
		}
		else if (select == 3) {
			printf("\n\n=================优先级算法================\n\n");
			Input(n);
			sort(arr, arr + n, cmp2);			//按服务时间排序
			Priority(n, roundtime, weightime);	//优先级算法
			Output(n, roundtime, weightime);
		}
		else if (select == 0) {
			exit(0);
		}
		else {
			cout << "请选择正确的序号！" << endl;
			system("pause");
		}

	}
	system("pause");
}

void Menu() {
	system("cls");
	printf("请选择算法：\n");
	printf("1.先来先服务算法\n");
	printf("2.短作业优先算法\n");
	printf("3.优先级算法\n");
	printf("0.退出程序\n");
	printf("请选择序号:");
}
void Input(int& n) {
	printf("请输入进程数：");
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
	{
		printf("%d 进程名:", i + 1);
		cin >> arr[i].name;
		printf("到达时间: ");
		scanf("%f", &arr[i].ArriveTime);
		printf("服务时间：");
		scanf("%f", &arr[i].ServerTime);
	}
}
void Output(int n, float t1, float t2) {
	float avr_t1 = 0.0f;	//平均周转时间
	float avr_t2 = 0.0f;	//平均带权周转时间
	avr_t1 = t1 / n;
	avr_t2 = t2 / n;
	printf("\n");
	printf("平均周转时间为：%f \n", avr_t1);
	printf("平均带权周转时间为：%f \n", avr_t2);
	system("pause");
}
void Show(int i) {
	printf("\n------------------------------------------------------\n");
	cout << "进程名：" << arr[i].name << " ";
	cout << "到达时间：" << arr[i].ArriveTime << " ";
	cout << "服务时间：" << arr[i].ServerTime << endl;
	cout << "完成时间：" << arr[i].FinishTime << endl;
	cout << "周转时间：" << arr[i].RoundTime << endl;
	cout << "带权周转时间" << arr[i].WeightTime << endl;
}

//先来先服务算法
void FCFS(int n, float& t1, float& t2)
{
	int i;
	arr[0].FinishTime = arr[0].ArriveTime + arr[0].ServerTime;	//完成时间=到达时间-服务时间
	arr[0].RoundTime = arr[0].FinishTime - arr[0].ArriveTime;	//周转时间=完成时间-提交时间
	arr[0].WeightTime = arr[0].RoundTime / arr[0].ServerTime;	//带权时间=周转时间/服务时间
	for (i = 1; i < n; i++) {
		if (arr[i].ArriveTime < arr[i - 1].FinishTime) {		//当前到达时间在上一个作业结束时间之前
			arr[i].FinishTime = arr[i - 1].FinishTime + arr[i].ServerTime;	//完成时间=上一个完成时间+服务时间
			arr[i].RoundTime = arr[i].FinishTime - arr[i].ArriveTime;		//周转时间=完成时间-到达时间
			arr[i].WeightTime = arr[i].RoundTime / arr[i].ServerTime;		//带权时间=周转时间/服务时间
		}
		else {	//当前到达时间在上一个作业结束时间之后
			arr[i].FinishTime = arr[i].ArriveTime + arr[i].ServerTime;
			arr[i].RoundTime = arr[i].FinishTime - arr[i].ArriveTime;
			arr[i].WeightTime = arr[i].RoundTime / arr[i].ServerTime;
		}
	}
	for (int i = 0; i < n; i++) {
		Show(i);
		t1 += arr[i].RoundTime;
		t2 += arr[i].WeightTime;
	}
}
//短作业优先算法
void SJF(int n, float& t1, float& t2)
{
	struct PCB t;
	arr[0].FinishTime = arr[0].ArriveTime + arr[0].ServerTime;	//完成时间=到达时间+服务时间
	arr[0].RoundTime = arr[0].FinishTime - arr[0].ArriveTime;	//周转时间=完成时间-提交时间
	arr[0].WeightTime = arr[0].RoundTime / arr[0].ServerTime;	//带权时间=周转时间/服务时间
	for (int i = 1; i < n; i++) {
		for (int j = i; j < n - 1; j++) {
			for (int k = i + 1; k < n; k++) {
				if ((arr[i - 1].FinishTime >= arr[j].ArriveTime) && (arr[i - 1].FinishTime >= arr[k].ArriveTime) && (arr[j].ServerTime > arr[k].ServerTime)) {
					t = arr[j];
					arr[j] = arr[k];
					arr[k] = t;
				}
			}
		}
		if (arr[i].ArriveTime < arr[i - 1].FinishTime) {		//当前到达时间在上一个作业结束时间之前
			arr[i].FinishTime = arr[i - 1].FinishTime + arr[i].ServerTime;	//完成时间=上一个完成时间+服务时间
			arr[i].RoundTime = arr[i].FinishTime - arr[i].ArriveTime;		//周转时间=完成时间-到达时间
			arr[i].WeightTime = arr[i].RoundTime / arr[i].ServerTime;		//带权时间=周转时间/服务时间
		}
		else {	//当前到达时间在上一个作业结束时间之后
			arr[i].FinishTime = arr[i].ArriveTime + arr[i].ServerTime;
			arr[i].RoundTime = arr[i].FinishTime - arr[i].ArriveTime;
			arr[i].WeightTime = arr[i].RoundTime / arr[i].ServerTime;
		}
	}
	for (int i = 0; i < n; i++) {
		Show(i);
		t1 += arr[i].RoundTime;
		t2 += arr[i].WeightTime;
	}
}
//优先级算法
void Priority(int n, float& t1, float& t2)
{
	PCB temp;
	arr[0].FinishTime = arr[0].ArriveTime + arr[0].ServerTime;	//完成时间=到达时间+服务时间
	arr[0].RoundTime = arr[0].FinishTime - arr[0].ArriveTime;	//周转时间=完成时间-提交时间
	arr[0].WeightTime = arr[0].RoundTime / arr[0].ServerTime;	//带权时间=周转时间/服务时间
	for (int i = 1; i < n; i++) {
		for (int j = i; j < n - 1; j++) {
			for (int k = i + 1; k < n; k++) {
				if ((arr[i - 1].FinishTime >= arr[j].ArriveTime) && (arr[i - 1].FinishTime >= arr[k].ArriveTime) && (arr[j].ServerTime > arr[k].ServerTime)) {
					temp = arr[j];
					arr[j] = arr[k];
					arr[k] = temp;
				}
			}
		}
		if (arr[i].ArriveTime < arr[i - 1].FinishTime) {	//当前到达时间在上一个作业结束时间之前
			arr[i].FinishTime = arr[i - 1].FinishTime + arr[i].ServerTime;	//完成时间=上一个完成时间+服务时间
			arr[i].RoundTime = arr[i].FinishTime - arr[i].ArriveTime;		//周转时间=完成时间-到达时间
			arr[i].WeightTime = arr[i].RoundTime / arr[i].ServerTime;		//带权时间=周转时间/服务时间
		}
		else {	//当前到达时间在上一个作业结束时间之后
			arr[i].FinishTime = arr[i].ArriveTime + arr[i].ServerTime;
			arr[i].RoundTime = arr[i].FinishTime - arr[i].ArriveTime;
			arr[i].WeightTime = arr[i].RoundTime / arr[i].ServerTime;
		}
	}
	for (int i = 0; i < n; i++) {
		Show(i);
		t1 += arr[i].RoundTime;
		t2 += arr[i].WeightTime;
	}
}