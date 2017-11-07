/* 
 * File:   main.cpp
 * Author: Trey.Dalziel
 *
 * Created on October 31, 2017, 11:51 AM
 */

/*
 * Hangman game for one or more players. The game prompts user for how many
 * people want to play (one or more players.) If one person, the game will choose
 * a random word from the words.txt file. If more than one person, the game will ask for 
 * a word to guess.
 * 
 * One player: A random word is chosen from a file containing many words. The 
 * chosen word is passed in from the file and "serialized" out with
 * blanks to be displayed on screen. The player is then prompted a letter to 
 * guess. If the letter is correct, the letter is displayed in the word. If the 
 * letter is incorrect, the number of guesses left is subtracted by one and a 
 * body part is drawn.
 * There should be six body parts total, resulting in a total of six guesses. 
 * 
 * Two or more players: Players are asked to enter a word, then the game
 * is played the same as a 'One Player' game.
 */

/*
 * Version History
 * 
 * 1.0 10/31/17
 * -Initial hangman game
 * 
 * 1.1 11/6/17
 * -Bug fixes and handling of 'serializing' the word improved
 * 
 * 1.2 11/7/17
 * -Better handling on input, accounted for upper/lower case input
 */
 
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <ctime>

using namespace std;
const int GLOBAL_CONST_GUESSES = 6; //one guess per body part

bool mainMenu();
void drawNoose(int guesses, string& hiddenWord, vector<char> guessed);
string getWord();
string fetchWord();
string serializeWord(string& word);
bool checkLetter(char input, string& word, int& numCorrect);
string fillLetter(string& hiddenWord, string& word, char input);
char getInput();
bool didYaWinYet(int wordLen, int& numCorrect);
void playAgain();

int main(int argc, char** argv) {

    bool singlePlayer; //used to determine if single or multiplayer
    string word; //the word to guess
    string hiddenWord; //the word converted to blanks ("serialized")
    int guesses = GLOBAL_CONST_GUESSES; //guesses remaining
    int wordLen; //word length
    int numCorrect = 0; //if wordLen==numCorrect, winner
    char input; //letter being guessed by player
    vector<char> guessed;
    bool exists = false;

    //prompt one or more players
    singlePlayer = mainMenu();
    if (singlePlayer) {
        //retrieve word from file
        word = fetchWord();
    } else {
        //get user entered word to guess
        word = getWord();
    }
    //determine length of word
    wordLen = word.length();
    //serialize word (blanks)
    hiddenWord = serializeWord(word);
    
    //begin guessing
    do {
        drawNoose(guesses, hiddenWord, guessed);   
        if(guesses > 0){
            do{
                exists = false;
                input = getInput();
                for(int i = 0;i < guessed.size(); i++){
                    if(guessed[i]==input){
                        cout<<"You already guessed '" << input << "'!" <<endl;
                        exists = true;
                    }
                }
            }while(exists);
            guessed.push_back(input);
        }
        bool found = checkLetter(input, word, numCorrect);
        
        if (found) {
            //fill in letter in blanks
            hiddenWord = fillLetter(hiddenWord, word, input);
        } else {
            //incorrect guess
            guesses -= 1;
        }
        if(didYaWinYet(wordLen,numCorrect)){
            drawNoose(guesses,hiddenWord,guessed);
            cout << "\n\t**************************************************************\n"
                 << "\t|                         WINNER                             |\n"
                 << "\t**************************************************************\n\n"
                 << "\n\t\t*The word was: " << word
                 << "\n\n\t\t*************************************\n"
                 << "\t\tEnter 1 to play again\n"
                 << "\t\tEnter any other key to quit...\n";
                char play;
                cin >> play;
                if(play=='1')
                    playAgain();
                return 0;
        }
    } while (guesses >= 0);
    cout << "\n\t\t*The word was: " << word << endl << endl
         << "\t\t*************************************\n"
         << "\t\tEnter 1 to play again\n"
         << "\t\tEnter any other key to quit..." << endl;
    char play;
    cin >> play;
    if(play=='1')
        playAgain();
    
    return 0;
}
/* Main menu 'skulls' picture taken from: 
 * http://www.chris.com/ascii/index.php?art=holiday/halloween
 * done by artist 'jgs'
 */
