#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H
#include <cstring>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <iostream>
#define MIN_CAPA 100
namespace aisdi
{

template <typename Type>
class Vector
{
public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
private:
    size_type capa;
    pointer elements;
    size_type start;
    size_type finish;
public:
  Vector():
      capa(MIN_CAPA), start(0), finish(0)
  {
      elements = new value_type[capa];
  }

  Vector(std::initializer_list<Type> l):
      start(0),finish(0)
  {
      capa = l.size();
      elements = new value_type[capa];
    for (auto element: l)
    {
        append(element);
    }
  }

  Vector(const Vector& other):
      capa(other.capa),start(other.start), finish(other.finish)
  {
      elements = new value_type [capa];
      memcpy(elements + start, other.elements + other.start,sizeof(value_type) * getSize());
  }

  Vector(Vector&& other):
      capa(other.capa),start(other.start),finish(other.finish)
  {
      elements = other.elements;
      other.elements = nullptr;
  }

  ~Vector()
  {
      delete [] elements;
  }

  Vector& operator=(const Vector& other)
  {
	if( this == &other )
			return *this;
    capa = other.capa;
    start = other.start;
    finish = other.finish;
    delete [] elements;
    elements = new value_type[capa];
    memcpy(elements+start,other.elements+other.start,sizeof(value_type)*getSize());
    return *this;
  }

  Vector& operator=(Vector&& other)
  {
	if( this == &other )
			return *this;
    finish = other.finish;
    capa = other.capa;
    start = other.start;
    delete [] elements;
    elements = new value_type[capa];
    elements = other.elements;
    other.elements = nullptr;
    return *this;
  }

  bool isEmpty() const
  {
    if (getSize() == 0)
        return true;
    return false;
  }

  size_type getSize() const
  {
    return finish - start;
  }



  void resizeIfNeeded()
  {
    if(getSize() >= capa)
    {
      pointer newElements;
      capa = capa + capa;
      newElements = new value_type[capa];
      for(size_type i = 0; i < getSize(); i++)
      {
        newElements[i] = elements[i];
      }
      delete[] elements;
      elements = newElements;
    }
  }




  void append(const Type& item)
  {
       resizeIfNeeded();
            elements[finish++] = item;
  }

  void prepend(const Type& item)
  {
      resizeIfNeeded();
        for(int i = getSize()-1; i >= 0; i--)
        {
            elements[i+1] = elements[i];
        }
        elements[0] = item;
        finish++;
  }
    size_type get_capa(){
            return capa;
    }
  void insert(const const_iterator& insertPosition, const Type& item)
  {

	if(insertPosition == end())
		{
			append(item);
			return;

		}

	if( insertPosition == begin())
		{
			prepend(item);
			return;
		}
	if(finish == capa)
		{			
			pointer newElements = new value_type[capa += capa];

			size_type I = start;

			for( auto i = begin(); i != insertPosition ; i++ )
			{
				newElements[I] = elements [I];
				I++;

			}	
			newElements[I] = item;	

			for( auto i = iterator(insertPosition); i != end(); i ++ )
			{
				newElements[I + 1] = elements[I];
				I++;
			}

			delete[] elements;
			elements = newElements;
			finish++;
		}

	else
		{
			finish++;
			for ( auto i = end() - 1 ; i != insertPosition ; i -- )
			{
				*i = std::move( *( i - 1 ) );
			}

			*iterator( insertPosition ) = item;
		}

  }

  Type popFirst()
  {
    if (isEmpty())
        throw std::out_of_range( "Can't pop from an empty vector." );
    value_type val = elements[start];
    erase (const_iterator(begin()));
    return val;
  }

  Type popLast()
  {
    if (isEmpty())
        throw std::out_of_range( "Can't pop from an empty vector." );
    value_type val = elements[finish-1];
    erase (const_iterator(end()-1));
    return val;
  }

  void erase(const const_iterator& possition)
  {
	erase(possition, possition + 1 );	//no code duplication
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {

	if(firstIncluded == lastExcluded)
			return;

		auto toReturn = iterator( firstIncluded );

		for ( auto lEx = iterator( lastExcluded ); lEx != end(); ++lEx )
		{
			*toReturn = *lEx;
			++toReturn;
		}

		size_type counter = 0;
		for( auto it = firstIncluded; it != lastExcluded; ++it )
		{
			counter++;
		}
		finish -= counter;
		return;

  }

  iterator begin()
  {
        return iterator(cbegin());
  }

  iterator end()
  {
        return iterator(cend());
  }

  const_iterator cbegin() const
  {
        return ConstIterator(this, &elements[start]);
  }

  const_iterator cend() const
  {
        return ConstIterator(this, &elements[finish]);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename Type>
class Vector<Type>::ConstIterator
{
private:
    friend class Vector;
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;

private:
    pointer  val;
    Vector<Type> const *Vec;

public:
    explicit ConstIterator( Vector<Type> const * VecPointer, pointer ValPointer )
		: val( ValPointer ), Vec( VecPointer )
  {}

  reference operator*() const
  {
    if (*this == (Vec->end()))
        throw std::out_of_range("Vector end");
    return *val;
  }
  ConstIterator& operator++()
  {
    if( *this == ( Vec -> end() ) )
			throw std::out_of_range( "Incrementing vector end" );
    ++val;
    return *this;
  }

  ConstIterator operator++( int )
	{
		auto toReturn = *this;
		operator++();
		return toReturn;
	}

	ConstIterator& operator--()
	{
		if( *this == ( Vec -> begin() ) )
			throw std::out_of_range( "Decrementing vector begin" );

		--val;
		return *this;
	}

	ConstIterator operator--( int )
	{
		auto toReturn = *this;
		operator--();
		return toReturn;
	}

	ConstIterator operator+( difference_type d ) const
	{
		auto toReturn = *this;

		for (; d > 0 ; d--)
			--toReturn;

		return toReturn;
	}

	ConstIterator operator-( difference_type d ) const
	{
		auto toReturn = *this;

		for (; d > 0; d--)
			--toReturn;

		return toReturn;
	}

	bool operator==( const ConstIterator& other ) const
	{
		if( val == other.val )
			return true;

		return false;
	}

	bool operator!=( const ConstIterator& other ) const
	{
		if ( val != other.val )
			return true;

		return false;
	}



};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
private:
    friend class Vector;
public:
	using pointer = typename Vector::pointer;
	using reference = typename Vector::reference;

	explicit Iterator()
	{}

	Iterator( const ConstIterator& other )
		: ConstIterator( other )
	{}

	Iterator& operator++()
	{
		ConstIterator::operator++();
		return *this;
	}

	Iterator operator++( int )
	{
		auto toReturn = *this;
		ConstIterator::operator++();
		return toReturn;
	}

	Iterator& operator--()
	{
		ConstIterator::operator--();
		return *this;
	}

	Iterator operator--( int )
	{
		auto toReturn = *this;
		ConstIterator::operator--();
		return toReturn;
	}

	Iterator operator+( difference_type d ) const
	{
		return ConstIterator::operator+( d );
	}

	Iterator operator-( difference_type d ) const
	{
		return ConstIterator::operator-( d );
	}

	reference operator*() const
	{
		return const_cast<reference>( ConstIterator::operator*() );
	}
};

}

#endif // AISDI_LINEAR_VECTOR_H
