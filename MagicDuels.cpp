// ==========================================
// Program name:  Magic Duel Game
// ==========================================
// Written by:  Matthew Hamilton
// ==========================================
// For:         CS 110
//              Assignment 6
// Date:        November, 24, 2015
// ==========================================
// Purpose:     To create a fighting game (Player vs. Computer)
// Problem:     Use arrays to store max and min values of tools (magic) and create a fighting game,
//              using arrays, functions, and other previous code learned
// Algorithm / process:
//			   Program begins,
//                  (prompts user for difficulty,
//					user turn begins, magic chosen, deals random damage to computer, 
//					computer turn beings, random magic chosen, deals random damage to user,
//					displays current round information,) * However many rounds until someone wins or loses
//			   duel ends, prompts user to continue, ends.
// Major Variables:
//     int userHealth, compHealth, userMagic, compMagic, userDamageDealt, compDamageDealt
//     int[] userMinDamage, userMaxDamage, compMinDamage, compMaxDamage		Note: Needed to make a user and computer damage so the computer didn't increase magic damage along with the user
//     string[] magicName
// ==========================================
// TestCase Explanations:
// LogFile - displays an example of a duel where the player wins, and then plays another, the "leveling system", magic boundaries, different difficulties
// scrn1 - part1: displays an example different magic being used, difficulty boundaries
// scrn2 - part2: more of the game rounds, magic boundaries displayed on round 6
// scrn3 - part3: extra level up system for magic, losing and exiting the game.
// ==========================================
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void DoUserTurn(int[], int[], int&, int&);					//Initiates Users turn
void DoComputerTurn(int[], int[], int&, int&);				//Initiates Computers turn
void DisplayInfo(string[], string, int, int, int[], int[], int[], int[]); //Displays round and duel over information
void DisplayLevelMessages();								//Checks if the user leveled up and displays appropriate message
void PromptUser(int&, int&, int[], int[]);					//Prompts user with magic choices
bool Winner();												//Checks if the user or computer won the duel
int RandomBetween(int, int);

/* All of these global variables are used a lot througout the program, and are not harmful for this program to set them as a global variable,
   and it cleans up the amount of variables needed to be passed to functions*/

int const gNumAtt = 6;			//Number of attacks the arrays will hold

int gLevel = 1;					//Level based off wins or tie in the rare case
bool gAlreadyLevel2 = false;    //Disallows a message customized for level 2 if the user has already been level 2

int userHealth;					//Always 100 health for user
int userMagic = 0;				//Users array selection of magic
int compHealth;					//Computers health changes depending on difficulty
int compMagic = 0;				//Computers array selection of magic

int gUserWeaponLevel[gNumAtt] = { 1, 1, 1, 1, 1, 1 }; //Current weapon user weapon level
int gUserWeaponExp[gNumAtt] = { 0, 0, 0, 0, 0, 0 };
int gCompWeaponLevel[gNumAtt] = { 1, 1, 1, 1, 1, 1 };
int gCompWeaponExp[gNumAtt] = { 0, 0, 0, 0, 0, 0 };

ofstream myLogFile;

