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
    if (len < 0) throw "TBitField::TBitField: len is negative";
    if (len == 0) {
        pMem = nullptr;
        BitLen = 0;
        MemLen = 0;
    }
    else {
        BitLen = len;  // длина битового поля
        if (len % 8 == 0)
            MemLen = BitLen /*len*/ / sizeof(TELEM);  // если остаток от деления len на 8 = 0, то кол-во эл-в битового поля равно len
        else {                              // кол-во эл-в битового поля равно len+1 (чтобы вместить оставшиеся эл-ы)
            MemLen = (BitLen /*len*/ / sizeof(TELEM) + 1);
        }
        pMem = new TELEM[MemLen];       // выделяю память для битового поля 
        for (int i = 0; i < MemLen; i++) pMem[i] = 0;  // заполняю массив БП нулями по умолчанию
    }
    
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;   // копирование длины
    MemLen = bf.MemLen;   //  копирование кол-ва эл-в
    pMem = new TELEM[bf.MemLen];
    for (size_t i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];     // копирование битов 
    }
}

TBitField::~TBitField()
{
    if (pMem != nullptr) delete[] pMem;  // удаление выделенной памяти
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n > BitLen) throw "GetMemIndex: wrong n";
    return n / (sizeof(TELEM) * 8);    // получить индекс в памяти
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if ((n < 0) || (n > BitLen)) throw "GetMemMask: wrong n";
    return  (1 << ((n - 1) % (sizeof(TELEM) * 8)));        // побитовый сдвиг в лево bitset<4> int i(1) = 0100; i << 1 = 1000
}

//// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n < 0) || (n > BitLen)) throw "SetBit: wrong n";
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n > BitLen)) throw "ClrBit: wrong n";
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}


int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n < 0) || (n > BitLen)) throw "GetBit: wrong n";
    if ((pMem[GetMemIndex(n)] & GetMemMask(n)) > 0) return 1;
    else return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this != &bf) {
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        this->~TBitField();
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen == bf.BitLen && MemLen == bf.MemLen) {
        int f = 0;
        for (int i = 0; i < MemLen; i++)
            if (pMem[i] != bf.pMem[i]) f++;
        if (f) return 0; // Если хотябы один бит отличается - False
        else return 1;   // значит длина поля, кол-во эл-в, биты одинаковые - True
    }
    else return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen || MemLen != bf.MemLen) return 1; 
    else
    {
        int f = 0;  // флаг, по умолчанию ложь (все одинаковые)
        for (int i = 0; i < MemLen; i++)
            if (pMem[i] != bf.pMem[i]) f = 1;  // если все биты одинаковые флаг останется ложь иначе истина
        if (f) return 1; // Разные - True
        else return 0;   // Одинаковые - False
    }
    
    //if (*this == bf) return 0;   // сравнение самого себя с bf
    //else return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int maxlen = BitLen;
    if (maxlen < bf.BitLen) // длина итогового поля равна длине самого длинного поля
        maxlen = bf.BitLen;
    TBitField t(maxlen);
    for (int i = 0; i < MemLen; i++)
        t.pMem[i] = pMem[i];   // копируем все эл-ты в t
    for (int i = 0; i < bf.MemLen; i++)
        t.pMem[i] |= bf.pMem[i]; // побитовое или с ->
    return t;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int maxlen = BitLen;
    if (maxlen < bf.BitLen)
        maxlen = bf.BitLen;
    TBitField t(maxlen);
    for (int i = 0; i < MemLen; i++)
        t.pMem[i] = pMem[i];
    for (int i = 0; i < bf.MemLen; i++)
        t.pMem[i] &= bf.pMem[i];
    return t;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField A(BitLen);
    for (int i = 0; i < BitLen; i++)
    {
        if (GetBit(i) == 0)
            A.SetBit(i);
    }
    return A;
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
