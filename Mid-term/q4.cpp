// Instructions:

// Implement a Linked List data structure using structures in C++. Follow the steps below:

// 1) Scan the no of elements to insert in to the Linked list, followed by the elements.
// 2) Insert the following elements into the Linked List.
// 3) Ensure that new elements are always appended at the end of the Linked List.
// 4) Print the last element in the Linked List after each insertion.
// 5) Scan no of elements to search in Linked List, followed by elements.
// 6) After the insertion operation is completed, perform a search for elements within your Linked List data 
// structure. Determine whether each element is "found" or "not found".
// 7) Discuss the time complexities associated with searching for elements, explaining your findings.

 


// Sample Input:

// No of Elements to Insert: 6

// Elements to Insert: 24, 12, 13, 46, 107, 26

// No of Search Elements: 2

// Elements to Search: 24, 19

 


// Sample Output:

// Last element after inserting 24: 24

// Last element after inserting 12: 12

// Last element after inserting 13: 13

// Last element after inserting 46: 46

// Last element after inserting 107: 107

// Last element after inserting 26: 26

// Search Result for element 24: Found

// Search Result for element 19: Not Found


#include <iostream>

using namespace std;

struct Node{
    int data;
    Node* next;
};

class LinkedList{
    private:
        Node* head;
        Node* tail;
    public:
        LinkedList(){
            head = nullptr;
            tail = nullptr;
        }
        void insert(int data){
            Node* newNode = new Node();

            newNode->data = data;
            newNode->next = nullptr;

            if(head == nullptr){
                head = newNode;
                tail = newNode;
            }
            else{
                tail->next = newNode;
                tail = newNode;
            }


            cout << "Last element after inserting " << data << ": " << tail->data << endl;

        }

        bool search(int data){
            Node* currentNode = head;

            while (currentNode != nullptr)
            {
                if(currentNode->data == data)
                    return true;
                    
                currentNode = currentNode->next;
            }
            
            return false;
        }
};


int main()
{


    int noOfElementsToInsert = 0, noOfElementsToSearch = 0, element = 0;


    LinkedList* list = new LinkedList();


    cout << "No of Elements to Insert: ";

    cin >> noOfElementsToInsert;


    for(int i = 0; i < noOfElementsToInsert; i++)
    {
     
        cout << "Element to Insert: ";

        cin >> element;   
        list->insert(element);
    }

    cout << "No of Elements to Search: ";

    cin >> noOfElementsToSearch;

        for(int i = 0; i < noOfElementsToSearch; i++)
    {
     
        cout << "Element to Search: ";

        cin >> element;   
        bool searchResult = list->search(element);

        cout << "Search Result for element " << element << ": " << (searchResult ? "Found" : "Not Found") << endl;
    }


    return 0;
}