int main()
{
	string userContinue;    //Users choice about playing again or quitting.
	string compName;		//Computers name varies depending on difficulty
	int difficulty = 1;		//Difficulty changes Computers health
	int userDamageDealt = 0;//User damage dealt the current round
	int compDamageDealt = 0;//Computer damage dealt the current round
	int round;				//Current round
	int drainCap = 3;		//Total amount of uses for the lvl 2+ magic spell Drain
	int compDrainCap = 3;   //Total amount of uses for the computers magic spell Drain
	
	int userMinDamage[gNumAtt] = {3, 5, 2, 0, 4, 2};		//Users minimum magic damage
	int userMaxDamage[gNumAtt] = {10, 8, 12, 15, 9, 20};	//Users minimum magic damage
	int compMinDamage[gNumAtt] = { 3, 5, 2, 0, 4, 2 };		//Computers minimum magic damage
	int compMaxDamage[gNumAtt] = { 10, 8, 12, 15, 9, 20 };	//Computers maximum magic damage
	string magicName[gNumAtt] = { "Fireball", "Ice-lance", "Sand-Slash", "Hurricane", "Tsunami", "Drain"}; //Magic names

	srand((unsigned int)time(NULL)); //Random seed for rand()
	myLogFile.open("A6out_200361163.txt"); //Output file opened/created

	cout << "======= Welcome to the Magic Duels game =======\n";
	myLogFile << "======= Welcome to the Magic Duels game =======\n";

	myLogFile << "Magic at Base Damage: \n [1] Fireball (3 to 10 damage)\n [2] Ice-Lance (5 to 8 damage)\n [3] Sand-Slash (2 to 12 damage)\n";
	myLogFile << " [4] Hurricane (0 to 15 damage)\n [5] Tsunami (4 to 9 damage)\n [6] Drain (2 to 20 health restored) //Can only be used when user is level 2+\n\n";
	myLogFile << "==Duel==\n" << endl;
	

	//Loop for replaying the game
	do{
		userHealth = 100; //Duel always begins with the users health at 100
		round = 1;		  //Duel always begins with the round 1

		do{
			//Prompts for difficulty
			cout << "Choose your difficulty (1-3): ";
			cin >> difficulty;

			if (difficulty == 1){
				compHealth = 100;
				compName = "Nova";
				cout << "Your opponent is Nova a Begginer Wizard! Have fun!\n";
				myLogFile << "Difficulty chosen: " << difficulty << endl;
			}
			else if (difficulty == 2){
				compHealth = 110;
				compName = "Mavis";
				cout << "Your opponent is Mavis, a Apprentice Wizard! Let's begin!\n";
				myLogFile << "Difficulty chosen: " << difficulty << endl;
			}
			else if (difficulty == 3){
				compHealth = 125;
				compName = "Gandalf";
				cout << "Your opponent is Gandalf a Legendary Wizard! Good luck!\n";
				myLogFile << "Difficulty chosen: " << difficulty << endl;
			}
			else{
				cout << "Please re-enter a difficulty between 1 and 3\n\n";
			}

		} while (difficulty < 1 || difficulty > 3); //Loops until difficulty is between 1 and 3

		//Main Loop
		while (!Winner()){ //Checks if there is the duel has been won

			cout << "\n==Round " << round << "==" << endl; //Displays current round
			myLogFile << "\nRound: " << round << endl;

			DoUserTurn(userMinDamage, userMaxDamage, userDamageDealt, drainCap);			//Calls function DoUserTurn
			DoComputerTurn(userMinDamage, userMaxDamage, compDamageDealt, compDrainCap);	//Calls function DoComputerTurn
			DisplayInfo(magicName, compName, userDamageDealt, compDamageDealt, userMinDamage, userMaxDamage, compMinDamage, compMaxDamage); //Calls function DisplayInfo
			
			
			if (Winner()){	  //After the info is displayed checks if the user wants to duel again
				drainCap = 3; //Duel ended resets drainCap to 3
				compDrainCap = 3;

				//Loops if not yes or no answer
				do {
					cout << "Do you wish to fight again? (yes/no)\n";
					cin >> userContinue;

					for (int i = 0; userContinue[i]; i++){
						userContinue[i] = tolower(userContinue[i]);
					}

					if (!(userContinue == "yes" || userContinue == "no")){
						cout << "Please re-enter you choice." << endl;
						continue;
					}

					break;

				} while (true); //Won't exit until a break is reached
			}
			else {
				round++; //Round increments if no winner yet
			}

		}

		if (userContinue == "yes"){
			myLogFile << "\n==New Duel== " << endl;
		}

	} while (userContinue == "yes"); //When the user enters yes the game will run again


	return 0;
}

/**************************** RandomBetween ******************************
Chooses a random number between a the lowest and highest number supplied.
*************************************************************************/
int RandomBetween(int low, int high){
/* PRE-CONDITION: int low: must have an integer value assigned, and be <= high
				  int high: must have an integer value assigned, and be >= high
  POST-CONDITION: int damage is returned, a number between low and high
*/

	int damage = rand() % (high - low) + low;

	return damage;
}

