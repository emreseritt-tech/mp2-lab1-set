// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

#include <string>

static const int BITS_IN_ELEM = sizeof(TELEM) * 8;

TBitField::TBitField(int len)
{
  if (len < 0)
    throw "negative bitfield length";
  BitLen = len;
  MemLen = (len + BITS_IN_ELEM - 1) / BITS_IN_ELEM;
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++)
    pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++)
    pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
  delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  return n / BITS_IN_ELEM;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  return (TELEM)1 << (n % BITS_IN_ELEM);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  if (n < 0 || n >= BitLen)
    throw "bit index out of range";
  pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if (n < 0 || n >= BitLen)
    throw "bit index out of range";
  pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if (n < 0 || n >= BitLen)
    throw "bit index out of range";
  return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
  if (this == &bf)
    return *this;
  if (MemLen != bf.MemLen)
  {
    delete[] pMem;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
  }
  BitLen = bf.BitLen;
  for (int i = 0; i < MemLen; i++)
    pMem[i] = bf.pMem[i];
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if (BitLen != bf.BitLen)
    return 0;
  for (int i = 0; i < MemLen; i++)
    if (pMem[i] != bf.pMem[i])
      return 0;
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  TBitField res(BitLen > bf.BitLen ? BitLen : bf.BitLen);
  for (int i = 0; i < MemLen; i++)
    res.pMem[i] = pMem[i];
  for (int i = 0; i < bf.MemLen; i++)
    res.pMem[i] |= bf.pMem[i];
  return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
  TBitField res(BitLen > bf.BitLen ? BitLen : bf.BitLen);
  int minMemLen = MemLen < bf.MemLen ? MemLen : bf.MemLen;
  for (int i = 0; i < minMemLen; i++)
    res.pMem[i] = pMem[i] & bf.pMem[i];
  return res;
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField res(BitLen);
  for (int i = 0; i < MemLen; i++)
    res.pMem[i] = ~pMem[i];
  // обнуляем "лишние" биты последнего элемента, не входящие в поле
  int tail = BitLen % BITS_IN_ELEM;
  if (tail != 0)
    res.pMem[MemLen - 1] &= ((TELEM)1 << tail) - 1;
  return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  // формат: строка из символов '0' и '1', i-й символ - значение i-го бита
  string s;
  istr >> s;
  for (int i = 0; i < bf.BitLen; i++)
  {
    if (i < (int)s.size() && s[i] == '1')
      bf.SetBit(i);
    else
      bf.ClrBit(i);
  }
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  for (int i = 0; i < bf.BitLen; i++)
    ostr << bf.GetBit(i);
  return ostr;
}
