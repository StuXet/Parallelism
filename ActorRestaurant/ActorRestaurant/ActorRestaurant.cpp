#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <random>
#include <chrono>

using namespace std;

mutex mtx;

// Function to generate a random number between min and max
int getRandomNumber(int min, int max) 
{
    static thread_local mt19937 generator(chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

// Actor classes
class Customer {
public:
    void enterRestaurant() 
    {
        cout << "Customer enters the restaurant" << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }

    vector<int> placeOrder()
    {
        vector<int> ingredients;
        for (int i = 0; i < 3; ++i)
        {
            ingredients.push_back(getRandomNumber(1, 10));
        }
        cout << "Customer orders ingredients: ";
        for (int ingredient : ingredients) 
        {
            cout << ingredient << " ";
        }
        cout << endl;
        this_thread::sleep_for(chrono::seconds(1));
        return ingredients;
    }

    void eatFood() 
    {
        cout << "Customer eats the food" << endl;
        this_thread::sleep_for(chrono::seconds(2));
    }

    void leaveRestaurant()
    {
        cout << "Customer leaves the restaurant" << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
};

class Waiter 
{
public:
    void takeOrder()
    {
        cout << "Waiter takes the order" << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }

    void serveFood() 
    {
        cout << "Waiter serves the food" << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
};

class Cooker 
{
public:
    void cookOrder(const vector<int>& ingredients)
    {
        cout << "Cooker starts to cook the order with ingredients: ";
        for (int ingredient : ingredients)
        {
            cout << ingredient << " ";
        }
        cout << endl;
        this_thread::sleep_for(chrono::seconds(3));
        cout << "Cooker finished cooking the order" << endl;
    }
};

class Chef 
{
public:
    void makeMeal(const vector<int>& ingredients) 
    {
        cout << "Chef makes a meal from the cooked order ingredients: ";
        for (int ingredient : ingredients)
        {
            cout << ingredient << " ";
        }
        cout << endl;
        this_thread::sleep_for(chrono::seconds(2));
    }
};

int main() 
{
    Customer customer;
    Waiter waiter;
    Cooker cooker;
    Chef chef;

    customer.enterRestaurant();

    vector<int> order = customer.placeOrder();

    waiter.takeOrder();

    // Use a mutex to synchronize the cooking process
    {
        lock_guard<mutex> lock(mtx);
        thread cookerThread(&Cooker::cookOrder, &cooker, order);
        cookerThread.join();
    }

    chef.makeMeal(order);
    waiter.serveFood();
    customer.eatFood();
    customer.leaveRestaurant();

    return 0;
}