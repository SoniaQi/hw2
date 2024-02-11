#include <sstream>
#include <iomanip>
#include <set>
#include "movie.h"
#include "util.h"
#include <cmath>

using namespace std;

Movie::Movie(const std::string category,const std::string name,double price, int qty,const std::string genre,const std::string rating) :
  Product(category, name, price, qty)
{
  genre_ = genre;
  rating_ = rating;
}

Movie::~Movie()
{

}

std::set<std::string> Movie::keywords() const
{
  std::set<std::string> kw = parseStringToWords(name_);
  std::set<std::string> genre = parseStringToWords(genre_);
  kw.insert(genre.begin(),genre.end());
  return kw;
}

std::string Movie::displayString() const
{
  std::string str = name_;
  str += "\n";
  str += "Genre: ";
  str += genre_;
  str += " Rating: ";
  str += rating_;
  str += "\n";
  double pricet = std::round(getPrice()*100)/100.0;
  std::stringstream ss;
  ss << pricet;
  std::string price_str = ss.str();
  str += price_str;
  str += " ";
  str += std::to_string(getQty());
  str += " left.";
  str += "\n";
  //str += "Enter command: ";
  return str;
}

void Movie::dump(std::ostream& os) const
{
  os << category_ << "\n" << name_ << "\n" << getPrice() << "\n" << getQty() << "\n" << genre_ << "\n" << rating_ << endl;
}



