#include <iostream>
#include <string>
#include <vector>
#include <locale>
#include <sstream>
#include <set>
#include <algorithm>

#define FLAGS_POS  1

using namespace std;

/*
 * Represents the attributes
 * of a command-line argument.
 */
struct Flag {
  size_t  index;            // Column index to sort. Referenced from 0.
  int     precedence;       // Precedence of the flag.
  bool    is_descending;    // Is the column sorted in descending order?
  bool    is_string_sorted; // Is the column sorted as a string?
};

/*
 * Represents a row of columns (a line).
 */
struct Line {
  vector<string>  data;     // Each string represents a column
};

bool operator<(const Flag& lhs, const Flag& rhs) {
  return lhs.precedence < rhs.precedence;
}

int               validate_args(int, char*[]);
int               bad_usage(char*[]);
int               show_help(char*[]);
vector<string>    split(const string&, const string&);
vector<string>    tokenize(const string& s, const string& delim);
set<Flag>         get_flags(char*&);
Line              parse_line(const string&);
vector<Line>      get_lines(istream&);
vector<Line>      get_valid_lines(const vector<Line>&, const set<Flag>&);
vector<Line>      get_sorted_lines(const vector<Line>&, const set<Flag>&);
Flag              get_flag_by_precedence(const set<Flag>&, int);
vector<Line>      sort_as_string_by_index(const vector<Line>&, int&); 
string            rtrim(const string&);
string&           reverse(string&);
string            trim(const string&);
void              print_lines(ostream&, vector<Line>);

/*
 *  Class used to sort Line objects
 *  based on data in a set of Flags.
 */
class SortByFlags {
public:
  SortByFlags(set<Flag> f) : flags_{f}{}
  bool operator()(const Line& lhs, const Line& rhs) const {
    auto fit = flags_.begin();

    while(1){
      if(fit == flags_.end())              // base-case
        return 0; 
      int i = fit->index;

      if(fit->is_string_sorted) {         // the column is string-sorted
        string lhstr(trim(lhs.data[i])), rhstr(trim(rhs.data[i]));
        if(lhstr == rhstr) {              // lhs & rhs strings are equal
          fit++;                          // compare by next flag
          continue;
        }
        // strings are not equal. Compare based on asc/desc
        return fit->is_descending ? lhstr > rhstr : lhstr < rhstr;
      } else {                            // the column is number-sorted
        double lval, rval;    
        istringstream lhstr(lhs.data[i]), rhstr(rhs.data[i]);
        lhstr >> lval;        
        rhstr >> rval;
        if( !(lhstr) && !(rhstr) ) {      // both lhs & rhs fail to prase
            fit++;                        // compare next flag
            continue;  
        }

        if(!lhstr)                        // lhs can't be parsed
          return 0;                       // return not-less-than
        if(!rhstr)                        // rhs can't be parsed
          return 1;                       // return less-than 


        if(lval == rval) {                // the values are equal
          fit++;                          // compare by next flag
          continue;
        }
        
        // numbers parsed successfully. Compare them based on asc/desc order
        return fit->is_descending ? lval > rval : lval < rval; 
      } 
    }
    return 1;   // not reached
  }
private:
  set<Flag>   flags_;
};

/* 
 * Main
 *
 * Parses flags from command line arguments.
 * Parses comma-separated values (columns) from lines in cin.
 * Sorts these lines/rows based on criteria in flags.
 * Prints the sorted lines to cout.
 */
int main(int argc, char* argv[]) {
  set<Flag>     flags;                      // Holds the command-line flags    
  vector<Line>  lines;                      // Holds the rows parsed from cin

  if(int er_no = validate_args(argc, argv)) // if validating args fails
    return er_no;                           // return error

  flags = get_flags(argv[FLAGS_POS]);       // Parse flags from argv
  if( flags.empty() )                       // parsing flags fails
    return bad_usage(argv);                 // return error

  // Drop lines that are too short
  lines = get_valid_lines(get_lines(cin), flags);  

  // Sort the collection of Lines based on set of Flags.
  sort(lines.begin(), lines.end(), SortByFlags(flags));

  // Print the sorted lines to cout
  print_lines(cout, lines);

  return 0; 
}

