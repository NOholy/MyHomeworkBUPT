#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<math.h>
using namespace std;
//出现的问题：partition函数不能有效工作； 
typedef double Type;
typedef struct Pair1{
	Type a;
	Type b;
	Type dis;
}Pair1;

typedef struct Pair2{
	Type x1;
	Type y1;
	Type x2;
	Type y2;
	Type dis;
}Pair2;
Type random_data();
Pair1 closest_pair1(Type s[],int p,int r);
void bubble_sort(Type a[],int p,int r);
Type linear_select(Type a[],int p,int r,int k);
int linear_partition(Type a[],int low,int high,Type k);

int Partition(Type a[],int p,int r,Type k);
void swap_element(Type a[],int i,int j);
void show(Type a[],int n);
void quick_sort(Type a[],int low,int high);
int quick_partition(Type a[],int low,int high);
Type Min(Type s[],int p,int r);
Type Max(Type s[],int p,int r);
void read_file(string file_name);
//当数据过多时，出现崩溃现象 
int main()
{

	srand(time(0));
	int n=2000;
	Type s[n];
	for(int i=0;i<n;i++)
		s[i]=random_data();
	//show(s,n);
	
	Pair1 d=closest_pair1(s,0,n-1);
	cout<<"最接近点的距离："<<d.dis<<endl<<"坐标分别为："<<d.a<<"和"<<d.b<<endl;
	return 0;	
}

Pair2 closest_Pair2()
{
	
}

Type linear_select(Type a[],int p,int r,int k)
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
	
	Type x=linear_select(a,p,p+(r-p-4)/5,(r-p+6)/10);//选取中位数的中位数 ，(r-p+6)/10为 a[p,p+(r-p-4)/5]数组元素个数的中位数； 
	
	i=linear_partition(a,p,r,x);// 
	j=i-p+1;//左子段+中间子段的长度；
	 
	//一分为三，减少递归深度； 	
	if(k==j)
		return a[j];
	else 
	{
		if(k<j) 
			return	linear_select(a,p,i-1,k);
		else
			return linear_select(a,i+1,r,k-j);
	}
}

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

void swap_element(Type a[],int i,int j)
{
	Type tmp=a[i];
	a[i]=a[j];
	a[j]=tmp;
}
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
void show(Type a[],int n)
{
	for(int i=0;i<n;i++)
		cout<<a[i]<<" ";
	cout<<endl;
}
int Partition(Type a[],int low,int high,Type k)
{
	int tmp=low;
	while(a[tmp]!=k)
	{
		if(a[tmp]==k)
			break;
		tmp++;
	}
	cout<<"a["<<tmp<<"]="<<a[tmp]<<endl;
	swap_element(a,tmp,low);
	
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

Pair1 closest_pair1(Type s[],int p,int r)
{
	Pair1 d;
	d.a=0;d.b=0;d.dis=65535;
	if(r-p<=0)//只有一个点的情况； 
		return d;
	if(r-p==1)//只有两个点的情况； 
	{
		d.a=s[p];
		d.b=s[r];
		d.dis=fabs(s[p]-s[r]);
		return d;
	}  
	Type x=linear_select(s,p,r,(r-p)/2+1); 
	int pivot_loc=Partition(s,p,r,x);//根据s的中点x，将s分成s1 s2; 
	
	//cout<<"x="<<x<<"s["<<pivot_loc<<"]="<<s[pivot_loc]<<endl;
	
	Pair1 d1,d2,d3;
	d1=closest_pair1(s,p,pivot_loc);
	d2=closest_pair1(s,pivot_loc+1,r);
	
	Type min,max;
	max=Max(s,p,pivot_loc);//s1最大的点； 
	min=Min(s,pivot_loc+1,r);//s2最小的点；
	
	//cout<<"左边max="<<x<<"右边min="<<min<<endl;
	d3.a=min;
	d3.b=max;
	d3.dis=fabs(max-min);//d3为s1 s2跨段最接近的点； 
	
	if(d1.dis<d2.dis)
	{
		if(d1.dis<d3.dis)
			return d1;
		else
			return d3;
	}
	else 
	{ 
		if(d2.dis<d3.dis)
			return d2;
		else 
			return d3;
	} 
}
Type random_data()
{
	Type num1=(Type) (rand()%101)/101;
	int num2=rand()%10000+1;
	return num1+num2;
}

Type Min(Type s[],int p,int r)
{
	Type num=s[p];
	for(int i=p+1;i<=r;i++)
		if(num>s[i])
			num=s[i];
//	cout<<"min="<<num<<endl;
	return num;
}

Type Max(Type s[],int p,int r)
{
	Type num=s[p];
	for(int i=p+1;i<=r;i++)
		if(num<s[i])
			num=s[i];
//	cout<<"max="<<num<<endl;
	return num;
}

