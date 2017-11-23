//
//  akb453_hw8_q2.cpp
//  HW8
//
//  Created by Amar on 7/18/17.
//  Copyright © 2017 Amar. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include <ctime>        //needed to place orgnamisms randomly

using namespace std;

//--------------------------------------------------------------------------------------------------------
//Class: World
/*Function: Creates a <char> vector of n*n elements (default is 20*20). The char '-' is placed inside each element.*/
class World{
public:
    //    default constructor 20x20
    World();
    //    constructor
    World(int n);
    //    getter function. return the size of the vector
    int get_length() {return (int)grid.size();};
    //    getter function. return the <char> at the specified index of the vector
    char get_organism(int ind){return grid[ind];};
    //    friend function. place a specified <char> 'a' at index 'ind2' and insert '-' at 'ind1'
    friend void placeOrganism(World& x, int ind1, int ind2, char a);
    //    default friend function. print the elements of the vector in 20x20 array.
    friend void printWorld(World& x);
    //    friend function. print the elements of the vector in nxn array.
    friend void printWorld(World& x, int n);
private:
    vector<char> grid;
};

//default constructor 20x20
World::World(){
    //    initialize vector with '-'
    for(int i=0; i<20*20; i++){
        grid.push_back('-');
    }
}

//constructor
World::World(int n){
    //    initialize vector with '-'
    for(int i=0; i<n*n; i++){
        grid.push_back('-');
    }
}

//    friend function. place a specified <char> 'a' at index 'ind2' and insert '-' at 'ind1'
void placeOrganism(World& x, int ind1, int ind2, char a){
    x.grid[ind1] = '-';
    x.grid[ind2] = a;
}

//    default friend function. print the elements of the vector in 20x20 array.
void printWorld(World& x){
    for(int i=1; i<=x.get_length(); i++){
        if(i%20 != 0){
            cout << x.get_organism(i-1);
        }
        else{
            cout << x.get_organism(i-1) << endl;    //print the 21st element in a new line
        }
    }
}

//    friend function. print the elements of the vector in nxn array.
void printWorld(World& x, int n){
    for(int i=1; i<=x.get_length(); i++){
        if(i%n != 0){
            cout << x.get_organism(i-1);
        }
        else{
            cout << x.get_organism(i-1) << endl;    //print the n+1 element in a new line
        }
    }
}

//--------------------------------------------------------------------------------------------------------

//Class: Organism
/*Function: Abstract class. Intended to be used as a base class for the Ant and Doodlebug classes*/
class Organism{
public:
    //    default constructor. <char> k is 'o' for ants and 'X' for doodlebugs. used to place k at a random position
    Organism(World& x, char k);
    //    constructor. used by member functions to place <char> k at specified position in the world
    Organism(World& x, int position, char k);
    //    getter function. returns the current position of the organism
    int getPosition() {return positionCurrent;};
    //    getter function. returns how long (turns) an organism has been alive. used by member functions (breed and starve)
    long getCounter() {return counter;};
    //    setter function. updates the current position of an organism. used by member functions move & breed
    void setPosition(int ind){this->positionCurrent = ind;};
    //    setter function. updates how long an organism has been alive
    void updateCounter(){this->counter = this->counter+1;};
    //    virtual function move. this function makes the Organism class an abstract class. compiler will choose appropriate function based on the derivative class at runtime
    virtual void move(World& x)=0;
    //    member variables
    long counter;       //how long an organism has been alive
    int positionCurrent;
};

//default constructor
Organism::Organism(World& x, char k){
    int n = 399;
    counter = 0;
    int positionFuture = rand()%(n + 1);
    while(x.get_organism(positionFuture) != '-'){
        positionFuture = rand()%(n + 1);
    }
    positionCurrent = positionFuture;
    placeOrganism(x, positionCurrent, positionCurrent, k);
    
}

//constructor
Organism::Organism(World& x, int position, char k){
    counter = 0;
    positionCurrent = position;
    placeOrganism(x, positionCurrent, positionCurrent, k);
}

//-------------------------------------------------------------------------------------------------------

//Class: Ant
/*Function: defines class (species) specific move and breed rules*/
class Ant:public Organism{
public:
    //    default constructor
    Ant(World& x):Organism(x,'o'){};
    //    constructor. used by member functions to place an ant at specified position in the world
    Ant(World& x, int position):Organism(x,position,'o'){};
    //    member function. used to move an ant randomly in the world
    void move(World& x);
    //    member function
    void breed(World& x, vector<Ant> &a, int bugs);
    //    for debugging
    friend bool operator <(Ant& ant1,Ant& ant2);
private:
    int rand_num, positionFuture, positionPrevious;
};

