#include <iostream>
#include <vector>
#include <algorithm>    // std::sort
#include <assert.h>
#include "RNG.h"

using namespace std;

//Our Person class will define the person ID and
//birth and death year
class Person
{
public:
	Person(int id, int Birthyear, int DeathYear)
	{
		m_id = id;
		m_iBirthYear = Birthyear;
		m_iDeathyear = DeathYear;
	}
	int m_id;
	int m_iBirthYear;
	int m_iDeathyear;
};

//This vector will contain a list of all our people
//it's a global variable since we are running a single 
//threaded application and there will be no concurrency issues
vector<Person*> vec_Population; 


//utility function. Takes Array of years and returns
//the index (one index) of the year with the most people alive
int GetMostLivedYear(vector<int>& YearsArray)
{
	int YearsArrayCount = YearsArray.size();
	int iRetYearIndex = 0;
	for (int i = 1; i < YearsArrayCount; i++)
	{
		if (YearsArray[i] > YearsArray[iRetYearIndex])
		{
			iRetYearIndex = i;
		}
	}

	return iRetYearIndex + 1900;
}

//utility function. Adds a person to our population list
void AddToPopulation(Person* p)
{
	vec_Population.push_back(p);
}

//This function modifies the Years Array.
//it will increment 1 in the index of the years
//the person has lived.
//so if John for example lived from 1930 to 1980 then
//this function will go to index 30 through 80 and increment them by one
//and it will loop through all the population
void FillYearsData(vector<int>& YearsArrayToFill)
{
	int NumberOfPeople = vec_Population.size();

	for (int i = 0; i < NumberOfPeople; i++)
	{
		Person* p = vec_Population[i];

		for (int k = p->m_iBirthYear; k <= p->m_iDeathyear; k++)
		{
			int iIndex = k - 1900;
			YearsArrayToFill[iIndex] = YearsArrayToFill[iIndex]++;
		}
	}
}


int main()
{
	CRandom RandomObj;
	RandomObj.Randomize();

	//the years here are mapped to 1900 -> 2000. so index 0 is the year 1900.
	//index 1 is the year 1901 and so on.
	vector<int> YearsArray;
	YearsArray.resize(101);//to include both ends [1900, 200]


	for (int i = 0; i < 1000000; i++)
	{
		int minVal = RandomObj.Random(100 + 1) + 1900;
		int MaxVal = RandomObj.RandomRange(minVal, 2000 /*inclusive*/ );

		if(minVal < 1900 || MaxVal > 2000)
			assert(false);//Not supposed to happen

		//creates a randomly generated person and adds the object 
		//to our list
		Person* p = new Person(i, minVal, MaxVal);

		if(p == NULL)
			assert(false);//Ran out of memory

		AddToPopulation(p);
		p = NULL;
	}

	FillYearsData(YearsArray);

	cout<<"Year with most people alive is: "<<GetMostLivedYear(YearsArray)<<endl;



	//clean up our dynamic memory allocations on the heap
	for (std::vector< Person* >::iterator it = vec_Population.begin() ; it != vec_Population.end(); ++it)
	{
		delete (*it);
	} 
	vec_Population.clear();//to prevent dangling pointers


}