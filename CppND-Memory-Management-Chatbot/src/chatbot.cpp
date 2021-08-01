#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>

#include "chatlogic.h"
#include "graphnode.h"
#include "graphedge.h"
#include "chatbot.h"

// constructor WITHOUT memory allocation
ChatBot::ChatBot()
{
    // invalidate data handles
    _image = nullptr;
    _chatLogic = nullptr;
    _rootNode = nullptr;
}

// constructor WITH memory allocation
ChatBot::ChatBot(std::string filename)
{
    std::cout << "ChatBot Constructor" << std::endl;
    
    // invalidate data handles
    _chatLogic = nullptr;
    _rootNode = nullptr;

    // load image into heap memory
    _image = new wxBitmap(filename, wxBITMAP_TYPE_PNG);
}

ChatBot::~ChatBot()
{
    std::cout << "ChatBot Destructor" << std::endl;

    // deallocate heap memory
    if(_image != NULL) // Attention: wxWidgets used NULL and not nullptr
    {
        delete _image;
        _image = NULL;
    }
}

//// STUDENT CODE
////
// rule-of-five
// destructor already implemented
// copy assignment operator
ChatBot& ChatBot::operator=(const ChatBot &source) // 3 : copy assignment operator
{
    // std::cout << "assigning ChatBot content of instance " << &source << " to instance " << this << std::endl;
    std::cout << "ChatBot Copy Assignment Operator" << std::endl;
    if (this == &source)
        return *this;

    // Delete existing image
    if(_image != NULL) // Attention: wxWidgets used NULL and not nullptr
    {
        delete _image;
    }
    // Allocate memory for image to copy to, wxBitmap
    _image = new wxBitmap(source._image->ConvertToImage());
    // _image = new wxBitmap(source.GetWidth(), source.GetHeight(), source.GetDepth());
    _chatLogic = source._chatLogic;
    _rootNode = source._rootNode;

    return *this;
}

// copy constructor
ChatBot::ChatBot(const ChatBot &source) // 2 : copy constructor
{
    /*
    takes an lvalue reference to the source instance, allocates a block of 
    memory of the same size as in the source and then copies the data into its 
    members (as a deep copy)
    */
   std::cout << "ChatBot Copy Constructor" << std::endl;
   // std::cout << "COPYING content of instance " << &source << " to instance " << this << std::endl;
    // Delete existing image
    delete _image;
    // Allocate memory for image to copy to, wxBitmap
    _image = new wxBitmap(source._image->ConvertToImage());
    _chatLogic = source._chatLogic;
    _rootNode = source._rootNode;
}
// move constructor
ChatBot::ChatBot(ChatBot &&source) // 4 : move constructor
{
    /*
    the move constructor builds an instance of a class using a source instance. 
    The key difference between the two is that with the move constructor, the source 
    instance will no longer be usable afterwards.
    */
    // std::cout << "MOVING (c’tor) instance " << &source << " to instance " << this << std::endl;
    std::cout << "ChatBot Move Constructor" << std::endl;
    // Copy handlers
    _image = source._image;
    _chatLogic = source._chatLogic;
    _rootNode = source._rootNode;
    // Null out source handlers
    source._image = nullptr;
    source._chatLogic = nullptr;
    source._rootNode = nullptr;
}
// move assignment operator
ChatBot& ChatBot::operator=(ChatBot &&source) // 5 : move assignment operator
{
    /*
    As with the move constructor, the data handle is copied from source to 
    target which is coming in as an rvalue reference again. Afterwards, the data 
    members of source are invalidated.
    */
    std::cout << "ChatBot Move Assignment Operator" << std::endl;
    // std::cout << "MOVING (assign) instance " << &source << " to instance " << this << std::endl;
    if (this == &source)
        return *this;

    // Delete existing image
    delete _image;

    // Copy handlers
    _image = source._image;
    _chatLogic = source._chatLogic;
    _rootNode = source._rootNode;
    // Null out source handlers
    source._image = nullptr;
    source._chatLogic = nullptr;
    source._rootNode = nullptr;

    return *this;
}


////
//// EOF STUDENT CODE

void ChatBot::ReceiveMessageFromUser(std::string message)
{
    // loop over all edges and keywords and compute Levenshtein distance to query
    typedef std::pair<GraphEdge *, int> EdgeDist;
    std::vector<EdgeDist> levDists; // format is <ptr,levDist>

    for (size_t i = 0; i < _currentNode->GetNumberOfChildEdges(); ++i)
    {
        GraphEdge *edge = _currentNode->GetChildEdgeAtIndex(i);
        for (auto keyword : edge->GetKeywords())
        {
            EdgeDist ed{edge, ComputeLevenshteinDistance(keyword, message)};
            levDists.push_back(ed);
        }
    }

    // select best fitting edge to proceed along
    GraphNode *newNode;
    if (levDists.size() > 0)
    {
        // sort in ascending order of Levenshtein distance (best fit is at the top)
        std::sort(levDists.begin(), levDists.end(), [](const EdgeDist &a, const EdgeDist &b) { return a.second < b.second; });
        newNode = levDists.at(0).first->GetChildNode(); // after sorting the best edge is at first position
    }
    else
    {
        // go back to root node
        newNode = _rootNode;
    }

    // tell current node to move chatbot to new node
    _currentNode->MoveChatbotToNewNode(newNode);
}

void ChatBot::SetCurrentNode(GraphNode *node)
{
    // update pointer to current node
    _currentNode = node;

    // select a random node answer (if several answers should exist)
    std::vector<std::string> answers = _currentNode->GetAnswers();
    std::mt19937 generator(int(std::time(0)));
    std::uniform_int_distribution<int> dis(0, answers.size() - 1);
    std::string answer = answers.at(dis(generator));

    // send selected node answer to user
    _chatLogic->SendMessageToUser(answer);
}

int ChatBot::ComputeLevenshteinDistance(std::string s1, std::string s2)
{
    // convert both strings to upper-case before comparing
    std::transform(s1.begin(), s1.end(), s1.begin(), ::toupper);
    std::transform(s2.begin(), s2.end(), s2.begin(), ::toupper);

    // compute Levenshtein distance measure between both strings
    const size_t m(s1.size());
    const size_t n(s2.size());

    if (m == 0)
        return n;
    if (n == 0)
        return m;

    size_t *costs = new size_t[n + 1];

    for (size_t k = 0; k <= n; k++)
        costs[k] = k;

    size_t i = 0;
    for (std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i)
    {
        costs[0] = i + 1;
        size_t corner = i;

        size_t j = 0;
        for (std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j)
        {
            size_t upper = costs[j + 1];
            if (*it1 == *it2)
            {
                costs[j + 1] = corner;
            }
            else
            {
                size_t t(upper < corner ? upper : corner);
                costs[j + 1] = (costs[j] < t ? costs[j] : t) + 1;
            }

            corner = upper;
        }
    }

    int result = costs[n];
    delete[] costs;

    return result;
}