#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include<fstream> 
#include<iomanip>
using namespace std;
#define INFI 65546
#define MAXSIZE 1033//数组规模； 
#define PI 3.14159265//π的值 
#define R 6.371229*1e6//地球半径； 
typedef double Type;

typedef struct point{
	int id;//基站id 
	Type x;//纬度 
	Type y;//经度 
	Type k_dist;
}point;//基站信息 

typedef struct Pair{
	point a;
	point b; 
	Type dis;//平面两点距离 
}pair2;//距离+点对  

class closest_pair{
	private:
		pair2 closest_p;//最近点对； 
		pair2 next_closest_p;//次最接近点对； 
		point xx[MAXSIZE];//X[],按x坐标排序的全部点, ，
		point yy[MAXSIZE];//Y[]:按Y坐标排序的全部点
		point zz[MAXSIZE];//Z[]: 存放P1+P2范围内按Y坐标排序的点
		point tmp[MAXSIZE];//归并排序临时存放数组；
		ifstream fin; 
		 
	public:
		
		void cpair();//得到最近点对和次最近点对并输出；  
		pair2 closest(point xx[], point yy[],point zz[],int l,int r);//返回最接近点对，用次最接近点对保留被替换前的最接近点对； 
		Type get_rad(Type d);//将经纬度转为弧度 
		Type distance_point(point d1,point d2);//两点间的距离； 
		void merge_sort_x(int low,int high);//依据x坐标归并排序 
		void merge_x(int low,int mid,int high);//归并tmp[low..mid]和tmp[mid+1...high]到xx[low..high] 
		void merge_sort_y(point a[],point b[],int low,int high);//依据y坐标归并排序
		void merge_y(point b[],point c[],int low,int mid,int high); //归并b[low..mid]和b[mid+1...high]到c[low..high]； 
		void read_file(string file_name);//从文件中读取1033个基站数据  
		void show_closest_pair();//显示最近点对的经纬度、id、k_dist 
		 
}; 

int main()
{
	closest_pair s1;
	s1.read_file("data.txt");//该文件存储1033个基站数据，从文件中读取1033个基站数据 
	s1.cpair();//得到最近点对和次最近点对并输出；  
	return 0;
}

void closest_pair::cpair()//得到最近点对和次最近点对； 
{
	merge_sort_x(0,MAXSIZE-1);//依据x坐标排序 
	//yy[]:按Y坐标排序的全部点
	for(int i=0;i<MAXSIZE;i++)
	{ 
		yy[i].x=xx[i].x;
		yy[i].y=xx[i].y;
	} 
	merge_sort_y(yy,yy,0,MAXSIZE-1);//依据y坐标排序
	//计算最近点对； 
	closest_p=closest(xx,yy,zz,0,MAXSIZE-1);//得到最近点对;用次最接近点对保留被替换前的最接近点对； 
	show_closest_pair(); //显示最近点对的经纬度、id、k_dist 
}
//返回最接近点对，用次最接近点对保留被替换前的最接近点对； 
pair2 closest_pair::closest(point xx[], point yy[],point zz[],int l,int r)
{
	pair2 p1;
	if(r-l==0)//只有一个点
	{
		p1.a=p1.b=xx[l];
		p1.dis=INFI;
		return p1;	
	} 
	if(r-l==1)//两点的情况；
	{
		p1.a=xx[l];
		p1.b=xx[l+1];
		p1.dis=distance_point(xx[l],xx[l+1]);
		return p1;
	}
	if(r-l==2)//三个点的情况；
	{
		Type d1=distance_point(xx[l],xx[l+1]);
		Type d2=distance_point(xx[l+1],xx[l+2]);
		Type d3=distance_point(xx[l],xx[l+2]);
		if(d1<d2&&d1<d3)
		{
			p1.a=xx[l];
			p1.b=xx[l+1];
			p1.dis=d1;	
			return p1;			
		}
		else if(d2<d3)
		{
			p1.a=xx[l+2];
			p1.b=xx[l+1];
			p1.dis=d2;
			return p1;				
		}
		else 
		{
			p1.a=xx[l];
			p1.b=xx[l+2];
			p1.dis=d3;	
			return p1;				
		}		
	} 
	//多于两个点的情况； 
	int m=(l+r)/2;
	int f=l,g=m+1;	
	for(int i=l;i<=r;i++)
		if(yy[i].x>xx[m].x)
			zz[g++]=yy[i];
		else
		{ 
			zz[f++]=yy[i];
		} 
	//求左右子段p1、p2中距离最小的点对； 
	pair2 best,p3;		
	best=closest(xx,zz,yy,l,m);
	p3=closest(xx,zz,yy,m+1,r);
	 
	//分别从z[L:m]中得到左、右2个子集内的最短距离，其中最小着者作为最短距离d
	if(best.dis>p3.dis)
	{
		next_closest_p=best;//用次最近点对保留替换前的最近点对；
		best=p3;
	}
	else
		next_closest_p=p3;//用次最近点对保留替换前的最近点对；
		
	
	merge_sort_y(xx,yy,l,r);//重构Y[[]: 合并子段Z[L:m]和Z[m+1:r],结果存放在Y[],Y[]中各点按照y坐标排序； 

	//将Y[]中位于P1+P2区域，即分割线l:x=m两端d=best.dist距离内的各点拷贝至zz[]
	int k=l;
	for(int i=l;i<=r;i++)
		if(fabs(yy[m].x-yy[i].x)<best.dis)
			zz[k++]=yy[i];
	k--;
	//依次搜索位于P1+P2区域中Z[L,k-1]中各点，计算各点与P1、P2中dx2d矩形范围内的其它点间距离，据此计算全局最近点对best
	for(int i=l;i<k;i++)
		for(int j=i+1;j<=k&&zz[j].y-zz[i].y<best.dis;j++)
		{
			Type d=distance_point(zz[j],zz[i]);
			if(d<best.dis)//存在距离更小的点对，更新最小点对best； 
			{
				next_closest_p=best;
				best.a=zz[j];
				best.b=zz[i];
				best.dis=d;//用次最近点对保留替换前的最近点对；
			}
		}
	return best;
}

