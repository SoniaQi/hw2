#include <sstream>
#include <iomanip>
#include <set>
#include "clothing.h"
#include "util.h"
#include <cmath>

using namespace std;

Clothing::Clothing(const std::string category,const std::string name,double price, int qty,const std::string size,const std::string brand) :
  Product(category, name, price, qty)
{
  size_ = size;
  brand_ = brand;
}

Clothing::~Clothing()
{

}

std::set<std::string> Clothing::keywords() const
{
  std::set<std::string> kw = parseStringToWords(name_);
  std::set<std::string> brand = parseStringToWords(brand_);
  kw.insert(brand.begin(),brand.end());
  return kw;
}

std::string Clothing::displayString() const
{
  std::string str = name_;
  str += "\n";
  str += "Size: ";
  str += size_;
  str += " Brand: ";
  str += brand_;
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

void Clothing::dump(std::ostream& os) const
{
  os << category_ << "\n" << name_ << "\n" << getPrice() << "\n" << getQty() << "\n" << size_ << "\n" << brand_ << endl;
}