//friend function. strictly for debugging
bool operator <(Ant& ant1,Ant& ant2){
    return ant1.getPosition()<ant2.getPosition();
}

//member function. returns nothing. uses placeOrganism function to place ants randomly in the <World> vector
void Ant::move(World& x){
    //    generate a random number between 1 and 4
    rand_num = rand()%4 + 1;
    //    glaring example of lazy programming. I used this variable to scale the world. Note to self: update it so the end user does not have to manipulate the source code
//    int n = 20;
    //    use the random number to move up,down,left,or right
    switch (rand_num) {
        case 1:
            //            get current position
            positionPrevious = getPosition();
            //            calculate tentative future position (up)
            positionFuture = positionPrevious-x.get_length();
            //            if the tentative future position is within 0 to 399 and it is empty move to this position
            if(0<=positionFuture && positionFuture<x.get_length()*x.get_length() && x.get_organism(positionFuture)=='-'){
                //                update the current position
                setPosition(positionFuture);
                //                place the ant in the the updated current position in the world vector
                placeOrganism(x, positionPrevious, positionFuture, 'o');
            }
            break;
        case 2:
            positionPrevious = getPosition();
            positionFuture = positionPrevious+x.get_length();
            if(0<=positionFuture && positionFuture<x.get_length()*x.get_length() && x.get_organism(positionFuture)=='-'){
                setPosition(positionFuture);
                placeOrganism(x, positionPrevious, positionFuture, 'o');
            }
            break;
        case 3:
            positionPrevious = getPosition();
            positionFuture = positionPrevious+1;
            if(0<=positionFuture && positionFuture<x.get_length()*x.get_length() && x.get_organism(positionFuture)=='-'){
                setPosition(positionFuture);
                placeOrganism(x, positionPrevious, positionFuture, 'o');
            }
            break;
        case 4:
            positionPrevious = getPosition();
            positionFuture = positionPrevious-1;
            if(0<=positionFuture && positionFuture<x.get_length()*x.get_length() && x.get_organism(positionFuture)=='-'){
                setPosition(positionFuture);
                placeOrganism(x, positionPrevious, positionFuture, 'o');
            }
            break;
        default:
            break;
    }
}


//The miracle of life!
//member function. returns nothing. if an ant has survived for 3 turn (in this dreary virtual world) then it has earned the privilege to spawn (create) a new ant object (baptised as antBaby (imaginative, I know!)) and adds it to the ants vector.
void Ant::breed(World& x, vector<Ant> &a, int bugs){
    //    lazy programming. don't do this
//    int n = 20;
    //    get the current position. this will be used to check for empty space around it
    int position = getPosition();
    //    ONE BABY PER 3 TURNS POLICY
    if(getCounter() != 0 && getCounter()%3 == 0){       //ARE YOU APPROVED?
        
        //        check if the position above the current position is empty ('-')
        if(0<=position-x.get_length() && position-x.get_length()<x.get_length()*x.get_length() && x.get_organism(position-x.get_length()) == '-'){
            //            create a new ant object
            Ant antBaby(x,position-x.get_length());
            //            add it to the vector
            if(a.size()<(x.get_length()*x.get_length())-bugs){
                a.push_back(antBaby);
            }
        }
        else if(0<=position+x.get_length() && position+x.get_length()<x.get_length()*x.get_length() && x.get_organism(position+x.get_length()) == '-'){
            Ant antBaby(x,position+x.get_length());
            if(a.size()<(x.get_length()*x.get_length())-bugs){
                a.push_back(antBaby);
            }
        }
        else if(0<=position+1 && position+1<x.get_length()*x.get_length() && x.get_organism(position+1) == '-'){
            Ant antBaby(x,position+1);
            if(a.size()<(x.get_length()*x.get_length())-bugs){
                a.push_back(antBaby);
            }
        }
        else if(0<=position-1 && position-1<x.get_length()*x.get_length() && x.get_organism(position-1) == '-'){
            Ant antBaby(x,position-1);
            if(a.size()<(x.get_length()*x.get_length())-bugs){
                a.push_back(antBaby);
            }
        }
    }
}

//------------------------------------------------------------------------------------------

