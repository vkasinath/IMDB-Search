HOMEWORK 9: IMDB SEARCH


NAME: Vishaal Kasinath


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

http://www.cs.rmit.edu.au/online/blackboard/chapter/05/documents/contribute/chapter/05/linear-probing.html
http://www.partow.net/programming/hashfunctions/#top


Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  20


HASH FUNCTION DESCRIPTION

I used the same hash function that was provided for Lab 10. 
I also looked up and found out this hash function is a 
bitwise hash function

HASH TABLE IMPLEMENTATION

The hash table is a vector of pair. The pair is the combination
of the query string and the address of the movie.
There are 6 query parts for each movie.
Therefore, there are 64 possible combinations that can be queried 
from the movie query parts (2^6).
All possible subests of the query parts were created.
The strings in the subset were concatenated together (space delimited)
to form the query string that was then hashed and associated with
the address at which the movie object was located.
The empty subset indicates unfiltered querying.
I used a special word "NULL" in the query string to
signify an unfiltered query.

using 3k_example_movies.txt and 3k_example_actors.txt
using movie pointers
keys	load(ms)	find(ms)	# of movies	query
192000	1266		 6		 	300			(1 Comedy)
192000	1267		 7		 	566			(1 Drama)
192000	1277		 6		  	  9			(1 Sci-Fi)
192000	1283		 6		  	 30			(1 Romance)
192000	1292		 6		   	  6			(2 Horror Mystery)
192000	1269		 6		  	 11			(3 Drama Mystery Thriller)
192000	1279		 6		  	 17			(3 Crime Drama Mystery)
192000	1285		 5		  	 26			(year 1990) 
192000	1291		 6		  	 35			(year 2000) 
192000	1299		 6		  	 78			(year 2010)
192000	1286		 8		   3000		    (all)

Used chrono library to capture run time for find and insert
As you can see in the table above the find is a constant time
of about 6ms for various queries and different output movie counts.

MISC. COMMENTS TO GRADER:  
(optional, please be concise!)




