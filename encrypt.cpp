/** 
 *	@file encrypt.cpp
 *	@title FileEncrypter
 *	@author Matthew Chan
 *	@date 7/19/2016
 *	@version 0.1
 *	@details Encrypt your text files by choosing two numbers to seed a random number generator. 
 *	Each character in your text file will be encrypted using those seeds you pick, meaning that
 *	your file can only be decrypted using the same seeds. In a sense, those two seeds are two
 *	numerical passwords which are used to access your files. My use case for this program is
 *	for encrypting a text file containing all of my passwords and sensitive data -- this way I can
 *	keep my passwords file in plain view without having to worry about others stealing my passwords
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <iomanip>
#include <string>
#include "DecoderRing.h"

/**
 * Read seeds for the default_random_generator from the user
 * @param seed1 is the first seed inputted by the user
 * @param seed2 is the second seed inputted by the user
 */
void readSeeds(unsigned int& seed1, unsigned int& seed2) {
	std::cout << "EXEC: " << "Enter inner seed: ";
	try {
		std::cin >> seed1;
		std::cout << std::endl;
	}
	catch (std::exception& e) {
		std::cerr << "ERROR: " << "Error reading input: " << e.what() << std::endl;
	}
	std::cout << "EXEC: " << "Enter outer seed: ";
	try {
		std::cin >> seed2;
		std::cout << std::endl;
	}
	catch (std::exception& e) {
		std::cerr << "ERROR: " << "Error reading input: " << e.what() << std::endl;
	}
}


/**
 * Open an input file stream to a file and check if the file was successfully opened
 * @param file is the input file stream used to open the text file
 * @param filepath is the absolute path to the text file
 * @return Whether the file was successfully opened or not
 */
bool openFile(std::ifstream& file, std::string filepath) {
	try {
		file.open(filepath);
	}
	catch (std::exception& e) {
		std::cerr << "ERROR: " << "Invalid console input: " << e.what() << std::endl;
	}
	if (!file.fail()) {
		std::cout << "INFO : " << "File opened successfully." << std::endl;
	}
	else {
		std::cerr << "ERROR: " << "File could not be opened." << std::endl;
		return false;
	}
	return true;
}

/**
 * Encrypt each character in the file and write the contents to an output string stream
 * @param file is the text file
 * @param decoder_ring is the decoder ring
 * @return The encrypted/decrypted version of the file as a stringstream
 */
std::ostringstream encryptFile(std::ifstream& file, DecoderRing decoder_ring) {

	std::ostringstream oss;
	
	// Read each character from the text file and encrypt it using the decoder ring
	while (file.peek() != -1) {
		unsigned int ascii = file.get();

		// Only letters corresponding to keys in the map are encrypted
		if (decoder_ring.getValue(ascii) != -1) {
			oss << static_cast<char>(decoder_ring.getValue(ascii));
		}
		else {
			oss << static_cast<char>(ascii);
		}
	}
	// Close the input stream to the file
	file.close();
	return oss;
}

/* Linux/Unix version

#include <termios.h>
#include <unistd.h>

int main()
{
	// remove console input
	termios oldt;
	tcgetattr(STDIN_FILENO, &oldt);
	termios newt = oldt;
	newt.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	// restore console input
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return 0;
}
*/
/**
 * Function that hides user typing in console
 */
void hideConsoleInput(HANDLE& hStdin, DWORD& mode) {
	GetConsoleMode(hStdin, &mode);
	SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
	
}

/**
 * Function to show user typing in console
 */
void showConsoleInput(HANDLE& hStdin, DWORD& mode) {
	SetConsoleMode(hStdin, mode);
}

/**
 * Print out an ASCII header
 * @param header is the message to display in the terminal
 * @param width is the row width to print out
 * @param is the character used to surround the header
 */
void printHeader(std::string header, unsigned int width, char fillChar) {
	unsigned int center = width / 2 + header.length() / 2;
	std::cout << std::setw(width) << std::setfill(fillChar) << "" << std::endl;
	std::cout << std::setw(center) << header << std::setw(width - center) << "" << std::endl;;
	std::cout << std::setw(width) << "" << std::endl;
}

