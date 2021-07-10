#include <iostream>
#include <string>


using namespace std;

struct column_songs {

	string songTitle;
	string songSinger;
	string songDuration; //4:34 2:34
	//pointer refering to next song
	column_songs* nextSong; //next column 
};

struct row_playlists {

	string playlistName;
	//pointers pointing to next row node (^ v traverse)
	row_playlists* nextRow;
	//pointers refer to next column (< > traverse)
	column_songs* columnHead;
};

//songs collection prototypes
void addSong();
void displaySong();
void deleteSong();
void playlistSearch_Song();

//playlist collection prototypes
void createPlaylist();
void displayPlaylist();
void addSong_to_Playlist();
void viewSong_frm_Playlist();
void removeSong_frm_Playlist();
void navigation_Playlist();
void deletePlaylist();

//menu prototypes
void songMenu();
void playlistMenu();
void userPromptPlaylist();
void display_playlist_without_menu();
void display_song_without_menu();

//playlist navigation (doubly circular linked list)
struct navigationNode {
	string songTitle;
	string songSinger;
	navigationNode* next;
	navigationNode* prev;

}*head, * tail; //creating 2 node pointers named head and tail


typedef struct choice {
	int choiceNum;
	char selection;
}choice;

//create a null head
row_playlists* matrixHead = NULL;

int main() {
	struct choice selection;
	cout << "==================================================" << endl;
	cout << "\tWelcome To Music Management Studio" << endl;
	cout << "==================================================" << endl;
	cout << "1. Manage songs collection" << endl;
	cout << "2. Manage Playlist" << endl;
	cout << "0. Exit" << endl;
	cout << "Enter your choice (0 - 2) : ";

	cin >> selection.choiceNum;
	switch (selection.choiceNum) {
	case 0:
		return 0;
	case 1:
		songMenu();
		break;
	case 2:
		playlistMenu();
		break;
	default:
		cout << "Please try again." << endl;
		main();
		break;
	}

	return 0;
}

void createPlaylist() {
	choice playlistChoice;
	string playlistName;
	choice choice;
	bool isDupli = false;

	if (matrixHead == NULL) {
		cout << "Please create song first thank you" << endl;
		cout << "1. Main Menu" << endl;
		cout << "Any key. Exit" << endl;
		cout << "Your selection: ";
		cin >> playlistChoice.choiceNum;

		if (playlistChoice.choiceNum == 1) {
			main();
		}
		else {
			exit(0);
		}
	}
	else {
		cout << "Please enter the playlist name: ";
		cin.clear();
		cin.ignore();
		getline(cin, playlistName);

		row_playlists* currentPlaylist = matrixHead;

		while (currentPlaylist != NULL) {
			currentPlaylist = currentPlaylist->nextRow;
		}

		if (currentPlaylist == NULL) {
			currentPlaylist = matrixHead;
			while (currentPlaylist->nextRow != NULL) {
				currentPlaylist = currentPlaylist->nextRow;
				if (currentPlaylist->playlistName == playlistName) {
					cout << "Duplicate playlist detected!" << endl;
					createPlaylist();
					break;
				}
			}

			row_playlists* newPlaylist = new row_playlists();
			newPlaylist->columnHead = NULL;
			newPlaylist->playlistName = playlistName;
			newPlaylist->nextRow = NULL;

			//connection
			currentPlaylist->nextRow = newPlaylist;
			//user prompt
			cout << "Your playlist " << playlistName << " has been successfully created." << endl;
			userPromptPlaylist();

		}

	}
}

void displayPlaylist() {


	if (matrixHead == NULL || matrixHead->nextRow == NULL) {
		cout << "There is no playlist here!" << endl;
		playlistMenu();
	}
	else {
		char choice;
		//call function to print playlists
		display_playlist_without_menu();
		cout << "All playlists have been printed. Proceed to menu? (Y/N to exit): ";
		cin >> choice;

		if (choice == 'Y') {
			playlistMenu();
		}
		else if (choice == 'N') {
			exit(0);
		}
	}
}

