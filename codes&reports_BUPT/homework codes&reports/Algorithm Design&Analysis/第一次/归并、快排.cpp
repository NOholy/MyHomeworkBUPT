#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <sys/time.h>
using namespace std;
#define MAXSIZE 20000//当前最大数组规模； 
#define MAX 20000//当前最大最大值 

typedef int KeyType,InfoType;
typedef struct RedType{
	KeyType key;//关键字 
	InfoType otherinfo;
}RedType; 
typedef struct SqList{//顺序存储； 
	RedType r[MAXSIZE+1];//r[0]闲置或作哨兵；
	int length;//当前序列的长度； 
}SqList;
class sort{
	private:
		SqList L;//序列L 		
		SqList tmp;//临时数组； 
		int DD;//无序度 
		float ADD;//平均无序度 		 
	public:
		int depth;//递归深度
		void InitSqList(int n,int N);//生成序列长度为n，最大值为N的元素相同但顺序不同的随机序列； 
		int MergeSort(int low,int high);//递归归并排序，并返回递归深度； 
		void Merge(int low,int mid,int high);//归并L[low..mid]和l[mid+1...high] 
		void MergeSort_not_recursion();//非递归方式的归并排序；
		int QuickSort(int low,int high);//快速排序 
		int	Partition(int low,int high); //对顺序表L中的子序列L.[low...high]进行非递减排序;
		int Randomized_quickSort(int low,int high);//具有随机选择划分的快排； 
		int RandomizedPartition(int low,int high);//具有随机选择，对顺序表L中的子序列L.[low...high]进行非递减排序; 
		void ShowSqList();//显示此时的序列；
		void swap_element(int i,int j); //交换元素； 
		void show_DD_ADD_level();//显示DD,ADD，递归层次； 
};
int main()
{
	srand(time(0));
	sort s1,s2,s3,s4;
	
	int n=MAXSIZE,N=MAX;	
	cout<<"请选择输入的方式：1、递归归并排序；2、非递归递归归并排序；3、快速排序；4、具有随机划分的快速排序;5、终止程序"<<endl;
	s1.InitSqList(n,N);
	s2=s3=s4=s1;
	int key;
	cin>>key;
	//记录其实时间和终止时间，毫秒级别； 
	struct timeval t_start,t_end;
	while(key!=5)
	{
		
		switch(key){
			case 1:				
				cout<<"递归归并排序："<<endl;
				gettimeofday(&t_start,NULL);//程序开始时间				
				s1.depth=s1.MergeSort(1,MAXSIZE);
				gettimeofday(&t_end,NULL);//程序终止时间；				
				s1.show_DD_ADD_level();	//输出无序度，平均无序度和递归深度；			
				break;
			case 2:				
				cout<<"非递归递归归并排序："<<endl;
				gettimeofday(&t_start,NULL);//程序开始时间	
				s2.MergeSort_not_recursion();
				gettimeofday(&t_end,NULL);	//程序终止时间；				
				s2.show_DD_ADD_level();	//输出无序度，平均无序度和递归深度；							
				break;
			case 3:				
				cout<<"快速排序："<<endl;
				gettimeofday(&t_start,NULL);//程序开始时间	
				s3.depth=s3.QuickSort(1,MAXSIZE);
				gettimeofday(&t_end,NULL);//程序终止时间；	
				s3.show_DD_ADD_level();//输出无序度，平均无序度和递归深度；											
				break;
			case 4:				
				cout<<"具有随机划分的快速排序："<<endl;
				gettimeofday(&t_start,NULL);//程序开始时间	
				s4.depth=s4.Randomized_quickSort(1,MAXSIZE);	
				gettimeofday(&t_end,NULL);//程序终止时间；	
				s4.show_DD_ADD_level();//输出无序度，平均无序度和递归深度；													
				break;
			default:
				break;
		}
		cout<<"运行时间："<<t_end.tv_usec-t_start.tv_usec+1000000*(t_end.tv_sec-t_start.tv_sec)<<"微秒"<<endl;
		cin>>key;
	}
	return 0;
} 


