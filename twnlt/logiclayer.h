#pragma once
#pragma once
class STOP {//站点
	int numb;//标号
	int x, y;//坐标
public:
	STOP(int n = 0, int x = 0, int y = 0);
	virtual int& X();
	virtual int& Y();
	virtual int& N();
};
class LINE {//一条公交线路
	const int numb;
	int* const stop;
	const int nofs;
public:
	LINE(int numb = 0, int nofs = 0, int* stop = nullptr);
	LINE(const LINE& r);
	LINE(LINE&& r) noexcept;
	LINE& operator=(const LINE& r);
	LINE& operator = (LINE&& r)noexcept;
	virtual int has(int s)const;
	virtual int cross(const LINE& b)const;
	virtual operator int()const;
	virtual int NOFS()const;
	virtual double dist(int d, int a)const;
	virtual int& operator[](int x);
	virtual ~LINE()noexcept;

};
class TRAN {//从线路from经过stop转至线路to
	int from;
	int to;
	int stop;
public:
	TRAN(int form = 0, int to = 0, int stop = 0);
	int operator==(const TRAN& t)const;
	virtual int& F();
	virtual int& T();
	virtual int& S();
};
class ROUTE {//转乘路径
	TRAN* const tran;//所有转乘站点
	const int noft;//转乘次数
public:
	ROUTE(TRAN* tran = nullptr, int noft = 0);
	ROUTE(const TRAN& t);
	ROUTE(const ROUTE& r);
	ROUTE(ROUTE&& r)noexcept;
	virtual operator int()const;//转乘次数
	virtual int operator==(const ROUTE& r)const;
	virtual ROUTE operator *()const;//去重
	virtual TRAN& operator[](int);//获取所有转乘站点
	virtual ROUTE operator+(const ROUTE& r)const;//转乘路径连接
	virtual ROUTE& operator=(const ROUTE& r);
	virtual ROUTE& operator=(ROUTE&& t)noexcept;
	virtual ROUTE& operator+=(const ROUTE& r);
	virtual ~ROUTE()noexcept;
	virtual int print()const;//打印转乘路径
};
class NODE {//矩阵元素，记载转乘次数和路线
	ROUTE* const p;//记载所有转乘方案
	int n;//转乘方案数
public:
	NODE(ROUTE* p, int n);
	NODE(int n = 0);
	NODE(const NODE& n);
	NODE(NODE&& n)noexcept;
	virtual NODE operator*()const;//去环
	virtual NODE operator+(const ROUTE& n)const;//元素增加路径
	virtual NODE operator+(const NODE& n)const;//元素路径增加
	virtual NODE operator*(const NODE& n)const;//转乘路径连接
	virtual NODE& operator=(const NODE& n);
	virtual NODE& operator+=(const NODE& n);
	virtual NODE& operator+=(const ROUTE& n);
	virtual NODE& operator*=(const NODE& n);
	virtual NODE& operator=(NODE&& n)noexcept;
	virtual ROUTE& operator [](int x);//获得第x个转乘路径
	virtual operator int& ();//返回可转乘路径数a
	virtual ~NODE()noexcept;
	virtual void print()const;//打印转乘矩阵元素

};
class TMAP {//所有公交转乘元素的“闭包矩阵”
	NODE* const p;//指向闭包矩阵的r*c个元素
	const int r, c;//行列数

public:
	TMAP(int r = 0, int c = 0);
	TMAP(const TMAP& a);
	TMAP(TMAP&& a)noexcept;
	virtual ~TMAP();
	virtual int notZero()const;//若有不可达站点，则返回0
	virtual int miniTran(int b, int e, int& noft, ROUTE(&r)[100])const;//返回最少转乘的路径数，起点站为b，终点站为e，r存十条路径
	virtual int miniDist(int b, int e, double& dist, ROUTE(&r)[100])const;//返回最短距离的路径数，同上
	static double getDist(int b, int e, ROUTE& r);//站次b到e基于路径r的距离
	virtual NODE* operator[](int r);//得到矩阵某行元素的首地址
	virtual int& operator()(int r, int c);//得到r到c的转乘路径数目
	virtual TMAP operator*(const TMAP& a)const;//闭包乘法；
	virtual TMAP operator+(const TMAP& a)const; //闭包加法
	virtual TMAP& operator=(const TMAP& a);
	virtual TMAP& operator=(TMAP&& a)noexcept;
	virtual TMAP& operator += (const TMAP& a);
	virtual TMAP& operator*=(const TMAP& a);
	virtual TMAP& operator()(int r, int c, const ROUTE& a);//将路径a加入矩阵元素中
	virtual void print()const;//打印矩阵元素
};
struct GIS {//描述地理信息系统的类
	static STOP* st;
	static LINE* ls;
	static int ns, nl;
	static TMAP raw, tra;//原始转乘矩阵row，闭包转乘矩阵tra
	static int obs;//对象数
public:
	GIS();
	GIS(const char* flstop, const char* filine);//用站点及路线文件加载地图
	int miniTran(int fx, int fy, int ts, int ty, int& f, int& t, int& n, ROUTE(&r)[100]);
	int miniDist(int fx, int fy, int tx, int ty, int& f, int& t, double& d, ROUTE(&r)[100]);
	~GIS();
};
extern GIS* gis;

