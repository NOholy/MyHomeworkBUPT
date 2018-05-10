#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<fstream>
#include<string.h>
using namespace std;
typedef float Type;
typedef struct data{
	int id;//��վid 
	Type x,y,k_dist;//xΪά�ȣ�yΪ���ȣ� 
}data;//��վ���� 
int depth=0;//�ݹ���ȣ�
int flag=0; 
void swap_element(Type a[],int i,int j);
void bubble_sort(Type a[],int p,int r);//����r-p������������ʱ��ð������ 
Type linear_select_3(Type a[],int p,int r,int k);//һ��Ϊ��������ʱ��ѡ���kС������ 
Type linear_select_2(Type a[],int p,int r,int k);//һ��Ϊ��������ʱ��ѡ���kС������ 
int linear_partition(Type a[],int low,int high,Type k);//���չؼ���k��a[]���ֳ����Ӷιؼ���С��k�����Ӷιؼ��ִ���k 
void read_file(string file_name,data node_b[],Type dist[]);//���ļ��ж�ȡ1033����վ���� 
data find_data(data node_b[],int num,Type key);//Ѱ��k_distΪkey�Ļ�վ��Ϣ��
int main()
{
	int n=1033;
	Type num[n];
	data node_b[n];
	string file_name="data.txt";//�ļ����� 
	read_file(file_name,node_b,num);//���ı���ȡ���ݵ��ڴ� 
	int rank[4]={1,5,50,n};//d��rank[i]С�Ļ�վ�� 
	cout<<"һ��Ϊ��"<<endl; 
	for(int i=0;i<4;i++)
	{ 
		depth=0;
		Type key=linear_select_3(num,0,n-1,rank[i]);//�ҵ����rank[i]С��k_dist
		data node_key=find_data(node_b,n,key);	//�ҵ����kС��k_dist��Ӧ�Ļ�վ��Ϣ�� 
		cout<<"�ݹ����="<<depth<<endl<<"��"<<rank[i]<<"С:"<<"id:["<<node_key.id<<"]"<<"γ��="<<node_key.x<<" ����="<<node_key.y<<" k_dist="<<key<<" "<<endl; 		
	}
	cout<<"һ��Ϊ��"<<endl;
	for(int i=0;i<4;i++)
	{ 
		depth=0;
		Type key=linear_select_2(num,0,n-1,rank[i]);//�ҵ����rank[i]С��k_dist
		data node_key=find_data(node_b,n,key);//�ҵ����rank[i]С��k_dist��Ӧ�Ļ�վ��Ϣ��
		cout<<"�ݹ����="<<depth<<endl<<"��"<<rank[i]<<"С:"<<"id:["<<node_key.id<<"]"<<"γ��="<<node_key.x<<" ����="<<node_key.y<<" k_dist="<<key<<" "<<endl; 		
	}	
	return 0;
}

void read_file(string file_name,data tmp[],Type dist[])//���ļ��ж�ȡ1033����վ���� 
{
	ifstream fin;
	cout<<"read file"<<endl;
	fin.open(file_name.c_str(),ios_base::in);
	if(!fin)
		cout<<"�ļ���ʧ��"<<endl;
	int num;
	fin>>num;
	for(int i=0;i<num;i++)//��ȡÿһ�е����ݣ� 
	{
		fin>>tmp[i].id>>tmp[i].x>>tmp[i].y>>tmp[i].k_dist;
		dist[i]=tmp[i].k_dist;
	}
	
}
data find_data(data node_b[],int num,Type key)//Ѱ��k_distΪkey�Ļ�վ��Ϣ�� 
{
	for(int i=0;i<num;i++)
		if(node_b[i].k_dist==key)
			return node_b[i];
}
Type linear_select_3(Type a[],int p,int r,int k)
{
	if(r-p<20)//a[]�㹻С �������� 
	{
		bubble_sort(a,p,r);
		return a[p+k-1];
	}
	
	int i,j;
	//Ѱ��ÿһ��ĵ��е㣻 
	for(i=0;i<=(r-p-4)/5;i++)
	{
		int s=p+5*i,t;
		t=s+4;
		bubble_sort(a,s,t);//����ð������ 
		swap_element(a,p+i,s+2);//����i�����λ�� ��a[p+i]�������Ƶ�a[p.r]��ǰ�ˣ� 
	}
	flag=1;
	Type x=linear_select_3(a,p,p+(r-p-4)/5,(r-p+6)/10);//ѡȡ��λ������λ����Ϊ���ֻ�׼ ��(r-p+6)/10Ϊ a[p,p+(r-p-4)/5]����Ԫ�ظ�������λ���� 
	flag=0;
	if(flag==0)
		depth++;
	i=linear_partition(a,p,r,x);//���ݻ��ֻ�׼x=10����a[p:r]����Ϊ2���֣�1.���Ӷ�Ϊa[p:r]�����Ӷγ���j=i-p+1; 
	
	j=i-p+1;//���Ӷ�+�м��Ӷεĳ��ȣ� 
	
	//һ��Ϊ�������ٵݹ���ȣ� 
	if(k==j)//����k�����Ӷγ���j�ıȽϣ����ü��η� 
		return a[j];
	else 
	{
		if(k<j) 
			return	linear_select_3(a,p,i-1,k);
		else
			return linear_select_3(a,i+1,r,k-j);
	}
}
Type linear_select_2(Type a[],int p,int r,int k)
{
	if(r-p<20)
	{
		bubble_sort(a,p,r);
		return a[p+k-1];
	}
	
	int i,j;
	for(i=0;i<=(r-p-4)/5;i++)
	{
		int s=p+5*i,t;
		t=s+4;
		bubble_sort(a,s,t);
		swap_element(a,p+i,s+2);//����i�����λ�� ��a[p+i]������ 
	}
	
	//ѡȡ��λ������λ�� ��(r-p+6)/10Ϊ a[p,p+(r-p-4)/5]����Ԫ�ظ�������λ����
	flag=1;
	Type x=linear_select_2(a,p,p+(r-p-4)/5,(r-p+6)/10); 
	flag=0;
	if(flag==0)
		depth++;
	i=linear_partition(a,p,r,x);// 
	j=i-p+1;//���Ӷ�+�м��Ӷεĳ��ȣ� 
	//һ��Ϊ�������ٵݹ���ȣ� 
		if(k<=j) 
			return	linear_select_2(a,p,i-1,k);
		else
			return linear_select_2(a,i+1,r,k-j);
}

//���չؼ���k��a[]���ֳ����Ӷιؼ���С��k�����Ӷιؼ��ִ���k 
int linear_partition(Type a[],int low,int high,Type k)
{
	swap_element(a,low,low+(high-low-4)/10);
	
	Type pivotkey=a[low];
	while(low<high)
	{
		while(high>low&&a[high]>=pivotkey)
			high--;
		a[low]=a[high];
		while(high>low&&a[low]<=pivotkey)
			low++;
		a[high]=a[low];
	}
	a[low]=pivotkey;
	return low;
}
//����Ԫ�� 
void swap_element(Type a[],int i,int j)
{
	Type tmp=a[i];
	a[i]=a[j];
	a[j]=tmp;
}
//ð������ 
void bubble_sort(Type a[],int p,int r)
{
	int i,j;
	Type tmp;
	for(i=0;i<r-p;i++)
		for(j=p;j<r-i;j++)
			if(a[j]>a[j+1])
			{
				tmp=a[j];
				a[j]=a[j+1];
				a[j+1]=tmp;
			}	
} 