int sort::MergeSort(int low,int high)//递归归并排序； 
{	
	if(low==high)
	{ 
		tmp.r[low]=L.r[low];
		return 1;//不能继续分解时，递归深度为1； 
	} 
	else
	{
		int mid=(low+high)/2;
		int depth_l=MergeSort(low,mid);//左子问题求解，返回递归深度； 
		int depth_r=MergeSort(mid+1,high);//右子问题求解 
		Merge(low,mid,high);//将左右排好序的数组合并到另一个数组； 
		return depth_l>depth_r?depth_l+1:depth_r+1;
	}
} 
void sort::Merge(int low,int mid,int high)//归并L[low..mid]和l[mid+1...high] 
{
	int i;//左子段的搜索比较指针的起点
	int j;//右子段的搜索比较指针的起点
	int k;//目标数组d的指针起点，符合递增顺序的tmp.r[ ]中的元素被拷贝至L.r[k]

			
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
void sort::MergeSort_not_recursion()//非递归方式的归并排序；
{
	int buck=1,low,mid,high;//buck为当前子序列长度； 
	while(buck<L.length)
	{
		for(low=1;low+buck<=L.length;)
		{
			mid=low+buck-1;
			high=mid+buck;
			if(high>L.length)
				high=L.length;
			//将tmp.r[]中长度为s的子序列对合并到数组L.r[]；合并完后，长度buck增加，为下次合并做准备	
			Merge(low,mid,high);
			low=high+1;

		}
		buck=buck*2;
	}
}

int sort::QuickSort(int low,int high)
{	
	int sorted=1;
	//从左向右扫描a[p:r]中全部元素，判断a[p:r]是否满足非递减顺序，满足则不进行排序； 
	for(int i=low;i<=high;i++)
		if(L.r[i].key>L.r[i+1].key)
		{ 			
			sorted=0;
			break; 
		} 
	if(low<high&&sorted==0)
	{		
		int pivotloc=Partition(low,high);//找到枢轴点； 
		int depth_l=QuickSort(low,pivotloc-1);//进行左子序排序； 
		int depth_r=QuickSort(pivotloc+1,high);//进行右子序排序； 
		return depth_l>depth_r?depth_l+1:depth_r+1;
	}
	else
		return 1;
}

int sort::Partition(int low,int high)//对顺序表L中的子序列L.[low...high]进行非递减排序;
{
	L.r[0]=L.r[low];
	int pivotkey=L.r[low].key;
	while(low<high)
	{
		while(high>low&&L.r[high].key>=pivotkey)//寻找小于 pivotkey的下标；
			high--;
		L.r[low]=L.r[high];
		while(high>low&&L.r[low].key<=pivotkey)
			low++;
		L.r[high]=L.r[low];
	}
	L.r[low]=L.r[0];//最后将最开始的pivotkey存放到low中，使序列 L.[low...high]左边都小于pivotkey右边都大于pivotkey；
	return low;
}

	//随机选择划分元素
int sort::Randomized_quickSort(int low,int high)
{
	int sorted=1; 
	//从左向右扫描a[p:r]中全部元素，判断a[p:r]是否满足非递减顺序，满足则不进行排序；
	for(int i=low;i<=high;i++)
		if(L.r[i].key>L.r[i+1].key)
		{ 
			swap_element(i,i+1); 
			sorted=0;
			break; 
		} 
	if(low<high&&sorted==0)
	{
		int pivotloc=RandomizedPartition(low,high);//找到枢轴点； 
		int depth_l=QuickSort(low,pivotloc-1);//进行左子序排序； 
		int depth_r=QuickSort(pivotloc+1,high);//进行右子序排序； 
		return depth_l>depth_r?depth_l+1:depth_r+1;
	}
	else
		return 1;
}
int sort::RandomizedPartition(int low,int high)
{
	int x=rand()%(high-low+1)+low;//随机选一个L.r[x]
	swap_element(low,x);//L.r[x],与 L.r[low]交换； 
	L.r[0]=L.r[low];
	int pivotkey=L.r[low].key;
	while(low<high)
	{
		while(high>low&&L.r[high].key>=pivotkey)//寻找小于 pivotkey的下标；
			high--;
		L.r[low]=L.r[high];
		while(high>low&&L.r[low].key<=pivotkey)
			low++;
		L.r[high]=L.r[low];
	}
	L.r[low]=L.r[0];//最后将最开始的pivotkey存放到low中，使序列 L.[low...high]左边都小于pivotkey右边都大于pivotkey；
	return low;	
}
void sort::InitSqList(int n,int N)
{
	ADD=0;
	DD=0;
	L.length=n;//序列的长度； 
	cout<<"初始化序列"<<endl;
	//先顺序生成1-N的数； 
	for(int i=1;i<=L.length;i++)
		L.r[i].key=i;
	for(int i=1;i<N;i++)
	{
		//将第i个数与r[i+1...N]中随机某个数交换；
		int j=rand()%(N-i)+i+1; 
		RedType tmp=L.r[i];
		L.r[i]=L.r[j];
		L.r[j]=tmp;
	}
		
	//计算序列DD的值 
	for(int i=1;i<=L.length;i++)
		 for(int j=1;j<=L.length;j++)
		 	if(i<j&&L.r[i].key>L.r[j].key)
		 		DD++;
	ADD=DD/L.length; 
		 		
}

void sort::swap_element(int i,int j)//交换元素 
{
	
	RedType tmp=L.r[i];
	L.r[i]=L.r[j];
	L.r[j]=tmp;
}
void sort::show_DD_ADD_level()
{
	cout<<"DD="<<DD<<endl<<"ADD="<<ADD<<endl<<"递归深度="<<depth<<endl;
}


