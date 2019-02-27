#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList
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

	struct Node
	{
		value_type value;
		Node* prev;
		Node* next;

		Node( value_type v, Node* p = nullptr ,Node* n= nullptr ):
			value(v), prev(p), next(n)
		{}
	};

	size_type size;
	Node* head;
	Node* tail;

public:


	LinkedList():
		size(0), head(nullptr), tail(nullptr)
	{}

	LinkedList( std::initializer_list<Type> l ):
		LinkedList()
	{
		for( auto element : l )
		{
			append(element);
		}
	}

	LinkedList( const LinkedList& other ):
		LinkedList()
	{
		*this = other;
	}

	LinkedList( LinkedList&& other ):
		LinkedList()
	{
		*this = std::move(other);
	}

	~LinkedList()
	{
		erase(begin(), end());
	}

	LinkedList& operator=( const LinkedList& other )
	{
		if(this == &other)
			return *this;

		erase( begin(), end() );
		for( auto element : other )
		{
			append(element);
		}

		return *this;
	}

	LinkedList& operator=( LinkedList&& other )
	{
		if(this == &other)
			return *this;

		erase( begin(), end() );
		head = other.head;
		tail = other.tail;
		size = other.size;
		other.head = nullptr;
		other.tail = nullptr;
		other.size = 0;
		return *this;
	}

	bool isEmpty() const
	{
		if (getSize() == 0)
			return true;
		else
			return false;
	}

	size_type getSize() const
	{
		return size;
	}

	void append(const Type& item)
	{
		Node* newnode = new Node(item);
		if(isEmpty())
		{
			head = newnode;
			tail = newnode;
		}
		else
		{
			tail -> next = newnode;
			newnode -> prev = tail;
			tail = newnode;
		}
		size++;
	}

	void prepend(const Type& item)
	{
		Node* newnode = new Node(item);

		if(isEmpty())
		{
			head = newnode;
			tail = newnode;
		}
		else
		{
			head -> prev = newnode;
			newnode -> next = head;
			head = newnode;
		}

		size++;
	}

	void insert( const const_iterator& insertPosition, const Type& item)
	{
		if (insertPosition == begin())
		{
			prepend(item);
			return;
		}
		else if(insertPosition == end())
		{
			append(item);
			return;
		}
		else
		{

			Node * oneAfter = insertPosition.n_point;
			Node * onePrev = oneAfter -> prev;
			Node * newNode = new Node(item, onePrev, oneAfter);
			onePrev -> next = newNode;
			oneAfter -> prev = newNode;
		}
	}
	Type popFirst()
	{
		if( isEmpty() )
		{
			throw std::out_of_range( "Can't pop from an empty list." );
		}

		Type toReturn = head->value;
		erase( const_iterator(head, head, tail));
		return toReturn;
	}

	Type popLast()
	{
		if(isEmpty())
		{
			throw std::out_of_range( "Can't pop from an empty list." );
		}
		Type toReturn = tail->value;
		erase(const_iterator(tail, head, tail));
		return toReturn;
	}

	void erase(const const_iterator& possition)
	{
		if( possition == end() )
		{
			throw std::out_of_range( "Can't erase the end." );
		}
		else
			erase(possition, possition + 1);	//no code duplication

	}

	void erase( const const_iterator& firstIncluded, const const_iterator& lastExcluded )
	{
		for ( iterator it = firstIncluded; it != lastExcluded; it++ )
		{
		    Node *deleteNode = it.node_p;
		    if (deleteNode == head && deleteNode == tail)
            {
                head = nullptr;
                tail = nullptr;
                delete deleteNode;
            }
            else if (deleteNode == head)
            {
                head = head -> next;
                head -> prev = nullptr;
                delete deleteNode;
            }
            else if (deleteNode == tail)
            {
                tail = tail -> prev;
                tail -> next = nullptr;
                delete deleteNode;
            }
            else
            {
                Node *oneAfter = deleteNode -> next;
		Node *onePrev = deleteNode -> prev;
                oneAfter -> prev = onePrev;
		onePrev -> next = oneAfter;
                delete deleteNode;
            }
            size --;
		}

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
		return ConstIterator(head, head, tail);
	}

	const_iterator cend() const
	{
		return ConstIterator(nullptr, head, tail);
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
class LinkedList<Type>::ConstIterator
{
private:
	friend class LinkedList;

public:
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = typename LinkedList::value_type;
	using difference_type = typename LinkedList::difference_type;
	using pointer = typename LinkedList::const_pointer;
	using reference = typename LinkedList::const_reference;

private:
	Node* node_p;
	Node* head;
	Node* tail;

public:

	explicit ConstIterator( Node* node_px, Node* h, Node* t ):
		node_p( node_px ), head( h ), tail( t)
	{}

	reference operator*() const
	{
		if( node_p == nullptr )
			throw std::out_of_range( "The end of the list out of range" );
		else
			return node_p -> value;
	}

	ConstIterator& operator++()
	{
		if ( node_p == nullptr )
		{
			throw std::out_of_range( "The end of the list out of range" );
		}
		else
		{
			node_p = node_p -> next;
			return *this;
		}
	}

	ConstIterator operator++( int )
	{
		auto result = *this;
		operator++();
		return result;
	}

	ConstIterator& operator--()
	{
		if( node_p == head )
		{
			throw std::out_of_range( "Beginning of the list out of range" );
		}

		else if ( node_p == nullptr )
		{
			node_p = tail;
			return *this;
		}

		else
		{
			node_p = node_p -> prev;
			return *this;
		}
	}

	ConstIterator operator--(int)
	{
		auto result = *this;
		operator--();
		return result;
	}

	ConstIterator operator+(difference_type d) const
	{
		auto result = *this;

		for (; d > 0 ; d--)
			++result;

		return result;
	}

	ConstIterator operator-(difference_type d) const
	{
		auto result = *this;

		for (; d > 0 ; d--)
			--result;

		return result;
	}

	bool operator==(const ConstIterator& other) const
	{
		if(node_p == other.node_p)
			return true;
		else
			return false;
	}

	bool operator!=(const ConstIterator& other) const
	{
		if(node_p == other.node_p)
			return false;
		else
			return true;
	}
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
	using pointer = typename LinkedList::pointer;
	using reference = typename LinkedList::reference;

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

	Iterator operator++(int)
	{
		auto result = *this;
		ConstIterator::operator++();
		return result;
	}

	Iterator& operator--()
	{
		ConstIterator::operator--();
		return *this;
	}

	Iterator operator--(int)
	{
		auto result = *this;
		ConstIterator::operator--();
		return result;
	}

	Iterator operator+(difference_type d) const
	{
		return ConstIterator::operator+( d );
	}

	Iterator operator-(difference_type d) const
	{
		return ConstIterator::operator-( d );
	}

	reference operator*() const
	{
		// ugly cast, yet reduces code duplication.
		return const_cast<reference>( ConstIterator::operator*() );
	}
};

}

#endif // AISDI_LINEAR_LINKEDLIST_H
