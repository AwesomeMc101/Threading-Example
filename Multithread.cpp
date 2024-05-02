#include <iostream> //cout
#include <Windows.h> //CreateThread, Sleep, LPVOID type
#include <thread> //std::thread

/* Define a struct to set our parameters for the first thread function. The "result" variable
is a pointer because we want to actually change its value, mimicking a return value. */
struct args {
    float a;
    float b;
    float* result;
};

void thread_add(LPVOID input) {
    args arguments = *(static_cast<args*>(input));
    
    /* ^^^^
    Cast your input args (LPVOID just as pointer) to a args* pointer
    and dereference it in the same line. 

    In multiple lines, you can imagine it like this:
    args* args_ptr = static_cast<args*>(input);
    args arguments = *args_ptr;
    
    In C (aka using C-style type casting), this is done as:
    args arguments = *((args*)input);
    
    */

    float ans = (arguments.a + arguments.b); //add up the struct's values of a & b
    *arguments.result = ans; //set the pointers result to our answer
    //^^^^^ this is essentially the return value.
}

//the following functions are self explanatory :)
void std_thread_add_ptr(float a, float b, float* result) {
    *result = (a + b); //deref result and change its value
}

void std_thread_add_reference(float a, float b, float& result) {
    result = (a + b); //change referenced variables value
}

int main()
{

    //In the end, this function is the same as doing:
    //result = add(1,6);
    
    //Threading is slower like this, but becomes a lot faster the more data being processed (at least that's
    //how I find it to be.)

    /* define our struct and the values in it. */
    args ar;
    ar.a = 1.f;
    ar.b = 6.f;

    /* create our original result variable and set the pointer in our args struct */
    float result; //this is the float we read from. the thread changes this.
    ar.result = &result; //set ar.result as a pointer to our result float.

    /* Create the thread.
    - 128 is the stack size. You can leave that as 0 as well. 
    - The 3rd argument uses c-casting and sets the function thread_add to the thread's 
    start routine type. The type is essentially what holds the address to the function
    that you want the thread to run. 
    - The fourth param passes a pointer to our arguments struct. This is how we pass data
    to the thread. */
    CreateThread(0, 128, (LPTHREAD_START_ROUTINE)thread_add, (&ar), 0, 0);
    Sleep(1); //give the thread time to execute.

    /* If your execution needs longer, you could write define the result and do something like this
    
    const float undefined_float = -4;
    float result = undefined_float;
    CreateThread(...);
    while(result == undefined_float){
        Sleep(1); //or no sleep at all!
    }
    //Execution will continue when value has changed
    std::cout ...;
    

    Similarly, you could add another variable just called "bool completed = false" and have that value
    be changed to true when the thread is done. Then, here, you can await it's value being true. There's
    lots of ways!

    */


    std::cout << "Result: " << result << "\n";

    // Now to try the std library version of threading, instead of the windows version.
    //This is in the <thread> file
    // 
    //One example will display using a pointer as an arg (std_thread_add_ptr) 
    //The second example will display using an address/reference as the result arg (std_thread_add_reference)
    //-> Using both of these should show how multithreading 2+ threads works.

    float ptr_result, ref_result;  //define our result variables

    //Create the threads. The std lib does a lot of the work for you when you do threads
    //this way. You simply write the function name and each of the args. When passing 
    //a reference to a variable (i.e. parameter is float& result), you must use
    //the std::ref function as shown. 
    //For pointers, you pass the address (using the & symbol).
    std::thread thread_add_ptr(std_thread_add_ptr, 1.f, 7.f, &ptr_result);
    std::thread thread_add_ref(std_thread_add_reference, 1.f, 7.f, std::ref(ref_result));

    //The .join function awaits completion. This is much simpler than awaiting CreateThread's completion
    //in my opinion.
    thread_add_ptr.join(); //wait for thread to complete
    thread_add_ref.join();

    //You can also detach the thread and let it run independently
    //with thread.detach().
    
    std::cout << "Pointer thread result: " << ptr_result
        << "\nReference thread result: " << ref_result << "\n";
}