void displaySong()
{
	char choice;
	if (matrixHead == NULL || matrixHead->columnHead == NULL) {
		cout << "There is no songs here!" << endl;
		songMenu();
	}
	else {
		//call function
		display_song_without_menu();
		cout << "All songs have been printed. Proceed to menu? (Y/N to exit): ";
		cin >> choice;

		if (choice == 'Y') {
			songMenu();
		}
		else if (choice == 'N') {
			exit(0);
		}
	}
}

void display_song_without_menu() {
	column_songs* currentColumn = matrixHead->columnHead;

	//iterate until the dead end
	cout << "Available Songs: " << endl;
	while (currentColumn != NULL) {
		cout << "Title: " << currentColumn->songTitle << "\t" <<
			"Artist: " << currentColumn->songSinger << "\t" <<
			"Song Duration: " << currentColumn->songDuration << "\t" << "\n\n";

		currentColumn = currentColumn->nextSong;
	}
	delete currentColumn; //free ram
	cout << endl;
}

void display_playlist_without_menu() {
	cout << "Available playlist(s) are: \n";
	//matrixHead->nextRow to skip "Unknown Playlist"
	row_playlists* currentPlaylist = matrixHead->nextRow;
	while (currentPlaylist != NULL) {
		cout << currentPlaylist->playlistName << endl;
		currentPlaylist = currentPlaylist->nextRow;
	}
	delete currentPlaylist; //free ram
}

void addSong_to_Playlist()
{
	string playlistName, songTitle, songArtist, songDuration;
	choice choice;
	row_playlists* currentRow = matrixHead;
	bool isFoundPlaylist = false;
	


	cin.clear(); //garbage clear
	cin.ignore();
	display_playlist_without_menu();
	cout << "Please enter the playlist name: " << endl;
	getline(cin, playlistName);


	while (currentRow != NULL) {
		if (currentRow->playlistName == playlistName) {
			isFoundPlaylist = true; //playlist found
			cin.clear(); //garbage clear
			

			//solved 1: while loop to insert song until break
			while (isFoundPlaylist == true) {
				//call function to print songs
				display_song_without_menu();
				//declare isfoundsong = false/reset every iteration
				bool isFoundSong = false;
				cout << "Please enter the song name you wish to add into playlist " << playlistName << ": " << endl;

				getline(cin, songTitle);

				//step 1: search if songTitle == matrixHead->songTitle
				column_songs* currentSong = matrixHead->columnHead;
				while (currentSong != NULL) {
					if (currentSong->songTitle == songTitle) {
						//catch
						isFoundSong = true;
						songArtist = currentSong->songSinger;
						songDuration = currentSong->songDuration;

						currentSong = currentRow->columnHead;
						//step 2.1 if playlist doesnt have song
						if (currentSong == NULL) {
							column_songs* newSong = new column_songs; //malloc
							newSong->songSinger = songArtist; //assignment
							newSong->songDuration = songDuration;
							newSong->songTitle = songTitle;
							newSong->nextSong = NULL;

							currentRow->columnHead = newSong;
						}
						//step 2.2 if playlist have song //problem
						else {
							while (currentSong->nextSong != NULL) {
								currentSong = currentSong->nextSong;
							}
							column_songs* newSong = new column_songs; //malloc
							newSong->songSinger = songArtist; //assignment
							newSong->songDuration = songDuration;
							newSong->songTitle = songTitle;
							newSong->nextSong = NULL;

							currentSong->nextSong = newSong;
						}
						break;
					}
					currentSong = currentSong->nextSong;
				}
				if (currentSong == NULL && isFoundSong == false) {
					cout << "There is no song named " << songTitle << " in the songs collection.\n";
					main();
				}
				else {
					cout << "The song " << songTitle << " is added succesfully to " << playlistName << "\n" << endl;
					cout << "1) Add another song to playlist " << playlistName << endl;
					cout << "2) Back to playlist menu" << endl;
					cin >> choice.choiceNum;
					switch (choice.choiceNum) {
					case 1:
						cin.clear();
						cin.ignore();
						continue;
						break;
					case 2:
						playlistMenu();
						break;
					default:
						cout << "Enter correct input!" << endl;
						playlistMenu();
					}
				}
			}
		}
		currentRow = currentRow->nextRow;
	}
	while (currentRow == NULL && isFoundPlaylist == false) {
		cout << "There is no playlist named " << songTitle << " in the songs collection.\n";
		playlistMenu();
	}


}

