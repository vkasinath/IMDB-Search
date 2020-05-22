#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

//hash table parameters defined externally in main.cpp
extern unsigned int DEF_TAB_SIZE_N;
extern double DEF_OCC;

#include "CMovies.h"

// =================================================================
// CMOVIE IMPLEMENTATION
  
    // Constructor 

CMovie::CMovie() : title{""}, year_release{""}, 
                  runtime_m{""}, 
                  genre_count{""}, genre{{}}, 
                  actor_count{""}, actor{{}},
                  role_count{""},  role{{}}
                  {}

CMovie::CMovie(const std::string &t, 
              const std::string &yr, 
              const std::string rtm, 
              const std::string gc, 
              const std::vector<std::string> &g,
              const std::string ac, 
              const std::vector<std::string> &a,
              const std::string rc, 
              const std::vector<std::string> &r)
  {
    title = t;
    year_release = yr;
    runtime_m = rtm;
    genre_count = gc;
    genre = g;
    actor_count = ac;
    actor = a;
    role_count = rc;
    role = r;
  }

// utility function to print out movie info
void CMovie::print(const std::map<std::string, std::string> &actors){
  std::cout << title << std::endl;
  std::cout << year_release << std::endl;
  std::cout << runtime_m << std::endl;
  std::cout << genre_count;
  for ( int i = 0; i < std::stoi(genre_count); i++)
    std::cout << " " << genre[i];
  std::cout << std::endl;
  std::cout << actor_count;
  for ( int i = 0; i < std::stoi(actor_count); i++){

    //attempt to find the actor name from the input map that matches the id
    //if we dont find a match, use the actors id as the name
    std::string actor_name;
    if(actors.count(actor[i]) > 0)
      actor_name = actors.find(actor[i])->second;
    else 
      actor_name = actor[i];
    std::cout << " " << actor_name << " (" << role[i] << ")";
  }
  std::cout << std::endl;
}


// =================================================================
// HASH_TABLE IMPLEMENTATION


// destructor 
void hash_table::destroy() {
  // use set to collect all distinct movie pointers
  // then delete those pointers and erase our hash table
  std::set<CMovie *> s;
  for (unsigned int i=0; i< m_table.size(); i++)
      if (m_table[i].second != NULL)
          s.insert({m_table[i].second});
  // delete these distinct movie pointers
  for (const CMovie* p:s)
      delete p;
  // final clean up: erase the vector (m_table), and set size to 0
  m_size = 0;
  m_table.clear();
}

//function to insert entry into hash table
bool hash_table::insert(const pair_q_m &movie_obj){

  if (this->size() >= m_table.size()*DEF_OCC)
    resize_table(2*m_table.size()+1);

  unsigned int hash_val = HashFunc(movie_obj.first);           // get hash of movie obejct query string
  unsigned int index = hash_val % m_table.size();            // find vector index for hash_val

  unsigned int end_marker = m_table.size();
  //find the first empty slot in m_table, staring with position = index
  //if unable to find, continue linear probing for next slot between index and marker (m_table_size())
  while (index < end_marker){
    if (m_table[index].second == NULL)
      break;
    else
      index++;
  }
  if (index < end_marker && m_table[index].second == NULL){  // we found an empty slot on or after index
    m_table[index] = movie_obj;
    m_size++;
    return true;
  }
 
  // if here: we traversed from index to end, and from 0 to index, but still unable to find a slot
  // one last ditch attempt: resize the table ONCE, and then try to insert
  // if the insert is successful... well and good
  // else, unable to find an empty slot, something else is going on (a bug? or maybe new HashFunc needed?)
  // write an error and bail.
  resize_table(2*m_table.size()+1);
  bool l_last_att_ins = insert(movie_obj);
  if (l_last_att_ins)
    return true;
  else{
    std::cerr << "write to hash table failed unable to get index position" << std::endl;
    return false;
  }

}

std::vector<CMovie> hash_table::find(const std::string &key){

  unsigned int hash_val = HashFunc(key);                      // get hash of movie obejct query string
  unsigned int index = hash_val % m_table.size();           // find vector index for hash_val

  std::vector<CMovie> ret;                                  // result set vector

  //no results found return empty set
  if (m_table[index].second == NULL){
    return ret;
  }

  unsigned int end_marker = m_table.size();
  //find the first empty slot in m_table, staring with position = index
  //if unable to find, continue linear probing for next slot between index and marker (m_table_size())
  while (index < end_marker){
    if (m_table[index].second != NULL && m_table[index].first == key)
      ret.push_back(*m_table[index].second);
    index++;
  }
  return ret;

  }

unsigned int hash_table::HashFunc(const std::string &key){
   //  This implementation comes from 
  //  http://www.partow.net/programming/hashfunctions/
  // using the same hash funtion provided in the lab

  //  This is a general-purpose, very good hash function for strings.
  unsigned int hash = 1315423911;
  for(unsigned int i = 0; i < key.length(); i++)
    hash ^= ((hash << 5) + key[i] + (hash >> 2));
  return hash;
}

//private utility function to resize the table
void hash_table::resize_table(const unsigned int &new_size){

  // creat an empty hash table of new_size
  std::vector<pair_q_m> new_table(new_size);

  for (unsigned int i=0; i<m_table.size(); i++){
    if (m_table[i].second != NULL){

      unsigned int hash_val = HashFunc(m_table[i].first);           // get hash of movie obejct query string
      unsigned int index = hash_val % new_size;            // find vector index for hash_val

      unsigned int end_marker = new_size;
      //find the first empty slot in m_table, staring with position = index
      //if unable to find, continue linear probing for next slot between index and marker (m_table_size())
      while (index < end_marker){
        if (new_table[index].second == NULL)
          break;
        else
          index++;
      }
      if (index < end_marker)  // we found an empty slot on or after index
        new_table[index] = m_table[i];

    }
  }
  m_table = new_table;
}

/* End of hash table class */
