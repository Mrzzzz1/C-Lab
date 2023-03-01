#include"Head.h"
//��ʼ�����У��������m��Ԫ��
QUEUE::QUEUE(int m) :elems(new int[m]), max(m), head(0), tail(0) {}
//��q�����ʼ������
QUEUE::QUEUE(const QUEUE& q) : elems(new int[q.max]), max(q.max) {
	if (elems == NULL)throw("new elems failed!");
	head = q.head;
	tail = q.tail;
	for (int i = 1; i < max; i++) {
		elems[i] = q.elems[i];
	}
}
//��q�ƶ���ʼ������
QUEUE::QUEUE(QUEUE&& q) noexcept :elems(q.elems), max(q.max), head(q.head), tail(q.tail) {
	q.head = q.tail = 0;
	*(int**)&q.elems = NULL;
	*(int*)&q.max = 0;
}
//���ض��е�ʵ��Ԫ�ظ���
QUEUE::operator int() const noexcept {
	return tail >= head ? tail - head : tail + max - head;
}
//���ض�������Ԫ�ظ���max
int QUEUE::size() const noexcept {
	return max;
}
//��e�����β���������ص�ǰ����
QUEUE& QUEUE::operator <<(int e) {
	if ((tail + 1) % max == head)throw"QUEUE is full!";
	elems[tail] = e;
	tail = (tail + 1) % max;
	return *this;
}

//�Ӷ��׳�Ԫ�ص�e�������ص�ǰ����
QUEUE& QUEUE::operator >> (int& e) {
	if (head == tail) {
		throw"QUEUE is empty!";
	}
	e = elems[head];
	head = (head + 1) % max;
	return *this;
}
//�����ֵ�����ر���ֵ����
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

//�ƶ���ֵ�����ر���ֵ����
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
//��ӡ������s������s
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

//���ٵ�ǰ����
QUEUE::~QUEUE() {
	if (elems != NULL) {
		delete elems;
		*(int**)&elems = NULL;
		*(int*)&max = 0;
		head = tail = 0;
	}
}