/**************************** PromptUser ******************************
User is prompted to select a magic spell, the information on the amount of, 
and damage of the spells depends on their level.
***********************************************************************/
void PromptUser(int& magicChoice, int& drainCap, int uMinDmg[], int uMaxDmg[]){
/* PRE-CONDITION: int draincap: must have an integer value assigned
				  int uMinDmg[], uMaxDmg[]: must have an integer value >= 0 assigned
   POST-CONDITION: the user is prompted to choose a magic spell, 
   and if magicChoice is 0 it displays information on the spells using drainCap,
   minDmg and maxDmg
*/
	//If the users current level is 1 or drain has been used 3 times
	if (gLevel == 1 || (drainCap <= 0)){
		
		do{
			cout << "Which magic would you like to use? (1-5, 0 to list) -> ";
			cin >> magicChoice;
			magicChoice--; //Decrements magic choice to use the correct array value

			if (magicChoice == -1){
				cout << "[1] [Lvl." << gUserWeaponLevel[0] << "] Fireball ( " << uMinDmg[0] << " to " << uMaxDmg[0] << " damage)\n";
				cout << "[2] [Lvl." << gUserWeaponLevel[1] << "] Ice-Lance ( " << uMinDmg[1] << " to " << uMaxDmg[1] << " damage)\n";
				cout << "[3] [Lvl." << gUserWeaponLevel[2] << "] Sand-Slash ( " << uMinDmg[2] << " to " << uMaxDmg[2] << " damage)\n";
				cout << "[4] [Lvl." << gUserWeaponLevel[3] << "] Hurricane ( " << uMinDmg[3] << " to " << uMaxDmg[3] << " damage)\n";
				cout << "[5] [Lvl." << gUserWeaponLevel[4] << "] Tsunami ( " << uMinDmg[4] << " to " << uMaxDmg[4] << " damage)\n";
			}

		} while (magicChoice < 0 || magicChoice > 4); //Will keep prompting until a number between the selection 1 and 5 is entered

	}
	else if (gLevel > 1 && (drainCap > 0)){ //If the users current level is greater than 1 and drain has been used less than 3 times
		 
		do {
			cout << "Which magic would you like to use? (1-6, 0 to list) -> ";
			cin >> magicChoice;
			magicChoice--; // Decrements magic choice to use the correct array value

			if (magicChoice == -1){
				cout << "[1] [Lvl." << gUserWeaponLevel[0] << "] Fireball ( " << uMinDmg[0] << " to " << uMaxDmg[0] << " damage)\n";
				cout << "[2] [Lvl." << gUserWeaponLevel[1] << "] Ice-Lance ( " << uMinDmg[1] << " to " << uMaxDmg[1] << " damage)\n";
				cout << "[3] [Lvl." << gUserWeaponLevel[2] << "] Sand-Slash ( " << uMinDmg[2] << " to " << uMaxDmg[2] << " damage)\n";
				cout << "[4] [Lvl." << gUserWeaponLevel[3] << "] Hurricane ( " << uMinDmg[3] << " to " << uMaxDmg[3] << " damage)\n";
				cout << "[5] [Lvl." << gUserWeaponLevel[4] << "] Tsunami ( " << uMinDmg[4] << " to " << uMaxDmg[4] << " damage)\n";
				cout << "[6] [Lvl." << gUserWeaponLevel[4] << "] Drain ( " << uMinDmg[5] << " to " << uMaxDmg[5] << " health restored) Note: " << drainCap << " use(s) remain for current duel.";
			}

			if (magicChoice == 5){
				drainCap--;
			}

		} while (magicChoice < 0 || magicChoice > 5); //Will keep prompting until a number between the selection 1 and 6 is entered

	}

}

/**************************** DoUserTurn *******************************
Users turn begins by prompting to select a magic spell, and depending
on the users level and random chance it will either deal damage, no damage,
or heal the user, and grant experience to the magic spell used.
***********************************************************************/
void DoUserTurn(int uMinDmg[], int uMaxDmg[], int& uDamage, int& drainCap){ //uDamage returns the damage dealt, and uDrainCap is the uses left on drain
/* PRE-CONDITION: int uMinDmg[], uMaxDmg[]: must have an integer value >= 0 assigned
   POST-CONDITION: The user is prompted to enter a spell, either deals damage or no damage
   to the enemy, or heals the user, and grants experience to the magic spell used
*/

	PromptUser(userMagic, drainCap, uMinDmg, uMaxDmg); //Calls function PromptUser
	uDamage = RandomBetween(uMinDmg[userMagic], uMaxDmg[userMagic]);  //Calls function RandomBetween and sets uDamage to that random number which is damage or health
	
	//If it is not the healing spell "Drain" do damage
	if (userMagic != 5){
		compHealth -= uDamage;
	}
	else {
		userHealth += uDamage;
	}

	//Grants exp to the magic used
	if (userMagic == 0){
		gUserWeaponExp[0]++;
	}
	else if (userMagic == 1){
		gUserWeaponExp[1]++;
	}
	else if (userMagic == 2){
		gUserWeaponExp[2]++;
	}
	else if (userMagic == 3){
		gUserWeaponExp[3]++;
	}
	else if (userMagic == 4){
		gUserWeaponExp[4]++;
	}
	else {
		gUserWeaponExp[5]++;
	}

}

