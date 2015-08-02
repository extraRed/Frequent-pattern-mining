// 我真诚地保证：
// 我自己独立地完成了整个程序从分析、设计到编码的所有工作。
// 如果在上述过程中，我遇到了什么困难而求教于人，那么，我将在程序实习报告中
// 详细地列举我所遇到的问题，以及别人给我的提示。
// 我的程序里中凡是引用到其他程序或文档之处，
// 例如教材、课堂笔记、网上的源代码以及其他参考书上的代码段,
// 我都已经在程序的注释里很清楚地注明了引用的出处。
// 我从未抄袭过别人的程序，也没有盗用别人的程序，
// 不管是修改式的抄袭还是原封不动的抄袭。
// 我编写这个程序，从来没有想过要去破坏或妨碍其他计算机系统的正常运转。
// 刘敏行 刘子渊 
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<map>
#include<cstdlib>
#include<cstring>
#include<time.h>
using namespace std;
const double min_sup=0.6; 
const int T=/*8124;*/88162;//100000	//事务集大小 
const int I=/*120;*/16469;//1000	//物品集大小 
int min_num;					//最小阈值数目 
int tot;						//总频繁集数 
bool vis[T+1][I+1];				//标记物品i是否在第j个事务集中出现 
vector< vector<int> > oridata;	//事务集 
vector< vector<int> > candidate;//候选集 
vector< vector<int> > frequent;	//频繁集 
int count_freq[I+1];			//1-频繁集计数 
int level;						//频繁集长度 
vector<int> merged;				//两个k-1频繁集连接而成的k项集 
void Get_one_item()
{
	int orisiz=oridata.size(),orixsiz;
	for(int x=0;x<orisiz;++x)
	{
		orixsiz=oridata[x].size();
		for(int y=0;y<orixsiz;++y)
		{
			vis[x][oridata[x][y]]=true;	
			++count_freq[oridata[x][y]];	//计数 
		}
	}
	vector<int> t;
	t.clear();
	for(int i=1;i<=I;++i)
	{
		if(count_freq[i]>=min_num)		//大于阈值 
		{
			printf("%d: %d\n",i,count_freq[i]);
			t.push_back(i);
			frequent.push_back(t);
			t.clear();
		}
	}
	//tot+=frequent.size();
}	 
bool check_frequent()	//看看候选集的k-1元子集是不是在k-频繁集中 
{
	bool tag;
	vector<int> temp;
	for(int i=0;i<=level;++i)
	{
		tag=true;
		temp=merged;
		temp.erase(temp.begin()+i);	//分别产生子集 
		int lef=0,righ=frequent.size()-1,mid;
		while(lef<=righ)			//二分查找 
		{
			tag=true;
			mid=(lef+righ)/2;
			for(int j=0;j<level;++j)
			{
				if(temp[j]>frequent[mid][j])
				{
					lef=mid+1;
					tag=false;
					break;
				}
				else if(temp[j]<frequent[mid][j])
				{
					righ=mid-1;
					tag=false;
					break;
				}	
			}
			if(tag)				//找到了，继续 
				break;
		}
		if(tag==false)			//没找到，直接返回false 
			break;
	}
	return tag;
}	
	
void apriori_gen()				//生成k-候选集 
{
	bool mer;
	int fresiz=frequent.size();
	for(int i=0;i<fresiz;++i)
		for(int j=i+1;j<fresiz;++j)
		{
			mer=true;
			for(int c=0;c<level-1;++c)
				if(frequent[i][c]!=frequent[j][c])
				{
					mer=false;
					break;
				}
			if(mer)							//可以连接 
			{
				merged.clear();
				for(int k=0;k<level-1;++k)
					merged.push_back(frequent[i][k]);
				merged.push_back(frequent[i][level-1]);
				merged.push_back(frequent[j][level-1]);
												 
				if(check_frequent())		//对候选集剪枝	
					candidate.push_back(merged);
			}
			else		//因为频繁集元素内部有序，所以如果i和j无法连接，则i和j之后的项肯定无法连接 
				break;
		}
}

