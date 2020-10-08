#include <iostream>
using namespace std;
#include <vector>
#include "./Map.h"
#include <stack>

//Overload insertion stream operator
ostream& operator<<(ostream& output, Continent& obj){
    output << obj.getContinentName() <<endl;
    return output;
}
ostream& operator<<(ostream& output, Node& obj){
    output <<"The Node contains the following territory: "<<obj.getData().getTerritoryName() 
            <<" and has "<<obj.getE().size()<<" edges."<<endl;
    return output;
}
ostream& operator<<(ostream& output, Territory& obj){
    output << obj.getTerritoryName() <<" which belongs to" <<obj.getContinent()->getContinentName() <<endl;
    return output;
}
ostream& operator<<(ostream& output, Graph& obj){
    output <<"The graph has "<< obj.getV().size()<< " vertices and contains "
            <<obj.getListOfContinents().size()<<" continents."<<endl;
    return output;
}

//--------------------------------Continent class----------------------------//
//-------------- Constructors --------------//
Continent::Continent(string continentName){
    this->continentName = continentName;
}

Continent::Continent(const Continent& original){ //copy constructor
    continentName = original.continentName;
}

//-------------- Overloads --------------//
//overload assignment operator
Continent& Continent::operator=(const Continent& rhs){
    if(this != &rhs){
        continentName = rhs.continentName;
    }
    return *this;
}

//-------------- Getters --------------//
string Continent::getContinentName(){
    return continentName;
}


//-------------- Setters --------------//
void Continent::setContinentName(string newContinentName){
    continentName = newContinentName;
}


//--------------------------------Territory class----------------------------//
//-------------- Constructors --------------//
Territory::Territory(){}

Territory::Territory(string territoryName, Continent* continent, int ownerId, int numberOfArmies){
    this->territoryName = territoryName;
    this->ownerId = ownerId;
    this->numberOfArmies = numberOfArmies;
    this->continent = continent;
}

Territory::Territory(string territoryName, Continent* continent){
    this->territoryName = territoryName;
    this->continent = continent;
}

Territory::Territory(const Territory& original){//copy constructor
    territoryName = original.territoryName;
    ownerId = original.ownerId;
    numberOfArmies = original.numberOfArmies;
    continent = original.continent;//we want shallow copy
} 

//-------------- Overloads --------------//
//overload assignment operator
Territory& Territory::operator=(const Territory& rhs){
        if(this != &rhs){
            territoryName = rhs.territoryName;
            ownerId = rhs.ownerId;
            numberOfArmies = rhs.numberOfArmies;
            continent = rhs.continent; //shallow is wanted
    }
    return *this;
}

//-------------- Getters --------------//
string Territory::getTerritoryName(){
    return territoryName;
}

int Territory::getOwnerId(){
    return ownerId;
}

int Territory::getNumberOfArmies(){
    return numberOfArmies;
}

Continent* Territory::getContinent(){
    return continent;
}

//-------------- Setters --------------//
void Territory::setTerritoryName(string newTerritoryName){
    territoryName = newTerritoryName;
}
void Territory::setOwnerId(int newOwnerId){
    ownerId = newOwnerId;
}
    
void Territory::setNumberOfArmies(int newNumberOfArmies){
    numberOfArmies = newNumberOfArmies;
}

void Territory::setContinent(Continent* newContinent){
    this->continent = newContinent;
}

//-------------------------- Node class  --------------------------//
Node::Node(){}

Node::Node(Territory data){
    Territory* temp = new Territory(data.getTerritoryName(), data.getContinent());
    this->data = temp;
}

Node::Node(const Node& original){ //Copy construcotr
    data = new Territory(*original.data);
    E = original.E;
}

//-------------- Overloads --------------//
//overload assignment operator
Node& Node::operator=(const Node& rhs){
    if(this != &rhs){
        *data = *rhs.data;
        E = rhs.E;
    }

    return *this;
}

//-------------- Getters --------------//
Territory Node::getData(){
    return *data;
}

Territory* Node::getDataPtr(){
    return data;
}

vector<string> Node::getE(){
    return E;
}

//-------------- Setters --------------//
void Node::setData(Territory* data){
    this->data = data;
}

//-------------- Other functions --------------//
void Node::addEdge(string edge){
    E.push_back(edge);
}


//-------------------------- Graph class --------------------------//
//-------------- Constructor --------------//
Graph::Graph(){}

Graph::Graph(const Graph& original){//Copy constructor
    //create a deep copy of V
    for(auto i=0;i<original.getV().size();i++){
        V.push_back(new Node(*original.V[i]));
    }
    
    //create a deep copy of listOfContinents
    for(int i=0;i<original.getListOfContinents().size();i++){
        listOfContinents.push_back(new Continent(*original.listOfContinents[i]));
    }
}

