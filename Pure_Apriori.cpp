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
const double min_sup=0.6; 
const int T=/*8124;*/88162;//100000	//���񼯴�С 
const int I=/*120;*/16469;//1000	//��Ʒ����С 
int min_num;					//��С��ֵ��Ŀ 
int tot;						//��Ƶ������ 
bool vis[T+1][I+1];				//�����Ʒi�Ƿ��ڵ�j�������г��� 
vector< vector<int> > oridata;	//���� 
vector< vector<int> > candidate;//��ѡ�� 
vector< vector<int> > frequent;	//Ƶ���� 
int count_freq[I+1];			//1-Ƶ�������� 
int level;						//Ƶ�������� 
vector<int> merged;				//����k-1Ƶ�������Ӷ��ɵ�k� 
void Get_one_item()
{
	int orisiz=oridata.size(),orixsiz;
	for(int x=0;x<orisiz;++x)
	{
		orixsiz=oridata[x].size();
		for(int y=0;y<orixsiz;++y)
		{
			vis[x][oridata[x][y]]=true;	
			++count_freq[oridata[x][y]];	//���� 
		}
	}
	vector<int> t;
	t.clear();
	for(int i=1;i<=I;++i)
	{
		if(count_freq[i]>=min_num)		//������ֵ 
		{
			printf("%d: %d\n",i,count_freq[i]);
			t.push_back(i);
			frequent.push_back(t);
			t.clear();
		}
	}
	//tot+=frequent.size();
}	 
bool check_frequent()	//������ѡ����k-1Ԫ�Ӽ��ǲ�����k-Ƶ������ 
{
	bool tag;
	vector<int> temp;
	for(int i=0;i<=level;++i)
	{
		tag=true;
		temp=merged;
		temp.erase(temp.begin()+i);	//�ֱ�����Ӽ� 
		int lef=0,righ=frequent.size()-1,mid;
		while(lef<=righ)			//���ֲ��� 
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
			if(tag)				//�ҵ��ˣ����� 
				break;
		}
		if(tag==false)			//û�ҵ���ֱ�ӷ���false 
			break;
	}
	return tag;
}	
	
void apriori_gen()				//����k-��ѡ�� 
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
			if(mer)							//�������� 
			{
				merged.clear();
				for(int k=0;k<level-1;++k)
					merged.push_back(frequent[i][k]);
				merged.push_back(frequent[i][level-1]);
				merged.push_back(frequent[j][level-1]);
												 
				if(check_frequent())		//�Ժ�ѡ����֦	
					candidate.push_back(merged);
			}
			else		//��ΪƵ����Ԫ���ڲ������������i��j�޷����ӣ���i��j֮�����϶��޷����� 
				break;
		}
}

void get_fre(int level)			//������k-��ѡ��ͳ��֧�ֶ� 
{
	bool check;
	int temp_cnt;
	int cansiz=candidate.size();
	for(int i=0;i<cansiz;++i)
	{
		temp_cnt=0;
		check=true;	
		for(int j=0;j<T;++j)	//�������в��� 
		{
			check=true;
			for(int k=0;k<level;++k)
			{
				if(vis[j][candidate[i][k]]==false)
				{
					check=false;		//û�ҵ� 
					break;
				}
			}
			if(check)					//�ҵ��� 
			{
				++temp_cnt;
				continue;
			}
			if(temp_cnt+T-j<min_num)	//��֦�������ʹ�������ȫ����Ҳ�޷��ﵽ��ֵ���˳� 
				break;
		}
		if(temp_cnt>=min_num)	
		{
			frequent.push_back(candidate[i]);	//����k-Ƶ���� 
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
		oridata.push_back(t);		//�������� 
	}
	min_num=int((double)(T*min_sup));
	Get_one_item();				//�õ�1-Ƶ���� 
	if(frequent.empty())		//1-Ƶ����Ϊ�� 
	{
		int en=clock();
		printf("total time cost: %.3f second(s)\n",float(en-st)/1000);
		//printf("no frequent 1-item!\n");
		return 0;
	}		
	level=1;					//level-Ƶ���� 	
	while(frequent.size()>=level+1)		//�������Ƶ������СС��k+1,��Ҳ��������k+1Ƶ������ 
	{
		candidate.clear();
		apriori_gen();			//�õ�k-��ѡ�� 
		if(candidate.empty())	//��ѡ��Ϊ�� 
		{
			//printf("no %d level frequent members!\nthe end!\n",level+1);
			break;
		}
		frequent.clear();
		get_fre(level+1);		//�õ�k-Ƶ���� 
		if(frequent.empty())	//Ƶ����Ϊ�� 
		{
			//printf("no %d level frequent members!\nthe end!\n",level+1);
			break;
		}
		++level;
		//tot+=frequent.size();
		shorten_fre();			//Ƶ������֦ 
	}
	//printf("total cases: %d\n",tot);			
	int en=clock();
	printf("total time cost: %.3f second(s)\n",float(en-st)/1000);
	return 0;
}
