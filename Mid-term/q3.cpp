// Instructions:

// Write a program that demonstrates array creation and resizing in C++. Follow the steps below:

// 1) Scan an integer that specifies the initial capacity of the array.
// 2) Create a one-dimensional array with a given initial capacity.
// 3) Scan the no of elements to insert in to the array, followed by the array elements.
// 4) While scanning the elements, insert the following elements into the array.
// 5) Implement an array resizing algorithm that doubles the size of the array when it reaches its capacity limit.
// 6) The resizing algorithm should create a new array with double the capacity.
// 7) Copy all elements from the original array into the new array.
// 8) Insert the additional element into the new array.
// 9) Print the array elements each time the insertion operation is performed. Refer to the sample output provided.
// 10) Analyze and explain the time complexity of the array resizing operation.




// Sample Input:

// Initial Array Capacity: 3

// No of Elements to Insert: 8 

// Elements to Insert: 4, 17, 13, 12, 45, 19, 64, 76

 


// Sample Output:

// Array after inserting 4: [4]

// Array after inserting 17: [4, 17]

// Array after inserting 13: [4, 17, 13]

// Array reached capacity. Resizing to size: 6

// Array after inserting 12: [4, 17, 13, 12]

// Array after inserting 45: [4, 17, 13, 12, 45]

// Array after inserting 19: [4, 17, 13, 12, 45, 19]

// Array reached capacity. Resizing to size: 12

// Array after inserting 64: [4, 17, 13, 12, 45, 19, 64]

// Array after inserting 76: [4, 17, 13, 12, 45, 19, 64, 76]

#include <iostream>

using namespace std;

void printArray(int* array, int size)
{
    cout << " [";
    for(int i = 0; i < size; i++)
    {
        cout << array[i];
        if(i != size - 1)
        {
            cout << ", ";
        }
    }
    cout << "]" << endl;

}

int main()
{
    int capacity = 0, noOfElements = 0;



    cout << "Initial Array Capacity: ";

    cin >> capacity;

        cout << "No of Elements to Insert: ";

    cin >> noOfElements;

    int currentCapacity = capacity;

    int* array = new int[currentCapacity];

    int elementToInsert = 0;

    for(int i = 0; i < noOfElements; i++)
    {
        cout << "Element to Insert: ";

        cin >> elementToInsert;


        if(i == currentCapacity)
        {
            int* newArray = new int[currentCapacity * 2];


            for(int j = 0; j < currentCapacity; j++)
            {
                newArray[j] = array[j];
            }

            currentCapacity *= 2;
            
            cout <<  "Array reached capacity. Resizing to size: " << currentCapacity << endl;

            delete[] array;

            array = newArray;
        }

        array[i] = elementToInsert;

        cout << "Array after inserting " << elementToInsert << ":";
        printArray(array, i + 1);
    }

    return 0;
}
