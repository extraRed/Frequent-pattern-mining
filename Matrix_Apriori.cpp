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
const double min_sup=0.005;
const int T=8124;//88162//100000	//事务集大小 
const int I=120;//16469//1000		//物品集大小 
int min_num;
int tsize,isize;
int tot;
bool visited[I+1];
vector< vector<int> > booldata;
vector< vector<int> > oridata;	//事务集 
vector< vector<int> > can;	//候选集 
vector<int> tempcan(I); 
vector<int> tempfre(I);
vector< vector<int> > frequent;	//频繁集 
int level;
void Get_one_item()
{
	vector<int> temp;
	booldata.resize(T+2);
	int templ,i,j;
	tsize=T,isize=I;
	for(i=0;i<=T+1;++i)
	{
		booldata[i].resize(I+1);
		for(j=0;j<=I;++j)
		{
			if(i==0)
				booldata[i][j]=j;
			else
				booldata[i][j]=0;
		}
	}
	booldata[0][0]=-1;
	booldata[T][0]=-1;
	for(i=1;i<=T;++i)
	{
		templ=oridata[i-1].size();
		booldata[i][0]=templ;
		for(j=0;j<templ;++j)
		{
			booldata[i][oridata[i-1][j]]=1;	
			++booldata[T+1][oridata[i-1][j]];
		}
	}
	//对比矩阵sum_c行，如果某元素值小于min_num则删除矩阵中该元素对应的列，余下的项都是频繁1一项集
	//重新计算矩阵的sum_r列，如果某元素值等于0，则删除矩阵中该元素对应的行。
	for(i=1;i<=isize;++i)
	{
		if(booldata[T+1][i]<min_num)
		{
			for(j=0;j<=T+1;++j)
			{
				if(j!=T+1 && j!=0)
					booldata[j][0]-=booldata[j][i];
				booldata[j].erase(booldata[j].begin()+i);
			}
			--isize;
			--i;
		}
		else
		{
			temp.clear();
			temp.push_back(booldata[0][i]);
			frequent.push_back(temp);
			printf("%d %d\n",booldata[0][i],booldata[T+1][i]);
		}
	}
	for(i=1;i<=tsize;++i)
	{
		if(booldata[i][0]==0)
		{
			booldata.erase(booldata.begin()+i);
			--tsize;
			--i;
		}
	}
}	 

void shorten()
{
	//压缩矩阵。删除在频繁(level一1)项集中出现次数小于(level一1)的项I对应的矩阵列;
	int val_cnt[I+1],i,j;
	memset(val_cnt,0,sizeof(val_cnt));
	int fresiz=frequent.size();
	for(i=0;i<fresiz;++i)
		for(j=0;j<level-1;++j)
			++val_cnt[frequent[i][j]];

	for(i=1;i<=isize;++i)
	{
		if(val_cnt[booldata[0][i]]<level-1)				//删去不必要的一列 
		{
			for(j=0;j<=tsize+1;++j)
			{
				if(j!=tsize+1 && j!=0)
					booldata[j][0]-=booldata[j][i];
				booldata[j].erase(booldata[j].begin()+i);
			}
			--isize;
			--i;
		}
	}
	//重新计算矩阵的sum_r列，如果某元素值小于level，则删除矩阵中该元素对应的行;
	//重新计算矩阵的sum_c行，如果某元素值小于min_num，则删除矩阵中该元素对应的列;
	//反复压缩矩阵，直到不能再压缩		
	bool cut;
	while(1)
	{
		cut=false;		
		for(i=1;i<=tsize;++i)			//重新计算每一个事项集的长度 
		{
			if(booldata[i][0]<level)		//删去不必要的行 
			{
				cut=true;
				for(int j=1;j<=isize;++j)
					booldata[tsize+1][j]-=booldata[i][j];
				booldata.erase(booldata.begin()+i);
				--tsize;
				--i;
			}
			if(tsize<min_num)
				break;
		}
		if(tsize<min_num || cut==false)
			break;
		cut=false;
		for(i=1;i<=isize;++i)			//重新计算某一物品出现的总次数 
		{
			if(booldata[tsize+1][i]<min_num)	//删去不必要的列 
			{
				cut=true;
				for(int j=0;j<=tsize+1;++j)
				{
					if(j!=tsize+1 && j!=0)
						booldata[j][0]-=booldata[j][i];
					booldata[j].erase(booldata[j].begin()+i);
				}
				--isize;
				--i;
			}
			if(isize<level)
				break;
		}
		if(isize<level || cut==false)
			break;
	}
}

void Gen_can(int pi,int st)		//枚举产生候选集 
{
	for(int i=pi;i<=isize;++i)
	{
		if(st==level)
		{
			can.push_back(tempcan); 
			return ;
		}
		if(visited[i]==false)
		{
			visited[i]=true;
			tempcan[st]=i;
			Gen_can(i,st+1);
			visited[i]=false;	
		}
	}
}
void Get_freq()				//产生频繁集 
{
	int i,j,k,canlen=can.size(),sup_cnt,ci;
	for(i=0;i<canlen;++i) 	//对每一个候选项 
	{
		sup_cnt=0;
		for(j=1;j<=tsize;++j)
		{
		 	ci=1;
		 	for(k=0;k<level;++k)		//相当于做“&”操作 
		 	{
				ci&=booldata[j][can[i][k]];
				if(ci==0)
					break;
			}
			sup_cnt+=ci;
			if(sup_cnt+tsize-j+1<min_num)
				break;
		}
		if(sup_cnt>=min_num)			//找到一个频繁集 
		{
			for(k=0;k<level;++k)
			{
				tempfre[k]=booldata[0][can[i][k]];
				printf("%d ",tempfre[k]);
			}
			printf(": %d\n",sup_cnt);
			frequent.push_back(tempfre);
		}		
	}
}
int main()
{	
	int st=clock(),en;
	//freopen("mushroom.txt","w",stdout);
	//freopen("retail.txt","w",stdout);
	//freopen("T10I4D100K.txt","w",stdout);
	ifstream infile("mushroom.dat");
	//ifstream infile("retail.dat");
	//ifstream infile("T10I4D100K.dat");
	int sum=0,i=0;
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
		oridata.push_back(t);
	}
	min_num=int((double)(i*min_sup));
	Get_one_item();		//得到1-频繁集 
	tot+=frequent.size();
	if(frequent.empty())
	{
		//printf("no frequent 1-item!\nno any cases!\nthe end!\n");
		en=clock();
		printf("total time cost: %d\n",en-st);
		return 0;
	}		
	level=1;			//level-频繁集 	
	while(frequent.size()>=level+1)
	{
		++level;
		shorten();		//对矩阵进行压缩 
		if(isize<level || tsize<min_num)
		{
			//printf("the end!\nno %d frequency set!\n",level);
			break;
		}
		memset(visited,0,sizeof(visited));
		can.clear();
		Gen_can(1,0);			//生成所有候选集 
		frequent.clear();
		Get_freq();
		tot+=frequent.size();
	}			
	en=clock();
	//printf("total cases: %d\n",tot);
	printf("total time cost: %.3f second(s)\n",float(en-st)/1000);
	return 0;
}
