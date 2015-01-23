all: 
g++ -c BTree.cpp BTreeNode.cpp Pageallocator.cpp -std=c++11 -shared -fPIC -o libmydb.so