//Class: Doodlebug
/*Function: defines class (species) specific move and breed rules. also adds starve rules*/
class DoodleBug:public Organism{
public:
    //    default constructor. uses the default organism constructor
    DoodleBug(World& x):Organism(x,'X'){};
    //    constructor. used by member functions to place an ant at specified position in the world
    DoodleBug(World& x,int position):Organism(x,position,'X'){};
    //    getter function. returns true if the bug has eaten
    bool getHungerStatus(){return hasEaten;};
    //    setter function. set the member variable has eaten to true if it has eaten
    void setHungerStatus(bool status){this->hasEaten=status;};
    //    member function. inherited from the base class. will be over loaded
    void move(World& x);
    //    over loaded member function. used to move the bug
    void move(World& x, vector<Ant> &a);
    //    member function. class specific
    void starve(World& x,vector<DoodleBug> &d, int ind, int &bugSize);
    //    member function. class specific
    void breed(World& x, vector<DoodleBug> &d, int ant);
private:
    int rand_num, positionFuture, positionPrevious;
    bool hasEaten=false;
    int findMatch(int position, vector<Ant> &a);
    void eatAnt(int ind, vector<Ant> &a);
};

//member function. if a bug has not eaten remove it from the vector and update the world vector
void DoodleBug::starve(World& x,vector<DoodleBug> &d, int ind, int &bugSize){
    if(getCounter()!=0 && getCounter()%3==0 && getHungerStatus()!= true){
        placeOrganism(x, getPosition(), getPosition(), '-');
        //        "is no more", "has ceased to be", "bereft of life, it rests in peace","this is an ex-parrot"
        d.erase(d.begin()+ind);
        bugSize=bugSize-1;
    }
    //    if it has eaten set the status to false
    else if(getCounter()!=0 && getCounter()%3==0 && getHungerStatus()== true){
        setHungerStatus(false);
    }
}

//private member function. remove an ant from the vector at the specified index
void DoodleBug::eatAnt(int ind, vector<Ant> &a){
    a.erase(a.begin()+ind);
}

//private member function. used by function eatAnt. return the index of an ant in the ant vector a whose current position matches the parameter "position. also used in the member function move
int DoodleBug::findMatch(int position, vector<Ant> &a){
    //    variable to store index
    int ind = 0;
    for(Ant i:a){
        //        get the current position of the ant and match it with the given parameter
        if(i.getPosition()==position){
            return ind;
        }
        else{
            ind++;
        }
    }
    exit(1);
    //    return 0;
}

//inherited member function. not used
void DoodleBug::move(World& x){
    //nothing. over loading the function
}

//over loaded move function
void DoodleBug::move(World& x, vector<Ant> &a){
    //    lazy. lazy. lazy
    int n = 20;
    //    get the current location
    positionPrevious = getPosition();
    //    search and destroy. class specific rules. first looks for ants in the adjacent positions.
    if(0<=positionPrevious-n && positionPrevious-n<n*n && x.get_organism(positionPrevious-n)=='o'){
        positionFuture = positionPrevious-n;
        eatAnt(findMatch(positionFuture, a), a);
        setHungerStatus(true);
        //        update the current location of the bug
        setPosition(positionFuture);
        //        update the world vector
        placeOrganism(x, positionPrevious, positionFuture, 'X');
    }
    else if(0<=positionPrevious+n && positionPrevious+n<n*n && x.get_organism(positionPrevious+n)=='o'){
        positionFuture = positionPrevious+n;
        eatAnt(findMatch(positionFuture, a), a);
        setHungerStatus(true);
        setPosition(positionFuture);
        placeOrganism(x, positionPrevious, positionFuture, 'X');
    }
    else if(0<=positionPrevious+1 && positionPrevious+1<n*n && (positionPrevious+1%n)!=0 && x.get_organism(positionPrevious+1)=='o'){
        positionFuture = positionPrevious+1;
        eatAnt(findMatch(positionFuture, a), a);
        setPosition(positionFuture);
        setHungerStatus(true);
        placeOrganism(x, positionPrevious, positionFuture, 'X');
    }
    else if(0<=positionPrevious-1 && positionPrevious-1<n*n && positionPrevious-1!=19 && (positionPrevious-1)%n!=9 && x.get_organism(positionPrevious-1)=='o'){
        positionFuture = positionPrevious-1;
        eatAnt(findMatch(positionFuture, a), a);
        setHungerStatus(true);
        setPosition(positionFuture);
        placeOrganism(x, positionPrevious, positionFuture, 'X');
    }
    else{
        //        same as Ant::move
        rand_num = rand()%4 + 1;
        switch (rand_num) {
            case 1:
                positionPrevious = getPosition();
                positionFuture = positionPrevious-n;
                if(0<=positionFuture && positionFuture<n*n && x.get_organism(positionFuture)=='-'){
                    setPosition(positionFuture);
                    placeOrganism(x, positionPrevious, positionFuture, 'X');
                }
                break;
            case 2:
                positionPrevious = getPosition();
                positionFuture = positionPrevious+n;
                if(0<=positionFuture && positionFuture<n*n && x.get_organism(positionFuture)=='-'){
                    setPosition(positionFuture);
                    placeOrganism(x, positionPrevious, positionFuture, 'X');
                }
                break;
            case 3:
                positionPrevious = getPosition();
                positionFuture = positionPrevious+1;
                if(0<=positionFuture && positionFuture<n*n && x.get_organism(positionFuture)=='-'){
                    setPosition(positionFuture);
                    placeOrganism(x, positionPrevious, positionFuture, 'X');
                }
                break;
            case 4:
                positionPrevious = getPosition();
                positionFuture = positionPrevious-1;
                if(0<=positionFuture && positionFuture<n*n && x.get_organism(positionFuture)=='-'){
                    setPosition(positionFuture);
                    placeOrganism(x, positionPrevious, positionFuture, 'X');
                }
                break;
            default:
                break;
        }
        
    }
}

