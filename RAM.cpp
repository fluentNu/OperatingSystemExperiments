#include<iostream>
#include<list>
#include<algorithm>
using namespace std;

#define Free 0		//空闲状态标识符
#define Busy 1		//已用状态标识符
#define ERROR 0		//错误标识符
#define OK 1		//完成标识符
#define MaxLength 640 //最大内存空间为640KB
typedef int Status;
bool flag;

struct node			//定义一个空闲分区表的结点结构
{
	long size;		//分区大小
	long address;	//分区地址
	int state;		//状态
};
list<node> RAMList;				//双向链表模拟空闲分区表
Status AllocBlock(int request);	//内存分配
Status FreeBlock(int number);	//内存回收
Status FirstFit(int request);	//首次适应算法
Status BestFit(int request);	//最佳适应算法
Status WorstFit(int request);	//最差适应算法
void Show();					//查看分配情况

int main()
{
	int op;					//算法选择标记
	cout << "请输入所使用的内存分配算法：\n";
	cout << "(1)首次适应算法\n(2)最佳适应算法\n(3)最差适应算法\n";

	cin >> op;
	while (op < 1 || op>3)
	{
		cout << "输入错误，请重新输入所使用的内存分配算法：\n";
		cin >> op;
	}
	RAMList.clear();	//空闲分区表初始化
	node t;
	t.address = 0;
	t.size = MaxLength;
	t.state = Free;
	RAMList.insert(RAMList.begin(), t);
	int choice;			//操作选择标记
	while (1)
	{
		Show();
		cout << "请输入您的操作：";
		cout << "\n1: 分配内存\n2: 回收内存\n0: 退出\n";

		cin >> choice;
		if (choice == 1)
			AllocBlock(op);		// 分配内存
		else if (choice == 2)	// 内存回收
		{
			int flag;
			cout << "请输入您要释放的分区号：";
			cin >> flag;
			FreeBlock(flag);
		}
		else if (choice == 0) {
			break;				//退出
		}
		else					//输入操作有误
		{
			cout << "输入有误，请重试！" << endl;
			continue;
		}
	}
}

//显示内存分配情况
void Show()
{
	int number = 0;
	cout << "\n主存分配情况:\n";
	cout << "----------------------------------------------\n";
	cout << "分区号\t起始地址\t分区大小\t状态\n\n";
	for (list<node>::iterator i = RAMList.begin(); i != RAMList.end(); ++i)
	{
		cout << "  " << number++ << "\t";
		cout << "  " << (*i).address << "\t\t";
		cout << " " << (*i).size << "KB\t\t";
		if ((*i).state == Free)
			cout << "空闲\n\n";
		else
			cout << "已分配\n\n";
	}
	cout << "----------------------------------------------\n\n";
}

Status AllocBlock(int op) {
	//分配主存
	int request = 0;
	cout << "请输入需要分配的主存大小(单位:KB)：";
	cin >> request;
	if (request < 0 || request == 0)
	{
		cout << "分配大小不合适，请重试！" << endl;
		return ERROR;
	}
	//选择最佳适应算法
	if (op == 2)
	{
		if (BestFit(request) == OK)
			cout << "分配成功！" << endl;
		else
			cout << "内存不足，分配失败！" << endl;
		return OK;
	}
	//选择最差适应算法
	if (op == 3)
	{
		if (WorstFit(request) == OK)
			cout << "分配成功！" << endl;
		else
			cout << "内存不足，分配失败！" << endl;
		return OK;
	}
	//默认首次适应算法
	else
	{
		if (FirstFit(request) == OK)
			cout << "分配成功！" << endl;
		else
			cout << "内存不足，分配失败！" << endl;
		return OK;
	}
}
Status FreeBlock(int count) {
	if (RAMList.size() - 1 < count)
		return ERROR;
	list<node>::iterator it = RAMList.begin();
	for (int i = 0; i < count; i++)
		++it;
	//cout << it->address << it->size << it->state << endl;
	it->state = Free;
	if (it == RAMList.begin()) {
		list<node>::iterator old = it;
		++it;
		if (it->state == Busy)
			return OK;
		else {
			old->size += it->size;
			RAMList.erase(it);
			return OK;
		}
	}
	--it;
	list<node>::iterator prev = it;
	++it;
	++it;
	list<node>::iterator next = it;
	--it;
	if (prev->state == Free)//与前面的空闲块相连
	{
		prev->size += it->size;
		RAMList.erase(it);
		it = prev;
	}
	if (next->state == Free)//与后面的空闲块相连
	{
		it->size += next->size;
		RAMList.erase(next);
	}
	return OK;
}
Status FirstFit(int request) {
	//为申请作业开辟新空间且初始化
	node* temp = new node;
	temp->size = request;
	temp->state = Busy;

	list<node>::iterator it = RAMList.begin();
	while (it != RAMList.end())
	{
		if (it->state == Free && it->size == request)
		{//有大小恰好合适的空闲块
			it->state = Busy;
			return OK;
			break;
		}
		if (it->state == Free && it->size > request)
		{//有空闲块能满足需求且有剩余
			temp->address = it->address;
			RAMList.insert(it, *temp);
			it->address = temp->address + temp->size;
			it->size -= request;
			return OK;
			break;
		}
		++it;
	}
	return ERROR;
}
Status BestFit(int request) {
	list<node>::iterator best = RAMList.end();
	for (list<node>::iterator it = RAMList.begin(); it != RAMList.end(); ++it) {
		if (it->state == Free && it->size > request) {
			if (best == RAMList.end()) {
				best = it;
			}
			else if (it->size < best->size) {
				best = it;
			}
		}
	}
	if (best == RAMList.end()) {
		return ERROR;
	}
	node* temp = new node;
	temp->size = request;
	temp->state = Busy;
	temp->address = best->address;
	best->address += temp->size;
	best->size -= temp->size;
	RAMList.insert(best, *temp);
	return OK;
}
Status WorstFit(int request) {
	list<node>::iterator worst = RAMList.end();
	for (list<node>::iterator it = RAMList.begin(); it != RAMList.end(); ++it) {
		if (it->state == Free && it->size > request) {
			if (worst == RAMList.end()) {
				worst = it;
			}
			else if (it->size > worst->size) {
				worst = it;
			}
		}
	}
	if (worst == RAMList.end()) {
		return ERROR;
	}
	node* temp = new node;
	temp->size = request;
	temp->state = Busy;
	temp->address = worst->address;
	worst->address += temp->size;
	worst->size -= temp->size;
	RAMList.insert(worst, *temp);
	return OK;
}