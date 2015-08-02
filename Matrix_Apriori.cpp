// ����ϵر�֤��
// ���Լ��������������������ӷ�������Ƶ���������й�����
// ��������������У���������ʲô���Ѷ�������ˣ���ô���ҽ��ڳ���ʵϰ������
// ��ϸ���о��������������⣬�Լ����˸��ҵ���ʾ��
// �ҵĳ������з������õ�����������ĵ�֮����
// ����̲ġ����ñʼǡ����ϵ�Դ�����Լ������ο����ϵĴ����,
// �Ҷ��Ѿ��ڳ����ע����������ע�������õĳ�����
// �Ҵ�δ��Ϯ�����˵ĳ���Ҳû�е��ñ��˵ĳ���
// �������޸�ʽ�ĳ�Ϯ����ԭ�ⲻ���ĳ�Ϯ��
// �ұ�д������򣬴���û�����Ҫȥ�ƻ���������������ϵͳ��������ת��
// ������ ����Ԩ 
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
const int T=8124;//88162//100000	//���񼯴�С 
const int I=120;//16469//1000		//��Ʒ����С 
int min_num;
int tsize,isize;
int tot;
bool visited[I+1];
vector< vector<int> > booldata;
vector< vector<int> > oridata;	//���� 
vector< vector<int> > can;	//��ѡ�� 
vector<int> tempcan(I); 
vector<int> tempfre(I);
vector< vector<int> > frequent;	//Ƶ���� 
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
	//�ԱȾ���sum_c�У����ĳԪ��ֵС��min_num��ɾ�������и�Ԫ�ض�Ӧ���У����µ����Ƶ��1һ�
	//���¼�������sum_r�У����ĳԪ��ֵ����0����ɾ�������и�Ԫ�ض�Ӧ���С�
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
	//ѹ������ɾ����Ƶ��(levelһ1)��г��ִ���С��(levelһ1)����I��Ӧ�ľ�����;
	int val_cnt[I+1],i,j;
	memset(val_cnt,0,sizeof(val_cnt));
	int fresiz=frequent.size();
	for(i=0;i<fresiz;++i)
		for(j=0;j<level-1;++j)
			++val_cnt[frequent[i][j]];

	for(i=1;i<=isize;++i)
	{
		if(val_cnt[booldata[0][i]]<level-1)				//ɾȥ����Ҫ��һ�� 
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
	//���¼�������sum_r�У����ĳԪ��ֵС��level����ɾ�������и�Ԫ�ض�Ӧ����;
	//���¼�������sum_c�У����ĳԪ��ֵС��min_num����ɾ�������и�Ԫ�ض�Ӧ����;
	//����ѹ������ֱ��������ѹ��		
	bool cut;
	while(1)
	{
		cut=false;		
		for(i=1;i<=tsize;++i)			//���¼���ÿһ������ĳ��� 
		{
			if(booldata[i][0]<level)		//ɾȥ����Ҫ���� 
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
		for(i=1;i<=isize;++i)			//���¼���ĳһ��Ʒ���ֵ��ܴ��� 
		{
			if(booldata[tsize+1][i]<min_num)	//ɾȥ����Ҫ���� 
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

void Gen_can(int pi,int st)		//ö�ٲ�����ѡ�� 
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
void Get_freq()				//����Ƶ���� 
{
	int i,j,k,canlen=can.size(),sup_cnt,ci;
	for(i=0;i<canlen;++i) 	//��ÿһ����ѡ�� 
	{
		sup_cnt=0;
		for(j=1;j<=tsize;++j)
		{
		 	ci=1;
		 	for(k=0;k<level;++k)		//�൱������&������ 
		 	{
				ci&=booldata[j][can[i][k]];
				if(ci==0)
					break;
			}
			sup_cnt+=ci;
			if(sup_cnt+tsize-j+1<min_num)
				break;
		}
		if(sup_cnt>=min_num)			//�ҵ�һ��Ƶ���� 
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
	while(getline(infile,tempstr))//����һ�� 
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
	Get_one_item();		//�õ�1-Ƶ���� 
	tot+=frequent.size();
	if(frequent.empty())
	{
		//printf("no frequent 1-item!\nno any cases!\nthe end!\n");
		en=clock();
		printf("total time cost: %d\n",en-st);
		return 0;
	}		
	level=1;			//level-Ƶ���� 	
	while(frequent.size()>=level+1)
	{
		++level;
		shorten();		//�Ծ������ѹ�� 
		if(isize<level || tsize<min_num)
		{
			//printf("the end!\nno %d frequency set!\n",level);
			break;
		}
		memset(visited,0,sizeof(visited));
		can.clear();
		Gen_can(1,0);			//�������к�ѡ�� 
		frequent.clear();
		Get_freq();
		tot+=frequent.size();
	}			
	en=clock();
	//printf("total cases: %d\n",tot);
	printf("total time cost: %.3f second(s)\n",float(en-st)/1000);
	return 0;
}
