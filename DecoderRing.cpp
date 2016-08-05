/** 
 *	@file DecoderRing.cpp
 *  @author Matthew Chan
 *  @date 7/19/2016
 *	@version 0.1
 *  @class DecoderRing
 *  @brief This class generates a decoder ring using two seeds for std::default_random_engine
 */
#include "DecoderRing.h"

/**
 * Constructor for generating the decoder ring
 * @param seed1 is the first seed used by the random engine
 * @param seed2 is the second seed used by the random engine
 */
DecoderRing::DecoderRing(unsigned int seed1, unsigned int seed2) {
	// Add ASCII values for possible password characters into two vectors
	for (unsigned int i = 31; i <= 126; ++i) {
		inner.push_back(i);
		outer.push_back(i);
	}
	// Shuffle the order of each vector using different seeds
	engine.seed(seed1);
	std::random_shuffle(inner.begin(), inner.end(), [&](int i) -> int {return engine() % i;});
	engine.seed(seed2);
	std::random_shuffle(outer.begin(), outer.end(), [&](int i) -> int {return engine() % i; });

	/* Print the vectors' contents to the console
	std::cout << "Inner Ring: | ";
	for (auto element : inner) {
	std::cout << element << " | ";
	}
	std::cout << std::endl;
	std::cout << "Outer Ring: | ";
	for (auto element : outer) {
	std::cout << element << " | ";
	}
	std::cout << std::endl;
	*/

	// Add the elements (in order) from each vector as a key/value pair to the map
	for (unsigned int i = 0; i < inner.size(); ++i) {
		map.insert(std::pair<unsigned int, unsigned int>(inner[i], outer[i]));
	}

	/* Print out the key/value pairs as letters
	for (auto element : ring) {
	std::cout << static_cast<char>(element.first) << ": " << static_cast<char>(element.second) << std::endl;
	}
	*/
}

/**  
 * Return the value of the inner ring corresponding to the outer ring
 * @param i is the ASCII value of the character in the outer ring to lookup
 * @return The ASCII value of the corresponding character in the inner ring
 */
unsigned int DecoderRing::getValue(unsigned int i) {
	// If i exists in the map as a key, return its corresponding value
	if (map.find(i) != map.end()) return map.find(i)->second;
	// Return -1 if the key value entered was not found
	return -1;
}
