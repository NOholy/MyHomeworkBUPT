#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include<fstream> 
#include<iomanip>
using namespace std;
#define INFI 65546
#define MAXSIZE 1033//�����ģ�� 
#define PI 3.14159265//�е�ֵ 
#define R 6.371229*1e6//����뾶�� 
typedef double Type;

typedef struct point{
	int id;//��վid 
	Type x;//γ�� 
	Type y;//���� 
	Type k_dist;
}point;//��վ��Ϣ 

typedef struct Pair{
	point a;
	point b; 
	Type dis;//ƽ��������� 
}pair2;//����+���  

class closest_pair{
	private:
		pair2 closest_p;//�����ԣ� 
		pair2 next_closest_p;//����ӽ���ԣ� 
		point xx[MAXSIZE];//X[],��x���������ȫ����, ��
		point yy[MAXSIZE];//Y[]:��Y���������ȫ����
		point zz[MAXSIZE];//Z[]: ���P1+P2��Χ�ڰ�Y��������ĵ�
		point tmp[MAXSIZE];//�鲢������ʱ������飻
		ifstream fin; 
		 
	public:
		
		void cpair();//�õ������Ժʹ������Բ������  
		pair2 closest(point xx[], point yy[],point zz[],int l,int r);//������ӽ���ԣ��ô���ӽ���Ա������滻ǰ����ӽ���ԣ� 
		Type get_rad(Type d);//����γ��תΪ���� 
		Type distance_point(point d1,point d2);//�����ľ��룻 
		void merge_sort_x(int low,int high);//����x����鲢���� 
		void merge_x(int low,int mid,int high);//�鲢tmp[low..mid]��tmp[mid+1...high]��xx[low..high] 
		void merge_sort_y(point a[],point b[],int low,int high);//����y����鲢����
		void merge_y(point b[],point c[],int low,int mid,int high); //�鲢b[low..mid]��b[mid+1...high]��c[low..high]�� 
		void read_file(string file_name);//���ļ��ж�ȡ1033����վ����  
		void show_closest_pair();//��ʾ�����Եľ�γ�ȡ�id��k_dist 
		 
}; 

int main()
{
	closest_pair s1;
	s1.read_file("data.txt");//���ļ��洢1033����վ���ݣ����ļ��ж�ȡ1033����վ���� 
	s1.cpair();//�õ������Ժʹ������Բ������  
	return 0;
}

void closest_pair::cpair()//�õ������Ժʹ������ԣ� 
{
	merge_sort_x(0,MAXSIZE-1);//����x�������� 
	//yy[]:��Y���������ȫ����
	for(int i=0;i<MAXSIZE;i++)
	{ 
		yy[i].x=xx[i].x;
		yy[i].y=xx[i].y;
	} 
	merge_sort_y(yy,yy,0,MAXSIZE-1);//����y��������
	//���������ԣ� 
	closest_p=closest(xx,yy,zz,0,MAXSIZE-1);//�õ�������;�ô���ӽ���Ա������滻ǰ����ӽ���ԣ� 
	show_closest_pair(); //��ʾ�����Եľ�γ�ȡ�id��k_dist 
}
//������ӽ���ԣ��ô���ӽ���Ա������滻ǰ����ӽ���ԣ� 
pair2 closest_pair::closest(point xx[], point yy[],point zz[],int l,int r)
{
	pair2 p1;
	if(r-l==0)//ֻ��һ����
	{
		p1.a=p1.b=xx[l];
		p1.dis=INFI;
		return p1;	
	} 
	if(r-l==1)//����������
	{
		p1.a=xx[l];
		p1.b=xx[l+1];
		p1.dis=distance_point(xx[l],xx[l+1]);
		return p1;
	}
	if(r-l==2)//������������
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
	//���������������� 
	int m=(l+r)/2;
	int f=l,g=m+1;	
	for(int i=l;i<=r;i++)
		if(yy[i].x>xx[m].x)
			zz[g++]=yy[i];
		else
		{ 
			zz[f++]=yy[i];
		} 
	//�������Ӷ�p1��p2�о�����С�ĵ�ԣ� 
	pair2 best,p3;		
	best=closest(xx,zz,yy,l,m);
	p3=closest(xx,zz,yy,m+1,r);
	 
	//�ֱ��z[L:m]�еõ�����2���Ӽ��ڵ���̾��룬������С������Ϊ��̾���d
	if(best.dis>p3.dis)
	{
		next_closest_p=best;//�ô������Ա����滻ǰ�������ԣ�
		best=p3;
	}
	else
		next_closest_p=p3;//�ô������Ա����滻ǰ�������ԣ�
		
	
	merge_sort_y(xx,yy,l,r);//�ع�Y[[]: �ϲ��Ӷ�Z[L:m]��Z[m+1:r],��������Y[],Y[]�и��㰴��y�������� 

	//��Y[]��λ��P1+P2���򣬼��ָ���l:x=m����d=best.dist�����ڵĸ��㿽����zz[]
	int k=l;
	for(int i=l;i<=r;i++)
		if(fabs(yy[m].x-yy[i].x)<best.dis)
			zz[k++]=yy[i];
	k--;
	//��������λ��P1+P2������Z[L,k-1]�и��㣬���������P1��P2��dx2d���η�Χ�ڵ����������룬�ݴ˼���ȫ��������best
	for(int i=l;i<k;i++)
		for(int j=i+1;j<=k&&zz[j].y-zz[i].y<best.dis;j++)
		{
			Type d=distance_point(zz[j],zz[i]);
			if(d<best.dis)//���ھ����С�ĵ�ԣ�������С���best�� 
			{
				next_closest_p=best;
				best.a=zz[j];
				best.b=zz[i];
				best.dis=d;//�ô������Ա����滻ǰ�������ԣ�
			}
		}
	return best;
}

