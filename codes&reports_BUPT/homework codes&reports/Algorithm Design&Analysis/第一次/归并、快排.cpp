#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <sys/time.h>
using namespace std;
#define MAXSIZE 20000//��ǰ��������ģ�� 
#define MAX 20000//��ǰ������ֵ 

typedef int KeyType,InfoType;
typedef struct RedType{
	KeyType key;//�ؼ��� 
	InfoType otherinfo;
}RedType; 
typedef struct SqList{//˳��洢�� 
	RedType r[MAXSIZE+1];//r[0]���û����ڱ���
	int length;//��ǰ���еĳ��ȣ� 
}SqList;
class sort{
	private:
		SqList L;//����L 		
		SqList tmp;//��ʱ���飻 
		int DD;//����� 
		float ADD;//ƽ������� 		 
	public:
		int depth;//�ݹ����
		void InitSqList(int n,int N);//�������г���Ϊn�����ֵΪN��Ԫ����ͬ��˳��ͬ��������У� 
		int MergeSort(int low,int high);//�ݹ�鲢���򣬲����صݹ���ȣ� 
		void Merge(int low,int mid,int high);//�鲢L[low..mid]��l[mid+1...high] 
		void MergeSort_not_recursion();//�ǵݹ鷽ʽ�Ĺ鲢����
		int QuickSort(int low,int high);//�������� 
		int	Partition(int low,int high); //��˳���L�е�������L.[low...high]���зǵݼ�����;
		int Randomized_quickSort(int low,int high);//�������ѡ�񻮷ֵĿ��ţ� 
		int RandomizedPartition(int low,int high);//�������ѡ�񣬶�˳���L�е�������L.[low...high]���зǵݼ�����; 
		void ShowSqList();//��ʾ��ʱ�����У�
		void swap_element(int i,int j); //����Ԫ�أ� 
		void show_DD_ADD_level();//��ʾDD,ADD���ݹ��Σ� 
};
int main()
{
	srand(time(0));
	sort s1,s2,s3,s4;
	
	int n=MAXSIZE,N=MAX;	
	cout<<"��ѡ������ķ�ʽ��1���ݹ�鲢����2���ǵݹ�ݹ�鲢����3����������4������������ֵĿ�������;5����ֹ����"<<endl;
	s1.InitSqList(n,N);
	s2=s3=s4=s1;
	int key;
	cin>>key;
	//��¼��ʵʱ�����ֹʱ�䣬���뼶�� 
	struct timeval t_start,t_end;
	while(key!=5)
	{
		
		switch(key){
			case 1:				
				cout<<"�ݹ�鲢����"<<endl;
				gettimeofday(&t_start,NULL);//����ʼʱ��				
				s1.depth=s1.MergeSort(1,MAXSIZE);
				gettimeofday(&t_end,NULL);//������ֹʱ�䣻				
				s1.show_DD_ADD_level();	//�������ȣ�ƽ������Ⱥ͵ݹ���ȣ�			
				break;
			case 2:				
				cout<<"�ǵݹ�ݹ�鲢����"<<endl;
				gettimeofday(&t_start,NULL);//����ʼʱ��	
				s2.MergeSort_not_recursion();
				gettimeofday(&t_end,NULL);	//������ֹʱ�䣻				
				s2.show_DD_ADD_level();	//�������ȣ�ƽ������Ⱥ͵ݹ���ȣ�							
				break;
			case 3:				
				cout<<"��������"<<endl;
				gettimeofday(&t_start,NULL);//����ʼʱ��	
				s3.depth=s3.QuickSort(1,MAXSIZE);
				gettimeofday(&t_end,NULL);//������ֹʱ�䣻	
				s3.show_DD_ADD_level();//�������ȣ�ƽ������Ⱥ͵ݹ���ȣ�											
				break;
			case 4:				
				cout<<"����������ֵĿ�������"<<endl;
				gettimeofday(&t_start,NULL);//����ʼʱ��	
				s4.depth=s4.Randomized_quickSort(1,MAXSIZE);	
				gettimeofday(&t_end,NULL);//������ֹʱ�䣻	
				s4.show_DD_ADD_level();//�������ȣ�ƽ������Ⱥ͵ݹ���ȣ�													
				break;
			default:
				break;
		}
		cout<<"����ʱ�䣺"<<t_end.tv_usec-t_start.tv_usec+1000000*(t_end.tv_sec-t_start.tv_sec)<<"΢��"<<endl;
		cin>>key;
	}
	return 0;
} 