/*
 * Returns a vector of Lines containing all lines 
 * that satisfy the minimum-column count required by flags.
 */
vector<Line> get_valid_lines(const vector<Line>& lines, const set<Flag>& flags) {
  vector<Line> res;             // holds valid lines 
  size_t min_col_count = 0;     // minimum column-count

  // calculate minimum column count
  for(auto f : flags) {        
    min_col_count = f.index > min_col_count ? f.index : min_col_count;
  }

  // Add lines that meet the minimum-column-count to the result container.
  for(auto l : lines)            
    if(!(l.data.size() < (size_t)min_col_count))
      res.push_back(l);

  return res;
}

/* 
 * Returns vector where each element is a line
 * parsed from the istream.
 */
vector<Line> get_lines(istream& is) {
  vector<Line> res;
  string line;

  while(getline(is, line) > 0) { 
    res.push_back(parse_line(line));
  }

  return res;
}

/*
 * Returns a Line object, containing 
 * tokenized data from the string parameter
 */
Line parse_line(const string& l) {
  Line res;
  vector<string> tokens = tokenize(l, ",");

  for(auto t : tokens)
    res.data.push_back(t);

  return res;
}

/*
 * Returns a set<Flag> containing Flag objects parsed from argv.
 * Validates argv for incorrect flag-requests.
 * Returns empty set on failure.
 */
set<Flag> get_flags(char*& argv) {
  set<Flag> flags;                    
  int x;                              // Holds the index-integer
  int count = 0;                      // The flags precedence

  // For all the tokens in argv, separated by ","
  for(auto s : split(argv, ",")) {
    Flag f;                           // The flag to add
    f.precedence = count++;           // Set precedence

    istringstream iss(s); 
    if(!(iss >> x)) {                 // character is not an int;
      flags.clear();            
      return flags;                   // return empty set
    }  
    if(x == 0) {                      // 0 is not a valid argument
      flags.clear();
      return flags;                   // return empty set
    }
    
    // Is x negative? Set is_descending
    f.is_descending = x < 0 ? true : false;
    // Set flags index
    f.index = abs(x)-1;
    // Is the flag's index string-sorted?
    f.is_string_sorted = s.find_first_of("s") != string::npos ? true : false; 
 
    if(iss >> x) {                    // If another int can be parsed
      flags.clear();                  // in same token, flag invalid
      return flags;                   // return empty set 
    }
    if( !flags.insert(f).second ) {   // If flag-index specified more than once
      flags.clear();
      return flags;                   // return empty set
    }
  }
  return flags;                       
}

/*
 *  Validates characters in argv.
 *  Ensures characters are numeric or 's' 
 *  Does not ensure correct ordering, E.g. "s1" will pass.
 *  Return 0 on success
 */
int validate_args(int argc, char* argv[]) {
  if( argc == 1 ) {             // 1 argument is bad usage
    return bad_usage(argv);
  } else if ( argc == 2 ) {     // 2 arguments may be '-h' flag
    string arg(argv[1]);
    if(arg == "-h") 
      return show_help(argv);
  } else {                      // >2 arguments is bad usage
    return bad_usage(argv); 
  }
  // Correct number of command line arguments. "-h" flag not specified.
 
  string s = argv[FLAGS_POS];   // Parse flag-section from argv   

  if(s.find_first_not_of("1234567890s,-", 0) != string::npos)
    return bad_usage(argv);
  // all characters are valid

  return 0;
}
/*
 *  Splits string s based on delim, allowing empty tokens.
 *  Returns vector of token-strings.
 */
