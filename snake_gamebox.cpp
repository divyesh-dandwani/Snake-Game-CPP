#include <iostream>
using namespace std;

int main() 
{
    
    const int HEIGHT = 12; 
    const int WIDTH = 30; 

    
    for (int row = 0; row < HEIGHT; ++row) 
    {
        
        for (int col = 0; col < WIDTH; ++col) 
        {
           
            if (row == 0 || row == HEIGHT - 1 || col == 0 || col == WIDTH - 1) 
            {
                cout << '#';  
            } else 
            {
                cout << ' ';  
            }
        }
        cout << '\n'; 
    }

    return 0;
}
