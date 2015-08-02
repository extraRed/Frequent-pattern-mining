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
#include<cstdlib>
#include<cstring>
#include<time.h>
using namespace std;
const double min_sup=0.05;
const int T=8124;//88162//100000	//事务集大小 
const int I=120;//16469//1000		//物品集大小 
int min_num;				//最小阈值数目 
int tot;					//总频繁集数 
vector< vector<int> > fre;		 //k+1-频繁集 
vector< vector<int> > frequent;	//k-频繁集 
vector<unsigned int*> _Bitand;	//k+1-行集 
vector<unsigned int*> Bitand;	//k行集 
int count_freq[I+1];			//1-项集计数
vector< vector<unsigned int> > tid;	//对每一个物品统计它所处的行号并存储  
int level;						//代表频繁集长度 
//unsigned int Btemp[20000][T/32+1];
//int bcnt;
void Get_one_item()
{
	vector<int> t;
	t.clear();
	for(int i=0;i<=I;++i)
	{
		if(count_freq[i]>=min_num)
		{
			unsigned int *Btemp=new unsigned int[T/32+1];	//将一列行号位压缩至一个unsigned int数组中 
			for(int ii=0;ii<=T/32;++ii)
			//	Btemp[bcnt][ii]=0;
				Btemp[ii]=0; 
			int tidsiz=tid[i].size();
			for(int j=0;j<tidsiz;++j)
			//	Btemp[bcnt][tid[i][j]/32]+=1<<(tid[i][j]%32);
				Btemp[tid[i][j]/32]+=1<<(tid[i][j]%32);
			//Bitand.push_back(Btemp[bcnt]);					
			Bitand.push_back(Btemp); 						//放入1-行集
			//++bcnt;
			printf("%d : %d\n",i,count_freq[i]);	
			t.push_back(i);
			frequent.push_back(t);							//支持度满足要求的放入1-频繁集 
			t.clear();
		}
	}
	//tot+=frequent.size();
}	 	
void apriori_gen()			//由(k-1)频繁集直接生成k频繁集 
{
	vector<int> merged;		//由两个(k-1)频繁项连接成的新候选频繁项 
	bool mer;
	int fresiz=frequent.size();
	for(int i=0;i<fresiz;++i)	//在(k-1)频繁集里查找 
	{
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
				int tid_cnt=0;
				unsigned int *vv=new unsigned int[T/32+1];	//将两个频繁项的行号向量做与操作
															//统计新的到的行号向量中1的个数得知其支持度 
				for(int cal=0;cal<=T/32;++cal)
				{
					vv[cal]=Bitand[i][cal] & Bitand[j][cal];
					unsigned int xx=vv[cal];
					//Btemp[bcnt][cal]=Bitand[i][cal] & Bitand[j][cal];
					//unsigned int xx=Btemp[bcnt][cal];
					while(xx)
					{
						if(xx&1)
							++tid_cnt;
						xx>>=1;
					}
				}	
				if(tid_cnt>=min_num)
				{
					fre.push_back(merged);			//存入k-频繁集 
					//_Bitand.push_back(Btemp[bcnt]);	
					_Bitand.push_back(vv);	//存入k-行集 
					//++bcnt;
					for(int cc=0;cc<=level;++cc)
						printf("%d ",merged[cc]);
					printf(": %d\n",tid_cnt);
				} 
			}
			else			//因为频繁集元素内部有序，所以如果i和j无法连接，则i和j之后的项肯定无法连接 
				break;
		}
	}
}
void shorten_fre()			//裁剪k-频繁集 
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
			Bitand.erase(Bitand.begin()+i);
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
	ifstream infile("mushroom.dat");
	//ifstream infile("retail.dat");
	//ifstream infile("T10I4D100K.dat");
	int sum=0,row=0;
	tid.resize(I+1); 
	string tempstr;
	while(getline(infile,tempstr))//输入一行转成vector<int>存入事务集 
	{
		int l=tempstr.length();
		for(int i=0;i<l;++i)
		{
			if(tempstr[i]==' ')
			{
				tid[sum].push_back(row);
				++count_freq[sum]; 
				sum=0;
			}
			else
				sum=10*sum+tempstr[i]-'0';
		}
		++row;
	}
	min_num=int((double)(T*min_sup));
	Get_one_item();		//得到1-频繁集 
	if(frequent.empty())
	{
		printf("no frequent 1-item!\nthe end!\n");
		printf("total cases: %d\n",tot);			
		int en=clock();
		printf("total time cost: %.3f second(s)\n",float(en-st)/1000);
		return 0;
	}		
	level=1;			//level-频繁集 	
	while(frequent.size()>=level+1)			//如果现在频繁集大小小于k+1,那也不可能有k+1频繁集了 
	{
		apriori_gen();						//生成频繁集 
		if(fre.empty())						//没能生成频繁集 
		{
			printf("no %d level frequent members!\nthe end!\n",level+1);
			break;
		}
		++level;			
		//tot+=fre.size();
		frequent=fre;						//项集数目提升，对一些量进行归位 
		for(vector<unsigned int*>::iterator p=Bitand.begin();p!=Bitand.end();++p)
			delete *p;
		Bitand=_Bitand;
		_Bitand.clear();
		fre.clear();
		shorten_fre();						//频繁集剪枝 
	}
	//printf("total cases: %d\n",tot);			
	int en=clock();
	printf("total time cost: %.3f second(s)\n",float(en-st)/1000);
	return 0;
}