bool mainMenu(){
    char playerSelection;
    //used to clear the screen every time
    cout << "\033[2J\033[1;1H";
    cout << "    (                                (" << endl
            << "        )           )        (                   )" << endl
            << "     (                       )      )            .---." << endl
            << " )              (     .----.       (        (   /     \\ " << endl
            << "( .-''-.  (      )   / _  _ \\        )       )  |() ()|" << endl
            << " / _  _ \\   )        |(_\\/_)|  .---.   (        (_ 0 _)" << endl
            << " |(_)(_)|  ( .---.   (_ /\\ _) /     \\    .-\"\"-.  |xxx|" << endl
            << " (_ /\\ _)   /     \\   |v==v|  |<\\ />|   / _  _ \\ '---'" << endl
            << "  |wwww|    |(\\ /)|(  '-..-'  (_ A _)   |/_)(_\\|" << endl
            << "  '-..-'    (_ o _)  )  .---.  |===|    (_ /\\ _)" << endl
            << "             |===|  (  /     \\ '---'     |mmmm|" << endl
            << "             '---'     |{\\ /}|           '-..-'" << endl
            << "                       (_ V _)" << endl
            << "                        |\"\"\"|" << endl
            << "                        '---' " << endl
            << "**************************************************************\n"
            << "|                       HANGMAN v1.2                         |\n"
            << "**************************************************************\n\n"
            << "SELECT PLAYERS:    (A)Singleplayer      (B)Multiplayer\n";
    do{
        cout << "------------->";
        cin >> playerSelection;
        playerSelection = toupper(playerSelection);
        
        if(playerSelection !='A' && playerSelection !='B')
            cout << "Please enter a valid input." << endl;
 
    }while(playerSelection !='A' && playerSelection !='B');
    
    if (playerSelection == 'A')
        return true;
    else if (playerSelection == 'B')
        return false;
}

void drawNoose(int guesses, string& hiddenWord, vector<char> guessed) {
    //used to clear the console
    cout << "\033[2J\033[1;1H";
    if(guesses == 6){
        cout << "\n                       ___________\n"
             << "                       | \n"
             << "                       | \n"
             << "                       | \n"
             << "                       | \n"
             << "                       | \n"
             << "                       | \n"
             << "                       | \n\n";
    }
    else if(guesses == 5){
        cout << "\n                       ___________\n"
             << "                       |         |\n"
             << "                       |         0\n"
             << "                       | \n"
             << "                       | \n"
             << "                       | \n"
             << "                       | \n\n";
    }
    else if(guesses == 4){
        cout << "\n                       ___________\n"
             << "                       |         |\n"
             << "                       |         0\n"
             << "                       |         |\n"
             << "                       | \n"
             << "                       | \n"
             << "                       | \n\n";
    }
    else if(guesses == 3){
        cout << "\n                       ___________\n"
             << "                       |         |\n"
             << "                       |         0\n"
             << "                       |        /|\n"
             << "                       | \n"
             << "                       | \n"
             << "                       | \n\n";
    }
    else if(guesses == 2){
        cout << "\n                       ___________\n"
             << "                       |         |\n"
             << "                       |         0\n"
             << "                       |        /|\\\n"
             << "                       | \n"
             << "                       | \n"
             << "                       | \n\n";
    }
    else if(guesses == 1){
        cout << "\n                       ___________\n"
             << "                       |         |\n"
             << "                       |         0\n"
             << "                       |        /|\\\n"
             << "                       |        /\n"
             << "                       | \n"
             << "                       | \n\n";
    }
    else if(guesses == 0){
        cout << "\n                       ___________\n"
             << "                       |         |\n"
             << "                       |         0\n"
             << "                       |        /|\\\n"
             << "                       |        / \\\n"
             << "                       | \n"
             << "                       | GAME OVER\n\n";
    }
    
    cout << setw(20) << "Guess:  "; 
    for(int i = 0; i < hiddenWord.size(); i++){
        cout << hiddenWord[i] << " ";
    }
    cout << endl << endl;
    cout << setw(20) << "Guessed: ";
    for(int i = 0; i < guessed.size(); i++){
        cout << guessed[i];
    }
    cout << endl;
}

string getWord() {
    string word;
    cout << setw(20) << "Enter a single word to guess: ";
    cin >> word;
    return word;
}
string fetchWord(){
      vector<string> words;
      //reserve space for words to be read in (currently using 400k+)
      words.reserve(600000);
      ifstream file;
      file.open("words.txt");
      string line;
      
      while(file >> line){ 
          words.push_back(line);
      }
      file.close();
      srand(time(NULL));
      int index = (rand()%words.size());
      string word = words[index];
      
      return word;
}
string serializeWord(string& word){
    string hiddenWord = word;
  
    for(int i = 0; i < hiddenWord.length(); i++){
        hiddenWord[i] = '_';
    }
    return hiddenWord;
}
bool checkLetter(char input, string& word,int& numCorrect){
    bool found = false;
    
    for(int i = 0; i < word.length(); i++){
        if(input == word[i] || toupper(input) == word[i]){
            numCorrect++;
            found = true;
        }
    }
    return found;
}
string fillLetter(string& hiddenWord, string& word, char input){
    int loc;
    vector<int> indexes;
      
    for(int i = 0; i < word.length(); i++){
        if(word[i]==input || word[i]==toupper(input))
            indexes.push_back(i);   
    }
   
    for(int i = 0; i < indexes.size(); i++){
          loc = indexes.at(i);
          hiddenWord[loc] = input;
      }
    
    indexes.clear();
    return hiddenWord;
}
char getInput(){
    char letter;
    string input;
    cout << "\nEnter letter to guess: ";
    cin >> input;
    letter = input[0];
    
    return tolower(letter);
}
bool didYaWinYet(int wordLen, int& numCorrect){
    if(numCorrect == wordLen){
        return true;
    }
    return false;
}
void playAgain(){
    main(0,NULL);
}