//-------------- Overloads --------------//
//overload assignment operator
Graph& Graph::operator=(const Graph& rhs){
    if(this != &rhs){
        //create a deep copy of V
        for(auto i=0;i<rhs.getV().size();i++){
            V.push_back(new Node(*rhs.V[i]));
        }
        
        //create a deep copy of listOfContinents
        for(int i=0;i<rhs.getListOfContinents().size();i++){
            listOfContinents.push_back(new Continent(*rhs.listOfContinents[i]));
        }
    }
    return *this;
}

//-------------- Getters --------------//
vector<Node*> Graph::getV() const{
    return this->V;
}

vector<Continent*> Graph::getListOfContinents() const {
    return this->listOfContinents;
}

//-------------- Inserting and connecting territories --------------//
void Graph::insertATerritory(Territory data){
    Node* newTerritory = new Node(data);
    V.push_back(newTerritory);
}

void Graph::insertAndConnectTwoTerritories(Territory dataA, Territory dataB){
    //1- create two territories without connections
    insertATerritory(dataA);
    insertATerritory(dataB);
    //2- make the connection between the two territories
    connectTwoNodes(getV().end()[-1],getV().end()[-2]);
}

void Graph::connectTwoNodes(Node* A, Node* B){
    string edgeName = A->getData().getTerritoryName() + B->getData().getTerritoryName(); //AB
    //cout<<edgeName<<endl;
    A->addEdge(edgeName);
    B->addEdge(edgeName);
}

//-------------- Others --------------//
bool Graph::areConnected(Node* A, Node* B){
    string possibleEdge1 = A->getData().getTerritoryName() + B->getData().getTerritoryName(); //AB
    string possibleEdge2 = B->getData().getTerritoryName() + A->getData().getTerritoryName(); //BA
    
    for(string edge : A->getE()){//loop through all of the edges of A
        if(edge == possibleEdge1 || edge == possibleEdge2)
            return true;
         }

    return false;
}

void Graph::validate(){
    bool isErrorThrown = false;

    //check that the map is a connected graph
    try{
        if(!DFS(getV(),getV()[0]))
            throw std::logic_error("Unconnected Graph");

        // for(Node* node : getV()){
        //     if(node->getE().size() == 0){
        //         throw std::logic_error("Unconnected Graph: "+ node->getData().getTerritoryName() + " has no edges.");
        //     }
        // }
    }catch(const std::exception& e){
        isErrorThrown = true;
        std::cerr<<e.what()<<endl;
    }


    //check if all continents are connected sub-graphs
    try{
        for(Continent* continent : getListOfContinents()){
            vector<Node*> tempListOfTerritoriesInContinent;
            //getting all territories with the same continent in a single vector
            for(Node* node : getV()){
                if(node->getData().getContinent() == continent)
                    tempListOfTerritoriesInContinent.push_back(node);
            }
            
            if(!DFS(tempListOfTerritoriesInContinent,tempListOfTerritoriesInContinent[0]))
                throw std::logic_error("Unconnected Sub-Graph: "+ continent->getContinentName() + " continent is not connected.");

            //cout<<"tempListOfTerritoriesInContinent size: "<<tempListOfTerritoriesInContinent.size()<<endl;
            
            // for(Node* nodeInTemp : tempListOfTerritoriesInContinent){
            //     bool isConnected = false; //connected continent sub-graph
            //     for(Node* nodeToCompareInTemp : tempListOfTerritoriesInContinent){
            //         if(areConnected(nodeInTemp, nodeToCompareInTemp)){
            //             isConnected = true;
            //             break;
            //         }
            //     }

            //     if(!isConnected)
            //         throw std::logic_error("Unconnected Sub-Graph: "+ nodeInTemp->getData().getTerritoryName() + " is not connected with any node in the same continent.");
            // }
            tempListOfTerritoriesInContinent.clear();//earse the vector contentss
        }

    }catch(const std::exception& e){
        isErrorThrown = true;
        std::cerr<<e.what()<<endl;
    }

    //check if each country belongs to one and only one continent
    try{
        for(Node* node : getV()){
            int countNumOfMatches = 0; //count how many continents associate with each territory
            for(Continent* Continent : getListOfContinents()){
                if(node->getData().getContinent() == Continent)
                    countNumOfMatches++;
            }

            if(countNumOfMatches != 1)
                throw std::logic_error("No 1-1 associate between "+ node->getData().getTerritoryName() + " and the continents.");
        }
    }catch(const std::exception& e){
        isErrorThrown = true;
        std::cerr<<e.what()<<endl;
    }

    //will terminate the program if an error was thrown
    if(isErrorThrown)
        exit(0);
}

Continent* Graph::createContinent(string name){
    Continent* ptr = new Continent(name);
    listOfContinents.push_back(ptr);
    return ptr;
}

