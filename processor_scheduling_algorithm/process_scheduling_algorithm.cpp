// 通过c++模拟操作系统对处理器调度算法的实现
// 要求实现先进先出(FIFO)调度算法或最短作业优先(SJF)调度算法.
// 输入不少于5个作业的作业号、提交时间和执行时间，按照调度算法进行调度，输出调度作业序列
// 以上内容会转移到README.md中

// 编写者：钟奇林
// 时间：2024/05/30
// 注意：本程序仅供学习参考，请勿用于实际生产环境。
// 如有任何问题，请联系作者：<3100649706@qq.com>
// 包含必要的头文件
#include<iostream>
#include<algorithm>
#include<iomanip>
using std::cin;
using std::cout;
using std::setw;
using std::sort;

// 进程结构体
struct Process {
	int pid; // 进程ID
	double arrival_time; // 到达时间
	double expected_run_time; // 预计运行时间
	double start_time = 0; // 开始时间（默认为0）
	double end_time = 0; // 结束时间（默认为0）
};

void PrintMenu(); // 打印菜单
void printProcessTable(Process* p_a, int n); // 打印进程表

// 调度算法类
class SchedulingAlgorithm {
public:
	double cpu_run_time; // CPU运行时间
	void FIFO(Process* p_a, int n); // 先来先服务调度算法
	void SJF(Process* p_a, int n); // 最短作业优先调度算法
};

int main(int argc, char* argv) {
	std::ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0);
	PrintMenu(); // 打印菜单

	int choice, n;
	Process* p_a;
	cout << "请选择要实现的算法：";
	cin >> choice; // 输入要实现的算法
	cout << "请输入进程数目：";
	cin >> n; // 输入进程数目
	p_a = new Process[n]; // 动态分配进程数组空间
	cout << "请输入进程编号：（空格区分）";
	for (int i = 0; i < n; ++i)
		cin >> p_a[i].pid; // 输入进程编号
	cout << "请输入进程到达时间：（空格区分）";
	for (int i = 0; i < n; ++i)
		cin >> p_a[i].arrival_time; // 输入进程到达时间
	cout << "请输入进程预计运行时间：（空格区分）";
	for (int i = 0; i < n; ++i)
		cin >> p_a[i].expected_run_time; // 输入进程预计运行时间

	switch (choice)
	{
	case 1:
		SchedulingAlgorithm().FIFO(p_a, n); // 执行先来先服务调度算法
		break;
	case 2:
		SchedulingAlgorithm().SJF(p_a, n); // 执行最短作业优先调度算法
		break;
	default:
		cout << "我可没写其它的了" << "\n"; // 输入非法选项时提示
		break;
	}

	printProcessTable(p_a, n); // 打印进程表
}

// 打印菜单
void PrintMenu()
{
	cout << "+---------------------------------------+" << "\n";
	cout << "|\t>>> 算法清单 <<<\t\t|" << "\n";
	cout << "|\t1.先来先服务算法(FCFS)\t\t|" << "\n" << "|\t2.最短作业优先算法(SJF)\t\t|" << "\n";
	cout << "|\t0.退出\t\t\t\t|" << "\n";
	cout << "+---------------------------------------+" << "\n";
}

// 打印进程表
void printProcessTable(Process* p_a, int n) {
	// 输出模块
	sort(p_a, p_a + n, [](Process x, Process y)->bool {return x.pid < y.pid; }); // 按进程号排序
	cout << "\n" << "| 进程号\t| 进入时间\t| 服务时间\t| 开始时间\t| 结束时间\t| 周转时间\t| 带权周转时间\t|" << "\n";
	for (int i = 0; i < n; ++i) {
		double cicle_time = p_a[i].end_time - p_a[i].arrival_time; // 周转时间计算
		double carry_weight_cicle_time = cicle_time / p_a[i].expected_run_time; // 带权周转时间计算
		cout << "| 进程\t" << p_a[i].pid << "\t| " << p_a[i].arrival_time << "\t\t| " << p_a[i].expected_run_time\
			<< "\t\t| " << p_a[i].start_time << "\t\t| " << p_a[i].end_time << "\t\t| " << cicle_time << "\t\t| "\
			<< std::setprecision(2) <<carry_weight_cicle_time << "\t\t| " << "\n";
	}
}

// 先来先服务调度算法
void SchedulingAlgorithm::FIFO(Process* p_a, int n) {
	sort(p_a, p_a + n, [](Process x, Process y)->bool {return x.arrival_time < y.arrival_time; }); // 按到达时间排序
	this->cpu_run_time = p_a[0].arrival_time; // 初始CPU运行时间为第一个进程的到达时间
	for (int i = 0; i < n; ++i) {
		if (p_a[i].arrival_time > this->cpu_run_time) { // 到达时间超过当前cpu运行时间
			this->cpu_run_time = p_a[i].arrival_time; // 更新CPU运行时间
		}
		p_a[i].start_time = this->cpu_run_time; // 设置开始时间
		this->cpu_run_time += p_a[i].expected_run_time; // 更新CPU运行时间
		p_a[i].end_time = this->cpu_run_time; // 设置结束时间
	}
}

// 最短作业优先调度算法
void SchedulingAlgorithm::SJF(Process* p_a, int n) {
	sort(p_a, p_a + n, [](Process x, Process y)->bool {return x.arrival_time < y.arrival_time; }); // 按到达时间排序
	this->cpu_run_time = p_a[0].arrival_time; // 初始CPU运行时间为第一个进程的到达时间
	int i = 0;
	while (i < n) {
		p_a[i].start_time = this->cpu_run_time; // 设置开始时间
		this->cpu_run_time += p_a[i].expected_run_time; // 更新CPU运行时间
		p_a[i].end_time = this->cpu_run_time; // 设置结束时间

		// 更新
		++i;
		int amount = 0;
		for (int j = i; j < n; ++j)
			if (p_a[j].arrival_time <= this->cpu_run_time)amount++;
		sort(p_a + i, p_a + i + amount, [](Process x, Process y)->bool {return x.expected_run_time < y.expected_run_time; }); // 重新排序
		//printProcessTable(p_a, n);
	}
}