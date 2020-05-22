#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>
#include <algorithm>
#include <vector>

//initiate default hash table parameters
unsigned int DEF_TAB_SIZE_N = 100;
double DEF_OCC = 0.5;

#include "CMovies.h"


//Function forward declarations
bool loadMovies(hash_table &ht, std::ifstream& loadfile);
void parse_line(const std::string &str, std::string &count, std::vector<std::string> &vals);
std::vector<std::vector <std::string> > build_query_combo(const std::vector<std::string> &query_parts);
void loadActors(std::map<std::string, std::string> &actors, std::ifstream& loadfile);
std::string getQueryString(std::istream& commands);

// DO NOT CHANGE THIS FUNCTION
int main(int argc, char** argv){

  //create hash table
  hash_table ht;

  //map to store actor id and names
  std::map<std::string, std::string> actors;


  std::string token; //Read the next command
  while(std::cin >> token){
    if(token == "movies"){
      std::string filename;
      std::cin >> filename;

      std::ifstream loadfile(filename.c_str());
      if(!loadfile){
        std::cerr << "Problem opening " << filename 
        << " for reading movies file." << std::endl;
        return -1;
      }

      hash_table temp;
      ht = temp;
      bool l_load_m = loadMovies(ht, loadfile);
      if (!l_load_m){
        std::cerr << "Problem loading movie data into hash table" << std::endl;
        ht.destroy();
        return -1;
      }

    }

    else if(token == "actors"){

      std::string filename;
      std::cin >> filename;

      std::ifstream loadfile(filename.c_str());
      if(!loadfile){
        std::cerr << "Problem opening " << filename 
        << " for reading actors file." << std::endl;
        return -1;
      }
      else{
        actors.clear();
        loadActors(actors, loadfile);
      }

    }

    else if (token == "query"){
      //read next 6 lines, get query parts, build query string
      std::string qs = getQueryString(std::cin);

      std::vector<CMovie> result = ht.find(qs);

      if(result.size()==0)
        std::cout << "No results for query." << std::endl;
      else{
        std::cout << "Printing " << result.size() << " result(s):" << std::endl;
        for(unsigned int i=0; i < result.size(); i++)
          result[i].print(actors);
      }
      

    }

    else if (token == "table_size"){

      std::string ts;
      std::cin >> ts;

      unsigned int ts_int = std::stoi(ts);
      if (ts_int > 0){
        DEF_TAB_SIZE_N = ts_int;
        hash_table temp;
        ht = temp;
      }
    }

    else if (token == "occupancy"){

      std::string occ;
      std::cin >> occ;

      double occ_d = std::stod(occ);
      if (occ_d > 0 && occ_d < 1)
        DEF_OCC= occ_d;
    }

    else if (token == "quit")
      return 0;

  }
   
  return 0;
}


bool loadMovies(hash_table &ht, std::ifstream& loadfile){

  // movie data is going to in groups of 6 lines
  // line #1: title
  // line #2: year of release
  // line #3: run time in minutes
  // line #4: genre_count, followed by a list of genres
  // line #5: actor_count, followed by a list of actors
  // line #6: role_count, collowed by a list of actors
  // so we have to read lines in groups of 6 (instead of inline parsing with "">> var")
  std::string title;                          // movie title
  std::string year_release;                  // year of release
  std::string runtime_m;                     // run time in minutes
  std::string genre_count;                   // genre count
  std::vector<std::string> genre;             // list of genres
  std::string actor_count;                   // actor count
  std::vector<std::string> actor;             // list of actors
  std::string role_count;                    // role count
  std::vector<std::string> role;              // list of roles
  std::vector<std::string> query_parts;       // local vector to store query parts for a movie
                                              // we use this to build our query combinations
  std::string str;
  int linenum = 0;
  while (std::getline(loadfile, str)) {
    query_parts.push_back(str);                  // store individual lines in query_parts
                                                 // we need this for building our 64+ query strings
    linenum = linenum % 6;                       // cycle very 6 lines, using the  % operator for rollover
    if (linenum == 0) title = str;               // line #1 = title
    else if (linenum == 1) year_release = str;   // line #2 = year of release
    else if (linenum == 2) runtime_m = str;      // line #3 = run time in minutes
    else if (linenum == 3)                       // genre processing: parse 4th line
      parse_line(str, genre_count, genre);
    else if (linenum == 4)                       // actors processing: parse 5th line
      parse_line(str, actor_count, actor);
    else if (linenum == 5)                       // roles processing: parse 6th line
      parse_line(str, role_count, role);

    if (linenum == 5){                           // at last line. build query string combinations for the movie
      std::vector<std::vector<std::string> > query_combo = build_query_combo(query_parts);
      query_parts.clear();
      for (unsigned int i=0; i<query_combo.size(); i++){
        std::string qstr = "";
        for(unsigned int j = 0; j<query_combo[i].size(); j++){
          if(qstr.length() == 0)
            qstr = query_combo[i][j];
          else
            qstr += " " + query_combo[i][j];
        }
        //create new movie object and store the movie object address in m
        CMovie* m = new CMovie(title, year_release, runtime_m, genre_count, genre, actor_count, actor, role_count, role);

        //create a pair of query strings and the address of the movie
        std::pair<std::string, CMovie*> p;
        p.first = qstr;
        p.second = m;

        //push the pair into the hash table and check for successful load
        bool l_load_success = ht.insert(p);
        if (!l_load_success)
          return false;
      }
    }
    linenum++;

  }
  return true;
}

