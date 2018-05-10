#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<fstream>
#include<string.h>
using namespace std;
typedef float Type;
typedef struct data{
	int id;//基站id 
	Type x,y,k_dist;//x为维度，y为经度； 
}data;//基站定义 
int depth=0;//递归深度；
int flag=0; 
void swap_element(Type a[],int i,int j);
void bubble_sort(Type a[],int p,int r);//当在r-p个序列中搜索时，冒泡排序 
Type linear_select_3(Type a[],int p,int r,int k);//一分为三，线性时间选择第k小的数； 
Type linear_select_2(Type a[],int p,int r,int k);//一分为二，线性时间选择第k小的数； 
int linear_partition(Type a[],int low,int high,Type k);//按照关键字k将a[]划分成左子段关键字小于k，右子段关键字大于k 
void read_file(string file_name,data node_b[],Type dist[]);//从文件中读取1033个基站数据 
data find_data(data node_b[],int num,Type key);//寻找k_dist为key的基站信息；
int main()
{
	int n=1033;
	Type num[n];
	data node_b[n];
	string file_name="data.txt";//文件名； 
	read_file(file_name,node_b,num);//从文本读取数据到内存 
	int rank[4]={1,5,50,n};//d第rank[i]小的基站； 
	cout<<"一分为三"<<endl; 
	for(int i=0;i<4;i++)
	{ 
		depth=0;
		Type key=linear_select_3(num,0,n-1,rank[i]);//找到与第rank[i]小的k_dist
		data node_key=find_data(node_b,n,key);	//找到与第k小的k_dist对应的基站信息； 
		cout<<"递归深度="<<depth<<endl<<"第"<<rank[i]<<"小:"<<"id:["<<node_key.id<<"]"<<"纬度="<<node_key.x<<" 经度="<<node_key.y<<" k_dist="<<key<<" "<<endl; 		
	}
	cout<<"一分为二"<<endl;
	for(int i=0;i<4;i++)
	{ 
		depth=0;
		Type key=linear_select_2(num,0,n-1,rank[i]);//找到与第rank[i]小的k_dist
		data node_key=find_data(node_b,n,key);//找到与第rank[i]小的k_dist对应的基站信息；
		cout<<"递归深度="<<depth<<endl<<"第"<<rank[i]<<"小:"<<"id:["<<node_key.id<<"]"<<"纬度="<<node_key.x<<" 经度="<<node_key.y<<" k_dist="<<key<<" "<<endl; 		
	}	
	return 0;
}

void read_file(string file_name,data tmp[],Type dist[])//从文件中读取1033个基站数据 
{
	ifstream fin;
	cout<<"read file"<<endl;
	fin.open(file_name.c_str(),ios_base::in);
	if(!fin)
		cout<<"文件打开失败"<<endl;
	int num;
	fin>>num;
	for(int i=0;i<num;i++)//读取每一行的数据； 
	{
		fin>>tmp[i].id>>tmp[i].x>>tmp[i].y>>tmp[i].k_dist;
		dist[i]=tmp[i].k_dist;
	}
	
}
data find_data(data node_b[],int num,Type key)//寻找k_dist为key的基站信息； 
{
	for(int i=0;i<num;i++)
		if(node_b[i].k_dist==key)
			return node_b[i];
}
Type linear_select_3(Type a[],int p,int r,int k)
{
	if(r-p<20)//a[]足够小 ，简单排序； 
	{
		bubble_sort(a,p,r);
		return a[p+k-1];
	}
	
	int i,j;
	//寻找每一组的的中点； 
	for(i=0;i<=(r-p-4)/5;i++)
	{
		int s=p+5*i,t;
		t=s+4;
		bubble_sort(a,s,t);//采用冒泡排序 
		swap_element(a,p+i,s+2);//将第i组的中位数 与a[p+i]交换，移到a[p.r]的前端； 
	}
	flag=1;
	Type x=linear_select_3(a,p,p+(r-p-4)/5,(r-p+6)/10);//选取中位数的中位数作为划分基准 ，(r-p+6)/10为 a[p,p+(r-p-4)/5]数组元素个数的中位数； 
	flag=0;
	if(flag==0)
		depth++;
	i=linear_partition(a,p,r,x);//根据划分基准x=10，将a[p:r]划分为2部分：1.左子段为a[p:r]，左子段长度j=i-p+1; 
	
	j=i-p+1;//左子段+中间子段的长度； 
	
	//一分为三，减少递归深度； 
	if(k==j)//根据k与左子段长度j的比较，采用减治法 
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
		swap_element(a,p+i,s+2);//将第i组的中位数 与a[p+i]交换； 
	}
	
	//选取中位数的中位数 ，(r-p+6)/10为 a[p,p+(r-p-4)/5]数组元素个数的中位数；
	flag=1;
	Type x=linear_select_2(a,p,p+(r-p-4)/5,(r-p+6)/10); 
	flag=0;
	if(flag==0)
		depth++;
	i=linear_partition(a,p,r,x);// 
	j=i-p+1;//左子段+中间子段的长度； 
	//一分为三，减少递归深度； 
		if(k<=j) 
			return	linear_select_2(a,p,i-1,k);
		else
			return linear_select_2(a,i+1,r,k-j);
}

//按照关键字k将a[]划分成左子段关键字小于k，右子段关键字大于k 
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
//交换元素 
void swap_element(Type a[],int i,int j)
{
	Type tmp=a[i];
	a[i]=a[j];
	a[j]=tmp;
}
//冒泡排序 
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


