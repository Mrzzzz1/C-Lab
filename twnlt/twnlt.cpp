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
	//����ͨ������������������������Ҽ����������յ������ò�ͬ������ɫ
	QBrush qbrush(f == 1 ? QColor(210, 105, 45) : QColor(255, 170, 00));
	QPen qpens(qbrush, 4, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);
	//����f������ɫ
	QGraphicsRectItem* item = this;
	//ͼԪ��Ļ���
	item->setPen(qpens);
	item->setBrush(qbrush);
	//item->setFlag(QGraphicsltem:ItemIsMovable);
	//ȥ������Ϊû���ƶ�ͼԪ��Ĳ���
	checkAllStops();// �������վ�㣬�ҳ����������վ�㲢�������bs��
}
int& MyItem::x() { return cx; }
int& MyItem::y() { return cy; }
int& MyItem::f() { return cf; }
int& MyItem::c() { return cs;}
int& MyItem::operator[](int x) {
	if (x < 0 || x >= cs) throw "subscription overflow for stops around departure point!";
	return bs[x];//�����Ѽ��ı��Ϊx��վ����
}
int MyItem::operator()(int x, int y) {
	return (x - cx) * (x - cx) + (y - cy) * (y - cy);
}
//�����ڼ����������Զ�������ÿ���
MyItem& MyItem::operator<<(int s) {
	if (s < 0 || s >= GIS::ns) return *this;
	int d = (*this)(GIS::st[s].X(), GIS::st[s].Y());
	int m;
	//��bs��û��Ԫ�أ���cs--0�������վ��s����
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
// �����Լ��ĳ���MyScene���Ա��ܹ��������ָ�������¼�
MyScene::MyScene(QObject* parent) : QGraphicsScene(parent) {
	clearFocus();
	qgv = Q_NULLPTR;//û�м��ص�ͼ�ļ�ʱ
}
void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
	//ע������.ui�����ļ��У�mouseTracking���빴ѡ�����򲻻���ִ��¼�
	if (qgv == Q_NULLPTR) {
	QGraphicsScene::mouseMoveEvent(event);//���û�м��ص�ͼ�ļ�
		return;
	}
	QPointF qpointf = event->scenePos();//��ȡ���ָ���ƶ�ʱ������
	for (int n = 0; n < gis->ns; n++) {
		if (fabs(gis->st[n].X() - qpointf.x()) < 8 && fabs(gis->st[n].Y() - qpointf.y()) < 8) {
			//������ʾ��Ϣ����ʹ��fromLocal8Bit( )������������ʾ����ʾ����
			QString msg = QString::fromLocal8Bit("��");
			msg = msg + QString::number(n + 1, 10, 0) + QString::fromLocal8Bit("������վ��.");
			QtTipsDlgView dlg(msg);
			//������show0�����������ޱ߿�������dlg.exec0������������������
			//dlg.setAttribute(Qt:WA_ShowModal,true);
			dlg.startTimer(2000);
			QPointF m1 = qgv->mapToGlobal(QPoint(qpointf.x(), qpointf.y()));
			//������ͣ��ʾʱ��Ϊ2s��ʱ�䵽���Զ��ر�dg
			dlg.move(QPoint(m1.x(), m1.y()));
			dlg.exec();//��ʾվ����ʾ��Ϣ
		}
	}
	QGraphicsScene::mouseMoveEvent(event);// �ص��������ָ���¼�
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
//����ͼgraphicsview ��С���� scene��ʾ����Ĵ�С
QSize viewsize = parnt->size();
MyScene* scene;
if (parnt->scene() != Q_NULLPTR) delete parnt->scene();
scene = new MyScene(parnt);
scene->setSceneRect(0, 0, viewsize.width(), viewsize.height());
scene->qgv = parnt;
//��ʾ���й���վ��
QBrush qbrush(QColor(255, 0, 0));//������ɫ
QPen qpens(qbrush, 4, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin); //վ��ı�
for (int n = 0; n < gis->ns; n++) {
	scene->addEllipse(gis->st[n].X(), gis->st[n].Y(), 6, 6, qpens, qbrush);//��ʾ���й�����·
}
QPen qpenl(qbrush, 3, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin); //��·�ı�
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
	//���� Lambda ���ʽ�������Զ������Ƶ����棬���÷ǳ�Ա�����ĵ�ַ����
	connect(m_Timer, &QTimer::timeout, this, [=]() {
		QList<QGraphicsItem*>listItem = ui.graphicsView->scene()->items();
		deleteItems();// ��ѯ�����ʾʱ��һ������ɾ������������ͼԪ
		route = false;// ��ѯ�����ʾ��Ϻ󣬿�������ѡȡ�����������յ�
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
	if (ui.action_open->isChecked() == false)return;//��굥���������Σ��ڶ���ֱ�ӷ���
	ui.action_open->setChecked(false);//����һ�δ�������false����ֹ�ڶ��δ�������
	if (fl != Q_NULLPTR) {//�����ǰ�򿪹�վ�㼰��·���봰��
		fl->show();//��ֱ����ʾ�ô���
		return;
	}
	arrive = depart = false;
	fl = new QtWidgetsFL();
	fl->setWindowFlags(Qt::WindowStaysOnTopHint);//�����������ʾ
	fl->myShow(ui.graphicsView);
}
void twnlt::deleteItems() {
	if (gItem == Q_NULLPTR) return;
	// ɾ������������ͼԪ
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
	//�Ȼ�����������յ����꣬����������ת�˵�·��
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
	//��ʼ�齨ͼԪ�飺������ʾת�˷�����·��
	QGraphicsEllipseItem* myEItem;
	QGraphicsLineItem* myLItem;
	MyScene* scene = (MyScene*)(ui.graphicsView->scene());
	QBrush qbrush(QColor(210, 105, 45));
	QPen qpens(qbrush, 4, Qt::SolidLine, Qt :: SquareCap, Qt :: BevelJoin);
	QPen qpenl(qbrush, 3, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin); //��·�ı�
	route=true;
	int c, n = 0;
	ROUTE r[100];
	for (int d = 0; d < itemDepart->c(); d++) {
		int s = (*itemDepart)[d]; //������վ����s
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
				//�������յ��·��
				myEItem = new QGraphicsEllipseItem(itemArrive->x(), itemArrive->y(), 6, 6);
				myEItem->setPen(qpens);
				myEItem->setBrush(QBrush(QColor(255, 170, 00)));
				gItem->addToGroup(myEItem);
				scene->addItem(gItem);
				ui.graphicsView->setScene(scene);
				continue;
			}
			c = GIS::tra.miniTran(s, t, n, r);//�õ���ת�˷�����
			for (int m = 0; m < c; m++) {//���ڵ�m��ת�˷������� route=r[m]��ת�˴�����Ϊn
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
					//�����վ
					fm = r[m][0].F();
					//��i·��i·����ʱSO--1)����ת��
					bg = GIS::ls[fm].has(fr);
					//�ѳ���·���fm
					ed = GIS::ls[fm].has(to);
					//��ʼվ������·�е����
					if (bg > ed) { tm = bg; bg = ed; ed = tm; }
					//��ֹվ������·�е����
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
						//������ʼ��·��ת��վ��
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
						gItem->addToGroup(myLItem);//����һվ��·��
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
	//�Ȼ�����������յ����꣬��������̾����·��
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
	QPen qpens(qbrush, 4, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin); //վ��ı�
	QPen qpenl(qbrush, 3, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);//��·�ı�
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
				//�������յ��·��
				myEItem = new QGraphicsEllipseItem(itemArrive->x(), itemArrive->y(), 6, 6);
				myEItem->setPen(qpens);
				myEItem->setBrush(QBrush(QColor(255, 170, 00)));
				gItem->addToGroup(myEItem);
				scene->addItem(gItem);
				//�����յ��λ��
				ui.graphicsView->setScene(scene);
				continue;
			}
			c = GIS::tra.miniDist(s, t, dist, r);
			//�õ���ת�˷�����c
			for (int m = 0; m < c; m++) {
				//��m��ת�˷������� route=r[m]����̾��붼Ϊdist
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




