#pragma once


/////////////////////////////////////////////////////////////////////////////
// point - A 2-D point, similar to Windows POINT structure.

class CLASS_DECL_AURA point : public POINT
{
   public:
// Constructors

      typedef POINT BASE;

      typedef LONG TYPE;

      typedef ::LPRECT LPRECT;

      typedef ::rect RECT;

      typedef ::size SIZE;

      // create an uninitialized point
      point() NOTHROW;
      // create from two integers
      point(int64_t initX, int64_t initY) NOTHROW;
      // create from another point
      point(POINT initPt) NOTHROW;
      point(const POINT * pinitPt) NOTHROW;
      point(POINT64 initPt) NOTHROW;
      point(const POINT64 * pinitPt) NOTHROW;
      point(POINTD initPt) NOTHROW;
      point(const POINTD * pinitPt) NOTHROW;
      // create from a size
      point(SIZE initSize) NOTHROW;
      // create from an LPARAM: x = LOWORD(dw) y = HIWORD(dw)
      point(const ::lparam & dwPoint) NOTHROW;


      operator POINT *() NOTHROW;
      operator const POINT *() const NOTHROW;

// Operations

// translate the point
      void offset(int32_t xOffset, int32_t yOffset) NOTHROW;
      void offset(POINT point) NOTHROW;
      void offset(SIZE size) NOTHROW;
      void SetPoint(int32_t X, int32_t Y) NOTHROW;


      uint32_t uint32() const NOTHROW;
      uint64_t uint64() const NOTHROW;
      ::lparam lparam() const NOTHROW;

      operator ::lparam() const { return lparam(); }

      inline LONG get_dimension(e_orientation eorientation) const NOTHROW;
      inline LONG get_orthogonal_dimension(e_orientation eorientation) const NOTHROW;
      inline LONG get_orthogonal(e_orientation eorientation)const NOTHROW { return get_orthogonal_dimension(eorientation); }
      inline LONG get_normal_dimension(e_orientation eorientation) const NOTHROW { return get_orthogonal_dimension(eorientation); }
      inline LONG get_normal(e_orientation eorientation) const NOTHROW { return get_orthogonal_dimension(eorientation); }

      inline LONG set_dimension(e_orientation eorientation,LONG l) NOTHROW;
      inline LONG set_orthogonal_dimension(e_orientation eorientation,LONG l) NOTHROW;
      inline LONG set_orthogonal(e_orientation eorientation,LONG l)  NOTHROW { return set_orthogonal_dimension(eorientation,l); }
      inline LONG set_normal_dimension(e_orientation eorientation,LONG l)  NOTHROW { return set_orthogonal_dimension(eorientation,l); }
      inline LONG set_normal(e_orientation eorientation,LONG l) NOTHROW { return set_orthogonal_dimension(eorientation,l); }

};


inline void operator+=(point & p, SIZE size) NOTHROW;
inline bool operator==(const point & p, POINT point) NOTHROW;
inline bool operator!=(const point & p, POINT point) NOTHROW;
inline void operator-=(point & p, SIZE size) NOTHROW;
inline void operator+=(point & p, POINT point) NOTHROW;
inline void operator-=(point & p, POINT point) NOTHROW;
inline point operator+(const point & p, SIZE size) NOTHROW;
inline point operator-(const point & p, SIZE size) NOTHROW;
inline point operator-(const point & p) NOTHROW;
inline point operator+(const point & p, POINT pt) NOTHROW;
inline size operator-(const point & p, POINT  pt) NOTHROW;
inline rect operator+(const point & p, LPCRECT lpRect) NOTHROW;
inline rect operator-(const point & p, LPCRECT lpRect) NOTHROW;
inline bool operator < (const point & p, const point & pt) NOTHROW;

inline point null_point();


/////////////////////////////////////////////////////////////////////////////
// point64 - A 2-D point64, similar to Windows POINT64 structure.

