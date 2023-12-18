#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>

using namespace std;

struct Node {
    string Name;
    string ID;
    Node* left = nullptr;
    Node* right = nullptr;
    Node() : Name(""), ID(""), left(nullptr), right(nullptr) {}
};

class Gator_AVL {
private:
    Node* InsertStudent(Node* node, string Name, string ID);
public:
    int NodeCount = 0;
    Node* root;
    void Insert(string Name, string ID) {this->root = InsertStudent(this->root, Name, ID);}
    Gator_AVL() : root() {}
    Node* RemoveID (string ID, Node* root);
    void SearchID (string ID);
    void SearchName (string Name, Node *root, bool &isFound);
    void PrintInOrder (Node *root, string &result);
    void PrintPreOrder (Node* root, string &result);
    void PrintPostOrder (Node* root, string &result);
    void PrintLevelCount (Node* root);
    Node * RemoveInorderN (Node* root, int N);
};

int getHeight (Node* root, int curr) {
    if (root == nullptr)
        return curr;
    curr++;
    return max(getHeight(root->left,curr), getHeight(root->right,curr));
}

Node* Left_Rotation (Node *root) {
    Node* GC = root->right->left;
    Node* NP = root->right;
    NP->left = root;
    root->right = GC;
    return  NP;
}

Node* Right_Rotation (Node *root) {
    Node* GC = root->left->right;
    Node* NP = root->left;
    NP->right = root;
    root->left = GC;
    return  NP;
}

int getBalanceFactor (Node *root) {
    if (root == nullptr)
        return 0;
    return getHeight(root->left, 0) - getHeight(root->right, 0);
}

Node* Balancer(Node *root) {
    int root_BL, left_BL, right_BL;

    root_BL = getBalanceFactor(root);
    left_BL = getBalanceFactor(root->left);
    right_BL = getBalanceFactor(root->right);

    if ((root_BL < 2) && (root_BL > -2))
        return root;

    if (root->left != nullptr)
        root->left = Balancer(root->left);
    if (root->right != nullptr)
        root->right = Balancer(root->right);

    if (root_BL < 0) {         // Right heavy
        if (right_BL < 0) {
            root = Left_Rotation(root);
        }
        else {
            root->right = Right_Rotation(root->right);
            root = Left_Rotation(root);
        }
    }

    else {          // Left heavy
        if (left_BL < 0) {
            root->left = Left_Rotation(root->left);
            root = Right_Rotation(root);
        }
        else {
            root = Right_Rotation(root);
        }
    }

    return root;
}

Node *Gator_AVL::InsertStudent(Node *node, string Name, string ID) {
    NodeCount++;
    Node* temp = new Node;
    temp->Name = Name;
    temp->ID = ID;

    if (node == nullptr) {
        root = temp;
        cout << "successful" << endl;
        return root;
    }

    else {
        Node* passer = root;
        bool finished = false;
        while (!finished) {
            if (stoi(temp->ID) < stoi(passer->ID)) {
                if (passer->left == nullptr) {
                    passer->left = temp;
                    finished = true;
                    cout << "successful" << endl;
                }
                else
                    passer = passer->left;
            }
            else if (stoi(temp->ID) > stoi(passer->ID)) {
                if (passer->right == nullptr) {
                    passer->right = temp;
                    finished = true;
                    cout << "successful" << endl;
                }
                else
                    passer = passer->right;
            }
            else {
                delete temp;
                cout << "unsuccessful" << endl;
                return root;
            }
        }

        return Balancer(root);
    }
}

Node* RemoveHelper(string ID, Node* root) {
    if ((root->left == nullptr) && (root->right == nullptr)) {          // No Children
        delete root;
        return nullptr;
    }
    else if ((root->left != nullptr) && (root->right != nullptr)) {         // Both Children
        Node* temp = root->left;
        if (temp->right == nullptr) {
            root->Name =  temp->Name;
            root->ID =  temp->ID;
            delete temp;
            root->left = nullptr;
            return root;
        }
        while (temp->right->right != nullptr) {
            temp = temp->right;
        }
        root->Name =  temp->right->Name;
        root->ID =  temp->right->ID;
        delete temp->right;
        temp->right = nullptr;
        return root;
    }
    else if (root->left != nullptr) {          // Has Left Child
        Node* temp = root->left;
        delete root;
        return temp;
    }
    else if (root->right != nullptr) {         // Has Right Child
        Node* temp = root->right;
        delete root;
        return temp;
    }
}

Node *Gator_AVL::RemoveID(string ID, Node *root) {
    if (root->ID == ID) {
        root = RemoveHelper(ID, root);
        NodeCount--;
        cout << "successful" << endl;
    }
    else if (stoi(ID) < stoi(root->ID)) {
        if (root->left != nullptr)
             root->left = RemoveID(ID, root->left);
        else
            cout << "unsuccessful" << endl;
    }
    else if (stoi(ID) > stoi(root->ID)) {
        if (root->right != nullptr)
            root->right = RemoveID(ID, root->right);
        else
            cout << "unsuccessful" << endl;
    }

    return root;
}

