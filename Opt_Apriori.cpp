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
#include<cstdlib>
#include<cstring>
#include<time.h>
using namespace std;
const double min_sup=0.05;
const int T=8124;//88162//100000	//���񼯴�С 
const int I=120;//16469//1000		//��Ʒ����С 
int min_num;				//��С��ֵ��Ŀ 
int tot;					//��Ƶ������ 
vector< vector<int> > fre;		 //k+1-Ƶ���� 
vector< vector<int> > frequent;	//k-Ƶ���� 
vector<unsigned int*> _Bitand;	//k+1-�м� 
vector<unsigned int*> Bitand;	//k�м� 
int count_freq[I+1];			//1-�����
vector< vector<unsigned int> > tid;	//��ÿһ����Ʒͳ�����������кŲ��洢  
int level;						//����Ƶ�������� 
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
			unsigned int *Btemp=new unsigned int[T/32+1];	//��һ���к�λѹ����һ��unsigned int������ 
			for(int ii=0;ii<=T/32;++ii)
			//	Btemp[bcnt][ii]=0;
				Btemp[ii]=0; 
			int tidsiz=tid[i].size();
			for(int j=0;j<tidsiz;++j)
			//	Btemp[bcnt][tid[i][j]/32]+=1<<(tid[i][j]%32);
				Btemp[tid[i][j]/32]+=1<<(tid[i][j]%32);
			//Bitand.push_back(Btemp[bcnt]);					
			Bitand.push_back(Btemp); 						//����1-�м�
			//++bcnt;
			printf("%d : %d\n",i,count_freq[i]);	
			t.push_back(i);
			frequent.push_back(t);							//֧�ֶ�����Ҫ��ķ���1-Ƶ���� 
			t.clear();
		}
	}
	//tot+=frequent.size();
}	 	
void apriori_gen()			//��(k-1)Ƶ����ֱ������kƵ���� 
{
	vector<int> merged;		//������(k-1)Ƶ�������ӳɵ��º�ѡƵ���� 
	bool mer;
	int fresiz=frequent.size();
	for(int i=0;i<fresiz;++i)	//��(k-1)Ƶ��������� 
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
			if(mer)							//�������� 
			{
				merged.clear();
				for(int k=0;k<level-1;++k)
					merged.push_back(frequent[i][k]);
				merged.push_back(frequent[i][level-1]);
				merged.push_back(frequent[j][level-1]);
				int tid_cnt=0;
				unsigned int *vv=new unsigned int[T/32+1];	//������Ƶ������к������������
															//ͳ���µĵ����к�������1�ĸ�����֪��֧�ֶ� 
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
					fre.push_back(merged);			//����k-Ƶ���� 
					//_Bitand.push_back(Btemp[bcnt]);	
					_Bitand.push_back(vv);	//����k-�м� 
					//++bcnt;
					for(int cc=0;cc<=level;++cc)
						printf("%d ",merged[cc]);
					printf(": %d\n",tid_cnt);
				} 
			}
			else			//��ΪƵ����Ԫ���ڲ������������i��j�޷����ӣ���i��j֮�����϶��޷����� 
				break;
		}
	}
}
void shorten_fre()			//�ü�k-Ƶ���� 
{
	bool del;
	int val_cnt[I+1],i,j;
	memset(val_cnt,0,sizeof(val_cnt));
	int fresiz=frequent.size();
	for(i=0;i<fresiz;++i)
		for(j=0;j<level-1;++j)
			++val_cnt[frequent[i][j]];
	//���һ��������k-Ƶ�����г��ֵ�Ƶ��С��k��������к��и������Ƶ����ɾ��
	//��Ϊ�������ܶԲ���(k+1)-Ƶ�������� 
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
	while(getline(infile,tempstr))//����һ��ת��vector<int>�������� 
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
	Get_one_item();		//�õ�1-Ƶ���� 
	if(frequent.empty())
	{
		printf("no frequent 1-item!\nthe end!\n");
		printf("total cases: %d\n",tot);			
		int en=clock();
		printf("total time cost: %.3f second(s)\n",float(en-st)/1000);
		return 0;
	}		
	level=1;			//level-Ƶ���� 	
	while(frequent.size()>=level+1)			//�������Ƶ������СС��k+1,��Ҳ��������k+1Ƶ������ 
	{
		apriori_gen();						//����Ƶ���� 
		if(fre.empty())						//û������Ƶ���� 
		{
			printf("no %d level frequent members!\nthe end!\n",level+1);
			break;
		}
		++level;			
		//tot+=fre.size();
		frequent=fre;						//���Ŀ��������һЩ�����й�λ 
		for(vector<unsigned int*>::iterator p=Bitand.begin();p!=Bitand.end();++p)
			delete *p;
		Bitand=_Bitand;
		_Bitand.clear();
		fre.clear();
		shorten_fre();						//Ƶ������֦ 
	}
	//printf("total cases: %d\n",tot);			
	int en=clock();
	printf("total time cost: %.3f second(s)\n",float(en-st)/1000);
	return 0;
}
