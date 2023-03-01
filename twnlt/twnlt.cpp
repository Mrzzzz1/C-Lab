#include "twnlt.h"
#include "logiclayer.h"
#include"QtWidgetsFL.h"
#include <QDialog>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsEllipseItem>
#include "QtTipsDlgView.h"
GIS* gis = nullptr; bool route = false; bool depart = false; bool arrive = false;
MyItem::MyItem(int x, int y, int f) : QGraphicsRectItem(x - 3, y - 3, 7, 7) {
	cx = x; cy = y;
	cf = f; cs = 0;
	//以下通过鼠标左键单击步行起点和鼠标右键单击步行终点来设置不同画笔颜色
	QBrush qbrush(f == 1 ? QColor(210, 105, 45) : QColor(255, 170, 00));
	QPen qpens(qbrush, 4, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);
	//根据f设置颜色
	QGraphicsRectItem* item = this;
	//图元点的画笔
	item->setPen(qpens);
	item->setBrush(qbrush);
	//item->setFlag(QGraphicsltem:ItemIsMovable);
	//去掉：因为没有移动图元点的操作
	checkAllStops();// 检测所有站点，找出距离最近的站点并将其存于bs中
}
int& MyItem::x() { return cx; }
int& MyItem::y() { return cy; }
int& MyItem::f() { return cf; }
int& MyItem::c() { return cs;}
int& MyItem::operator[](int x) {
	if (x < 0 || x >= cs) throw "subscription overflow for stops around departure point!";
	return bs[x];//返回已检测的编号为x的站点编号
}
int MyItem::operator()(int x, int y) {
	return (x - cx) * (x - cx) + (y - cy) * (y - cy);
}
//仅用于检测两点距离的远近，不用开方
MyItem& MyItem::operator<<(int s) {
	if (s < 0 || s >= GIS::ns) return *this;
	int d = (*this)(GIS::st[s].X(), GIS::st[s].Y());
	int m;
	//若bs中没有元素，即cs--0，或距离站点s更近
	if (cs == 0 || d < (m = (*this)(GIS::st[bs[0]].X(), GIS::st[bs[0]].Y()))) {
		bs[0] = s;
		cs = 1;
		return *this;
	}
	if (d == m) {
		if (cs == 6) return *this;
		bs[cs] = s;
		cs++;
	}
	return *this;
}
int MyItem::checkAllStops() {
	for (int c = 0; c < GIS::ns; c++)
		operator<<(GIS::st[c].N());
	return cs;
}
void MyItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	setSelected(true);
	QGraphicsRectItem::mousePressEvent(event);
}
// 定义自己的场景MyScene，以便能够捕获鼠标指针或键盘事件
MyScene::MyScene(QObject* parent) : QGraphicsScene(parent) {
	clearFocus();
	qgv = Q_NULLPTR;//没有加载地图文件时
}
void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
	//注意在其.ui界面文件中，mouseTracking必须勾选，否则不会出现此事件
	if (qgv == Q_NULLPTR) {
	QGraphicsScene::mouseMoveEvent(event);//如果没有加载地图文件
		return;
	}
	QPointF qpointf = event->scenePos();//获取鼠标指针移动时的坐标
	for (int n = 0; n < gis->ns; n++) {
		if (fabs(gis->st[n].X() - qpointf.x()) < 8 && fabs(gis->st[n].Y() - qpointf.y()) < 8) {
			//以下提示信息必须使用fromLocal8Bit( )，否则中文提示会显示乱码
			QString msg = QString::fromLocal8Bit("第");
			msg = msg + QString::number(n + 1, 10, 0) + QString::fromLocal8Bit("个公交站点.");
			QtTipsDlgView dlg(msg);
			//若调用show0，则须设置无边框；若调用dlg.exec0，则不用如下设置属性
			//dlg.setAttribute(Qt:WA_ShowModal,true);
			dlg.startTimer(2000);
			QPointF m1 = qgv->mapToGlobal(QPoint(qpointf.x(), qpointf.y()));
			//设置悬停显示时间为2s，时间到后自动关闭dg
			dlg.move(QPoint(m1.x(), m1.y()));
			dlg.exec();//显示站点提示信息
		}
	}
	QGraphicsScene::mouseMoveEvent(event);// 回调基类鼠标指针事件
}
void MyScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	//qDebug("*********MyScene:mousePressEvent***************");
	if (qgv == Q_NULLPTR || route == true) {
		QGraphicsScene::mouseMoveEvent(event);
		return;
	}
	QPointF qpointf = event->scenePos();
	QList<QGraphicsItem*>listItem = items();
	int lb = 0;
	if (event->button() == Qt::LeftButton) { lb = 1; depart = true; }
	if (event->button() == Qt::RightButton) { lb = 2; arrive = true; }
	for (int i = listItem.length() - 1; i >= 0; i--) {
		MyItem* item = (MyItem*)listItem[i];
		if (item->f() == lb) {
			listItem.removeAt(i);
			delete item;
			break;
		}
	}
	addItem(new MyItem(qpointf.x(), qpointf.y(), lb));
	QGraphicsScene::mousePressEvent(event);
}
void MyScene :: stopLines(QGraphicsView* parnt){
//按视图graphicsview 大小设置 scene显示区域的大小
QSize viewsize = parnt->size();
MyScene* scene;
if (parnt->scene() != Q_NULLPTR) delete parnt->scene();
scene = new MyScene(parnt);
scene->setSceneRect(0, 0, viewsize.width(), viewsize.height());
scene->qgv = parnt;
//显示所有公交站点
QBrush qbrush(QColor(255, 0, 0));//设置颜色
QPen qpens(qbrush, 4, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin); //站点的笔
for (int n = 0; n < gis->ns; n++) {
	scene->addEllipse(gis->st[n].X(), gis->st[n].Y(), 6, 6, qpens, qbrush);//显示所有公交线路
}
QPen qpenl(qbrush, 3, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin); //线路的笔
	for (int n = 0; n < gis->nl; n++) {
		LINE& line = gis->ls[n];
		int stops = line.NOFS();
		for (int m = 1; m < stops; m++) {
			STOP s = gis->st[line[m - 1]];
			STOP t = gis->st[line[m]];
			QLineF ql = QLineF(s.X(), s.Y(), t.X(), t.Y());
			scene->addLine(ql, qpenl);
		}
	}
	parnt->setScene(scene);
	parnt->show();
}
twnlt::twnlt(QWidget* parent) : QMainWindow(parent) {
	ui.setupUi(this);
	fl = Q_NULLPTR;
	gItem = Q_NULLPTR;
	m_Timer = new QTimer(this);
	m_Timer->setSingleShot(true);
	connect(ui.action_open, SIGNAL(triggered(bool)), this, SLOT(loadmap()));
	connect(ui.action_exit, SIGNAL(triggered(boo1)), this, SLOT(closewnd()));
	connect(ui.action_zszc, SIGNAL(triggered(bool)), this, SLOT(zszc()));
	connect(ui.action_zdjl, SIGNAL(triggered(bool)), this, SLOT(zdjl()));
	//以下 Lambda 表达式可以用自动类型推导代替，或用非成员函数的地址代替
	connect(m_Timer, &QTimer::timeout, this, [=]() {
		QList<QGraphicsItem*>listItem = ui.graphicsView->scene()->items();
		deleteItems();// 查询结果显示时间一到，就删除场景的所有图元
		route = false;// 查询结果显示完毕后，可以重新选取步行起点或步行终点
		});
}
twnlt::~twnlt() {
	if (fl != Q_NULLPTR) {
		fl->hide();
		delete fl;
		fl = Q_NULLPTR;
		delete m_Timer;
		deleteItems();
		delete gis;
	}
}
void twnlt::closewnd() {
	if (ui.action_exit->isChecked() - false) return;
	ui.action_exit->setChecked(false);
	if (fl != Q_NULLPTR) {
		fl->hide();
		delete fl;
		fl = Q_NULLPTR;
	}
	close();
}
void twnlt::closeEvent(QCloseEvent* event) {
	if (fl != Q_NULLPTR) {
		fl->hide();
		delete fl;
		fl = Q_NULLPTR;
	}
}
void twnlt::loadmap() {
	if (ui.action_open->isChecked() == false)return;//鼠标单击触发两次，第二次直接返回
	ui.action_open->setChecked(false);//鼠标第一次触发设置false，防止第二次触发进入
	if (fl != Q_NULLPTR) {//如果先前打开过站点及线路输入窗口
		fl->show();//则直接显示该窗口
		return;
	}
	arrive = depart = false;
	fl = new QtWidgetsFL();
	fl->setWindowFlags(Qt::WindowStaysOnTopHint);//设置最外层显示
	fl->myShow(ui.graphicsView);
}
void twnlt::deleteItems() {
	if (gItem == Q_NULLPTR) return;
	// 删除场景的所有图元
	ui.graphicsView->scene()->removeItem(gItem);
	for (int i = gItem->childItems().size() - 1; i >= 0; i--) {
		QGraphicsItem* item = (gItem->childItems())[i];
		gItem->removeFromGroup(item);
		delete item;
	}
	delete gItem;
	gItem = Q_NULLPTR;
}
void twnlt::zszc() {
	//先获得起点坐标和终点坐标，并计算最少转乘的路径
	if (ui.action_zszc->isChecked() == false) return;
	ui.action_zszc->setChecked(false);
	QList<QGraphicsItem*>listItem;
	if ((depart && arrive) == false) return;
	listItem = ui.graphicsView->scene()->items(); 
	MyItem* itemDepart = (MyItem*)listItem[0];
	MyItem* itemArrive = (MyItem*)listItem[1];
	if (itemDepart->f() != 1) {
		itemDepart = (MyItem*)listItem[1];
		itemArrive = (MyItem*)listItem[0];
	}
	//开始组建图元组：用于显示转乘方案的路径
	QGraphicsEllipseItem* myEItem;
	QGraphicsLineItem* myLItem;
	MyScene* scene = (MyScene*)(ui.graphicsView->scene());
	QBrush qbrush(QColor(210, 105, 45));
	QPen qpens(qbrush, 4, Qt::SolidLine, Qt :: SquareCap, Qt :: BevelJoin);
	QPen qpenl(qbrush, 3, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin); //线路的笔
	route=true;
	int c, n = 0;
	ROUTE r[100];
	for (int d = 0; d < itemDepart->c(); d++) {
		int s = (*itemDepart)[d]; //获得起点站点编号s
		for (int a = 0; a < itemArrive->c(); a++) {
			int t = (*itemArrive)[a];
			if (s == t) {
				deleteItems();
				gItem = new QGraphicsItemGroup();
				myEItem = new QGraphicsEllipseItem(itemDepart->x(), itemDepart->y(), 6, 6);
				myEItem->setPen(qpens);
				myEItem->setBrush(qbrush);
				gItem->addToGroup(myEItem);
				myLItem = new QGraphicsLineItem(itemDepart->x(), itemDepart->y(), itemArrive->x(), itemArrive->y());
				myLItem->setPen(qpenl);
				gItem->addToGroup(myLItem);
				//到步行终点的路径
				myEItem = new QGraphicsEllipseItem(itemArrive->x(), itemArrive->y(), 6, 6);
				myEItem->setPen(qpens);
				myEItem->setBrush(QBrush(QColor(255, 170, 00)));
				gItem->addToGroup(myEItem);
				scene->addItem(gItem);
				ui.graphicsView->setScene(scene);
				continue;
			}
			c = GIS::tra.miniTran(s, t, n, r);//得到的转乘方案数
			for (int m = 0; m < c; m++) {//对于第m个转乘方案，即 route=r[m]，转乘次数都为n
				deleteItems();
				gItem = new QGraphicsItemGroup();
				myEItem = new QGraphicsEllipseItem(itemDepart->x(), itemDepart->y(), 6, 6);
				myEItem->setPen(qpens);
				myEItem->setBrush(qbrush);
				gItem->addToGroup(myEItem);
				int fr = s, to = t;
				int fm, tm;
				int bg, ed;
				myLItem = new QGraphicsLineItem(itemDepart->x(), itemDepart->y(), gis->st[s].X(), gis->st[s].Y());
				myLItem->setPen(qpenl);
				gItem->addToGroup(myLItem);
				if (n == 1 && r[m][0].S() == -1) {
					//到起点站
					fm = r[m][0].F();
					//从i路到i路（此时SO--1)不用转乘
					bg = GIS::ls[fm].has(fr);
					//已乘线路序号fm
					ed = GIS::ls[fm].has(to);
					//起始站点在线路中的序号
					if (bg > ed) { tm = bg; bg = ed; ed = tm; }
					//终止站点在线路中的序号
					for (int y = bg; y < ed; y++) {
						fr = GIS::ls[fm][y];
						to = GIS::ls[fm][y + 1];
						myLItem = new QGraphicsLineItem(GIS::st[fr].X(), GIS::st[fr].Y(),
							GIS::st[to].X(), GIS::st[to].Y());
						myLItem->setPen(qpenl);
						gItem->addToGroup(myLItem);
					}
				}
				else {
					for (int y = 0; y < n; y++) {

						fm = r[m][y].F();
						bg = GIS::ls[fm].has(fr);
						to = r[m][y].S();
						ed = GIS::ls[fm].has(to);
						//对于起始线路的转乘站点
						if (bg > ed) { tm = bg; bg = ed; ed = tm; }
						for (int u = bg; u < ed; u++) {
							int ff = GIS::ls[fm][u];
							int tt = GIS::ls[fm][u + 1];
							myLItem = new QGraphicsLineItem(GIS::st[ff].X(),
								GIS::st[ff].Y(), GIS::st[tt].X(), GIS::st[tt].Y());
							myLItem->setPen(qpenl);
							gItem->addToGroup(myLItem);
						}
						fr = to;
					}
					fm = r[m][n - 1].T();
					bg = GIS::ls[fm].has(fr);
					ed = GIS::ls[fm].has(t);
					if (bg > ed) { tm = bg; bg = ed; ed = tm; }
					for (int u = bg; u < ed; u++) {
						int ff = GIS::ls[fm][u];
						int tt = GIS::ls[fm][u + 1];
						myLItem = new QGraphicsLineItem(GIS::st[ff].X(), GIS::st[ff].Y(),
							GIS::st[tt].X(), GIS::st[tt].Y());
						myLItem->setPen(qpenl);
						gItem->addToGroup(myLItem);//到下一站的路径
					}
				}
				myLItem = new QGraphicsLineItem(GIS::st[t].X(), GIS::st[t].Y(), itemArrive->x(), itemArrive->y());
				myLItem->setPen(qpenl);
				gItem->addToGroup(myLItem);
				myEItem = new QGraphicsEllipseItem(itemArrive->x(), itemArrive->y(), 6, 6);
				myEItem->setPen(qpens);
				myEItem->setBrush(QBrush(QColor(255, 170, 00)));
				gItem->addToGroup(myEItem);
				scene->addItem(gItem);
				ui.graphicsView->setScene(scene);
			}
		}
	}
	this->m_Timer->start(5000);
}
void twnlt::zdjl() {
	//先获得起点坐标和终点坐标，并计算最短距离的路径
	if (ui.action_zdjl->isChecked() == false) return;
	ui.action_zdjl->setChecked(false);
	QList<QGraphicsItem*>listItem;
	if ((depart && arrive) == false) return;
	listItem = ui.graphicsView->scene()->items();
	MyItem* itemDepart = (MyItem*)listItem[0];
	MyItem* itemArrive = (MyItem*)listItem[1];
	if (itemDepart->f() != 1) {
		itemDepart = (MyItem*)listItem[1];
		itemArrive = (MyItem*)listItem[0];
	}
	QGraphicsEllipseItem* myEItem;
	QGraphicsLineItem* myLItem;
	MyScene* scene = (MyScene*)(ui.graphicsView->scene());
	QBrush qbrush(QColor(210, 105, 45));
	QPen qpens(qbrush, 4, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin); //站点的笔
	QPen qpenl(qbrush, 3, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);//线路的笔
	route = true;
	int c = 0;
	double dist = 0;
	ROUTE r[100];
	for (int d = 0; d < itemDepart->c(); d++) {
		int s = (*itemDepart)[d];
		for (int a = 0; a < itemArrive->c(); a++) {
			int t = (*itemArrive)[a];
			if (s == t) {
				deleteItems();
				gItem = new QGraphicsItemGroup();
				myEItem = new QGraphicsEllipseItem(itemDepart->x(), itemDepart->y(), 6, 6);
				myEItem->setPen(qpens);
				myEItem->setBrush(qbrush);
				gItem->addToGroup(myEItem);
				myLItem = new QGraphicsLineItem(itemDepart->x(), itemDepart->y(),
					itemArrive->x(), itemArrive->y());
				myLItem->setPen(qpenl);
				gItem->addToGroup(myLItem);
				//到步行终点的路径
				myEItem = new QGraphicsEllipseItem(itemArrive->x(), itemArrive->y(), 6, 6);
				myEItem->setPen(qpens);
				myEItem->setBrush(QBrush(QColor(255, 170, 00)));
				gItem->addToGroup(myEItem);
				scene->addItem(gItem);
				//步行终点的位置
				ui.graphicsView->setScene(scene);
				continue;
			}
			c = GIS::tra.miniDist(s, t, dist, r);
			//得到的转乘方案数c
			for (int m = 0; m < c; m++) {
				//第m个转乘方案，即 route=r[m]，最短距离都为dist
				deleteItems();
				gItem = new QGraphicsItemGroup();
				myEItem = new QGraphicsEllipseItem(itemDepart->x(), itemDepart->y(), 6, 6);
				myEItem->setPen(qpens);
				myEItem->setBrush(qbrush);
				gItem->addToGroup(myEItem);
				int fr = s, to = t;
				int fm, tm;
				int bg, ed;
				myLItem = new QGraphicsLineItem(itemDepart->x(), itemDepart->y(),
					gis->st[s].X(), gis->st[s].Y());
				myLItem->setPen(qpenl);
				gItem->addToGroup(myLItem);
				int n = r[m];
				if (n == 1 && r[m][0].S() == -1) {
					fm = r[m][0].F();
					bg = GIS::ls[fm].has(fr);
					ed = GIS::ls[fm].has(to);
					if (bg > ed) { tm = bg; bg = ed; ed = tm; }
					for (int y = bg; y < ed; y++) {
						fr = GIS::ls[fm][y];
						to = GIS::ls[fm][y + 1];
						myLItem = new QGraphicsLineItem(GIS::st[fr].X(), GIS::st[fr].Y(),
							GIS::st[to].X(), GIS::st[to].Y());
						myLItem->setPen(qpenl);
						gItem->addToGroup(myLItem);
					}
				}
				else {
					for (int y = 0; y < n; y++) {
						fm = r[m][y].F();
						bg = GIS::ls[fm].has(fr);
						to = r[m][y].S();
						ed = GIS::ls[fm].has(to);
						if (bg > ed) { tm = bg; bg = ed; ed = tm; }
						for (int u = bg; u < ed; u++) {

							int ff = GIS::ls[fm][u];
							int tt = GIS::ls[fm][u + 1];
							myLItem = new QGraphicsLineItem(GIS::st[ff].X(),
								GIS::st[ff].Y(), GIS::st[tt].X(), GIS::st[tt].Y());
							myLItem->setPen(qpenl);
							gItem->addToGroup(myLItem);
						}
						fr = to;
					}
					fm = r[m][n - 1].T(); bg = GIS::ls[fm].has(fr); ed = GIS::ls[fm].has(t);
					if (bg > ed) { tm = bg; bg = ed; ed = tm; }
					for (int u = bg; u < ed; u++) {
						int ff = GIS::ls[fm][u];
						int tt = GIS::ls[fm][u + 1];
						myLItem = new QGraphicsLineItem(GIS::st[ff].X(), GIS::st[ff].Y(),
							GIS::st[tt].X(), GIS::st[tt].Y());
						myLItem->setPen(qpenl);
						gItem->addToGroup(myLItem);
					}
				}
				myLItem = new QGraphicsLineItem(GIS::st[t].X(), GIS::st[t].Y(),
					itemArrive->x(), itemArrive->y());
				myLItem->setPen(qpenl);
				gItem->addToGroup(myLItem);
				myEItem = new QGraphicsEllipseItem(itemArrive->x(), itemArrive->y(), 6, 6);
				myEItem->setPen(qpens);
				myEItem->setBrush(QBrush(QColor(255, 170, 00)));
				gItem->addToGroup(myEItem);
				scene->addItem(gItem);
				ui.graphicsView->setScene(scene);
			}
		}
	}
	this->m_Timer->start(5000);
}