void viewSong_frm_Playlist() {
	choice choice;
	bool isExist = false;

	row_playlists* currentRow = matrixHead;
	string playlistName;

	cin.ignore();
	cin.clear();
	//func call to print playlist
	display_playlist_without_menu();
	cout << "Please enter the playlist name: ";
	getline(cin, playlistName);
	while (currentRow != NULL) {
		if (currentRow->playlistName == playlistName) {

			cout << "Playlist Name : " << currentRow->playlistName << "\n\n";
			column_songs* currentColumn = currentRow->columnHead;// Initialize current  
			while (currentColumn != NULL)
			{
				isExist = true;
				cout << "Song title : " << currentColumn->songTitle << "\n";
				cout << "Song singer : " << currentColumn->songSinger << "\n";
				cout << "Song duration : " << currentColumn->songDuration << "\n\n";
				currentColumn = currentColumn->nextSong;
			}
			break;
		}
		currentRow = currentRow->nextRow;
	}
	if (isExist == false) {
		cout << "There is no song in " << playlistName << "\n\n";
		do {
			cout << "1) Back to Playlist Menu" << endl;
			cout << "2) Exit" << endl;
			cin >> choice.choiceNum;
			if (choice.choiceNum == 1)
				playlistMenu();
			else if (choice.choiceNum == 2)
				exit(0);
		} while (choice.choiceNum == 1 || choice.choiceNum == 2);
	}
	else {
		do {
			cout << "All song in " << playlistName << " has been printed out!" "\n" << endl;
			cout << "1) Back to playlist menu" << endl;
			cout << "2) Back to main menu" << endl;
			cin >> choice.choiceNum;
			if (choice.choiceNum == 1)
				playlistMenu();
			else
				main();
		} while (choice.choiceNum == 1);
	}
}

void removeSong_frm_Playlist() {
	string playlistName, songTitle, songArtist, songDuration;
	choice choice;
	row_playlists* currentRow = matrixHead;
	bool isFoundPlaylist = false;
	bool isExist = false;


	cin.clear(); //garbage clear
	cin.ignore();
	//func call
	display_playlist_without_menu();
	cout << "Please enter the playlist name: ";
	getline(cin, playlistName);
	while (currentRow != NULL) {

		if (currentRow->playlistName == playlistName) {
			isFoundPlaylist = true; //playlist found
			cin.clear(); //garbage clear

			//print out all songs from the playlist
			column_songs* currentColumn = currentRow->columnHead;// Initialize current  
			while (currentColumn != NULL)
			{
				isExist = true;
				cout << "Song title : " << currentColumn->songTitle << "\n";
				cout << "Song singer : " << currentColumn->songSinger << "\n";
				cout << "Song duration : " << currentColumn->songDuration << "\n\n";
				currentColumn = currentColumn->nextSong;
			}
			//deletion of null pointer to avoid errors.
			delete currentColumn;
			cout << "Please enter the song name you wish to delete from playlist " << playlistName << ": " << endl;
			getline(cin, songTitle);

			//step 1: search if songTitle == titles inside the playlist
			column_songs* currentSong = currentRow->columnHead;
			while (currentSong != NULL) {
				if (currentSong->songTitle == songTitle) {
					//catch
					column_songs* toDelete = currentRow->columnHead;
					column_songs* previous = NULL;

					while (toDelete != NULL) {
						if (toDelete->songTitle == songTitle) {
							isExist = true;
							break;
						}
						previous = toDelete;
						toDelete = toDelete->nextSong;
					}
					if (isExist) {
						//if node is at first position.
						if (toDelete == currentRow->columnHead) {
							currentRow->columnHead = currentRow->columnHead->nextSong;
							delete toDelete;
						}
						else {
							previous->nextSong = toDelete->nextSong;
							delete toDelete;
						}
					}
					break;
				}
				currentSong = currentSong->nextSong;
			}
			if (currentSong == NULL && isExist == false) {
				cout << "There is no song named " << songTitle << " in the songs collection.\n";
				main();
			}
		}
		currentRow = currentRow->nextRow;
	}
	while (currentRow == NULL && isFoundPlaylist == false) {
		cout << "There is no playlist named " << playlistName << " in the songs collection.\n";
		playlistMenu();
	}

	do {
		cout << "The song " << songTitle << " is deleted succesfully at " << playlistName << "\n" << endl;
		cout << "1) Back to Playlist Menu" << endl;
		cout << "2) exit" << endl;
		cin >> choice.choiceNum;
		if (choice.choiceNum == 1) {
			playlistMenu();
		}
		else {
			exit(0);
		}
	} while (choice.choiceNum == 1);
}