class CLASS_DECL_AURA point64 : public POINT64
{
   public:
// Constructors
      typedef POINT64 BASE;

      typedef int64_t TYPE;

      typedef ::LPRECT64 LPRECT;

      typedef ::rect64 RECT;

      typedef ::size64 SIZE;

      // create an uninitialized point64
      point64() NOTHROW;
      // create from two integers
      point64(int64_t initX, int64_t initY) NOTHROW;
      // create from another point64
      point64(POINT64 initPt) NOTHROW;
      point64(POINT pt) NOTHROW;
      point64(const POINT64 * pinitPt) NOTHROW;
      // create from a size
      point64(SIZE64 initSize) NOTHROW;
      // create from an UINT64: x = LODWORD(dw) y = HIDWORD(dw)
      point64(const ::lparam & dwPoint) NOTHROW;

      operator POINT64 *() NOTHROW;
      operator const POINT64 *() const NOTHROW;

// Operations

// translate the point64
      void offset(int64_t xOffset, int64_t yOffset) NOTHROW;
      void offset(POINT64 point64) NOTHROW;
      void offset(SIZE64 size) NOTHROW;
      void SetPoint(int64_t X, int64_t Y) NOTHROW;

      bool operator==(POINT64 point64) const NOTHROW;
      bool operator!=(POINT64 point64) const NOTHROW;
      void operator+=(SIZE64 size) NOTHROW;
      void operator-=(SIZE64 size) NOTHROW;
      void operator+=(POINT64 point64) NOTHROW;
      void operator-=(POINT64 point64) NOTHROW;

// Operators returning point64 values
      point64 operator+(SIZE64 size) const NOTHROW;
      point64 operator-(SIZE64 size) const NOTHROW;
      point64 operator-() const NOTHROW;
      point64 operator+(POINT64 point64) const NOTHROW;

// Operators returning size values
      size64 operator-(POINT64 point64) const NOTHROW;

// Operators returning rect values
      rect64 operator+(const RECT64 * lpRect) const NOTHROW;
      rect64 operator-(const RECT64 * lpRect) const NOTHROW;

      uint32_t ui32() const NOTHROW;
      uint64_t ui64() const NOTHROW;
      LPARAM   lparam() const NOTHROW;

      inline int64_t get_dimension(e_orientation eorientation) const NOTHROW;
      inline int64_t get_orthogonal_dimension(e_orientation eorientation) const NOTHROW;
      inline int64_t get_orthogonal(e_orientation eorientation)const NOTHROW { return get_orthogonal_dimension(eorientation); }
      inline int64_t get_normal_dimension(e_orientation eorientation) const NOTHROW { return get_orthogonal_dimension(eorientation); }
      inline int64_t get_normal(e_orientation eorientation) const NOTHROW { return get_orthogonal_dimension(eorientation); }

      inline int64_t set_dimension(e_orientation eorientation,int64_t l) NOTHROW;
      inline int64_t set_orthogonal_dimension(e_orientation eorientation,int64_t l) NOTHROW;
      inline int64_t set_orthogonal(e_orientation eorientation,int64_t l)  NOTHROW { return set_orthogonal_dimension(eorientation,l); }
      inline int64_t set_normal_dimension(e_orientation eorientation,int64_t l)  NOTHROW { return set_orthogonal_dimension(eorientation,l); }
      inline int64_t set_normal(e_orientation eorientation,int64_t l) NOTHROW { return set_orthogonal_dimension(eorientation,l); }


};







/////////////////////////////////////////////////////////////////////////////
// pointd - A 2-D pointd, similar to Windows POINTD structure.

class CLASS_DECL_AURA pointd : public POINTD
{
   public:
// Constructors
      typedef POINTD BASE;

      typedef double TYPE;

      typedef ::LPRECTD LPRECT;

      typedef ::rectd RECT;

      typedef ::sized SIZE;