void parse_line(const std::string &str, std::string &count, std::vector<std::string> &vals){
  vals.clear();
  std::stringstream ss(str);              // set our str to be a stringstream
  std::string gstr;                       // local string variable for parsing
  bool l_first = true;                    // set first value flag to true
  while(ss >> gstr){                      // parse ss into gstr
    if (l_first){                         // this is the first parsed value = count
      l_first = false;                    // set flag to false (2,3rd etc. will be genres)
      count = gstr;                       // get count (of genre or actor or role)
    }
    else
      vals.push_back(gstr);               // 2nd, 3rd etc... create vals vector
  }
}

// function to create all possible combinations [all possible subsets]
// basically select n out of m, for n=1, n=2, n=3... n=m. In our case, m=6
// so we would have (2.power.6) combinations = 64 (including a null set)
// for the null set we will use an aritifical string "NULL"
// It will be the record retrieved if there are no filters
std::vector<std::vector <std::string> > build_query_combo(const std::vector<std::string> &query_parts){
  std::vector< std::vector<std::string> > sset;
  std::vector<std::string> empty;
  sset.push_back( empty );

    // we create all possible combinations
    // for example if our words are [ONE, TWO, SIX]
    // sset will have [], [ONE], [TWO], [SIX], [ONE,TWO], [ONE,SIX], [TWO,SIX], [ONE,TWO,SIX]
    // this is basically combination, without repetition (select m out of n)
    // for our purposes the empty set indicates "no filters"
    for (unsigned int i = 0; i < query_parts.size(); i++){
      std::vector< std::vector<std::string> > ssTemp = sset; 
      for (unsigned int j = 0; j < ssTemp.size(); j++){
        ssTemp[j].push_back( query_parts[i] );
         sset.push_back( ssTemp[j] );
      }
    }
    // the empty subset is artifically replaced with "NULL" - to assist with hashing & storage
    unsigned int i = 0;
    while (i < sset.size()){
      if (sset[i].size() == 0)
        sset[i].push_back({"NULL"});
      else
        i++;
    }

    return sset;
}

void loadActors(std::map<std::string, std::string> &actors, std::ifstream& loadfile){

  std::string k, v;

  while (loadfile >> k >> v)
    actors.insert({k, v});

}

std::string getQueryString(std::istream& commands){

  // query data is going to in a group of 6 lines
  // line #1: title
  // line #2: year of release
  // line #3: run time in minutes
  // line #4: genre_count, followed by a list of genres
  // line #5: actor_count, followed by a list of actors
  // line #6: role_count, collowed by a list of actors
  // so we have to read this group of 6 and build query string
  std::string title;                          // movie title
  std::string year_release;                  // year of release
  std::string runtime_m;                     // run time in minutes
  std::string genre_count;                   // genre count
  std::vector<std::string> genre;             // list of genres
  std::string actor_count;                   // actor count
  std::vector<std::string> actor;             // list of actors
  std::string role_count;                    // role count
  std::vector<std::string> role;              // list of roles
  std::vector<std::string> query_parts;       // local vector to store query parts for a movie
                                              // we use this to build our query combinations
  std::string str;
  std::string qstr = "";

  int linenum = 0;

  std::getline(commands, str);

  while (std::getline(commands, str)) {
    query_parts.push_back(str);                  // store individual lines in query_parts

    linenum++;
    if (linenum > 5)
      break;
  }

  //build the query string using the individual query parts seperated by a space
  //if the query part is a wildcard (0 or ?) do not build it into the query string
  for (unsigned int i=0; i<query_parts.size(); i++){
    if (query_parts[i] == "0" || query_parts[i] == "?")
      continue;

    if(qstr.length() == 0)
      qstr = query_parts[i];
    else
      qstr += " " + query_parts[i];
  }
  
  //if there are no query parts provided, select all the movies in the hash table 
  //in our hash table, this will be identified with a query string of NULL
  if(qstr.length() == 0)
    qstr = "NULL";

  return qstr;

}