stack<Node*> adjList;
vector<Node*> visited;
bool Graph::DFS(vector<Node*> graphNodes, Node* startNode){
    bool isConnected = false;
    //remove visited element
    for(int i=0; i<graphNodes.size();i++){
        if(startNode == graphNodes[i])
            graphNodes.erase(graphNodes.begin() + i);
    }

    visited.push_back(startNode);
    for(string str : visited.end()[-1]->getE()){
        for(Node* node : graphNodes){
            for(string edge : node->getE()){
                if(str == edge){
                    adjList.push(node);
                }
            }
        }
    }
    
    if(!adjList.empty()){//base condition is for adjList to be empty
        Node* temp = adjList.top();
        adjList.pop();
        return DFS(graphNodes, temp);
    }else{
        visited.clear();//to be ready for the next use
        if(graphNodes.size() == 0)
            isConnected = true; //connected graph
    }
    return isConnected;
}

/*
I'll create the following graph
Germany --> France --> Spain
*/
/*
int main(){
    Graph myGraph;
    
    //cout<<myGraph;
    Continent* southAmerica = myGraph.createContinent("South America");
    //cout<<*southAmerica;
    Territory venzuela("Venzuela", southAmerica);
    //cout<<venzuela;
    Territory brazil("Brazil", southAmerica);
    Territory argentina("Argentina", southAmerica);
    Territory peru("Peru", southAmerica);    
    myGraph.insertATerritory(peru);
    myGraph.insertATerritory(argentina); 
    Graph copy(myGraph);   
    cout<<*myGraph.getV()[0];
    myGraph.connectTwoNodes(myGraph.getV()[0],myGraph.getV()[1]);
    //test assignment operator
    Continent x = *southAmerica;
    //myGraph.insertATerritory(argentina);
    //myGraph.connectTwoNodes(myGraph.getV().end()[-1],myGraph.getV().end()[-2]);
    //myGraph.insertAndConnectTwoTerritories(venzuela, brazil);// venzuela --> brazil
    //myGraph.connectTwoNodes(myGraph.getV()[0], myGraph.getV().end()[-2]);// Peru --> Brazil

    myGraph.validate();
    cout<<"@ the end!"<<endl;
    //myGraph.insertAndConnectTwoTerritories(argentina, peru);// argentina --> peru
    //cout<<"connected? "<<myGraph.DFS(myGraph.getV(),myGraph.getV()[0])<<endl;
    //myGraph.validate();
    return 0;
    Continent* europe = myGraph.createContinent("Europe");
    Continent* xy = myGraph.createContinent("xy");
    Territory spain("Spain", europe); 
    Territory ran1("ran1", xy); 
    Territory ran2("ran2", xy); 

     //Unconnected node

    myGraph.insertATerritory(ran1); //Unconnected node
    myGraph.insertATerritory(ran2); //Unconnected node
    myGraph.connectTwoNodes(myGraph.getV()[1], myGraph.getV()[2]); //ran1 --> ran2

    Territory france("France", europe);
    Territory germany("Germany", europe);
    myGraph.insertAndConnectTwoTerritories(france, germany);// Germany --> France
    myGraph.connectTwoNodes(myGraph.getV()[0], myGraph.getV()[3]); //Spain --> France
    myGraph.connectTwoNodes(myGraph.getV()[0], myGraph.getV()[1]); //Spain --> Itali

    // for(Node* territory : myGraph.getV()){
    //     cout<<territory->getData().getTerritoryName() + " belongs to " + territory->getData().getContinent()->getContinentName()
    //         + " has the following edges:"<<endl;
    //     cout<<"The continent address is "<<territory->getData().getContinent()<<endl;
    //     for(string edge : territory->getE()){
    //         cout<<edge<<endl;
    //     }
    // }

    myGraph.validate();
    // cout<<"Are connected?"<<endl;
    // if(myGraph.areConnected(myGraph.getV()[0], myGraph.getV()[2]))
    //     cout<<"Yes Connected!"<<endl;
    // else
    //     cout<<"Not Connected!"<<endl;
        
    //cout<<myGraph.getV()[1]->getE().size()<<endl;
    
    //check validate function:
    
    //cout<<myGraph.getListOfContinents()[0]->getContinentName()<<endl;
    //cout<<myGraph.areConnected(myGraph.getV()[1], myGraph.getV()[0])<<endl;
    // Continent* ptr = myGraph.getV()[0]->getData().getContinent();
    // cout<<ptr<<endl;
    //cout<<myGraph.getV()[2]->getE()[0];   To check the edges in a specific node
    //myGraph.connectTwoNodes(myGraph.getV()[0],myGraph.getV()[1]);
    
    // cout<<"B4 "<<myGraph.getV()[0]->getDataPtr()->getOwnerId()<<endl;
    // myGraph.getV()[0]->getDataPtr()->setOwnerId(19);
    // cout<<"After "<<myGraph.getV()[0]->getDataPtr()->getOwnerId()<<endl;

 return 0;
}
*/