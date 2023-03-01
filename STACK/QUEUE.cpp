#include"Head.h"
//初始化队列：最多申请m个元素
QUEUE::QUEUE(int m) :elems(new int[m]), max(m), head(0), tail(0) {}
//用q深拷贝初始化队列
QUEUE::QUEUE(const QUEUE& q) : elems(new int[q.max]), max(q.max) {
	if (elems == NULL)throw("new elems failed!");
	head = q.head;
	tail = q.tail;
	for (int i = 1; i < max; i++) {
		elems[i] = q.elems[i];
	}
}
//用q移动初始化队列
QUEUE::QUEUE(QUEUE&& q) noexcept :elems(q.elems), max(q.max), head(q.head), tail(q.tail) {
	q.head = q.tail = 0;
	*(int**)&q.elems = NULL;
	*(int*)&q.max = 0;
}
//返回队列的实际元素个数
QUEUE::operator int() const noexcept {
	return tail >= head ? tail - head : tail + max - head;
}
//返回队列申请元素个数max
int QUEUE::size() const noexcept {
	return max;
}
//将e入队列尾部，并返回当前队列
QUEUE& QUEUE::operator <<(int e) {
	if ((tail + 1) % max == head)throw"QUEUE is full!";
	elems[tail] = e;
	tail = (tail + 1) % max;
	return *this;
}

//从队首出元素到e，并返回当前队列
QUEUE& QUEUE::operator >> (int& e) {
	if (head == tail) {
		throw"QUEUE is empty!";
	}
	e = elems[head];
	head = (head + 1) % max;
	return *this;
}
//深拷贝赋值并返回被赋值队列
QUEUE& QUEUE::operator=(const QUEUE& q) {
	if (this == &q)return *this;
	if (elems != NULL)delete elems;
	*(int**)&elems = new int[q.max];
	head = q.head;
	tail = q.tail;
	*(int*)&max = q.max;
	for (int i = 0; i < max; i++) {
		elems[i] = q.elems[i];
	}
	return *this;
}

//移动赋值并返回被赋值队列
QUEUE& QUEUE::operator=(QUEUE&& q) noexcept {
	if (this == &q) return *this;
	if (elems != NULL)delete elems;
	*(int**)&elems = q.elems;
	*(int*)&max = q.max;
	head = q.head;
	tail = q.tail;
	*(int**)&q.elems = NULL;
	*(int*)&q.max = 0;
	q.head = q.tail = 0;
	return *this;
}
//打印队列至s并返回s
char* QUEUE::print(char* s) const noexcept
{
	char a[10];
	int i; s[0] = 0;
	for (i = this->head; i != tail; i = (i + 1) % max)
	{
		sprintf_s(a, "%d,", this->elems[i]);
		strcat(s, a);
	}
	return s;
}

//销毁当前队列
QUEUE::~QUEUE() {
	if (elems != NULL) {
		delete elems;
		*(int**)&elems = NULL;
		*(int*)&max = 0;
		head = tail = 0;
	}
}
