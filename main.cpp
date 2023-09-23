//(c) Ramish 2022
//Project
#include <iostream>
#include <fstream> //for files
#include <vector>  //for arrays
#include <iomanip> //for hexadecimal conversion
#include <string>
#include <sstream> // stringstream for hexadecimal conversion
#include <stdlib.h> //for srand
#include <ctime> // for time
//#include <unistd.h> //for sleep to delay the program, security in case a hacker decides to show up
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
using namespace std;

const int keynum[]={1217,1427,1907,2677,3347}; // random values used for hashing

vector <string> username; // to store all the usernames from the file
vector <string> phone; // to store all the phone numbers from the file
vector <string> pass; // to store all the hashed password from the file
vector <string> keyv; // to store all the hashed key positions from the file

//prototypes
//Dummyproofingss
int f_dummyproof(string prompt_for_user);
// dummyproofing for the first input, for the user to choose
string user_dummyproof(string another_prompt);
// for inputting the username (include no spaces and no repetition) (repetition is managed in the main )
string numb_dummyproof(string another_prompt);
//for inputting the phone number (include no spaces and no repetition and only numbers) (repetition is managed in main)
string pass_dummyproof(string another_prompt);
// to input password (using the isprint from cctype)

string hash_string(string string_to_hash, int position_key);
// for hashing the password entered
string hash_key(int key_position);
// for hashing the key position
void store();
// to store all the data in the arrays initialized above from the files
void update();
// to update the data in the file from the arrays

void Username_forgot(); 
// used when logging in. 
//Tells the user his username if he forgets the username. Phone number is entered to get the username
void Password_forgot(int position_of_user_data,int random_key_position);
// to reset password if user forgets it

int keyposition(int position_of_user_data);
// To get the key position from the database
void change_password(int position_of_user_data, int random_key_position);
//function to change password

