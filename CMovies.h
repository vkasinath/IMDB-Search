#ifndef cmovie_h_
#define cmovie_h_


#include <iostream>
#include <string>
#include <vector>
#include <map>

class CMovie {
  public:
    // =================================================================
    // CMOVIE IMPLEMENTATION
  
    // Constructor 
    CMovie();

    CMovie(const std::string &t, 
           const std::string &yr, 
           const std::string rtm, 
           const std::string gc, 
           const std::vector<std::string> &g,
           const std::string ac, 
           const std::vector<std::string> &a,
           const std::string rc, 
           const std::vector<std::string> &r);

  // utility function to print out movie info
  void print(const std::map<std::string, std::string> &actors);
   
  private:
    // =================================================================
    // CMOVIE  REPRESENTATION
    std::string title;                          // movie title
    std::string year_release;                  // year of release
    std::string runtime_m;                     // run time in minutes
    std::string genre_count;                   // genre count
    std::vector<std::string> genre;             // list of genres
    std::string actor_count;                   // actor count
    std::vector<std::string> actor;             // list of actors
    std::string role_count;                    // role count
    std::vector<std::string> role;              // list of roles

};




class hash_table {

  private:
    typedef std::pair<std::string, CMovie *> pair_q_m;

  public:
    // ========================================================================
    // HASH SET IMPLEMENTATION
    // Constructor for the table accepts the size of the table.  Default
    // constructor for the hash function object is implicitly used.
    hash_table(unsigned int init_size = DEF_TAB_SIZE_N) : m_table(init_size), m_size(0) {}

    // destructor 
    ~hash_table(){destroy();}

    //function to clean up hash_table pointers and objects
    void destroy();

   // utility function to return #of distinct keys in hash_table
    unsigned int size() const { return m_size; }

    bool insert(const pair_q_m &movie_obj);

    std::vector<CMovie> find(const std::string &key);


  private:
    // hash table REPRESENTATION
    std::vector<pair_q_m>       m_table;                // actual table
    unsigned int                m_size;                 // number of keys

    unsigned int HashFunc(const std::string &key);

    void resize_table(const unsigned int &new_size);
};
/* End of hash table class */


#endif