/**************************** DoComputerTurn ***************************
Computers turn begins selects a random magic spell, and depending
on the computers level and random chance it will either deal damage, no damage,
or heal itself, and grant experience to the magic spell selected.
***********************************************************************/
void DoComputerTurn(int cMinDmg[], int cMaxDmg[], int& cDamage, int& cDrainCap){ //cDamage returns the damage dealt, and cDrainCap is the uses left on drain
/*  PRE-CONDITION: int cMinDmg[], cMaxDmg[]: must have an integer value >= 0 assigned
	POST-CONDITION: A random spell is selected, either deals damage or no damage
	to the enemy, or heals the computer, and grants experience to the magic spell selected
*/

	//Selects either from 5 or 6 spells depending on user's level
	if (gLevel == 1 || cDrainCap <= 0){
		compMagic = rand() % 5;
	}
	else{
		compMagic = rand() % 6;
		if (compMagic == 5){
			cDrainCap--;
		}
	}

	cDamage = RandomBetween(cMinDmg[compMagic], cMaxDmg[compMagic]); //Calls function RandomBetween and sets cDamage to that random number which is damage or health
	
	//If it is not the healing spell "Drain" do damage
	if (compMagic != 5){
		userHealth -= cDamage;
	}
	else {
		compHealth += cDamage;
	}

	//Grants exp to the magic used
	if (compMagic == 0){
		gCompWeaponExp[0]++;
	}
	else if (compMagic == 1){
		gCompWeaponExp[1]++;
	}
	else if (compMagic == 2){
		gCompWeaponExp[2]++;
	}
	else if (compMagic == 3){
		gCompWeaponExp[3]++;
	}
	else if (compMagic == 4){
		gCompWeaponExp[4]++;
	}
	else {
		gCompWeaponExp[5]++;
	}

}

