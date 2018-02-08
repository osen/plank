#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

template<class T>
class noncopyable
{
private:
  T m_data;

  noncopyable(const noncopyable&) { }
  const noncopyable& operator=( const noncopyable& ) { return *this; }

public:
  noncopyable() : m_data(0) { }
    
  T& operator=(const T& t)
  {
    m_data = t;
    return m_data;
  }

/*
  T& operator=(T& t)
  {
    m_data = t;
    return m_data;
  }
*/

  T& data() { return m_data ; }

  operator const T&() const
  {
    return m_data;
  }

  operator T&()
  {
    return m_data;
  }

  T& operator->()
  {
    return m_data;
  }

/*
  const T& operator->() const
  {
    return m_data;
  }
*/

};

#endif
