#include<iostream>
#include "server.hpp"

using namespace std;
int main(){

    cout<<"Hello world\n";
    cout<<"welcome\n";

    Server server(8085);

    server.start();

    return 0;
}   