/**************************** DisplayInfo *****************************
Displays the current round information, and if the duel is over it displays
the winner information.
***********************************************************************/
void DisplayInfo(string mgcName[], string cName, int uDamageDealt, int cDamageDealt, int uMinDmg[], int uMaxDmg[], int cMinDmg[], int cMaxDmg[]){
/*  PRE-CONDITION: string mgcName[]: must have the weapon names assigned.
				   string cName: must have a computer name assigned
				   int uDamageDealt, cDamageDealt: must have an integer value assigned
				   int uMinDmg[], uMaxDmg[], cMinDmg[], cMaxDmg[]: must have an integer value >= 0 assigned
	POST-CONDITION: The round information including what spells and the damage dealt by whom is displayed,
					who is winning, any user or weapon level increase, and if the duel is over or not
	*/

	//If the user didn't pick the healing spell "Drain" display damage dealt
	if (userMagic != 5){
		cout << "You chose " << mgcName[userMagic] << ". It dealt " << uDamageDealt << " damage.\n";
		myLogFile << "Player selected " << mgcName[userMagic] << " dealing " << uDamageDealt << " damage.\n";
		
		//Displays level status of magic, and increases it, if it levels up
		if (userMagic == 0 && (gUserWeaponExp[0] == 5 || gUserWeaponExp[0] == 20 || gUserWeaponExp[0] == 50 || gUserWeaponExp[0] == 100)){
			gUserWeaponLevel[0]++;
			cout << "\n[You leveled up Fireball to level " << gUserWeaponLevel[0] << "!]\n\n";
			uMinDmg[0] += 3;
			uMaxDmg[0] += 3;
		}
		else if (userMagic == 1 && (gUserWeaponExp[1] == 5 || gUserWeaponExp[1] == 20 || gUserWeaponExp[1] == 50 || gUserWeaponExp[1] == 100)){
			gUserWeaponLevel[1]++;
			cout << "\n[You leveled up Ice-Lance to level " << gUserWeaponLevel[1] << "!]\n\n";
			uMaxDmg[1] += 4;
		}
		else if (userMagic == 2 && (gUserWeaponExp[2] == 5 || gUserWeaponExp[2] == 20 || gUserWeaponExp[2] == 50 || gUserWeaponExp[2] == 100)){
			gUserWeaponLevel[2]++;
			cout << "\n[You leveled up Sand-Slash to level " << gUserWeaponLevel[2] << "!]\n\n";
			uMinDmg[2] += 4;
			uMaxDmg[2] += 4;
		}
		else if (userMagic == 3 && (gUserWeaponExp[3] == 5 || gUserWeaponExp[3] == 20 || gUserWeaponExp[3] == 50 || gUserWeaponExp[3] == 100)){
			gUserWeaponLevel[3]++;
			cout << "\n[You leveled up Hurricane to level " << gUserWeaponLevel[3] << "!]\n\n";
			uMinDmg[3] += 2;
			uMaxDmg[3] += 2;
		}
		else if (userMagic == 4 && (gUserWeaponExp[4] == 5 || gUserWeaponExp[4] == 20 || gUserWeaponExp[4] == 50 || gUserWeaponExp[4] == 100)){
			gUserWeaponLevel[4]++;
			cout << "\n[You leveled up Tsunami to level " << gUserWeaponLevel[4] << "!]\n\n";
			uMinDmg[4] += 3;
			uMaxDmg[4] += 3;
		}
		else if (userMagic == 5 && (gUserWeaponExp[5] == 5 || gUserWeaponExp[5] == 20 || gUserWeaponExp[5] == 50 || gUserWeaponExp[5] == 100)){
			gUserWeaponLevel[5]++;
			cout << "\n[You leveled up Drain to level " << gUserWeaponLevel[5] << "!]\n\n";
			uMinDmg[5] += 2;
			uMaxDmg[5] += 4;
		}
	}
	else{
		cout << "You chose " << mgcName[userMagic] << ". It healed for " << uDamageDealt << " health.\n";
		myLogFile << "Player selected " << mgcName[userMagic] << "healing " << uDamageDealt << " health.\n";
		
		 if (userMagic == 5 && (gUserWeaponExp[5] == 5 || gUserWeaponExp[5] == 20 || gUserWeaponExp[5] == 50 || gUserWeaponExp[5] == 100)){
			gUserWeaponLevel[5]++;
			cout << "\n[You leveled up Drain to level " << gUserWeaponLevel[5] << "!]\n\n";
			uMinDmg[5] += 2;
			uMaxDmg[5] += 4;
		}
	}
	
	//If the computer didn't pick the healing spell "Drain" display damage dealt
	if (compMagic != 5){
		cout << cName << " chose " << mgcName[compMagic] << ". It dealt " << cDamageDealt << " damage.\n";
		myLogFile << "Computer selected " << mgcName[compMagic] << " dealing " << cDamageDealt << " damage.\n";

		//Increases the damage of the computers spells if it they level up
		if (compMagic == 0 && (gCompWeaponExp[0] == 5 || gCompWeaponExp[0] == 20 || gCompWeaponExp[0] == 50 || gCompWeaponExp[0] == 100)){
			gCompWeaponLevel[0]++;
			cMinDmg[0] += 3;
			cMaxDmg[0] += 3;
		}
		else if (compMagic == 1 && (gCompWeaponExp[1] == 5 || gCompWeaponExp[1] == 20 || gCompWeaponExp[1] == 50 || gCompWeaponExp[1] == 100)){
			gCompWeaponLevel[1]++;
			cMaxDmg[1] += 4;
		}
		else if (compMagic == 2 && (gCompWeaponExp[2] == 5 || gCompWeaponExp[2] == 20 || gCompWeaponExp[2] == 50 || gCompWeaponExp[2] == 100)){
			gCompWeaponLevel[2]++;
			cMinDmg[2] += 4;
			cMaxDmg[2] += 4;
		}
		else if (compMagic == 3 && (gCompWeaponExp[3] == 5 || gCompWeaponExp[3] == 20 || gCompWeaponExp[3] == 50 || gCompWeaponExp[3] == 100)){
			gCompWeaponLevel[3]++;
			cMinDmg[3] += 2;
			cMaxDmg[3] += 2;
		}
		else if (compMagic == 4 && (gCompWeaponExp[4] == 5 || gCompWeaponExp[4] == 20 || gCompWeaponExp[4] == 50 || gCompWeaponExp[4] == 100)){
			gCompWeaponLevel[4]++;
			cMinDmg[4] += 3;
			cMaxDmg[4] += 3;
		}
		else if (compMagic == 5 && (gCompWeaponExp[5] == 5 || gCompWeaponExp[5] == 20 || gCompWeaponExp[5] == 50 || gCompWeaponExp[5] == 100)){
			gCompWeaponLevel[5]++;
			cMinDmg[5] += 2;
			cMaxDmg[5] += 4;
		}
	}
	else{
		cout << cName << " chose " << mgcName[compMagic] << ". It healed for " << cDamageDealt << " health.\n";
		myLogFile << "Computer selected " << mgcName[compMagic] << " healing " << cDamageDealt << " health.\n";
		
		if (compMagic == 5 && (gCompWeaponExp[5] == 5 || gCompWeaponExp[5] == 20 || gCompWeaponExp[5] == 50 || gCompWeaponExp[5] == 100)){
			gCompWeaponLevel[5]++;
			cMinDmg[5] += 2;
			cMaxDmg[5] += 4;
		}
	}

	//Displays the user and computers health
	cout << "Your health is " << userHealth << ", and " << cName << " has " << compHealth << " health.\n";
	myLogFile << "Player's health is " << userHealth << ", and the Computer has " << compHealth << " health.\n";

	//Displys who is winning or if they won
	if ((userHealth <= 0) && (userHealth < compHealth)){

		cout << cName << " won!" << " Rekt m8.\n";
		myLogFile << "The Computer won!\n";
		
		DisplayLevelMessages(); //Calls DisplayLevelMessages as the duel is over

	}
	else if ((compHealth <= 0) && (compHealth < userHealth)){

		cout << "You won! Your the best wizard 10/10!\n";
		myLogFile << "Player won!\n";
		gLevel++; //Win so level up
		if (gLevel != 2){
			myLogFile << "Player leveled up to level " << gLevel << "!\n";
			cout << "Congratulations you leveled up!\n";
		}
		
		DisplayLevelMessages(); //Calls DisplayLevelMessages as the duel is over

	}
	else if (compHealth < userHealth){

		cout << "You are winning!\n";

	}
	else if (userHealth < compHealth){

		cout << cName << " is winning!\n";

	}
	else if ((userHealth > 0) && (compHealth > 0) && (userHealth == compHealth)){
		cout << "Stalemate!\n";
	}
	else{
		cout << "A draw woah.. that was intense!\n";
		myLogFile << "A draw, nobody won!\n";
		gLevel++; //Tie so level up
		if (gLevel != 2){
			cout << "Congratulations you leveled up!\n";
			myLogFile << "Player leveled up to level " << gLevel << "!\n";
		}

		DisplayLevelMessages(); //Calls DisplayLevelMessages as the duel is over

	}

}

/**************************** DisplayLevelMessages *********************
Displays a message to the user if they leveled up, and if they didn't
displays their current level.
***********************************************************************/
void DisplayLevelMessages(){

	if ((gLevel == 2) && (gAlreadyLevel2 == false)){
		cout << "Congratulations you leveled up! You now have another spell to use called Drain!\n";
		cout << "Current Level:" << gLevel << endl;
		myLogFile << "Player leveled up to level " << gLevel << ", and can now use Drain!\n";
		gAlreadyLevel2 = true;
	}
	else{
		cout << "Current Level:" << gLevel << endl;
	}
}

/**************************** Winner ****************************
Checks if their is a winner, if anyone's health is equal to or
below 0 returns true if not false
****************************************************************/
bool Winner(){

	if ((userHealth <= 0) && (userHealth <= compHealth)){
		return true;
	}
	else if ((compHealth <= 0) && (compHealth <= userHealth)){
		return true;
	}
	else{
		return false;
	}
}