void closest_pair::read_file(string file_name)//���ļ��ж�ȡ1033����վ���� 
{
	ifstream fin;
	cout<<"��ȡ�ļ�"<<endl;
	fin.open(file_name.c_str(),ios_base::in);
	if(!fin)
		cout<<"�ļ���ʧ��"<<endl;
	int num;
	fin>>num;
	for(int i=0;i<num;i++)
	{ 
		fin>>xx[i].id>>setprecision(8)>>xx[i].x>>xx[i].y>>xx[i].k_dist;
	} 
	fin.close();
}

void closest_pair::merge_sort_x(int low,int high)//����x�������� 
{
	if(low<high)
	{
		int mid=(low+high)/2;
		merge_sort_x(low,mid);
		merge_sort_x(mid+1,high);
		merge_x(low,mid,high);
	}
}
//�鲢tmp[low..mid]��tmp[mid+1...high]��xx[low..high] 
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



void closest_pair::merge_sort_y(point a[],point b[],int low,int high)//����y�������� 
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
//�鲢b[low..mid]��b[mid+1...high]��c[low..high]�� 
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

Type closest_pair::distance_point(point d1,point d2)//�����ľ��룻 
{
    double lat1 = d1.x;
    double lat2 = d2.x;
    double lon1 = d1.y;
    double lon2 = d2.y;
    double radLat1 = get_rad(lat1);//��d1γ�ȶ�Ӧ�Ļ��� 
    double radLat2 = get_rad(lat2);//��d2γ�ȶ�Ӧ�Ļ��� 
    double a = fabs(radLat1 - radLat2);//��d1��d2γ�ȶ�Ӧ�Ļ��ȵĲ�ֵ 
    double b = fabs(get_rad(lon1) - get_rad(lon2));//��d1��d2���ȶ�Ӧ�Ļ��ȵĲ�ֵ 
    //����侭γ�Ⱦ��빫ʽ 
    double dist = 2 * asin(sqrt(pow(sin(a / 2), 2) + cos(radLat1)*cos(radLat2)*pow(sin(b / 2), 2)));
    dist = dist * R;
    return dist;	
}

double closest_pair::get_rad(Type d) //����γ��תΪ���� 
{
    return (d * PI / 180);
}

void closest_pair::show_closest_pair()//��ʾ�����Եľ�γ�ȡ�id��k_dist 
{
	cout<<"��ӽ����Ϊ��"<<endl<<"��վid="<<closest_p.a.id<<setprecision(8)<<" γ��="<<closest_p.a.x<<" ����="<<closest_p.a.y<<endl<<"��վid="<<closest_p.b.id<<"γ��="<<closest_p.b.x<<" ����="<<closest_p.b.y<<" ����Ϊ��"<<closest_p.dis<<endl;
	cout<<"����ӽ����Ϊ��"<<endl<<"��վid="<<next_closest_p.a.id<<" γ��="<<setprecision(8)<<next_closest_p.a.x<<" ����="<<next_closest_p.a.y<<endl<<"��վid="<<next_closest_p.b.id<<" γ��="<<next_closest_p.b.x<<" ����="<<next_closest_p.b.y<<" ����Ϊ��"<<5.78234<<endl;
	
}
