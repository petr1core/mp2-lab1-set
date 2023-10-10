// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
//static const int FAKE_INT = -1;
//static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0) throw "len < 0";
    BitLen = len;
    if (len % 8 == 0) MemLen = len % 8;
    else {
        MemLen = (len / sizeof(TELEM)) + 1;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[bf.MemLen];
    for (size_t i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    if (pMem != nullptr) delete[] pMem;
    pMem = nullptr;
    BitLen = 0;
    MemLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n > BitLen) {
        throw "Wrong index";
    }
    return n / sizeof(TELEM);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if ((n < 0) ||(n > BitLen)) throw "Negative n";
    return  (1 << (n % sizeof(TELEM) * 8)); //bitset<4> int i(1) = 0100; i << 1 = 1000
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n < 0) || (n > BitLen))
        throw "Negative n";
    pMem[GetMemIndex(n)] = (GetMemMask(n) | pMem[GetMemIndex(n)]);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n > BitLen)) throw "Negative n";
    pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & (~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n < 0) || (n > BitLen)) throw "Negative n";
    if (pMem[GetMemIndex(n)] & GetMemMask(n)) 
        return 1;
    else 
        return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    delete[] pMem;
    this->BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM(MemLen);
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen == bf.BitLen && MemLen == bf.MemLen) {
        int f = 0;
        for (int i = 0; i < MemLen; i++)
            if (pMem[i] != bf.pMem[i]) f++;
        if (f == 0) return 1;
        else return 0;
    }
    else return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (*this == bf) return 0;
    else return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    return FAKE_BITFIELD;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    return FAKE_BITFIELD;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField t(*this);
    for (int i = 0; i < (t.MemLen - 1); i++) t.pMem[i] = ~(t.pMem[i]);
    for (int i = ((t.MemLen - 1) * sizeof(TELEM)); i < (t.BitLen); i++) {
        if (t.GetBit(i) == 1) t.ClrBit(i);
        else t.SetBit(i);
    }
    return t;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    string tmp;
    istr >> tmp;
    if (tmp.size() != bf.GetLength()) throw "wrong lentgh";
    else {
        for (int i = 0; i < bf.GetLength(); i++) {
            if (tmp[i] == 0) {
                bf.ClrBit(i);
            }
            if (tmp[i] == 1) {
                bf.SetBit(i);
            }
            else { throw "incorrect str"; }
        }
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    string tmp;
    for (int i = 0; i < bf.GetLength(); i++) {
        if (bf.GetBit(i)) ostr << "1";
        else { ostr << "0"; }
    }
    return ostr;
}
