#ifndef MYDATASTORE_H
#define MYDATASTORE_H
#include <string>
#include <set>
#include <map>
#include <vector>
#include "datastore.h"

class MyDataStore : public DataStore {

public:
    MyDataStore();
    ~MyDataStore();

    /**
     * Adds a product to the data store
     */
    void addProduct(Product* p);

    /**
     * Adds a user to the data store
     */
    void addUser(User* u);

    /**
     * Performs a search of products whose keywords match the given "terms"
     *  type 0 = AND search (intersection of results for each term) while
     *  type 1 = OR search (union of results for each term)
     */
    std::vector<Product*> searchdbug(std::vector<std::string>& terms, int type, std::ostream& os);
    std::vector<Product*> search(std::vector<std::string>& terms, int type);

    /**
     * Reproduce the database file from the current Products and User values
     */
    void dump(std::ostream& ofile);

    int add(std::string uname, int hit);

    int view(std::string uname, std::ostream& os);

    int buy(std::string uname);

private:
    std::map<std::string, std::set<Product*>> kp;
    std::vector<Product*> products;
    std::vector<User*> users;
    std::map<std::string, std::vector<Product*>> carts;
    std::vector<Product*> hits;

};

#endif