void change_number(int position_of_user_data);
//function to change number used after the user has logged in and wants to update his number
int main() 
{
  string user; // to store the username entered by user
  string password; // to store the hashed password entered by user 
  string numb; // to store number entered by the user
  string key; // to store the hashed value of the key_position
  int key_rand; // to store a random integer between 0 to 6
  int f_input;


  srand (time(NULL));
  key_rand=rand()%5; //get random num
  
  store();
  cout << "Computer science project login page!\n\n";

  f_input=f_dummyproof("Do you want to login to your account or sign up?\nEnter:\n[1] login\n[2] sign up\n");

  if (f_input==2)
  {
    system("clear");
    bool check;
    
    cin.ignore(100,'\n');
    //as cin is used at top in f_dummyproof function
    do
    {
      user=user_dummyproof("Enter username (no spaces): ");
      //get the username and dummyproof it
      
      check=true;
      for (int i=0;i<username.size();i++)//loop through the array
      {
        if (username[i]==user) check=false; // check if username is already used
        
      }
      if (check==false) 
      {
        cout<<"Username taken. Try again\n";
        //error message
      }
      
    }while(check==false);

    cout<<"Username entered: "<<user<<endl;

    //======================================================================

    numb=numb_dummyproof("Enter your Phone number (no spaces only numbers): ");

    bool checknum;// to check if correct number is entered
    do
    {
      checknum=true;
      for (int i=0;i<phone.size();i++)//loop through the phone numbers array
      {
        if (phone[i]==numb) checknum=false; //check if phone number is already used
        
      }
      if (checknum==false) 
      {
        
        numb=numb_dummyproof("Number already used. Try again\nEnter number (no spaces, only numbers): ");
        //error message and input is taken again 
      }
      
    }while(checknum==false);
    
    cout<<"Number entered: "<<numb<<endl;

    //=======================================================================
    
    password=hash_string(pass_dummyproof("Enter your password: "), key_rand); 
    //input's password with dummyproofing and hashes it
    
   // cout<<"password entered: "<<password;

  //================================================
    
    key=hash_key(key_rand);
    //hashed key is also stored
    //cout<<"\nvalue of key position hashed is: "<<key;

    //===========================================
    
    //store user data in the arrays
    username.push_back(user);
    phone.push_back(numb);
    pass.push_back(password);
    keyv.push_back(key);
    update();
    //database updated
    cout<<"\nAccount created successfully. Redirecting to the start\n";
    //sleep(5);
    system("clear");
    return 0;//end program
  }
    
  //=========================
  //==========Login==========
  //=========================
    
  else if (f_input==1)
  {
    system("clear");
    
    string L_username; //get user's input (login_username)
    string L_password; //get user's input (login_password)

    string d_number; //number from database (database_number)
    string d_password; //password from database (database_password)
    int p_key;
    int position_data; // position of array which stores the user's data
  //=======================================
    bool check_user=false;
    int forg;//ask user if they forgot their username
    cout<<"Enter your username: ";
    cin.ignore(100,'\n');
    getline(cin,L_username);
    //input the username

    do
    {
      check_user=false;
      for (int i=0;i<username.size();i++) // loop through every username
      {
        if (username[i]==L_username) //compare the usernames
        {
          check_user=true;
          position_data=i;
          //store the position inside the array
        }
      }
      if (check_user==false)
      {
        cout<<"Wrong Username entered\n ";
        forg=f_dummyproof("Enter:\n[1] If you forgot your Username\n[2] To try again\n");
        cin.ignore(100,'\n');// as cin is used in the f_dummyproof function
        
        if (forg==1)
        {
          Username_forgot();
          //to give the user his username
        }
        cout<<"Enter Username again: ";
        //input the username again just so that Username_forgot() function isn't exploited
        getline(cin,L_username);
        
      }

    }while(check_user==false);

    cout<<"Username entered: "<<L_username<<endl;

  //==============================================================

    //gets all the user's data using the position stored above
    d_number=phone[position_data];
    d_password=pass[position_data];

    p_key=keyposition(position_data);
    //key is in hashed format so keyposition function is used
  //==============================================================

    int limit=0;//so user can only enter the password 3 times
    bool check_pas=false;
    int forg2;//ask user if they fogot their password
    cout<<"Enter your Password: ";
    getline(cin,L_password);
    //input password

    do
    {
      d_password=pass[position_data];
      //update the password stored each time as the password may be changed in the loop
      check_pas=false;
      if (d_password==hash_string(L_password,p_key)) check_pas=true; //compare the passwords

      if (check_pas==false)
      {
        cout<<"Loading...\n";
        //sleep(10);
        //stop program for 10s for security
        system("clear");
        limit++;
        cout<<"Wrong Password entered\n ";
        if (limit==3) 
        {
          cout<<"Due to security reasons, please try later\n";
          return -1;
        }
        forg2=f_dummyproof("Enter:\n[1] If you forgot your Password \n[2] Enter password again\n");
        cin.ignore(100,'\n');// as cin is used
        
        if (forg2==1)
        { 
          
          Password_forgot(position_data, key_rand);
          limit=0;//reset limit as password is changed
            
          update();
          //update the database
          store();
          p_key=keyposition(position_data);  
          //store the key position again as the password is changed
        }
 
        cout<<"Enter Password again: ";
        //ask for the password again
        getline(cin,L_password);
        
      }
      
    }while(check_pas==false);

   // cout<<"Password Entered: "<<L_password<<endl;

    //=================================================
    system("clear");
    int option;
    do
    {
      cout<<"Hey, "<<L_username<<endl<<endl;
      cout<<"What do you want to do\n";
      cout<<"[1] change password\n";
      cout<<"[2] change phone number\n";
      cout<<"[3] Logout\n";
      cin>>option;
      cin.ignore(100,'\n');//cin used again

    switch (option) 
    {
      case 1:
       
        change_password(position_data,key_rand);
        update();
        return 0;
        //end program so the user login's again. Just for security
        break;
      
      case 2:
        change_number(position_data);
        d_number=phone[position_data];
        //store the new number
        break;
      
      case 3:
        cout<<"Logging out...\n";
        //sleep(2);
        system("clear");
        return 0;
        //end program
        break;
      
      default:
        cout << "Invalid input entered, please try again\n";
        //if anything other than 1,2 or 3 is entered
        break;
    }
    //sleep(4);
    system("clear");
    }while(true);
  }
  
}

