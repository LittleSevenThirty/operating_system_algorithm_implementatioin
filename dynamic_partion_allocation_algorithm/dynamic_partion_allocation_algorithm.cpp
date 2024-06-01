// 利用c++语言模拟实现操作系统对存储区的动态分区分配算法
// 实现内容最佳适应算法，首次适应算法，最坏适应算法

// 编写者：钟奇林
// 时间：2024/05/30
// 注意：本程序仅供学习参考，请勿用于实际生产环境。
// 如有任何问题，请联系作者：<3100649706@qq.com>
#include<iostream>
#include<vector>
#include<algorithm>
using std::cin;
using std::cout;
using std::vector;
using std::sort;

// 空闲分区表项(free partion 
// 结构体定义：分区
struct Partion {
	int partion_size = 0;		// 分区大小
	int initial_address = 0;	// 分区起始地址
};

// 结构体定义：作业请求序列
struct Process {
	int p_size = 0;             // 作业大小
	int initial_address = -1;   // 作业初始地址
};
void PrintTable(vector<Partion>& table, vector<Process>& p_size_a) {
	sort(table.begin(), table.end(), [](Partion x, Partion y)->bool {return x.initial_address < y.initial_address; });
	cout << "分区号\t| " << "起始地址\t| " << "分区大小\t| " << "\n";
	for (int i = 0; i < table.size(); ++i)
		cout << i << "\t| " << table[i].initial_address << "\t\t| " << table[i].partion_size << "\t\t| " << "\n";
	cout << "\n进程号\t| " << "进程起始地址\t| " << "进程大小\t| " << "\n";
	for (int i = 0; i < p_size_a.size(); ++i)
		cout << i + 1 << "\t| " << p_size_a[i].initial_address << "\t\t| " << p_size_a[i].p_size << "\t\t| " << "\n";
}


// 程序运行结果:如果满足作业序列的请求,则输出分配后的空闲分区表，否则输出拒绝分配的提示即可。
// 动态分区分配算法
class DPA {
private:
	void Amend(vector<Partion>& table, vector<Process>& p_size_a, int index, int num);
	int Find(vector<Partion>& table, int p_size);
public:
	bool FirstFit(vector<Partion>& table, vector<Process>& p_size_a);
	bool BestFit(vector<Partion>& table, vector<Process>& p_size_a);
	bool WorstFit(vector<Partion>& table, vector<Process>& p_size_a);
};

void PrintMenu();

int main(int argc, char* argv) {
	std::ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0);
	int n;
	cout << "输入空闲分区数目:";
	cin >> n;
	vector<Partion> table(n);
	cout << "输入空闲分区起始地址:" << "\n";
	for (Partion& p : table)
		cin >> p.initial_address;
	cout << "输入空闲分区大小:" << "\n";
	for (Partion& p : table)
		cin >> p.partion_size;
	int m;
	cout << "输入作业数目:";
	cin >> m;
	vector<Process> p_size_a(m);
	cout << "输入作业大小" << "\n";
	for (Process& i : p_size_a)
		cin >> i.p_size;
	PrintMenu(); // 打印菜单
	int select;
	bool flag;
	cout << "选择实现:";
	cin >> select;
	switch (select) {
	case 1:flag = DPA().FirstFit(table, p_size_a); break;
	case 2:flag = DPA().BestFit(table, p_size_a); break;
	case 3:flag = DPA().WorstFit(table, p_size_a); break;
	default:break;
	}
	if (!flag)cout << "无法分配" << "\n";
	PrintTable(table, p_size_a);
	return 0;
}

// 打印菜单
void PrintMenu()
{
	cout << "+---------------------------------------+" << "\n";
	cout << "|\t>>> 算法清单 <<<\t\t|" << "\n";
	cout << "|\t1.首次适应\t\t\t|" << "\n" << "|\t2.最佳适应\t\t\t|" << "\n" << "|\t3.最坏适应\t\t\t|" << "\n";
	cout << "|\t0.退出\t\t\t\t|" << "\n";
	cout << "+---------------------------------------+" << "\n";
}

// 在类DPA中实现了动态分区分配算法

void DPA::Amend(vector<Partion>& table, vector<Process>& p_size_a, int index, int num) {
	// 修改指定索引的表项的partition_size，并更新相应的initial_address
	table[index].partion_size -= p_size_a[num].p_size;
	p_size_a[num].initial_address = table[index].initial_address;
	table[index].initial_address += p_size_a[num].p_size;
}

// 对给定的partition_size大小进行查找，返回可用的空闲分区的索引
int DPA::Find(vector<Partion>& table, int p_size) {
	int index = -1;
	for (int j = 0; j < table.size(); ++j)
		if (table[j].partion_size >= p_size) {
			index = j;
			break;
		}
	return index;
}

// 使用First Fit算法进行动态分区分配
bool DPA::FirstFit(vector<Partion>& table, vector<Process>& p_size_a) {
	// 按照initial_address对表进行升序排序
	sort(table.begin(), table.end(), [](Partion x, Partion y)->bool {return x.initial_address < y.initial_address; });
	int num = 0;
	// 对每个process使用First Fit算法进行分区分配
	for (Process& p : p_size_a) {
		int index = Find(table, p.p_size);
		if (index == -1) return false;
		// 修改数据
		this->Amend(table, p_size_a, index, num++);
	}
	return true;
}

// 使用Best Fit算法进行动态分区分配
bool DPA::BestFit(vector<Partion>& table, vector<Process>& p_size_a) {
	// 按照partion_size对表进行升序排序
	sort(table.begin(), table.end(), [](Partion x, Partion y)->bool {return x.partion_size < y.partion_size; });
	int num = 0;
	// 对每个process使用Best Fit算法进行分区分配
	for (Process& p : p_size_a) {
		int index = Find(table, p.p_size);
		if (index == -1) return false;
		// 修改数据
		this->Amend(table, p_size_a, index, num++);
		// 重新排序
		sort(table.begin(), table.end(), [](Partion x, Partion y)->bool {return x.partion_size < y.partion_size; });
	}
	return true;
}

// 使用Worst Fit算法进行动态分区分配
bool DPA::WorstFit(vector<Partion>& table, vector<Process>& p_size_a) {
	// 按照partion_size对表进行降序排序
	sort(table.begin(), table.end(), [](Partion x, Partion y)->bool {return x.partion_size > y.partion_size; });
	int num = 0;
	// 对每个process使用Worst Fit算法进行分区分配
	for (Process& p : p_size_a) {
		int index = Find(table, p.p_size);
		if (index == -1) return false;
		// 修改数据
		this->Amend(table, p_size_a, index, num++);
		// 重新排序
		sort(table.begin(), table.end(), [](Partion x, Partion y)->bool {return x.partion_size > y.partion_size; });
	}
	return true;
}