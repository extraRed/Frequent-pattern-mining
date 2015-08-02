// 我真诚地保证：
// 我自己独立地完成了整个程序从分析、设计到编码的所有工作。
// 如果在上述过程中，我遇到了什么困难而求教于人，那么，我将在程序实习报告中
// 详细地列举我所遇到的问题，以及别人给我的提示。
// 在此，我感谢 冯诺依曼 图灵 Knuth(D.E.Knuth)、Morris(J.H.Morris）
// 和Pratt(V.R.Pratt）张铭老师对我的启发和帮助。下面的报告中，我还会具体地提到
// 他们在各个方法对我的帮助。
// 我的程序里中凡是引用到其他程序或文档之处，
// 例如教材、课堂笔记、网上的源代码以及其他参考书上的代码段,
// 我都已经在程序的注释里很清楚地注明了引用的出处。
// 我从未抄袭过别人的程序，也没有盗用别人的程序，
// 不管是修改式的抄袭还是原封不动的抄袭。
// 我编写这个程序，从来没有想过要去破坏或妨碍其他计算机系统的正常运转。
// 刘子渊 刘敏行 
#include<algorithm>
#include<windows.h>
#include<iostream>
#include<fstream>
#include<cstring>
#include<cstdio>
using namespace std;
char *data,*file[3] = {"retail.dat","mushroom.dat","T10I4D100K.dat"};
char *out[3] = {"retailOptimizedVER.txt","mushroomOptimizedVER.txt","T10I4D100KOptimizedVER.txt"};
int ObNum[3] = {16469,120,1000},dataNum[3] = {88162,8124,100000};
int minsup,AnsNum = 0,t,maxlevel = 0;
FILE *fin,*fout;
struct Data{
	int data[110];
	int len; 
}trans[100010],dataIn[110],var;
void println(Data x){
	for(int i = 0;i<x.len;i++)
		printf("%d ",x.data[i]);
	printf("\n");
	return;	
}
struct Pattern{
	int data[110];
	int len;
	int count;
	Pattern():len(0),count(0){}
	Pattern(int x,int cnt):len(1),count(cnt){
		data[0] = x;
	}
}x[110],ans[4000010];
bool PatternCmp(const Pattern &a,const Pattern &b){
	if(a.len<b.len) return true;
	else if(a.len==b.len&&a.count<b.count) return true;
	return false;	
}
void println(Pattern x){
	for(int i = x.len-1;i>=0;i--)
		printf("%d ",x.data[i]);
	printf("\n");
	return;	
}
void println(Pattern x,FILE *f){
	for(int i = x.len-1;i>=0;i--)
		fprintf(f,"%d ",x.data[i]);
	fprintf(f," : %d\n",x.count);
	return;	
}
struct FPT{
	int val,count;
	FPT *father,*child,*sibling,*pre;
	FPT():val(-1),count(0){
		father = child = sibling = pre = NULL;
	}
	FPT(int v):val(v),count(0){
		father = child = sibling = pre = NULL;
	}
	FPT(FPT* f,int v):val(v),count(0){
		father = f;
		child = sibling = pre = NULL;
	}
}*root[110];
class Table{
	public:
		int tot,id,add;	
		FPT* last;
		Table(){};
		Table(int v):id(v),tot(0){
			last = new FPT(v);
		}
}table[110][20010];
bool TableCmp(const Table &a,const Table &b){
	return a.tot>b.tot;
}
bool DataCmp(const int &a,const int &b){
	return table[0][a].tot<table[0][b].tot;	
}
void insert(Data datas,int level){
	int len = datas.len,num,i;
	bool check = false;
	FPT *Cursor = root[level];
	for(i = len-1;i>=0;i--){
		num = datas.data[i];
		if(Cursor->child==NULL){
			Cursor->child = new FPT(Cursor,num);
			Cursor = Cursor->child;
			Cursor->pre = table[level][num].last;
			table[level][num].last = Cursor;
		} else {
			Cursor = Cursor->child;
			while(Cursor->sibling!=NULL&&Cursor->val!=num)
				Cursor = Cursor->sibling;	
			if(Cursor->val!=num){
				Cursor->sibling = new FPT(Cursor->father,num);
				Cursor = Cursor->sibling;
				Cursor->pre = table[level][num].last;
				table[level][num].last = Cursor;
			}	
		}
		if(level==0) Cursor->count++;
		else {
			Cursor->count += table[level][num].add;
			table[level][num].add = 0;
		}
	}
	return;
}
void RecursiveUp(FPT* now,int level){
	int k = 0;
	FPT *probe = now->father;
	while(probe!=NULL&&probe->val!=-1){
		var.data[k++] = probe->val;
		table[level][probe->val].tot += now->count;
		table[level][probe->val].add = now->count;
		probe = probe->father;
	}
	var.len = k;
	return ;
}
void TreeDestruction(int level){
	FPT *test;
	int i;
	for(i = 0;i<=ObNum[t];i++){
		while(table[level][i].last!=NULL){
			test = table[level][i].last;
			table[level][i].last = test->pre;
			delete test;
		}
	}
	return;
}
void FixIt(int left,int len,Pattern xx){
	//枚举当前所有祖先节点的组合 
	if(xx.len!=1) ans[AnsNum++] = xx;
	if(left>=len) return;
	int i;
	Pattern temp = xx;
	for(int i = left;i<len;i++){	
		temp.data[temp.len++] = var.data[i];
		FixIt(i+1,len,temp);
		temp.len--;
	}
	return;
}
void search(int level){
	int i,j;
	if(x[level].len>1) ans[AnsNum++] = x[level]; //记录频繁项； 
	if(root[level]->child==NULL) return;
	int now = level+1,k; 
	if(now>maxlevel){ //如果当前手工栈还没进行过初始化就进行初始化 
		for(i = 0;i<=ObNum[t];i++)
			table[now][i] = Table(i);
		maxlevel = now;	
	} 
	FPT *test;
	x[now] = x[level];
	for(i = ObNum[t];i>=0;i--){
		if(table[level][i].tot<minsup) continue;
		root[now] = new FPT();
		for(j = 0;j<=ObNum[t];j++){
			table[now][j].tot = 0;
			table[now][j].id = j;
			table[now][j].last = new FPT(j);
		}
		test = table[level][i].last;
		k = 0;
		while(test->pre!=NULL){ //对当前枚举的每一项进行上溯得到其前缀树（条件模式基） 
			RecursiveUp(test,now);
			insert(var,now); //插入函数 
			test = test->pre;
			k++;
		}
		sort(table[now],table[now]+ObNum[t]+1,TableCmp);
		x[now].data[x[now].len++] = table[level][i].id;
		//更新当前频繁项的信息； 
		if(x[now].count) x[now].count = min(x[now].count,table[level][i].tot);
		else x[now].count = table[level][i].tot;
		if(k==1) 
			FixIt(0,var.len,x[now]);
		else 
			search(now);
		x[now].len--;
		x[now].count = x[level].count;
		TreeDestruction(now);
		delete root[now];
	}
	return;
}
void dataOper(int k){
	int len = strlen(data),num = 0,ptr = 0,i;
	bool check = false;
	for(i = 0;i<len;i++){
		if(data[i]<='9'&&data[i]>='0'){ //获得以空格分割的int型变量的值 
			num = num*10+data[i]-'0';
			check = true;
		} else if(check){ //将值保存下来 
			trans[k].data[ptr++] = num;
			table[0][num].tot++;
			num = 0;
			check = false;
		}
	}
	trans[k].len = ptr;	
	return;
}
void BuildFPT(int k){
	int i,temp = 0;
	for(i = 0;i<k;i++){
		temp = 0;
		//对原始数据每次交易中的item按照总出现频数进行排序 
		sort(trans[i].data,trans[i].data+trans[i].len,DataCmp);
		//调用插入函数  
		insert(trans[i],0);
	}
}
int main(){
	double temp;
	int i,k = 0;
	clock_t start,end,end1,end2;
	scanf("%d",&t);  //输入要测试的文件编号 
	scanf("%lf",&temp); //输入阈值 
	temp = temp/100.0; //计算最小支持度 
	minsup = (int)(temp*dataNum[t]);
	start = clock(); //开始计时 
	fin = fopen(file[t],"rb"); //打开输入文件 
	fout = fopen(out[t],"w"); //打开输出文件 
	for(int i = 0;i<=ObNum[t];i++)
		table[0][i] = Table(i);	//表项初始化 
	data = (char *)malloc(sizeof(char)*5010); //给输入字符串分配内存； 
	root[0] =(FPT*)malloc(sizeof(FPT)); //初始化根节点 
	*root[0] = FPT();
	while(fgets(data,5010,fin)){ //每次读一行即一个交易 
		if(data==NULL) break;
		dataOper(k++); //调用字符串处理函数 
	}
	fclose(fin); //关闭输入文件； 
	BuildFPT(k); //建树 
	sort(table[0],table[0]+ObNum[t]+1,TableCmp); //对表项进行排序 
	for(i = 0;i<=ObNum[t];i++)
		if(table[0][i].tot>=minsup)
			//记录长度为1的频繁项 
			ans[AnsNum++] = Pattern(table[0][i].id,table[0][i].tot);  
	x[0] = Pattern(); //初始化频繁项 
	search(0); //调用数据挖掘函数 
	end = clock(); //第一个计时结束 
	sort(ans,ans+AnsNum,PatternCmp); //对所有频繁项进行排序 
	end1 = clock(); //第二个计时结束 
	for(i = 0;i<AnsNum;i++)
		println(ans[i],fout); //输出所有频繁项； 
	end2 = clock(); //全部计时结束 
	//输出三次计时的时间 
	fprintf(fout,"TIME:%.3lf\n",(double)(end-start)/1000.000);
	fprintf(fout,"TIME AFTER SORT:%.3lf\n",(double)(end1-start)/1000.000);
	fprintf(fout,"TIME AFTER SORT AND OUTPUT:%.3lf\n",(double)(end2-start)/1000.000);
	fclose(fout); 
	return 0;
}