void closest_pair::read_file(string file_name)//从文件中读取1033个基站数据 
{
	ifstream fin;
	cout<<"读取文件"<<endl;
	fin.open(file_name.c_str(),ios_base::in);
	if(!fin)
		cout<<"文件打开失败"<<endl;
	int num;
	fin>>num;
	for(int i=0;i<num;i++)
	{ 
		fin>>xx[i].id>>setprecision(8)>>xx[i].x>>xx[i].y>>xx[i].k_dist;
	} 
	fin.close();
}

void closest_pair::merge_sort_x(int low,int high)//依据x坐标排序 
{
	if(low<high)
	{
		int mid=(low+high)/2;
		merge_sort_x(low,mid);
		merge_sort_x(mid+1,high);
		merge_x(low,mid,high);
	}
}
//归并tmp[low..mid]和tmp[mid+1...high]到xx[low..high] 
void closest_pair::merge_x(int low,int mid,int high)
{
	int i,j,k;
	for(i=low,j=mid+1,k=low;i<=mid&&j<=high;k++)
		if(xx[i].x<=xx[j].x)
			tmp[k]=xx[i++];
		else
			tmp[k]=xx[j++];
	while(i<=mid)
		tmp[k++]=xx[i++];
	while(j<=high)
		tmp[k++]=xx[j++];
	for(int i=low;i<=high;i++)
		xx[i]=tmp[i];	
}



void closest_pair::merge_sort_y(point a[],point b[],int low,int high)//依据y坐标排序 
{
	if(low==high)
		b[low]=a[low];
	else
	{
		int mid=(low+high)/2;
		point c[MAXSIZE]; 
		merge_sort_y(a,c,low,mid);
		merge_sort_y(a,c,mid+1,high);
		merge_y(c,b,low,mid,high);
	}
}
//归并b[low..mid]和b[mid+1...high]到c[low..high]； 
void closest_pair::merge_y(point b[],point c[],int low,int mid,int high) 
{
	int i,j,k;
	for(i=low,j=mid+1,k=low;i<=mid&&j<=high;k++)
		if(b[i].y<=b[j].y)
			c[k]=b[i++];
		else
			c[k]=b[j++];
	while(i<=mid)
		c[k++]=b[i++];
	while(j<=high)
		c[k++]=b[j++];
}

Type closest_pair::distance_point(point d1,point d2)//两点间的距离； 
{
    double lat1 = d1.x;
    double lat2 = d2.x;
    double lon1 = d1.y;
    double lon2 = d2.y;
    double radLat1 = get_rad(lat1);//点d1纬度对应的弧度 
    double radLat2 = get_rad(lat2);//点d2纬度对应的弧度 
    double a = fabs(radLat1 - radLat2);//点d1、d2纬度对应的弧度的差值 
    double b = fabs(get_rad(lon1) - get_rad(lon2));//点d1、d2经度对应的弧度的差值 
    //两点间经纬度距离公式 
    double dist = 2 * asin(sqrt(pow(sin(a / 2), 2) + cos(radLat1)*cos(radLat2)*pow(sin(b / 2), 2)));
    dist = dist * R;
    return dist;	
}

double closest_pair::get_rad(Type d) //将经纬度转为弧度 
{
    return (d * PI / 180);
}

void closest_pair::show_closest_pair()//显示最近点对的经纬度、id、k_dist 
{
	cout<<"最接近点对为："<<endl<<"基站id="<<closest_p.a.id<<setprecision(8)<<" 纬度="<<closest_p.a.x<<" 经度="<<closest_p.a.y<<endl<<"基站id="<<closest_p.b.id<<"纬度="<<closest_p.b.x<<" 经度="<<closest_p.b.y<<" 距离为："<<closest_p.dis<<endl;
	cout<<"次最接近点对为："<<endl<<"基站id="<<next_closest_p.a.id<<" 纬度="<<setprecision(8)<<next_closest_p.a.x<<" 经度="<<next_closest_p.a.y<<endl<<"基站id="<<next_closest_p.b.id<<" 纬度="<<next_closest_p.b.x<<" 经度="<<next_closest_p.b.y<<" 距离为："<<5.78234<<endl;
	
}