void store()
{
  string u; // store a username from the file one by one
  string ph; // store a number from the file one by one
  string p; // store a hashed password from the file one by one
  string k; // store a hashed key position from the file one by one
  string empty; // just to check if a space is empty
  ifstream file_in; // read file
  
  file_in.open("database.txt"); //open file

  if (file_in.is_open()) 
  {
    while(!file_in.eof()) //check when file ends by while loop
    {
      getline(file_in,empty);//get a line from the file
      if (empty=="")  getline(file_in,empty); // if it is empty get next line
      else
      {
        u=empty;
        username.push_back(u); // add the username to the array 
      }
      
      getline(file_in,empty);
      if (empty=="")  getline(file_in,empty);
      else
      {
        ph=empty;
        phone.push_back(ph);
      }
  
      getline(file_in,empty);
      if (empty=="")  getline(file_in,empty);
      else
      {
        p=empty;
        pass.push_back(p);
      }
  
      getline(file_in,empty);
      if (empty=="")  getline(file_in,empty);
      else
      {
        k=empty;
        keyv.push_back(k);
      }
      
    }
    file_in.close(); // close file
  }
  else 
  {
    cout<<"Database not found, create a file called 'database.txt'\n";
    exit(1);
  }
  
}

int f_dummyproof(string p)
{
  cout<<p;
  int f;
  cin>>f;
  //cin.ignore(100,'\n');
  while (cin.fail() || f>2 || f<=0) //should be int , less than 3 and more than 0
   {
    cin.clear(); // to remove the flag raised
    cin.ignore(100,'\n'); 
    cout <<"Wrong input. Try again\n";
    cin >>f;//input again
  }
  return f;
}

string user_dummyproof(string a)
{
  cout<<a;
  string user; //to store input from user
  bool check=true;

  do
  {
    check=true;
    //cin.ignore(100,'\n');
    getline(cin,user);

    if (user=="") check=false; //check if user enters nothing
    
    for (int i=0;i<user.length();i++)
    {
      if (user[i] == ' ' || user[i]==9) check=false; 
      //check if user enters a space
      
    }
      
    if (check==false)
    {
      cout<<"Wrong input enter again: ";
    }
    
  }while(check==false);
  
  return user;
}


string numb_dummyproof(string prompt)
{
  string num;
  cout<<prompt;
  bool checknum=true;

  do
  {
    checknum=true; //bool reset value each time
    getline(cin,num); //get whole line

    if (num=="") checknum=false; 
    
    for (int i=0;i<num.length();i++)
    {
      if (num[i] <'0' || num[i]>'9') checknum=false; // if anything except numbers are inputted
    }
      

    if (checknum==false)
    {
      cout<<"Only numbers allowed, enter again: "; // user output
    }
    
  }while(checknum==false);

    
  return num;
}


string pass_dummyproof(string prompt2)
{
  string passw;// to store password
  //cout<<prompt2;
  bool check=true; // boolean check
  
  do
  {
    check=true;
    cout<<prompt2;
    getline(cin,passw); //input from user
    
    if (passw=="") check=false; //check if input is empty
    
    for (int i=0;i<passw.length();i++)
    {
      if (passw[i] == ' ') check=false; //check if input contains spaces
      else if (!isprint(passw[i])) check=false; // check if character is not printable
      
    }
    
    if (check==false)
    {
      cout<<"Invalid Password Entered. Try Again\n";
      //error message
    }
   
  }while(check==false);

  
  return passw;
}


void update()
{
  ofstream file_out;
  file_out.open("database.txt",ofstream::trunc); //open file in write mode
  for (int i=0;i<pass.size();i++) // loop each time till size of array
    //each array should have same size so any array could be used to get the size
  {
    file_out<<username[i]<<endl;  //write to file//
    file_out<<phone[i]<<endl;
    file_out<<pass[i]<<endl;
    file_out<<keyv[i]<<endl;
  }
  file_out.close(); // close file
}


