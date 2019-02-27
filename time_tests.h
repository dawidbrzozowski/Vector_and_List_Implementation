#include <cstddef>
#include <cstdlib>
#include <string>
#include <fstream>
#include "Vector.h"
#include "LinkedList.h"
#include <iostream>
#define SMALL_TEST 10000
#define BIG_TEST 100000
namespace
{

template <typename T>
using LinearCollection = aisdi::LinkedList<T>;
template <typename Typ>
using myVector = aisdi::Vector<Typ>;
}
std::fstream plik;

void prepend_test(int test_size, aisdi::Vector<int> vec, aisdi::LinkedList <int> linList)
{//vec
    clock_t vec_endTime;
    clock_t vec_begTime;
    double vec_time;
    clock_t list_endTime;
    clock_t list_begTime;
    double list_time;


    for (int i = 0; i < test_size; i++)
    {
        vec.append(1);
    }
    vec_begTime = clock();
    vec.prepend(0);
    vec_endTime = clock();
    list_begTime = clock();
    linList.prepend(0);
    list_endTime = clock();
    list_time = (list_endTime - list_begTime)/(CLOCKS_PER_SEC/1000000);
    vec_time = (list_endTime - vec_begTime)/(CLOCKS_PER_SEC/1000000);
    plik<< "For "<<test_size<<" elements, prepend for vector took: "<<vec_time<<"us"<<std::endl;
    plik<< "FOr "<<test_size<<" elements, prepend for list took: "<<list_time<<"us"<<std::endl;

}

void erase_from_middle_test(int test_size, aisdi::Vector<int> vec,aisdi::LinkedList<int> linList)
{
    clock_t vec_begin, vec_end, list_begin, list_end;
    double vec_time, list_time;
    for (int i = 0; i < test_size; i++)
        {
            vec.append(i);
            linList.append(i);
        }

    auto vec_it = vec.begin();
    auto list_it = linList.begin();

    for (int i = 0; i < test_size/2; i++)
    {
        vec_it++;
        list_it++;
    }

    vec_begin = clock();
    vec.erase(vec_it);
    vec_end = clock();
    vec_time = (vec_end - vec_begin)/(CLOCKS_PER_SEC/1000000);
    plik<< "For "<<test_size<<" elements in vector, erase erase took: "<<vec_time<<"us"<<std::endl;

    list_begin = clock();
    linList.erase(list_it);
    list_end = clock();
    list_time = (list_end - list_begin)/(CLOCKS_PER_SEC/1000000);
    plik<< "For "<<test_size<<" elements in list, erase took: "<<list_time<<"us"<<std::endl;

}


void test_all()
{
    plik.open( "Results.txt", std::ios::out );
    if( plik.good() == false )
    {
        std::cout<<"Problem with file."<<std::endl;
        plik.close();
    }

	aisdi::Vector <int> vec;
	aisdi::LinkedList <int> linList;
	for (int i = SMALL_TEST; i <= BIG_TEST; i += 10000)
	{
        prepend_test(i,vec,linList);
	}
	plik<<'\n';
	for (int i = SMALL_TEST; i <= BIG_TEST; i += 10000)
	{
        erase_from_middle_test(i,vec,linList);
	}
	plik.close();
}