//member function. same as Ant::breed. bug must survive 8 turns before breeding.
void DoodleBug::breed(World& x, vector<DoodleBug> &d, int ant){
    //    beating a dead horse
    int n = 20;
    int position = getPosition();
    //    check if the bug survived 8 turns and don't add more than the total amount of space available
    if(getCounter() != 0 && getCounter()%8 == 0){
        if(0<=position-n && position-n<n*n && x.get_organism(position-n) == '-'){
            DoodleBug bugBaby(x,position-n);
            if(d.size()<(n*n)-ant)
                d.push_back(bugBaby);
        }
        else if(0<=position+n && position+n<n*n && x.get_organism(position+n) == '-'){
            DoodleBug bugBaby(x,position+n);
            if(d.size()<(n*n)-ant)
                d.push_back(bugBaby);
        }
        else if(0<=position+1 && position+1<n*n && x.get_organism(position+1) == '-'){
            DoodleBug bugBaby(x,position+1);
            if(d.size()<(n*n)-ant)
                d.push_back(bugBaby);
        }
        else if(0<=position-1 && position-1<n*n && x.get_organism(position-1) == '-'){
            DoodleBug bugBaby(x,position-1);
            if(d.size()<(n*n)-ant)
                d.push_back(bugBaby);
        }
    }
}

int main(){
    //    initialize variables
    //    int antSize = 0;
    int bugSize = 0;
    //    needed to randomly place organisms
    srand(time(0));
    //    creating the world
    int n = 20;
    World board(n);
    //    create vectors for ants and doodlebugs
    vector<Ant> ants;
    vector<DoodleBug> bugs;
    int antNumber, bugNumber;
    cout << "Enter the numbers of ants to place: ";
    cin >> antNumber;
    cout << "Enter the numbers of doodlebugs to place: ";
    cin >> bugNumber;
    //    add some ants to the vector
    for(int i=0; i<antNumber; i++){
        Ant ant(board);
        ants.push_back(ant);
    }
    //    add some doodlebugs to the vector
    for(int i=0; i<bugNumber; i++){
        DoodleBug bug(board);
        bugs.push_back(bug);
    }
    int j=0;
    while(bugs.size()!=0 && ants.size()!=0){
        cout << "Turn: " << j << endl;
        printWorld(board, n);
        //        move doodlebugs
        for(int i=0; i<bugs.size(); i++){
            bugs[i].move(board, ants);
        }
        //        bugs breed
        for(int i=0; i<bugs.size(); i++){
            bugs[i].breed(board,bugs,(int)ants.size());
        }
        bugSize=(int)bugs.size();
        //        bugs starve to death
        for(int i=0; i<bugSize; i++){
            bugs[i].starve(board,bugs,i, bugSize);
        }
        //        move ants
        for(int i=0; i<ants.size(); i++){
            ants[i].move(board);
        }
        //        ants breed
        for(int i=0; i<ants.size(); i++){
            ants[i].breed(board,ants,(int)bugs.size());
        }
        //        update counter
        for(int i=0; i<bugs.size(); i++){
            bugs[i].updateCounter();
        }
        for(int i=0; i<ants.size(); i++){
            ants[i].updateCounter();
        }
        cin.ignore();
        j++;
        cout << "Bugs: " << bugs.size() << endl;
        cout << "Ants: " << ants.size() << endl;
    }
    
    cout << "One or more species went extinct!" << endl;
    
    return 0;
}