vector<string> split(const string& s, const string& delim) {
  vector<string> v;
  size_t start = 0, end = 0;

  while( (start = s.find_first_not_of(delim, end)) != string::npos ) {
    end = s.find_first_of(delim, end);
    if(end < start)
      v.push_back("");
    else
      v.push_back(s.substr(start,end-start));

    if( end != string::npos)
      end++;
  }

  if( s.find_first_of(delim,end-1) != string::npos)
    v.push_back("");

  return v;
}
/*
 *  Returns vector of non-empty string tokens
 *  parsed from s based on delim.
 */
vector<string> tokenize(const string& s, const string& delim) {
  vector<string> v;
  size_t start = 0, end = 0;
  while( (start = s.find_first_not_of(delim, end) ) != string::npos) {
    end = s.find_first_of(delim,start);
    v.push_back(s.substr(start, end-start));
  }
  return v;
}

/*
 * Modifies parameter string s - reverses it.
 * Returns reversed string.
 */
string& reverse(string& s) {
  string::size_type i,l;
  char t;
  
  for(i = 0, l = s.size(); i < (l/2); i++) {
    t = s[i];
    s[i] = s[(l-1)-i];
    s[(l-1)-i] = t;
  }

  return s;
}

/* 
 * Returns copy of string s where trailing whitespace removed.
 */
string rtrim(const string& s) {
  string r, tmp;
  string::size_type i,l;

  // reverse s and store in r
  for(i = 0, l = s.size(); i < l; i++) {
    r += s[(l-1)-i];
  }

  // Iterate passed leading whitespace from r
  for(i = 0, l = r.size(); i < l; i++) {
    if( !isspace(r[i]) )
      break;
  }
  r = r.substr(i);

  // Reverse tmp and store in r
  for(i = 0, l = r.size(); i < l; i++) {
    tmp += r[(l-1)-i]; 
  }

  return tmp;
}

/*
 *  Returns copy of string s, with leading and trailing whitespace removed.
 */
string trim(const string& s) {
  string res;
 
  res = rtrim(s);
  reverse(res);
  res = rtrim(res);
  reverse(res);

  return res;
}

/*
 *  Prints bad-usage message.
 *  Returns 2 to indicate improper application invocation.
 */
int bad_usage(char* argv[]) {
  cerr << "Incorrect usage.\n" << "Try: " << argv[0] << " -h" << endl;
  return 2;
}
/*
 *  Print usage instructions.
 *  Returns 1 to indicate "-h" was specified.
 */
int show_help(char* argv[]) {
  string name("csort");
  string border(name.size()+4, '*');

  cerr << border << endl;
  cerr << "* " << name << " *" << endl;
  cerr << border << endl;

  cerr << "Usage:\n" << argv[0] << " <column-index>[s],<column-index>[s],...";

  cerr << "\n\nSpecify column-indexes to sort by, separated by commas.\n";
  cerr << "Column indexes start at 1.\n";
  cerr << "As such, specifying 0 as any argument is invalid.\n\n";

  cerr << "Valid arguments:\n" << " s\t- specify String sorting for column\n";
  cerr << " 1-9\t- column-index to sort by\n\n";
  cerr << "Numbers < 0 (negative) indicate column should be sorted\n";
  cerr << "in descending order.\n\n";

  cerr << "Columns will be treated as numbers,\n";
  cerr << "unless post-fixed with an 's',\n";
  cerr << "in which case they will be treated as strings.\n\n";

  cerr << "E.g.: " << argv[0] << " " << "4,2s,1" << endl;

  return 1;
}

/*
 *  Prints a collection of Lines to os
 */
void print_lines(ostream& os, vector<Line> lines) {
  vector<string>::iterator  it;

  for(auto l : lines) {
    for(it = l.data.begin(); it != l.data.end(); ++it) { 
      if(it == l.data.end()-1)
        os << *it << endl; 
      else 
        os << *it << ",";
    }
  }
  return;  
}