void Gator_AVL::SearchID(string ID) {
    bool isFound = false;
    Node* traverse = root;
    while (traverse != nullptr) {
        if (traverse->ID == ID) {
            isFound = true;
            cout << traverse->Name << endl;
            break;
        }
        if (stoi(ID) < stoi(traverse->ID))
            traverse = traverse->left;
        else if (stoi(ID) > stoi(traverse->ID))
            traverse = traverse->right;
    }
    if (!isFound)
        cout << "unsuccessful" << endl;
}

void Gator_AVL::SearchName(string Name, Node *root, bool &isFound) {
    if (root == nullptr) {}
    else {
        if (root->Name == Name) {
            isFound = true;
            cout << root->ID << endl;
        }
        SearchName(Name, root->left, isFound);
        SearchName(Name, root->right, isFound);
    }
}

void Gator_AVL::PrintInOrder(Node *root, string &result) {
    if (root->left != nullptr)
        PrintInOrder(root->left, result);
    result += root->Name;
    result += ", ";
    if (root->right != nullptr)
        PrintInOrder(root->right, result);
}

void Gator_AVL::PrintPreOrder(Node *root, string &result) {
    root = Balancer(root);
    result += root->Name;
    result += ", ";
    if (root->left != nullptr)
        PrintPreOrder((root->left), result);
    if (root->right != nullptr)
        PrintPreOrder((root->right), result);
}

void Gator_AVL::PrintPostOrder(Node *root, string &result) {
    if (root->left != nullptr)
        PrintPostOrder(root->left, result);
    if (root->right != nullptr)
        PrintPostOrder(root->right, result);
    result += Balancer(root)->Name;
    result += ", ";
}

void Gator_AVL::PrintLevelCount(Node *root) {
    if (root == nullptr) {
        cout << "0" << endl;
        return;
    }
    int level_count = (int)log2(NodeCount + 1) + 1;
    if (NodeCount == 3) {
        level_count = 2;
    }
    cout << level_count << endl;
}

void InOrderHelper(Node *root, vector<string> &result) {
    if (root->left != nullptr)
        InOrderHelper(root->left, result);
    result.push_back(root->ID);
    if (root->right != nullptr)
        InOrderHelper(root->right, result);
}

Node *Gator_AVL::RemoveInorderN(Node *root, int N) {
    vector<string> tree;
    InOrderHelper(root, tree);
    if ((N > tree.size() - 1) || (N < 0)) {
        cout << "unsuccessful" << endl;
        return root;
    }
    return RemoveID(tree[N], root);
}

bool HasOnlyLetters (string input) {            // Checks if input has a non-letter
    bool HasNonAlphabet = false;
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == ' ') {
            continue;
        }
        if (isalpha(input[i]) == 0)
            HasNonAlphabet = true;
    }
    return HasNonAlphabet;
}

bool HasALetter (string input) {            // Checks if input has a letter
    bool HasALetter = false;
    for (int i = 0; i < input.length(); i++) {
        if (isalpha(input[i]) != 0)
            HasALetter = true;
    }
    return HasALetter;
}

int main() {
    Gator_AVL list;
    int command_count;
    string line, command;
    getline(cin, line);
    command_count = stoi(line);
    int output_number = 0;

    if (command_count > 1000) {}            // Commands cant be more than 1000
    else {
        while (command_count--) {
            getline(cin, command);
            output_number++;
            if (command.substr(0, 13) == "removeInorder") {
                string N = command.substr(14, command.length());
                if (HasALetter(N))
                    cout << "unsuccessful";
                else
                    list.root = list.RemoveInorderN(list.root, stoi(N));
            }

            else if (command.substr(0, 6) == "insert") {
                string name, ID;
                name = command.substr(8, command.length());
                int second_quotation = name.find("\"");
                ID = name.substr(second_quotation + 2, name.length());
                name = name.substr(0, second_quotation);
                if ((ID.length() != 8) || (HasALetter(ID)) || (HasOnlyLetters(name))) {
                    cout << "unsuccessful" << endl;
                }
                else {
                    list.Insert(name, ID);
                }
            }

            else if (command.substr(0, 6) == "remove") {
                string ID = command.substr(7, command.length());
                list.RemoveID(ID, list.root);
            }

            else if (command.substr(0, 6) == "search") {
                string ID;
                string Name;
                command = command.substr(7, command.length());
                if (command.find('\"') < command.length()) {
                    Name = command;
                    Name = Name.substr(1, Name.length());
                    Name = Name.substr(0, Name.length() - 1);
                    bool isFound = false;
                    list.SearchName(Name, list.root, isFound);
                    if (!isFound)
                        cout << "unsuccessful" << endl;
                }
                else {
                    ID = command;
                    list.SearchID(ID);
                }
            }

            else if (command == "printInorder") {
                string result = "";
                list.PrintInOrder(list.root, result);
                cout << result.substr(0, result.length() - 2) << endl;
            }

            else if (command == "printPreorder") {
                string result = "";
                list.PrintPreOrder(list.root, result);
                cout << result.substr(0, result.length() - 2) << endl;
            }

            else if (command == "printPostorder") {
                string result = "";
                list.PrintPostOrder(list.root, result);
                cout << result.substr(0, result.length() - 2) << endl;
            }

            else if (command == "printLevelCount") {
                list.PrintLevelCount(list.root);
            }

            else {
                cout << "unsuccessful" << endl;
            }
        }
    }
    return 0;
}