int main() {
	// Clear the terminal (Windows)
	std::system("CLS");

	// Create and print ASCII title
	static std::string title[] = {
		" /$$$$$$$$          /$$     /$$$$$$$$                                                     /$$",
		"|__  $$__/         | $$    | $$_____/                                                    | $$",
		"   | $$ /$$   /$$ /$$$$$$  | $$       /$$$$$$$   /$$$$$$$  /$$$$$$  /$$   /$$  /$$$$$$  /$$$$$$    /$$$$$$   /$$$$$$",
		"   | $$|  $$ /$$/|_  $$_/  | $$$$$   | $$__  $$ /$$_____/ /$$__  $$| $$  | $$ /$$__  $$|_  $$_/   /$$__  $$ /$$__  $$",
		"   | $$ \\  $$$$/   | $$    | $$__/   | $$  \\ $$| $$      | $$  \\__/| $$  | $$| $$  \\ $$  | $$    | $$$$$$$$| $$  \\__/",
		"   | $$  >$$  $$   | $$ /$$| $$      | $$  | $$| $$      | $$      | $$  | $$| $$  | $$  | $$ /$$| $$_____/| $$",
		"   | $$ /$$/\\  $$  |  $$$$/| $$$$$$$$| $$  | $$|  $$$$$$$| $$      |  $$$$$$$| $$$$$$$/  |  $$$$/|  $$$$$$$| $$",
		"   |__/|__/  \\__/   \\___/  |________/|__/  |__/ \\_______/|__/       \\____  $$| $$____/    \\___/   \\_______/|__/",
		"                                                                    /$$  | $$| $$",
		"     Created by Matthew Chan                                       |  $$$$$$/| $$",
		"     Copyright 2016                                                 \\______/ |__/"
	};
	std::cout << "\n" << std::setw(title[3].length()) << std::setfill('/') << "\n" << std::endl;
	for (unsigned int i = 0; i < sizeof(title) / sizeof(std::string); ++i) {
		std::cout << title[i] << std::endl;
	}
	std::cout << "\n" << std::setw(title[3].length()) << "" << std::endl << std::endl;

	// Prefixes for logging purposes
	std::string infoLog = "INFO: ";
	std::string errorLog = "ERROR: ";
	std::string execLog = "EXEC: ";

	std::string command;


	// Get the relative/absolute path to the text file
	std::cout << execLog << "Enter a filepath to open: ";
	std::string filepath;
	std::cin >> filepath;
	// Open an input file stream to the file
	std::ifstream file;

	// Continue running the program until the user quits
	while (command != "exit") {

		std::cout << infoLog << "Enter a command to execute: ";
		std::cin >> command;

		// Get the inner and outer seeds from the user
		unsigned int seed1, seed2;

		HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
		DWORD mode = 0;
		
		if (command == "encrypt") {
			std::cout << infoLog << "Starting encryption program..." << std::endl;

			// Get filepath of the file to encrypt from the user
			//std::cout << execLog << "Enter filepath to encrypt: ";
			//std::string filepath;
			//std::cin >> filepath;

			// Open an input file stream to the file
			//std::ifstream file;
			if (openFile(file, filepath)) {

				hideConsoleInput(hStdin, mode);
				// Create a encoder ring using two seeds
				readSeeds(seed1, seed2);
				DecoderRing encoder_ring(seed1, seed2);
				// Encrypt the text contents of the file
				std::ostringstream encryptedText = encryptFile(file, encoder_ring);
				showConsoleInput(hStdin, mode);

				// Print out the decrypted version of the text file to the console
				printHeader("File Contents", 50, '-');
				std::cout << encryptedText.str() << std::endl;
				std::cout << std::setw(50) << std::setfill('-') << "" << std::endl;

				// Prompt user with an option to overwrite the contents of the file
				std::cout << execLog << "Overwrite the contents of the encrypted file? (y/n)" << std::endl;
				char answer;
				std::cin >> answer;
				if (answer == 'y') {
					// Open an output stream to the same file
					std::ofstream out(filepath);
					// Overwrite the contents of the file with the decrypted version
					out << encryptedText.str();
					std::cout << infoLog << "Successfully overwritten." << std::endl;
					out.close();
				}
			}
		}
		else if (command == "decrypt") {
			std::cout << infoLog << "Decryption program started..." << std::endl;

			// Get filepath of the file to decrypt from the user
			//std::cout << execLog << "Enter filepath to decrypt: ";
			//std::string filepath;
			//std::cin >> filepath;
			//// Open an input file stream to the file
			//std::ifstream file;
			if (openFile(file, filepath)) {

				hideConsoleInput(hStdin, mode);
				// Create a decoder ring using two seeds
				readSeeds(seed1, seed2);
				DecoderRing decoder_ring(seed2, seed1);
				// Decrypt the text contents of the file
				std::ostringstream decryptedText = encryptFile(file, decoder_ring);
				showConsoleInput(hStdin, mode);

				// Print out the decrypted version of the text file to the console
				printHeader("File Contents", 50, '-');
				std::cout << decryptedText.str() << std::endl;
				std::cout << std::setw(50) << std::setfill('-') << "" << std::endl;

				// Prompt user with an option to overwrite the contents of the file
				std::cout << execLog << "Overwrite the contents of the encrypted file? (y/n)" << std::endl;
				char answer;
				std::cin >> answer;
				if (answer == 'y') {
					// Open an output stream to the same file
					std::ofstream out(filepath);
					// Overwrite the contents of the file with the decrypted version
					out << decryptedText.str();
					std::cout << infoLog << "Successfully overwritten." << std::endl;
					out.close();
				}
			}
		}
		else if (command == "append") {
			std::cout << infoLog << "Starting encryption program..." << std::endl;

			// Get the path of the file to write to
			//std::cout << execLog << "Enter filepath: ";
			//std::string filepath;
			//std::cin >> filepath;
			std::ofstream out;
			try {
				// Append to the file
				out.open(filepath, std::ios_base::app);
			}
			catch (std::exception& e) {
				std::cerr << errorLog << "Invalid filepath." << std::endl;
			}

			// Read in text from the user (100 character limit)
			std::cout << execLog << "Enter text to append: ";
			std::string rawText;
			
			std::getline(std::cin, rawText);
			std::getline(std::cin, rawText);

			hideConsoleInput(hStdin, mode);
			// Create a encoder ring using two seeds
			readSeeds(seed1, seed2);
			DecoderRing encoder_ring(seed1, seed2);
			showConsoleInput(hStdin, mode);

			// Encrypt the string
			std::string encryptedText;
			for (int i = 0; i < rawText.length(); ++i) {
				int ascii = static_cast<int>(rawText[i]);
				encryptedText += static_cast<char>(encoder_ring.getValue(ascii));
			}
			
			// Append the string to the file
			out << std::endl << encryptedText;
			std::cout << infoLog << encryptedText << " was appended to the file" << std::endl;
			out.close();
		}
		else if (command == "exit") {
			std::cout << infoLog << "Closing the program..." << std::endl;
		}
		else {
			std::cerr << errorLog << "Invalid command! Try again." << std::endl;
		}

		// Cleanup
		file.close();
	}

	// Placeholder to prevent console window from closing
	std::cout << "Press any key to close the program...";
	std::getchar();
	std::getchar();

	// Clear the screen
	std::system("CLS");
	return 0;
}