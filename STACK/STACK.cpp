#include"Head.h"
STACK::STACK(int m) :QUEUE(m), q(m) {}

STACK::STACK(const STACK& s) : QUEUE(s), q(s.q) {}

STACK::STACK(STACK&& s) noexcept :QUEUE((QUEUE&&)s), q((QUEUE&&)s.q) {}

int STACK::size() const noexcept
{
    return QUEUE::size() + q.size();
}

STACK::operator int() const noexcept
{
    return QUEUE::operator int() + q.operator int();
}

STACK& STACK::operator<<(int e)
{
    if (QUEUE::operator int() < QUEUE::size() - 1)
    {
        QUEUE::operator<<(e);
        return *this;
    }
    else if (q.operator int() < q.size() - 1)
    {
        int tmp = 0;
        QUEUE::operator>>(tmp);
        q.operator<<(tmp);
        QUEUE::operator<<(e);
        return *this;
    }
    else
    {
        throw("STACK is full!");
    }
}

STACK& STACK::operator>>(int& e)
{
    if (QUEUE::operator int())
    {
        int int_in_base = QUEUE::operator int(), tmp = 0;
        //循环一次让最后push的元素到达队首
        for (int i = 0; i < int_in_base - 1; i++)
        {
            QUEUE::operator>>(tmp);
            QUEUE::operator<<(tmp);
        }
        QUEUE::operator>>(e);
        return *this;
    }
    else if (q.operator int())
    {
        int int_in_q = q.operator int(), tmp = 0;
        for (int i = 0; i < int_in_q - 1; i++)
        {
            q.operator>>(tmp);
            q.operator<<(tmp);
        }
        q.operator>>(e);
        return *this;
    }
    else
    {
        throw("STACK is empty!");
    }
}

STACK& STACK::operator=(const STACK& s)
{
    QUEUE::operator=(s);
    q=(s.q);
    return *this;
}

STACK& STACK::operator=(STACK&& s) noexcept
{
    QUEUE::operator=((QUEUE&&)s);
    q=((QUEUE&&)s.q);
    return *this;
}

char* STACK::print(char* b) const noexcept
{
    char* s = new char[sizeof(char) * QUEUE::operator int() * 12];
    if (s == NULL)throw"Malloc faild";
    QUEUE::print(s);
    q.print(b);
    strcat(b, s);
    delete[]s;
    return b;
}

STACK::~STACK() noexcept
{
}