      // create an uninitialized pointd
      pointd() NOTHROW;
      // create from two integers
      pointd(double initX, double initY) NOTHROW;
      // create from another pointd
      pointd(POINTD initPt) NOTHROW;
      pointd(POINT initPt) NOTHROW;
      pointd(const POINTD * pinitPt) NOTHROW;
      pointd(POINT64 initPt) NOTHROW;
      pointd(const POINT64 * pinitPt) NOTHROW;
      // create from a sized
      pointd(SIZED initSize) NOTHROW;
      // create from an LPARAM: x = LOWORD(dw) y = HIWORD(dw)
      pointd(const ::lparam & dwPoint) NOTHROW;


      operator POINTD *() NOTHROW;
      operator const POINTD *() const NOTHROW;

// Operations

// translate the pointd
      void offset(int32_t xOffset, int32_t yOffset) NOTHROW;
      void offset(POINTD pointd) NOTHROW;
      void offset(SIZED sized) NOTHROW;
      void SetPoint(int32_t X, int32_t Y) NOTHROW;

      bool operator==(POINTD pointd) const NOTHROW;
      bool operator!=(POINTD pointd) const NOTHROW;
      void operator+=(SIZED sized) NOTHROW;
      void operator-=(SIZED sized) NOTHROW;
      void operator+=(POINTD pointd) NOTHROW;
      void operator-=(POINTD pointd) NOTHROW;

// Operators returning pointd values
      pointd operator+(SIZED sized) const NOTHROW;
      pointd operator-(SIZED sized) const NOTHROW;
      pointd operator-() const NOTHROW;
      pointd operator+(POINTD pointd) const NOTHROW;

// Operators returning sized values
      sized operator-(POINTD pointd) const NOTHROW;

// Operators returning rectd values
      rectd operator+(LPCRECTD lpRect) const NOTHROW;
      rectd operator-(LPCRECTD lpRect) const NOTHROW;

      uint32_t ui32() const NOTHROW;
      uint64_t ui64() const NOTHROW;
      LPARAM   lparam() const NOTHROW;

      inline double get_dimension(e_orientation eorientation) const NOTHROW;
      inline double get_orthogonal_dimension(e_orientation eorientation) const NOTHROW;
      inline double get_orthogonal(e_orientation eorientation)const NOTHROW { return get_orthogonal_dimension(eorientation); }
      inline double get_normal_dimension(e_orientation eorientation) const NOTHROW { return get_orthogonal_dimension(eorientation); }
      inline double get_normal(e_orientation eorientation) const NOTHROW { return get_orthogonal_dimension(eorientation); }

      inline double set_dimension(e_orientation eorientation,double l) NOTHROW;
      inline double set_orthogonal_dimension(e_orientation eorientation,double l) NOTHROW;
      inline double set_orthogonal(e_orientation eorientation,double l)  NOTHROW { return set_orthogonal_dimension(eorientation,l); }
      inline double set_normal_dimension(e_orientation eorientation,double l)  NOTHROW { return set_orthogonal_dimension(eorientation,l); }
      inline double set_normal(e_orientation eorientation,double l) NOTHROW { return set_orthogonal_dimension(eorientation,l); }

};


inline pointd null_pointd() { return pointd(0.0, 0.0); }



inline LONG point::get_dimension(e_orientation eorientation) const NOTHROW
{
   if(eorientation == orientation_horizontal)
   {
      return x;
   }
   else if(eorientation == orientation_vertical)
   {
      return y;
   }
   else
   {
      return 0;
   }
}


inline LONG point::get_orthogonal_dimension(e_orientation eorientation) const NOTHROW
{
   if(eorientation == orientation_horizontal)
   {
      return y;
   }
   else if(eorientation == orientation_vertical)
   {
      return x;
   }
   else
   {
      return 0;
   }
}


