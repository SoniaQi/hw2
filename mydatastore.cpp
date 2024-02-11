#include <sstream>
#include <iomanip>
#include <set>
#include "mydatastore.h"
#include "util.h"
#include <iostream>

using namespace std;

MyDataStore::MyDataStore() {
  kp = std::map<std::string, std::set<Product*>>();
  products = std::vector<Product*>();
  users = std::vector<User*>();
  carts = std::map<std::string, std::vector<Product*>>();
  hits = std::vector<Product*>();
}

MyDataStore::~MyDataStore() {
  
  std::map<std::string, std::vector<Product*>>::iterator itm;
  for (itm=carts.begin(); itm!=carts.end();++itm) {
    std::vector<Product*> p = itm->second;
    while (!p.empty()) {
      p.pop_back();
    }
    itm->second.clear();
  }
  std::map<std::string, std::set<Product*>>::iterator its;
  for (its=kp.begin(); its!=kp.end();++its) {
    std::set<Product*> s = its->second;
    s.erase(s.begin(),s.end());
    its->second.clear();
  }
  while (!hits.empty()) {
    hits.pop_back();
  }
  for (Product* product : products) {
    delete product;
  }
  for (User* user : users) {
    delete user;
  }
  
  /*
  std::map<std::string, std::set<Product*>>::iterator its;
  for (its=kp.begin(); its!=kp.end();++its) {
    std::set<Product*> s = its->second;
    std::set<Product*>::iterator itp;
    for (itp=s.begin();itp!=s.end();++itp) {
      if (*itp != NULL) delete *itp;
    }
    its->second.clear();
  }
  */
  carts.clear(); //version before dynamic dealloc.
  kp.clear();
  products.clear();
  users.clear();
  hits.clear();
}
/*
  for (Product* product : products) {
    delete product;
  }
  for (User* user : users) {
    delete user;
  }
  std::map<std::string, std::vector<Product*>>::iterator its;
  its = std::remove(carts.begin(), carts.end(), 0);
  carts.erase(its, carts.end());
  std::map<std::string, std::set<Product*>>::iterator itm;
  itm = std::remove(kp.begin(), kp.end(), 0);
  kp.erase(its, kp.end());
  carts.clear(); //version before dynamic dealloc.
  kp.clear();
  products.clear();
  users.clear();
}
*/


void MyDataStore::addProduct(Product* p) 
{
  products.push_back(p);
  std::set<std::string> keys = p->keywords();
  for(set<std::string>::iterator it=keys.begin(); it != keys.end(); ++it) {
    std::string key = *it;
    key = convToLower(key);
    if(kp.find(key) != kp.end()){
      kp[key].insert(p);
    }
    else {
      std::set<Product*> pvec;
      pvec.insert(p);
      kp.insert(std::make_pair(key,pvec));
    }
  }
}

void MyDataStore::addUser(User* u) {
  users.push_back(u);
}