void navigation_Playlist() {
	bool isExist = false;

	row_playlists* currentPlaylist = matrixHead;

	string playlistName;

	cin.ignore();
	cin.clear();

	currentPlaylist = matrixHead->nextRow;
	cout << "Available Playlists: " << endl;
	while (currentPlaylist != NULL) {
		cout << currentPlaylist->playlistName << endl;
		currentPlaylist = currentPlaylist->nextRow;
	}
	currentPlaylist = matrixHead->nextRow; //reset
	cout << "Please enter the playlist name: ";
	getline(cin, playlistName);

	while (currentPlaylist != NULL) {
		if (currentPlaylist->playlistName == playlistName) {

			cout << "Playlist Name : " << currentPlaylist->playlistName << "\n\n";
			column_songs* currentColumn = currentPlaylist->columnHead;// Initialize current  
			while (currentColumn != NULL) {
				isExist = true;
				//create a new doubly circular linked list
				navigationNode* newNode = new navigationNode;
				newNode->songSinger = currentColumn->songSinger;
				newNode->songTitle = currentColumn->songTitle;
				if (head == NULL) {
					head = tail = newNode;
					head->next = head;
					head->prev = head;
				}
				//if having some node, creating 1+n nodes
				else {
					//tail pointer->next store new node address
					tail->next = newNode;
					//previous node to store tail address (previous track)
					newNode->prev = tail;
					//next song to store head pointer (next track)
					newNode->next = head;
					//head prev pointer to store newnode address (prev repeat)
					head->prev = newNode;
					//tail to point to new node
					tail = newNode;
				}

				currentColumn = currentColumn->nextSong;
			}
			if (currentColumn == NULL) {
				navigationNode* naviPtr = head;
				choice choice;
				system("cls");
				cout << "Entering song navigation program. 0 to playlist menu.\n\n";
				cout << "Welcome to Sportify!" << endl;

				do {
					cout << "0 to playlist menu.\n\n\n\n";
					cout << "Now Playing: " << naviPtr->songTitle << endl;
					cout << "Artist: " << naviPtr->songSinger << endl;
					cout << "1. |<< Previous Track\t\t\t\t" << "2. Next Track >>|\n";
					cout << "Input: ";
					cin >> choice.choiceNum;

					switch (choice.choiceNum) {
					case 1:
						//previous song
						naviPtr = naviPtr->prev;
						system("cls");
						break;
					case 2:
						//next song
						naviPtr = naviPtr->next;
						system("cls");
						break;
					case 0:
						//close navigation program
						delete naviPtr;
						playlistMenu(); //garbage
						break;
					default:
						cout << "Enter Correct Input!\n\n";
						continue;
					}

				} while (choice.choiceNum == 1 || choice.choiceNum == 2 || choice.choiceNum == 0);
			}

		}
		currentPlaylist = currentPlaylist->nextRow;
	}


}

