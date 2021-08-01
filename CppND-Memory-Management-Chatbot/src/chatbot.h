#ifndef CHATBOT_H_
#define CHATBOT_H_

#include <wx/bitmap.h>
#include <string>

class GraphNode; // forward declaration
class ChatLogic; // forward declaration

class ChatBot
{
private:
    // data handles (owned)
    wxBitmap *_image; // avatar image

    // data handles (not owned)
    GraphNode *_currentNode;
    GraphNode *_rootNode;
    ChatLogic *_chatLogic;

    // proprietary functions
    int ComputeLevenshteinDistance(std::string s1, std::string s2);

public:
    // constructors / destructors
    ChatBot();                     // constructor WITHOUT memory allocation
    ChatBot(std::string filename); // constructor WITH memory allocation
    ~ChatBot();

    //// STUDENT CODE
    ////

    // In file chatbot.h / chatbot.cpp, make changes to the class ChatBot such that it complies with the Rule of Five. 
    // Make sure to properly allocate / deallocate memory resources on the heap and also copy member data where it makes 
    // sense to you. In each of the methods (e.g. the copy constructor), print a string of the type „ChatBot Copy Constructor“ 
    // to the console so that you can see which method is called in later examples. 
    // rule-of-five
    // destructor already implemented
    // assignment operator
    ChatBot& operator=(const ChatBot &source);
    // copy constructor
    ChatBot(const ChatBot &source);
    // move constructor
    ChatBot(ChatBot &&source);
    // move assignment operator
    ChatBot& operator=(ChatBot &&source);
    
    ////
    //// EOF STUDENT CODE

    // getters / setters
    void SetCurrentNode(GraphNode *node);
    void SetRootNode(GraphNode *rootNode) { _rootNode = rootNode; }
    void SetChatLogicHandle(ChatLogic *chatLogic) { _chatLogic = chatLogic; }
    ChatLogic* GetChatLogicHandle() { return _chatLogic; }
    wxBitmap *GetImageHandle() { return _image; }

    // communication
    void ReceiveMessageFromUser(std::string message);
};

#endif /* CHATBOT_H_ */
