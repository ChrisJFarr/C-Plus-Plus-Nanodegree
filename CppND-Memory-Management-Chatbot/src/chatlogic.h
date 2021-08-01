#ifndef CHATLOGIC_H_
#define CHATLOGIC_H_

#include <vector>
#include <string>
#include "chatgui.h"

/*
// valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind-out.txt ./membot
// valgrind --leak-check=yes --track-origins=yes --log-file=valgrind-out.txt ./membot

Task 3
In file chatlogic.h / chatlogic.cpp, adapt the vector _nodes 
in a way that the instances of GraphNodes to which the vector 
elements refer are exclusively owned by the class ChatLogic (done). 
Use an appropriate type of smart pointer to achieve this (unique-ptr). 
Where required, make changes to the code such that data 
structures and function parameters reflect the changes (done). 
When passing the GraphNode instances to functions, make 
sure to not transfer ownership (?) and try to contain the changes 
to class ChatLogic where possible.

Task 4
In files chatlogic.h / chatlogic.cpp and graphnodes.h / graphnodes.cpp 
change the ownership of all instances of GraphEdge in a way such that 
each instance of GraphNode exclusively owns the outgoing GraphEdges and 
holds non-owning references to incoming GraphEdges (done). Use appropriate smart 
pointers and where required, make changes to the code such that data structures 
and function parameters reflect the changes (done). When transferring ownership from 
class ChatLogic, where all instances of GraphEdge are created, into instances 
of GraphNode, make sure to use move semantics (?). 

Task 5
In file chatlogic.cpp, create a local ChatBot instance on the stack at the bottom 
of function LoadAnswerGraphFromFile. Then, use move semantics to pass the ChatBot 
instance into the root node (done). Make sure that ChatLogic has no ownership relation to 
the ChatBot instance and thus is no longer responsible for memory allocation and 
deallocation (done). Note that the member _chatBot remains so it can be used as a communication 
handle between GUI and ChatBot instance. Make all required changes in files 
chatlogic.h / chatlogic.cpp and graphnode.h / graphnode.cpp. When the program is 
executed, messages on which part of the Rule of Five components of ChatBot is called 
should be printed to the console. When sending a query to the ChatBot, the output 
should look like the following: 

ChatBot Constructor
ChatBot Move Constructor
ChatBot Move Assignment Operator
ChatBot Destructor
ChatBot Destructor 
*/

// forward declarations
class ChatBot;
class GraphEdge;
class GraphNode;

class ChatLogic
{
private:
    //// STUDENT CODE
    ////

    // data handles (owned)
    // std::vector<GraphNode *> _nodes;  // original
    std::vector<std::unique_ptr<GraphNode>> _nodes;  // new
    std::vector<GraphEdge *> _edges;

    ////
    //// EOF STUDENT CODE

    // data handles (not owned)
    GraphNode *_currentNode;
    ChatBot *_chatBot;
    ChatBotPanelDialog *_panelDialog;

    // proprietary type definitions
    typedef std::vector<std::pair<std::string, std::string>> tokenlist;

    // proprietary functions
    template <typename T>
    void AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element);

public:
    // constructor / destructor
    ChatLogic();
    ~ChatLogic();

    // getter / setter
    void SetPanelDialogHandle(ChatBotPanelDialog *panelDialog);
    void SetChatbotHandle(ChatBot *chatbot);

    // proprietary functions
    void LoadAnswerGraphFromFile(std::string filename);
    void SendMessageToChatbot(std::string message);
    void SendMessageToUser(std::string message);
    wxBitmap *GetImageFromChatbot();
};

#endif /* CHATLOGIC_H_ */