void deletePlaylist() {

	row_playlists* currentPlaylist = matrixHead;
	row_playlists* trailPlaylist = NULL;

	if (matrixHead->nextRow == NULL) {
		cout << "There is no playlist here!" << endl;
		userPromptPlaylist();

	}
	else {
		string playlistName;

		//view available playlists
		cout << "Available playlist(s) are: \n";
		//matrixHead->nextRow to skip "Unknown Playlist"
		row_playlists* currentPlaylist = matrixHead->nextRow;
		while (currentPlaylist != NULL) {
			cout << currentPlaylist->playlistName << endl;
			currentPlaylist = currentPlaylist->nextRow;
		}
		//reset curr playlist back to head address
		currentPlaylist = matrixHead;
		cin.ignore();
		cin.clear();
		cout << "\nPlease enter the playlist name to be deleted.\n";
		getline(cin, playlistName);

		while (currentPlaylist != NULL) {
			if (currentPlaylist->playlistName == playlistName) {
				//step 1: free memory by deleting songs inside the playlist
				while (currentPlaylist->columnHead != NULL) {
					column_songs* trailNode = NULL;
					column_songs* currentNode = currentPlaylist->columnHead;
					//traverse until end of list
					while (currentNode->nextSong != NULL) {
						trailNode = currentNode;
						currentNode = currentNode->nextSong;
					}
					//if temp matches the song list head 
					if (currentNode == currentPlaylist->columnHead) {
						//assume head is null
						currentPlaylist->columnHead = NULL;
						delete currentNode; //free ram
						//break from loop
						break;
					}
					else {
						//assign current song-> next song to null
						trailNode->nextSong = NULL;
					}
					delete currentNode;

				}

				//step 2 situation 1: if playlist is at the bottom
				if (currentPlaylist->nextRow == NULL) {
					//trail will become the bottom, set next row = null
					trailPlaylist->nextRow = NULL;
					//delete node that being pointed by curr
					delete currentPlaylist;
					break;
				}
				//step 2 situation 2: if playlist is at arbitrary position
				else {
					//if playlist is in the middle..
					//trail pointer to assign next row = current row->next row
					trailPlaylist->nextRow = currentPlaylist->nextRow;
					//delete current node
					delete currentPlaylist;
					break;

				}
			}
			//assign trail = current, then current = next row 
			trailPlaylist = currentPlaylist;
			currentPlaylist = currentPlaylist->nextRow;
		}
		if (currentPlaylist == NULL) {
			cout << "There is no playlist named " << playlistName << " here.\n";
			userPromptPlaylist();
		}
		else {
			cout << "Playlist named " << playlistName << " has been deleted.\n";
			userPromptPlaylist();
		}
	}
}

void addSong() {
	choice choice;


	string title, duration, singer;
	//first iteration (head == NULL)
	if (matrixHead == NULL) {
		//assignment (default playlist)
		//the songs will be first added into an unknown playlist
		//then delete when we assign it to another playlist
		row_playlists* unknownPlaylist = new row_playlists();
		unknownPlaylist->columnHead = NULL;
		unknownPlaylist->nextRow = NULL;
		unknownPlaylist->playlistName = "Unknown Playlist";

		//create column node
		column_songs* newColumn = new column_songs();
		//assignment
		cin.clear(); //garbage clear
		cin.ignore();

		cout << "Please enter the song title: "; //breakpoint
		getline(cin, title);
		cout << "Please enter the song singer: ";
		getline(cin, singer);
		cout << "Please enter the song duration: ";
		getline(cin, duration);


		newColumn->songTitle = title;
		newColumn->songSinger = singer;
		newColumn->songDuration = duration;

		//assign NULL to nextSong pointer
		newColumn->nextSong = NULL;

		//	assign new row -> column head pointer 
		//	with new column memory address
		unknownPlaylist->columnHead = newColumn;
		unknownPlaylist->nextRow = NULL;
		//assign matrix head pointer with unknown playlist pointer
		matrixHead = unknownPlaylist;

	}
	//if matrix head != NULL (n+1 songs)
	else {
		//assigning the (n+1)th song on the unknown playlist
		column_songs* currentColumn = matrixHead->columnHead;

		//run iteration until currentColumn->nextColumn reaches NULL
				//TO ENSURE we add a new node at the most bottom
		while (currentColumn->nextSong != NULL) {
			currentColumn = currentColumn->nextSong;
		}

		//create a new column at the right most
		column_songs* newColumn = new column_songs();
		//assignment
		cin.clear(); //garbage clear
		cin.ignore();

		cout << "Please enter the song title: ";
		getline(cin, newColumn->songTitle);

		cout << "Please enter the song singer: ";
		getline(cin, newColumn->songSinger);
		cout << "Please enter the song duration: ";
		getline(cin, newColumn->songDuration);


		//assign NULL to nextSong pointer
		newColumn->nextSong = NULL;

		//connect
		currentColumn->nextSong = newColumn;


	}

	do {
		cout << "The song has been successfully added." << endl;
		cout << "1) Add another song" << endl;
		cout << "2) Back to menu" << endl;
		cin >> choice.choiceNum;
		if (choice.choiceNum == 1)
			addSong();
		else
			main();
	} while (choice.choiceNum == 1 || choice.choiceNum == 2);


}

