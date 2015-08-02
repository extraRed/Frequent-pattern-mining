// ����ϵر�֤��
// ���Լ��������������������ӷ�������Ƶ���������й�����
// ��������������У���������ʲô���Ѷ�������ˣ���ô���ҽ��ڳ���ʵϰ������
// ��ϸ���о��������������⣬�Լ����˸��ҵ���ʾ��
// �ڴˣ��Ҹ�л ��ŵ���� ͼ�� Knuth(D.E.Knuth)��Morris(J.H.Morris��
// ��Pratt(V.R.Pratt��������ʦ���ҵ������Ͱ���������ı����У��һ��������ᵽ
// �����ڸ����������ҵİ�����
// �ҵĳ������з������õ�����������ĵ�֮����
// ����̲ġ����ñʼǡ����ϵ�Դ�����Լ������ο����ϵĴ����,
// �Ҷ��Ѿ��ڳ����ע����������ע�������õĳ�����
// �Ҵ�δ��Ϯ�����˵ĳ���Ҳû�е��ñ��˵ĳ���
// �������޸�ʽ�ĳ�Ϯ����ԭ�ⲻ���ĳ�Ϯ��
// �ұ�д������򣬴���û�����Ҫȥ�ƻ���������������ϵͳ��������ת��
// ����Ԩ ������ 
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
	//ö�ٵ�ǰ�������Ƚڵ����� 
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
	if(x[level].len>1) ans[AnsNum++] = x[level]; //��¼Ƶ��� 
	if(root[level]->child==NULL) return;
	int now = level+1,k; 
	if(now>maxlevel){ //�����ǰ�ֹ�ջ��û���й���ʼ���ͽ��г�ʼ�� 
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
		while(test->pre!=NULL){ //�Ե�ǰö�ٵ�ÿһ��������ݵõ���ǰ׺��������ģʽ���� 
			RecursiveUp(test,now);
			insert(var,now); //���뺯�� 
			test = test->pre;
			k++;
		}
		sort(table[now],table[now]+ObNum[t]+1,TableCmp);
		x[now].data[x[now].len++] = table[level][i].id;
		//���µ�ǰƵ�������Ϣ�� 
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
		if(data[i]<='9'&&data[i]>='0'){ //����Կո�ָ��int�ͱ�����ֵ 
			num = num*10+data[i]-'0';
			check = true;
		} else if(check){ //��ֵ�������� 
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
		//��ԭʼ����ÿ�ν����е�item�����ܳ���Ƶ���������� 
		sort(trans[i].data,trans[i].data+trans[i].len,DataCmp);
		//���ò��뺯��  
		insert(trans[i],0);
	}
}
int main(){
	double temp;
	int i,k = 0;
	clock_t start,end,end1,end2;
	scanf("%d",&t);  //����Ҫ���Ե��ļ���� 
	scanf("%lf",&temp); //������ֵ 
	temp = temp/100.0; //������С֧�ֶ� 
	minsup = (int)(temp*dataNum[t]);
	start = clock(); //��ʼ��ʱ 
	fin = fopen(file[t],"rb"); //�������ļ� 
	fout = fopen(out[t],"w"); //������ļ� 
	for(int i = 0;i<=ObNum[t];i++)
		table[0][i] = Table(i);	//�����ʼ�� 
	data = (char *)malloc(sizeof(char)*5010); //�������ַ��������ڴ棻 
	root[0] =(FPT*)malloc(sizeof(FPT)); //��ʼ�����ڵ� 
	*root[0] = FPT();
	while(fgets(data,5010,fin)){ //ÿ�ζ�һ�м�һ������ 
		if(data==NULL) break;
		dataOper(k++); //�����ַ��������� 
	}
	fclose(fin); //�ر������ļ��� 
	BuildFPT(k); //���� 
	sort(table[0],table[0]+ObNum[t]+1,TableCmp); //�Ա���������� 
	for(i = 0;i<=ObNum[t];i++)
		if(table[0][i].tot>=minsup)
			//��¼����Ϊ1��Ƶ���� 
			ans[AnsNum++] = Pattern(table[0][i].id,table[0][i].tot);  
	x[0] = Pattern(); //��ʼ��Ƶ���� 
	search(0); //���������ھ��� 
	end = clock(); //��һ����ʱ���� 
	sort(ans,ans+AnsNum,PatternCmp); //������Ƶ����������� 
	end1 = clock(); //�ڶ�����ʱ���� 
	for(i = 0;i<AnsNum;i++)
		println(ans[i],fout); //�������Ƶ��� 
	end2 = clock(); //ȫ����ʱ���� 
	//������μ�ʱ��ʱ�� 
	fprintf(fout,"TIME:%.3lf\n",(double)(end-start)/1000.000);
	fprintf(fout,"TIME AFTER SORT:%.3lf\n",(double)(end1-start)/1000.000);
	fprintf(fout,"TIME AFTER SORT AND OUTPUT:%.3lf\n",(double)(end2-start)/1000.000);
	fclose(fout); 
	return 0;
}
