// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem < 0 || Elem > MaxPower) {
        throw "IsMember: Elem is negative!";
    }
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem > MaxPower) 
        throw "InsElem: Elem is negative!";
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem > MaxPower) 
        throw "DelElem: Elem is negative!";
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if (BitField == s.BitField) return 1;
    else return 0;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    if (BitField != s.BitField) return 1;
    else return 0;
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet t(BitField | s.BitField); // a | b = 1 (01, 10, 11)
    return t;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet t(*this);
    if (Elem < 0 || Elem > MaxPower)
        throw "TSet::operator+: Elem is negative!";
    else t.InsElem(Elem);
    return t;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet t(*this);
    if (Elem < 0 || Elem > MaxPower)
        throw "TSet::operator-: Elem is negative!";
    else t.DelElem(Elem);
    return t;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet t(BitField & s.BitField);  // a & b = 1 (11)
    return t;
}

TSet TSet::operator~(void) // дополнение
{
    TSet t(~BitField);  // (a = 1 , ~a = 0)
    return t;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    cin >> s.BitField;
    s.MaxPower = s.BitField.GetLength();
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    cout << s.BitField;
    return ostr;
}