void deleteSong()
{
	//template to clear garbage
	cin.clear();
	cin.ignore();

	string songName;
	choice afterDelete;
	bool isExist = false;

	//situation 0: head pointer is null
	if (matrixHead == NULL) {
		cout << "There is no songs here." << endl;
		songMenu();
	}
	else {
		//func call to print songs
		display_song_without_menu();

		cout << "Please enter the song name you'd wish to delete: ";
		getline(cin, songName);

		//unknown playlist + other playlists are created

		row_playlists* currentPlaylist = matrixHead;
		column_songs* trailPtr = NULL;
		while (currentPlaylist != NULL) {
			//initialize current song&trail pointer to column head of different playlists
			column_songs* currentSong = currentPlaylist->columnHead;

			//traversing inside the particular playlist
			while (currentSong != NULL) {
				//traverse in this loop to scan through the songs
				if (currentSong->songTitle == songName) {
					isExist = true;
					//deletion of songs in this if
					//situation 1: if song == head position
					if (currentPlaylist->columnHead == currentSong) {
						//trail pointer assigned with &currentSong
						trailPtr = currentSong;
						//current song++
						currentSong = currentSong->nextSong;
						//assign head with new head
						currentPlaylist->columnHead = currentSong;
						delete trailPtr;
						break;
					}
					//situation 2: if song == last position
					else if (currentSong->nextSong == NULL) {
						//next song is null
						trailPtr->nextSong = NULL;
						//delete current pointer song
						delete currentSong;
						break;
					}
					//situation 3: if song == (n+1)th position
					else {
						//temp assigns its nextSong as current->next song (to skip the current song)
						trailPtr->nextSong = currentSong->nextSong;
						//delete current song
						delete currentSong;
						break;
					}
				}
				trailPtr = currentSong;
				currentSong = currentSong->nextSong;
			}
			currentPlaylist = currentPlaylist->nextRow;
		}
		if (isExist == true) {
			cout << "\nThe song named " << songName << " has been deleted.\n1. Delete another song\n2. Proceed to Menu\n0. Exit\n";
		}
		else {
			cout << "\nNo song named " << songName << " here.\n1. Find and delete another song\n2. Proceed to Menu\n0. Exit\n";

		}
		cout << endl << "Your selection: ";
		cin >> afterDelete.choiceNum;

		switch (afterDelete.choiceNum) {
		case 0:
			exit(0);
			break;
		case 1:
			deleteSong();
			break;
		case 2:
			main();
			break;
		default:
			cout << "Please try again." << endl;
			songMenu();
			break;
		}

	}
}

