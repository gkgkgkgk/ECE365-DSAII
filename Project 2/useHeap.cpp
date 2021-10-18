//
// This program allows the user to manipulate a binary heap.
// The program only inserts string ids with with associated keys
// into the heap. The heap class, however, is capable of storing
// arbitrary pointers along with each heap item.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <random> 
#include <functional>
#include <set>
#include <climits>

#include "heap.h"

#define FORCE test.force
#define FI test.force<int>
#define FS test.force<string>

using namespace std;

// Read an integer from standard input; if a non-integer is in the
// buffer, the state is fixed and the user is re-prompted;
// either way, the remainder of the buffer is cleared
void getInteger(string message, int &ref)
{
    bool inputGood = false;
    while (!inputGood)
    {
        inputGood = true;

        cout << message;
        cin >> ref;

        if (!cin)
        {
            // Non-integer in input buffer, get out of "fail" state
            cin.clear();
            inputGood = false;
        }
        while (cin.get() != '\n')
            ; // clear buffer
    }
}

class utester {
  public:
    utester() = default;
    template<class T> 
    void force(T input , T expectation, string message = "Invalid response"  ){
      if (input != expectation){
        cerr << "Failure: " << message << "\ncounter: " << counter<< " expected: " << expectation << " revieved: " << input << "\n"; 
        exit(-1); 
      }
      counter++; 
    }
  private:
    unsigned int counter = 1; 
};

void baseTest(){
  heap h(5000); 
  utester test; 
  int counter = 0; 
  FI(h.insert("string one",35),0, "Insert return failure 1");
  FI(h.insert("string two",55),0, "Insert return failure 2");
  FI(h.insert("string three",45),0, "Insert return failure 3");
  FI(h.insert("string four",5),0, "Insert return failure 4");
  FI(h.insert("string two",10),2, "Insert return unseccseful");
  FI(h.insert("string five",80),0, "Insert return failure 5");
  FI(h.insert("string six",20),0, "Insert return failure 6");
  FI(h.insert("string seven",70),0, "Insert return failure 7");
  int key;
  string id; 
  FI(h.deleteMin(&id,&key),0);
  FI(key,5, "Incorrect Key"); 
  FORCE<string>(id,"string four","Incorrect id"); 
  FI(h.remove("string four",&key),1); 
  FI(h.deleteMin(&id,&key),0);
  FI(key,20,"Incorrect Key"); 
  FORCE<string>(id,"string six", "Incorrect id");
  FI(h.setKey("string one", 50),0);
  FI(h.remove("string two",&key),0);
  FI(key,55); 
  FI(h.deleteMin(&id,&key),0);
  FI(key,45,"Incorrect Key"); 
  FORCE<string>(id,"string three","Incorrect id");
  FI(h.deleteMin(&id,&key),0);
  FI(key,50,"Incorrect Key"); 
  FORCE<string>(id,"string one","Incorrect ID"); 
  FI(h.setKey("string five",60),0);
  FI(h.deleteMin(&id,&key),0);
  FI(key,60);
  FORCE<string>(id,"string five"); 
  FI(h.deleteMin(&id,&key),0);
  FI(key,70);
  FORCE<string>(id,"string seven"); 
  FI(h.deleteMin(&id,&key),1);

  cout << "passed first test\n"; 
}

void insertionSort(vector<int> &arr, int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;
 
        /* Move elements of arr[0..i-1], that are
        greater than key, to one position ahead
        of their current position */
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void testOnlyInsertAndDelteMin(int iterations){
  std::random_device rd;
  cout << INT_MAX << "\n";
  cout << INT_MIN << "\n";     
  std::mt19937 rng(rd());    
  std::uniform_int_distribution<int> uni(INT_MIN,INT_MAX); 
  utester test; 
  heap h(iterations); 
  vector<int> sorted(iterations); 

  for(int i = 0; i<iterations; i++){
    int random_integer = uni(rng);
    FI(h.insert("string " + to_string(i), random_integer),0);
  }

  for(int i = 0; i<iterations;i++){
    int key; 
    FI(h.deleteMin(nullptr,&key),0);
    sorted[i] = key; 
  }

  vector<int> toBeSorted = sorted; 
  insertionSort(toBeSorted,iterations);

  for(int i = 0; i<iterations;i++){
    FI(sorted[i],toBeSorted[i]); 
  }

  cout << "passed" << endl;
  return; 

}

int main()
{

  testOnlyInsertAndDelteMin(500);

  return 0;

    // heap h(10);
    // h.insert("a", 10);
    // h.insert("b", 13);
    // h.insert("c", 8);
    // h.insert("d", 11);
    // h.insert("e", 18);
    // h.insert("f", 1);
    // h.insert("g", 9);
    // h.insert("h", 15);
    // h.print();
    // h.deleteMin();
    // cout << "delete!" << endl;
    // h.print();
    // h.setKey("c", 20);
    // cout << "key set!" << endl;
    // h.print();
    // return 0;
    // int capacity = 0;
    // int option;
    // string stringTmp;
    // int key, id;
    // int retVal;

    // // Have user choose capacity for binary heap
    // getInteger("Choose a capacity for the binary heap: ", capacity);

    // // Create the heap
    // heap myHeap1(capacity);

    // while (1)
    // {
    //     cout << "\nOptions:\n";
    //     cout << "1 - Insert a new item into the binary heap\n";
    //     cout << "2 - Set the key of a specified item\n";
    //     cout << "3 - Delete a specified item\n";
    //     cout << "4 - Perform a deleteMin\n";
    //     cout << "5 - Quit\n";

    //     // Have the user choose an option
    //     getInteger("Choose an option: ", option);
    //     switch (option)
    //     {

    //     case 1:
    //         // Get data to insert into heap from the user and insert it

    //         cout << "Enter an id string (to insert): ";
    //         getline(cin, stringTmp);

    //         getInteger("Enter an associated integer key: ", key);

    //         retVal = myHeap1.insert(stringTmp, key);

    //         cout << "\nCall to 'insert' returned: " << retVal << "\n";

    //         break;

    //     case 2:
    //         // Get id string and new key from user and change the key

    //         cout << "Enter an id string (to change its key): ";
    //         getline(cin, stringTmp);

    //         getInteger("Enter an associated integer key: ", key);

    //         retVal = myHeap1.setKey(stringTmp, key);
    //         cout << "\nCall to 'setKey' returned: " << retVal << "\n";

    //         break;

    //     case 3:
    //         // Get id string from user and delete it from the heap

    //         cout << "Enter an id string (to delete): ";
    //         getline(cin, stringTmp);

    //         retVal = myHeap1.remove(stringTmp, &key);
    //         cout << "\nCall to 'delete' returned: " << retVal << "\n";

    //         if (retVal == 0)
    //         {
    //             cout << "\nDeleted item with string id \"" << stringTmp
    //                  << "\" and key " << key << "\n";
    //         }

    //         break;

    //     case 4:
    //         // Perform the deleteMin operation on the heap

    //         retVal = myHeap1.deleteMin(&stringTmp, &key);
    //         cout << "\nCall to 'deleteMin' returned: " << retVal << "\n";

    //         if (retVal == 0)
    //         {
    //             cout << "\nDeleted item with string id \"" << stringTmp
    //                  << "\" and key " << key << "\n";
    //         }

    //         break;

    //     case 5:
    //         cout << "\nGoodbye!\n";
    //         exit(0);

    //     default:
    //         cerr << "Error, that input is not valid!\n";
    //         exit(1);
    //     }
    // }

    // cerr << "Error, we should never get here!\n";
    // exit(1);
}