int sort::MergeSort(int low,int high)//�ݹ�鲢���� 
{	
	if(low==high)
	{ 
		tmp.r[low]=L.r[low];
		return 1;//���ܼ����ֽ�ʱ���ݹ����Ϊ1�� 
	} 
	else
	{
		int mid=(low+high)/2;
		int depth_l=MergeSort(low,mid);//����������⣬���صݹ���ȣ� 
		int depth_r=MergeSort(mid+1,high);//����������� 
		Merge(low,mid,high);//�������ź��������ϲ�����һ�����飻 
		return depth_l>depth_r?depth_l+1:depth_r+1;
	}
} 
void sort::Merge(int low,int mid,int high)//�鲢L[low..mid]��l[mid+1...high] 
{
	int i;//���Ӷε������Ƚ�ָ������
	int j;//���Ӷε������Ƚ�ָ������
	int k;//Ŀ������d��ָ����㣬���ϵ���˳���tmp.r[ ]�е�Ԫ�ر�������L.r[k]

			
	for(i=low,j=mid+1,k=low;i<=mid&&j<=high;k++)
		if(L.r[i].key<=L.r[j].key)
			tmp.r[k]=L.r[i++];
		else
			tmp.r[k]=L.r[j++];
	while(i<=mid)
		tmp.r[k++]=L.r[i++];
	while(j<=high)
		tmp.r[k++]=L.r[j++];
	for(int i=low;i<=high;i++)
		L.r[i]=tmp.r[i];
}
void sort::MergeSort_not_recursion()//�ǵݹ鷽ʽ�Ĺ鲢����
{
	int buck=1,low,mid,high;//buckΪ��ǰ�����г��ȣ� 
	while(buck<L.length)
	{
		for(low=1;low+buck<=L.length;)
		{
			mid=low+buck-1;
			high=mid+buck;
			if(high>L.length)
				high=L.length;
			//��tmp.r[]�г���Ϊs�������жԺϲ�������L.r[]���ϲ���󣬳���buck���ӣ�Ϊ�´κϲ���׼��	
			Merge(low,mid,high);
			low=high+1;

		}
		buck=buck*2;
	}
}

int sort::QuickSort(int low,int high)
{	
	int sorted=1;
	//��������ɨ��a[p:r]��ȫ��Ԫ�أ��ж�a[p:r]�Ƿ�����ǵݼ�˳�������򲻽������� 
	for(int i=low;i<=high;i++)
		if(L.r[i].key>L.r[i+1].key)
		{ 			
			sorted=0;
			break; 
		} 
	if(low<high&&sorted==0)
	{		
		int pivotloc=Partition(low,high);//�ҵ�����㣻 
		int depth_l=QuickSort(low,pivotloc-1);//�������������� 
		int depth_r=QuickSort(pivotloc+1,high);//�������������� 
		return depth_l>depth_r?depth_l+1:depth_r+1;
	}
	else
		return 1;
}

int sort::Partition(int low,int high)//��˳���L�е�������L.[low...high]���зǵݼ�����;
{
	L.r[0]=L.r[low];
	int pivotkey=L.r[low].key;
	while(low<high)
	{
		while(high>low&&L.r[high].key>=pivotkey)//Ѱ��С�� pivotkey���±ꣻ
			high--;
		L.r[low]=L.r[high];
		while(high>low&&L.r[low].key<=pivotkey)
			low++;
		L.r[high]=L.r[low];
	}
	L.r[low]=L.r[0];//����ʼ��pivotkey��ŵ�low�У�ʹ���� L.[low...high]��߶�С��pivotkey�ұ߶�����pivotkey��
	return low;
}

	//���ѡ�񻮷�Ԫ��
int sort::Randomized_quickSort(int low,int high)
{
	int sorted=1; 
	//��������ɨ��a[p:r]��ȫ��Ԫ�أ��ж�a[p:r]�Ƿ�����ǵݼ�˳�������򲻽�������
	for(int i=low;i<=high;i++)
		if(L.r[i].key>L.r[i+1].key)
		{ 
			swap_element(i,i+1); 
			sorted=0;
			break; 
		} 
	if(low<high&&sorted==0)
	{
		int pivotloc=RandomizedPartition(low,high);//�ҵ�����㣻 
		int depth_l=QuickSort(low,pivotloc-1);//�������������� 
		int depth_r=QuickSort(pivotloc+1,high);//�������������� 
		return depth_l>depth_r?depth_l+1:depth_r+1;
	}
	else
		return 1;
}
int sort::RandomizedPartition(int low,int high)
{
	int x=rand()%(high-low+1)+low;//���ѡһ��L.r[x]
	swap_element(low,x);//L.r[x],�� L.r[low]������ 
	L.r[0]=L.r[low];
	int pivotkey=L.r[low].key;
	while(low<high)
	{
		while(high>low&&L.r[high].key>=pivotkey)//Ѱ��С�� pivotkey���±ꣻ
			high--;
		L.r[low]=L.r[high];
		while(high>low&&L.r[low].key<=pivotkey)
			low++;
		L.r[high]=L.r[low];
	}
	L.r[low]=L.r[0];//����ʼ��pivotkey��ŵ�low�У�ʹ���� L.[low...high]��߶�С��pivotkey�ұ߶�����pivotkey��
	return low;	
}
void sort::InitSqList(int n,int N)
{
	ADD=0;
	DD=0;
	L.length=n;//���еĳ��ȣ� 
	cout<<"��ʼ������"<<endl;
	//��˳������1-N������ 
	for(int i=1;i<=L.length;i++)
		L.r[i].key=i;
	for(int i=1;i<N;i++)
	{
		//����i������r[i+1...N]�����ĳ����������
		int j=rand()%(N-i)+i+1; 
		RedType tmp=L.r[i];
		L.r[i]=L.r[j];
		L.r[j]=tmp;
	}
		
	//��������DD��ֵ 
	for(int i=1;i<=L.length;i++)
		 for(int j=1;j<=L.length;j++)
		 	if(i<j&&L.r[i].key>L.r[j].key)
		 		DD++;
	ADD=DD/L.length; 
		 		
}

void sort::swap_element(int i,int j)//����Ԫ�� 
{
	
	RedType tmp=L.r[i];
	L.r[i]=L.r[j];
	L.r[j]=tmp;
}
void sort::show_DD_ADD_level()
{
	cout<<"DD="<<DD<<endl<<"ADD="<<ADD<<endl<<"�ݹ����="<<depth<<endl;
}