void playlistSearch_Song() {
	string songName;
	choice choice;
	bool isMatch = false;

	if (matrixHead == NULL) {
		cout << "The song is empty.\n\n";
		songMenu();
	}
	else { //toggle breakpoint
		cin.clear();
		cin.ignore();
		//call func to print song names
		display_song_without_menu();
		cout << "Enter song name: ";
		getline(cin, songName);

		row_playlists* currentRow = matrixHead->nextRow;
		if (currentRow == NULL) {
			cout << "No playlist here. " << endl;
			songMenu();
		}
		else {
			cout << "The playlists which have the song " << songName << " are :\n";

			while (currentRow != NULL) {
				column_songs* currentColumn = currentRow->columnHead;
				while (currentColumn != NULL) {
					if (currentColumn->songTitle == songName) {
						isMatch = true;
						cout << currentRow->playlistName << endl;
					}
					currentColumn = currentColumn->nextSong;
				}
				currentRow = currentRow->nextRow;
			}
			//catch
			if (isMatch == false) {
				cout << "There is no song named " << songName << " in the song collection!" << endl;
			}
			else {
				cout << "\nAll of the playlists have been printed.\n";
			}
		}
		cout << "1. return to song menu" << endl;
		cout << "2. exit" << endl;
		cout << "selection: ";
		cin >> choice.choiceNum;
		if (choice.choiceNum == 1)
			songMenu();
		else if (choice.choiceNum == 2)
			exit(0);
		else {
			cout << "Wrong input!";
		}
		songMenu();
	}
}

void songMenu() {
	struct choice selection;
	cout << "Songs Collection Management" << endl << endl;
	cout << "1. Add songs" << endl;
	cout << "2. Display songs" << endl;
	cout << "3. Delete songs" << endl;
	cout << "4. Playlist search" << endl;
	cout << "5. Main menu" << endl;
	cout << "0. Exit" << endl;
	cout << "Enter your choice (0 - 4) : ";
	cin >> selection.choiceNum;

	switch (selection.choiceNum) {
	case 0:
		exit(0);
		break;
	case 1:
		addSong();
		break;
	case 2:
		displaySong();
		break;
	case 3:
		deleteSong();
		break;
	case 4:
		playlistSearch_Song();
		break;
	case 5:
		main();
		break;
	default:
		cout << "Please try again." << endl;
		songMenu();
		break;
	}
}

void playlistMenu() {
	struct choice playlistSelection;
	cout << "Playlist Management" << endl;
	cout << "1. Create playlist" << endl;
	cout << "2. View playlist" << endl;
	cout << "3. Add songs to a playlist" << endl;
	cout << "4. View songs from a playlist" << endl;
	cout << "5. Remove songs from a playlist" << endl;
	cout << "6. Playlist navigation" << endl;
	cout << "7. Delete playlist" << endl;
	cout << "8. Main menu" << endl;
	cout << "0. Exit" << endl << endl;
	cout << "Enter your choice (0 - 7) : ";
	cin >> playlistSelection.choiceNum;

	switch (playlistSelection.choiceNum) {
	case 0:
		exit(0);
		break;
	case 1:
		createPlaylist();
		break;
	case 2:
		displayPlaylist();
		break;
	case 3:
		addSong_to_Playlist();
		break;
	case 4:
		viewSong_frm_Playlist();
		break;
	case 5:
		removeSong_frm_Playlist();
		break;
	case 6:
		navigation_Playlist();
		break;
	case 7:
		deletePlaylist();
		break;
	case 8:
		main();
		break;
	default:
		system("cls");
		cout << "Please try again." << endl;
		playlistMenu();
		break;
	}
}

void userPromptPlaylist() {
	choice choice;
	//user prompt
	cout << "1. Return to playlist menu" << endl;
	cout << "2. Return to main menu" << endl;
	cout << "3. Exit" << endl;
	cout << "Your selection: ";
	cin >> choice.choiceNum;
	do {
		switch (choice.choiceNum) {
		case 1:
			playlistMenu();
			break;
		case 2:
			main();
			break;
		case 3:
			exit(0);
			break;
		default:
			cout << "Enter correct input!" << endl;
		}
	} while (choice.choiceNum != 1 || choice.choiceNum != 2);
}






