
// Very very crude vector implementation
// Allocates memory on the heap
// Does not allow items to be removed
// Provides only the basic functionality we require for this project
// Not good practice either
// I would recommend not reading any further

#if !defined(__INC_VECTOR_H)
#define __INC_VECTOR_H

namespace not_std
{

#define INITIAL_SIZE    (16)

template<typename T>
class vector
{
public:
  vector(void)
  {
    m_buff = (T *)malloc(INITIAL_SIZE * sizeof(T));
  }

  ~vector()
  {
    free(m_buff);
  }

  void push_back(T item)
  {
    m_size++;

    if (m_size >= INITIAL_SIZE)
    {
      m_buff = (T *)realloc(m_buff, m_size * sizeof(T));
    }

    m_buff[m_size - 1] = item;
  }

  T& at(size_t idx)
  {
    if (idx < m_size)
    {
      return m_buff[idx];
    }
    else
    {
      while(1);
    }
  }

  size_t size(void)
  {
    return m_size;
  }

private:
  T *m_buff;
  size_t m_size;
};

}

#endif /* __INC_VECTOR_H */
