#if !defined(MYVECTOR)
#define MYVECTOR

#include <cmath>
#include <process.h>

const float EPS=0.005;
extern const float PI;

class vec2
{
	public:
		float x,y;
		vec2(){}
		vec2(float v):x(v),y(v){}
		vec2(float px,float py):x(px),y(py){}
		vec2(const vec2& v):x(v.x),y(v.y){}
		vec2& operator=(const vec2& v)
		{
			x=v.x;
			y=v.y;
			return *this;
		}
		vec2 operator +()const 
		{
			return *this;
		}
		vec2 operator -()const 
		{
			return vec2(-x,-y);
		}
		vec2& operator +=(const vec2& v)
		{
			x+=v.x;
			y+=v.y;
			return *this;
		}
		vec2& operator -=(const vec2& v)
		{
			x-=v.x;
			y-=v.y;
			return *this;
		}
		vec2& operator *=(const vec2& v)
		{
			x*=v.x;
			y*=v.y;
			return *this;
		}

		vec2& operator /=(const vec2& v)
		{
			x/=v.x;
			y/=v.y;
			return *this;
		}
		float& operator [](int index)
		{
			return *(index+&x);
		}
		int operator ==(const vec2& v)const
		{
			return x==v.x&&y==v.y;
		}
		int operator !=(const vec2& v)const
		{
			return x!=v.x&&y!=v.y;
		}
		operator float*()
		{
			return &x;
		}
		operator const float*()const
		{
			return &x;
		}
		float length()const
		{
			return (float)sqrt(x*x+y*y);
		}
		float lengthSq()const
		{
			return x*x+y*y;
		}
		vec2& normalize()
		{
			return (*this)/=length();
		}

		float maxLength()const
		{
			return max2((float)fabs(x),(float)fabs(y));
		}

		float distanceToSq(const vec2& p)const
		{
			return (float)sqrt(sqr(x-p.x)+sqr(y-p.y));
		}
		vec2 ort()const
		{
			return vec2(-y,x);
		}
		vec2& clamp(float lower, float upper)
		{
			if(x<lower)
				x=lower;
			else 
				if (x>upper)
					x=upper;
			if (y<lower)
				y=lower;
			else
				if (y>upper)
					y=upper;
			return *this;
		}
		static vec2 getRandomVector(float len=1);
		static inline vec2 vmin(const vec2& v1, const vec2& v2)
		{
			return vec2(v1.x<v2.x?v1.x:v2.x,v1.y<v2.y?v1.y:v2.y);
		}
		static inline vec2 vmax(const vec2& v1, const vec2& v2)
		{
			return vec2(v1.x>v2.x?v1.x:v2.x,v1.y>v2.y?v1.y:v2.y);
		}
		friend vec2 operator +(const vec2&, const vec2&);
		friend vec2 operator -(const vec2&, const vec2&);
		friend vec2 operator *(const vec2&, const vec2&);
		friend vec2 operator *(float, const vec2&);
		friend vec2 operator *(const vec2&, float);
		friend vec2 operator /(const vec2&, float );
		friend vec2 operator /(const vec2&, const vec2&);
		friend float operator &(const vec2&, const vec2&);
		void show()const
		{
		}
		void rotate(float angle){
			//angle Radian
			float xt=x*cos(angle)-y*sin(angle);
			float yt=x*sin(angle)+y*cos(angle);
			x=xt;
			y=yt;
		}
	private:
		float max2(float a,float b) const
		{
			return a>b?a:b;
		}
		float min2(float a,float b) const
		{
			return a<b?a:b;
		}
		float sqr(float x)const
		{
			return x*x;
		}
		static vec2 zero;
		static vec2 one;
};



inline vec2 operator + (const vec2& u, const vec2& v)
{
	return vec2 (u.x+v.x,u.y+v.y);
}

inline vec2 operator - (const vec2& u, const vec2& v)
{
	return vec2 (u.x-v.x,u.y-v.y);
}

inline vec2 operator * (const vec2& u, const vec2& v)
{
	return vec2 (u.x*v.x,u.y*v.y);
}

inline vec2 operator * (const vec2& u,  float a)
{
	return vec2 (u.x*a,u.y*a);
}

inline vec2 operator * ( float a,const vec2& u )
{
	return vec2 (u.x*a,u.y*a);
}

inline vec2 operator / ( const vec2& u,const vec2& v )
{
	return vec2 (u.x/v.x,u.y/v.y);
}
inline vec2 operator / ( const vec2& v,float a )
{
	return vec2 (v.x/a,v.y/a);
}
inline vec2 operator / ( float a, const vec2& v )
{
	return vec2 (a/v.x,a/v.y);
}

inline bool areCollinear ( const vec2& a, const vec2& b )
{
	return (a.ort()&b)<EPS;
}

inline vec2 lerp ( const vec2& a, const vec2& b, float t )
{
	return a+t*(b-a);
}

inline float dot ( const vec2& a, const vec2& b )
{
	return a.x*b.x+a.y*b.y;
}


#endif

