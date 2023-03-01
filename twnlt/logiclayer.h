#pragma once
#pragma once
class STOP {//վ��
	int numb;//���
	int x, y;//����
public:
	STOP(int n = 0, int x = 0, int y = 0);
	virtual int& X();
	virtual int& Y();
	virtual int& N();
};
class LINE {//һ��������·
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
class TRAN {//����·from����stopת����·to
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
class ROUTE {//ת��·��
	TRAN* const tran;//����ת��վ��
	const int noft;//ת�˴���
public:
	ROUTE(TRAN* tran = nullptr, int noft = 0);
	ROUTE(const TRAN& t);
	ROUTE(const ROUTE& r);
	ROUTE(ROUTE&& r)noexcept;
	virtual operator int()const;//ת�˴���
	virtual int operator==(const ROUTE& r)const;
	virtual ROUTE operator *()const;//ȥ��
	virtual TRAN& operator[](int);//��ȡ����ת��վ��
	virtual ROUTE operator+(const ROUTE& r)const;//ת��·������
	virtual ROUTE& operator=(const ROUTE& r);
	virtual ROUTE& operator=(ROUTE&& t)noexcept;
	virtual ROUTE& operator+=(const ROUTE& r);
	virtual ~ROUTE()noexcept;
	virtual int print()const;//��ӡת��·��
};
class NODE {//����Ԫ�أ�����ת�˴�����·��
	ROUTE* const p;//��������ת�˷���
	int n;//ת�˷�����
public:
	NODE(ROUTE* p, int n);
	NODE(int n = 0);
	NODE(const NODE& n);
	NODE(NODE&& n)noexcept;
	virtual NODE operator*()const;//ȥ��
	virtual NODE operator+(const ROUTE& n)const;//Ԫ������·��
	virtual NODE operator+(const NODE& n)const;//Ԫ��·������
	virtual NODE operator*(const NODE& n)const;//ת��·������
	virtual NODE& operator=(const NODE& n);
	virtual NODE& operator+=(const NODE& n);
	virtual NODE& operator+=(const ROUTE& n);
	virtual NODE& operator*=(const NODE& n);
	virtual NODE& operator=(NODE&& n)noexcept;
	virtual ROUTE& operator [](int x);//��õ�x��ת��·��
	virtual operator int& ();//���ؿ�ת��·����a
	virtual ~NODE()noexcept;
	virtual void print()const;//��ӡת�˾���Ԫ��

};
class TMAP {//���й���ת��Ԫ�صġ��հ�����
	NODE* const p;//ָ��հ������r*c��Ԫ��
	const int r, c;//������

public:
	TMAP(int r = 0, int c = 0);
	TMAP(const TMAP& a);
	TMAP(TMAP&& a)noexcept;
	virtual ~TMAP();
	virtual int notZero()const;//���в��ɴ�վ�㣬�򷵻�0
	virtual int miniTran(int b, int e, int& noft, ROUTE(&r)[100])const;//��������ת�˵�·���������վΪb���յ�վΪe��r��ʮ��·��
	virtual int miniDist(int b, int e, double& dist, ROUTE(&r)[100])const;//������̾����·������ͬ��
	static double getDist(int b, int e, ROUTE& r);//վ��b��e����·��r�ľ���
	virtual NODE* operator[](int r);//�õ�����ĳ��Ԫ�ص��׵�ַ
	virtual int& operator()(int r, int c);//�õ�r��c��ת��·����Ŀ
	virtual TMAP operator*(const TMAP& a)const;//�հ��˷���
	virtual TMAP operator+(const TMAP& a)const; //�հ��ӷ�
	virtual TMAP& operator=(const TMAP& a);
	virtual TMAP& operator=(TMAP&& a)noexcept;
	virtual TMAP& operator += (const TMAP& a);
	virtual TMAP& operator*=(const TMAP& a);
	virtual TMAP& operator()(int r, int c, const ROUTE& a);//��·��a�������Ԫ����
	virtual void print()const;//��ӡ����Ԫ��
};
struct GIS {//����������Ϣϵͳ����
	static STOP* st;
	static LINE* ls;
	static int ns, nl;
	static TMAP raw, tra;//ԭʼת�˾���row���հ�ת�˾���tra
	static int obs;//������
public:
	GIS();
	GIS(const char* flstop, const char* filine);//��վ�㼰·���ļ����ص�ͼ
	int miniTran(int fx, int fy, int ts, int ty, int& f, int& t, int& n, ROUTE(&r)[100]);
	int miniDist(int fx, int fy, int tx, int ty, int& f, int& t, double& d, ROUTE(&r)[100]);
	~GIS();
};
extern GIS* gis;