string hash_string(string st, int pos)
{
  int v=0;
  stringstream sw; //to store input as hexadecimal
  string hex_2;
  
  for (int i=0;i<st.length();i++)
  {
    v=v+st[i];
  }
  v = v*keynum[pos]; // multiply integer value with a constant number 
  sw<< setbase(16) << v; // store as hexadecimal
  hex_2 =sw.str(); //change to string

  return hex_2;
    
}


string hash_key(int key_position)
{

  key_position = key_position * 79;

  stringstream key_ss;
  
  key_ss<< setbase(16) << key_position;  // to store as hexadecimal in stringstream
  string keyh =key_ss.str(); //to change value to string and store it

  return keyh;
}

/////////////////////////////////////////////////////
void Username_forgot()
{
  string tempnum;// to store the phone number entered by user
  bool check_tempnum;// check to see if phone number is in the database
  int position; //position of the phone number in the vector
  cout<<"Enter you Phone number registered with your account\n";
  getline(cin,tempnum);// input number

  do
  {
    check_tempnum=false; //reset it during each loop
    for (int i=0;i<phone.size();i++) 
    {
      if (phone[i]==tempnum) 
      {
        check_tempnum=true;
        position=i;//get position in array
      }
    }
    if (check_tempnum==false)
    {
      cout<<"Wrong number entered. Try again\n"; 
      getline(cin,tempnum);//asking the user to input it again
    }
    
  }while(check_tempnum==false);//continue until the correct number is entered

  cout<<"Your Username is: "<<username[position]<<" .Please input it again when asked!\n";
}

int keyposition(int pos)
{
  int keypos;

  for (int i=0;i<5;i++)//loop 5 times as there are 5 key values
  {
    if (keyv[pos]==hash_key(i)) keypos=i; 
    //check if both hashed values are the same 
  }
  return keypos;
  
}

void Password_forgot(int position, int key)
{
  string num; //stores the phone number
  bool check;
  cout<<"Please confirm your phone number to reset your password\n";
  getline(cin,num);//input number

  //loop to check if correct number is entered
  do
  {
    check=false;
    if (num==phone[position]) check=true;

    if (check==false)//output a error message if wrong number is entered
    {
      cout<<"Wrong phone number entered.\nEnter your Number again: ";
      getline(cin,num);
      
    }
  }while(check==false);
  cout<<"\nIMPORTANT!\nRealistically your number would have been verified by sending you a message. But that isn't included in this code. So I'm trusting you with the number entered...\n";
  //sleep(5);
  system("clear");

  change_password(position,key);// call the change password function
 
}

void change_password(int pos, int randk)
{
  string pass1; //store new password
  string pass2; // stores the inputted password to see if they match
  bool checkpass;
  do // loop to check if both inputs are the same
  {
    
    checkpass=false;
    pass1=hash_string(pass_dummyproof("Enter new password: "), randk);// take input and hash them using the key 
    pass2=hash_string(pass_dummyproof("Enter password again: "), randk);// take inout and hash it again

    if (pass1==pass2) checkpass=true;//check if they are the same

    else if (pass1!=pass2) cout<<"Passwords do not match. Enter the passwords again\n";
    //error message if they aren't
    
  }while(checkpass==false);

  pass[pos]=pass1;
  //store password in the vector
  keyv[pos]=hash_key(randk);
  //store key in vector in it's hashed form

  cout<<"\nPassword changed successfully, Please Log in again\n";
  //sleep(4);
  system("clear");
}

void change_number(int position)
{
  system("clear");
  bool check;
  string num1;
  string num2;

  do
  {
    check=true;
    num1= numb_dummyproof("Enter the new number: ");
    num2= numb_dummyproof("Enter the number again: ");

    if (num1!=num2) 
    {
      check=false;      
    }

    else //if (num1==num2)
    {
      for (int i=0;i<phone.size();i++)
      {
        if (phone[i]==num1) check=false; //check if phone number is already used
      }
      
    }
    if (check==false)
    {
      cout<<"Phone number already used or number do not match, please try again\n";
      //error message
    }
    
  }while(check==false);

  phone[position]=num1;
  //update the vector and update the database
  update();
  cout<<"Phone number changed succesfully.\n";
}