std::vector<Product*> MyDataStore::searchdbug(std::vector<std::string>& terms, int type,std::ostream& os) 
{
  os << "keys" << endl;
  std::vector<Product*>::iterator itp;
  for(itp=products.begin(); itp!=products.end();++itp) {
     std::set<std::string> keys = (*itp)->keywords();
     for(set<std::string>::iterator it=keys.begin(); it != keys.end(); ++it) {
      os << *it << endl;
    }
  }
  os << "mapping" << endl;
  std::map<std::string, std::set<Product*>>::iterator itm;
  for (itm=kp.begin(); itm!=kp.end();++itm) {
    std::set<Product*>::iterator itpr;
    std::set<Product*> mp = itm->second;
    os << itm->first << endl;
    for(itpr=mp.begin(); itpr!=mp.end();++itpr) {
      os << (*itpr)->getName() << endl;
    }
  }
  std::vector<Product*> result;
  if (type==0){  
    os << "and" <<endl;
    std::set<Product*> inter;
    std::vector<std::string>::iterator it;
    for (it = terms.begin(); it != terms.end(); it++) {
      os << *it << endl;
      if (it==terms.begin()) {
        std::set<Product*> temp = kp[*it];
        std::set<Product*>::iterator itp;
        for (itp = temp.begin(); itp != temp.end(); itp++) {
          os << (*itp)->getName() << endl;
        }
        inter = kp[*it];
        os << "current inter:" << endl;
        for (itp = inter.begin(); itp != inter.end(); itp++) {
          os << (*itp)->getName() << endl;
        }
      }
      std::set<Product*> temp = kp[*it];
      std::set<Product*>::iterator itp;
      for (itp = temp.begin(); itp != temp.end(); itp++) {
        os << (*itp)->getName() << endl;
      }
      inter = setIntersection(inter,kp[*it]);
      os << "current inter:" << endl;
      for (itp = inter.begin(); itp != inter.end(); itp++) {
        os << (*itp)->getName() << endl;
      }
    }
    std::copy(inter.begin(), inter.end(), std::back_inserter(result));
  }
  else {  
    os << "or" <<endl;
    std::set<Product*> unionSet;
    std::vector<std::string>::iterator it;
    for (it = terms.begin(); it != terms.end(); it++) {
      os << *it << endl;
      if (kp.find(*it)==kp.end()) {
          os << "error" << endl;
        }
      std::set<Product*> temp = kp[*it];
      std::set<Product*>::iterator itp;
      for (itp = temp.begin(); itp != temp.end(); itp++) {
        os << (*itp)->getName() << endl;
      }
      unionSet = setUnion(unionSet,kp[*it]);
      os << "current union:" << endl;
      for (itp = unionSet.begin(); itp != unionSet.end(); itp++) {
        os << (*itp)->getName() << endl;
      }
    }
    std::copy(unionSet.begin(), unionSet.end(), std::back_inserter(result));
    os << "current union:" << endl;
    for (Product* p : result) {
        os << p->getName() << endl;
    }
  }
  hits = result;
  return result;
}

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type) 
{
  std::vector<Product*> result;
  if (type==0){  
    std::set<Product*> inter;
    std::vector<std::string>::iterator it;
    for (it = terms.begin(); it != terms.end(); it++) {
      if (it==terms.begin()) {
        inter = kp[*it];
      }
      inter = setIntersection(inter,kp[*it]);
    }
    std::copy(inter.begin(), inter.end(), std::back_inserter(result));
  }
  else {  
    std::set<Product*> unionSet;
    std::vector<std::string>::iterator it;
    for (it = terms.begin(); it != terms.end(); it++) {
      unionSet = setUnion(unionSet,kp[*it]);
    }
    std::copy(unionSet.begin(), unionSet.end(), std::back_inserter(result));
  }
  hits = result;
  return result;
}

void MyDataStore::dump(std::ostream& ofile) 
{
  ofile << "<products>" << endl;
  std::vector<Product*>::iterator itp;
  for(itp=products.begin(); itp!=products.end();++itp) {
    (*itp)->dump(ofile);
  }
  ofile << "</products>" << endl;
  ofile << "<users>" << endl;
  std::vector<User*>::iterator itu;
  for (itu = users.begin(); itu != users.end(); ++itu) {
    (*itu)->dump(ofile);
  }
  ofile << "</users>" << endl;
}

int MyDataStore::add(std::string uname, int hit) {
  bool findU=false;
  for(std::vector<User*>::iterator it=users.begin();it!=users.end();++it) {
    if ((*it)->getName()==uname) {
      findU=true;
      break;
    }
  }
  if (findU==false) {
    return -1;
  }
  if (carts.find(uname) != carts.end()) {
    carts[uname].push_back(hits[hit-1]);
  }
  else {
    std::vector<Product*> user_cart;
    user_cart.push_back(hits[hit-1]);
    carts.insert(std::make_pair(uname, user_cart));
  }
  return 0;
}

int MyDataStore::view(std::string uname, std::ostream& os) {
  bool findU=false;
  for(std::vector<User*>::iterator it=users.begin();it!=users.end();++it) {
    if ((*it)->getName()==uname) {
      findU=true;
      break;
    }
  }
  if (findU==false) {
    return -1;
  }
  std::vector<Product*> pvec = carts[uname];
  for(unsigned int i=0; i<pvec.size(); i++) {
    //(*it)->dump(os);
    os << "Item " << i+1 << "\n";
    os << (pvec[i])->displayString();
  }
  return 0;
}

int MyDataStore::buy(std::string uname) {
  bool findU = false;
  User* u;
  for(std::vector<User*>::iterator it=users.begin();it!=users.end();++it) {
    if ((*it)->getName()==uname) {
      u = *it;
      findU = true;
      break;
    }
  }
  if (findU==false) {
    return -1;
  }
  std::vector<Product*> pvec = carts[uname];
  std::vector<Product*> remP;
  for (unsigned int i=0; i<pvec.size();i++) {
    if (pvec[i]->getQty() >=1 && u->getBalance() >= pvec[i]->getPrice()) {
      pvec[i]->subtractQty(1);
      u->deductAmount(pvec[i]->getPrice());
    }
    else {
      remP.push_back(pvec[i]);
    }
  }
  carts[uname] = remP;
  return 0;
}