void get_fre(int level)			//对所有k-候选集统计支持度 
{
	bool check;
	int temp_cnt;
	int cansiz=candidate.size();
	for(int i=0;i<cansiz;++i)
	{
		temp_cnt=0;
		check=true;	
		for(int j=0;j<T;++j)	//在事务集中查找 
		{
			check=true;
			for(int k=0;k<level;++k)
			{
				if(vis[j][candidate[i][k]]==false)
				{
					check=false;		//没找到 
					break;
				}
			}
			if(check)					//找到了 
			{
				++temp_cnt;
				continue;
			}
			if(temp_cnt+T-j<min_num)	//剪枝，如果即使假设后面全出现也无法达到阈值，退出 
				break;
		}
		if(temp_cnt>=min_num)	
		{
			frequent.push_back(candidate[i]);	//加入k-频繁集 
			for(int w=0;w<level;++w)
				printf("%d ",candidate[i][w]);
			printf(": %d\n",temp_cnt);
		}
	}
}
void shorten_fre()
{
	bool del;
	int val_cnt[I+1],i,j;
	memset(val_cnt,0,sizeof(val_cnt));
	int fresiz=frequent.size();
	for(i=0;i<fresiz;++i)
		for(j=0;j<level-1;++j)
			++val_cnt[frequent[i][j]];
	//如果一个事物在k-频繁集中出现的频次小于k，则把所有含有该事物的频繁集删掉
	//因为它不可能对产生(k+1)-频繁集有用 
	for(i=0;i<fresiz;++i)
	{
		del=false;
		for(j=0;j<level-1;++j)
		{
			if(val_cnt[frequent[i][j]]<level-1)
			{
				del=true;
				break;
			}
		}
		if(del)
		{
			for(j=0;j<level-1;++j)
				--val_cnt[frequent[i][j]];
			frequent.erase(frequent.begin()+i);
			--i;
			--fresiz;
		}
	}
}
int main()
{	
	int st=clock();
	//freopen("mushroom.txt","w",stdout);
	//freopen("retail.txt","w",stdout);
	//freopen("T10I4D100K.txt","w",stdout);
	freopen("temp.txt","w",stdout); 
	//ifstream infile("D:\\mushroom.dat");
	ifstream infile("D:\\retail.dat");
	//ifstream infile("D:\\T10I4D100K.dat");
	int sum=0;
	vector<int> t;
	string tempstr;
	while(getline(infile,tempstr))//输入一行 
	{
		t.clear();
		int l=tempstr.length();
		for(int i=0;i<l;++i)
		{
			if(tempstr[i]==' ')
			{
				t.push_back(sum);
				sum=0;
			}
			else
				sum=10*sum+tempstr[i]-'0';
		}
		oridata.push_back(t);		//放入事务集 
	}
	min_num=int((double)(T*min_sup));
	Get_one_item();				//得到1-频繁集 
	if(frequent.empty())		//1-频繁集为空 
	{
		int en=clock();
		printf("total time cost: %.3f second(s)\n",float(en-st)/1000);
		//printf("no frequent 1-item!\n");
		return 0;
	}		
	level=1;					//level-频繁集 	
	while(frequent.size()>=level+1)		//如果现在频繁集大小小于k+1,那也不可能有k+1频繁集了 
	{
		candidate.clear();
		apriori_gen();			//得到k-候选集 
		if(candidate.empty())	//候选集为空 
		{
			//printf("no %d level frequent members!\nthe end!\n",level+1);
			break;
		}
		frequent.clear();
		get_fre(level+1);		//得到k-频繁集 
		if(frequent.empty())	//频繁集为空 
		{
			//printf("no %d level frequent members!\nthe end!\n",level+1);
			break;
		}
		++level;
		//tot+=frequent.size();
		shorten_fre();			//频繁集剪枝 
	}
	//printf("total cases: %d\n",tot);			
	int en=clock();
	printf("total time cost: %.3f second(s)\n",float(en-st)/1000);
	return 0;
}
