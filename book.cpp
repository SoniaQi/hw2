#include <sstream>
#include <iomanip>
#include <set>
#include "book.h"
#include "util.h"
#include <cmath>

using namespace std;

Book::Book(const std::string category,const std::string name,double price, int qty,const std::string ISBN,const std::string author) :
  Product(category, name, price, qty)
{
  ISBN_ = ISBN;
  author_ = author;
}

Book::~Book()
{

}

std::set<std::string> Book::keywords() const
{
  std::set<std::string> kw = parseStringToWords(name_);
  std::set<std::string> author = parseStringToWords(author_);
  kw.insert(author.begin(),author.end());
  kw.insert(ISBN_);
  return kw;
}

std::string Book::displayString() const
{
  std::string str = name_;
  str += "\n";
  str += "Author: ";
  str += author_;
  str += " ISBN: ";
  str += ISBN_;
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

void Book::dump(std::ostream& os) const
{
  
  os << category_ << "\n" << name_ << "\n" << getPrice() << "\n" << getQty() << "\n" << ISBN_ << "\n" << author_ << endl;
}



