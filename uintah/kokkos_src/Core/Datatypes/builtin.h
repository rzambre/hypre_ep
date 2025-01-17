/*
 * The MIT License
 *
 * Copyright (c) 1997-2017 The University of Utah
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/* builtin.h
 *
 *  Written by:
 *   Yarden Livnat
 *   Department of Computer Science
 *   University of Utah
 *   March 2001
 *
 *  Classes for built in datatypes
 */

#ifndef builtin_h
#define builtin_h

#include <Core/Datatypes/TypeName.h>

namespace Uintah {

class Scalar {
public:
  virtual ~Scalar() {}
  virtual operator char() = 0;
  virtual operator short() = 0;
  virtual operator int() = 0;
  virtual operator float() = 0;
  virtual operator double() = 0;
};

template<class T>
class ScalarType : public Scalar{
public:
  T val_;
  
  ScalarType() {}
  ScalarType( T v ) : val_(v) {}

  void operator=( const ScalarType &copy ) { val_ = copy.val_;}
  operator char()   { return char(val_); }
  operator short()  { return short(val_); }
  operator int()    { return int(val_); }
  operator float()  { return float(val_); }
  operator double() { return double(val_); }
};

typedef ScalarType<char>   Char;
typedef ScalarType<unsigned char>   UChar;
typedef ScalarType<short>  Short;
typedef ScalarType<unsigned short>  UShort;
typedef ScalarType<int>    Int;
typedef ScalarType<unsigned int>    UInt;
typedef ScalarType<long long> LongLong;
typedef ScalarType<float>  Float;
typedef ScalarType<double> Double;


inline const std::string find_type_name(Char*)  {return find_type_name((char *)0);}
inline const std::string find_type_name(UChar*) {return find_type_name((unsigned char *)0);}
inline const std::string find_type_name(Short*) {return find_type_name((short *)0);}
inline const std::string find_type_name(UShort*){return find_type_name((unsigned short *)0);}
inline const std::string find_type_name(Int*)   {return find_type_name((int *)0);}
inline const std::string find_type_name(UInt*)  {return find_type_name((unsigned int *)0);}
inline const std::string find_type_name(Float*) {return find_type_name((float *)0);}
inline const std::string find_type_name(Double*){return find_type_name((double *)0);}
inline const std::string find_type_name(LongLong*){return find_type_name((double *)0);}

template<class T> bool is_scalar() { return false; }
template<> inline bool is_scalar<char>() { return true; }
template<> inline bool is_scalar<unsigned char>() { return true; }
template<> inline bool is_scalar<short>() { return true; }
template<> inline bool is_scalar<unsigned short>() { return true; }
template<> inline bool is_scalar<int>() { return true; }
template<> inline bool is_scalar<float>() { return true; }
template<> inline bool is_scalar<double>() { return true; }


} // end namespace Uintah

#endif // builtin_h