inline LONG point::set_dimension(e_orientation eorientation,LONG l) NOTHROW
{
   if(eorientation == orientation_horizontal)
   {
      return x = l;
   }
   else if(eorientation == orientation_vertical)
   {
      return y = l;
   }
   else
   {
      return 0;
   }
}


inline LONG point::set_orthogonal_dimension(e_orientation eorientation,LONG l) NOTHROW
{
   if(eorientation == orientation_horizontal)
   {
      return y = l;
   }
   else if(eorientation == orientation_vertical)
   {
      return x = l;
   }
   else
   {
      return 0;
   }
}



inline int64_t point64::get_dimension(e_orientation eorientation) const NOTHROW
{
   if(eorientation == orientation_horizontal)
   {
      return x;
   }
   else if(eorientation == orientation_vertical)
   {
      return y;
   }
   else
   {
      return 0;
   }
}


inline int64_t point64::get_orthogonal_dimension(e_orientation eorientation) const NOTHROW
{
   if(eorientation == orientation_horizontal)
   {
      return y;
   }
   else if(eorientation == orientation_vertical)
   {
      return x;
   }
   else
   {
      return 0;
   }
}


inline int64_t point64::set_dimension(e_orientation eorientation,int64_t l) NOTHROW
{
   if(eorientation == orientation_horizontal)
   {
      return x = l;
   }
   else if(eorientation == orientation_vertical)
   {
      return y = l;
   }
   else
   {
      return 0;
   }
}


inline int64_t point64::set_orthogonal_dimension(e_orientation eorientation,int64_t l) NOTHROW
{
   if(eorientation == orientation_horizontal)
   {
      return y = l;
   }
   else if(eorientation == orientation_vertical)
   {
      return x = l;
   }
   else
   {
      return 0;
   }
}


inline double pointd::get_dimension(e_orientation eorientation) const NOTHROW
{
   if(eorientation == orientation_horizontal)
   {
      return x;
   }
   else if(eorientation == orientation_vertical)
   {
      return y;
   }
   else
   {
      return 0;
   }
}


inline double pointd::get_orthogonal_dimension(e_orientation eorientation) const NOTHROW
{
   if(eorientation == orientation_horizontal)
   {
      return y;
   }
   else if(eorientation == orientation_vertical)
   {
      return x;
   }
   else
   {
      return 0;
   }
}


inline double pointd::set_dimension(e_orientation eorientation,double l) NOTHROW
{
   if(eorientation == orientation_horizontal)
   {
      return x = l;
   }
   else if(eorientation == orientation_vertical)
   {
      return y = l;
   }
   else
   {
      return 0;
   }
}


inline double pointd::set_orthogonal_dimension(e_orientation eorientation,double l) NOTHROW
{
   if(eorientation == orientation_horizontal)
   {
      return y = l;
   }
   else if(eorientation == orientation_vertical)
   {
      return x = l;
   }
   else
   {
      return 0;
   }
}



template<> inline UINT HashKey<point> (point key)
{
   return key.x | key.y;
}



struct CLASS_DECL_AURA POINT3D
{
   double x;
   double y;
   double z;
};

class CLASS_DECL_AURA point3d:
   public POINT3D
{
   public:

      point3d() {}
      point3d(double xP,double yP,double zP) { x = xP; y=yP; z=zP; }

      point3d  operator +(const point3d &p1) const
      {
         point3d p;
         p.x =x + p1.x;
         p.y =y + p1.y;
         p.z =z + p1.z;
         return p;
      }
      point3d & operator +=(const point3d &p)
      {
         x += p.x;
         y += p.y;
         z += p.z;
         return *this;
      }

      point3d & operator -=(const point3d &p)
      {
         x -= p.x;
         y -= p.y;
         z -= p.z;
         return *this;
      }

      point3d & operator = (const point3d & p)
      {
         x = p.x;
         y = p.y;
         z = p.z;
         return *this;
      }

      point3d rotate(point3d d);

};

